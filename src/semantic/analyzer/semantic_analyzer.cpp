#include "semantic/analyzer/semantic_analyzer.hpp"
#include "util/globals.hpp"
#include "runtime/core/class/builtin_class_init.hpp"

void SemanticAnalyzer::analyze(const AST & ast)
{
	for (auto & statement : ast.statements)
	{
		analyze(statement.get());
	}
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze(ASTNode * node)
{
	switch (node->type)
	{
	case ASTNode::Type::N_VARIABLE_DECLARATION:
		return analyze_variable_declaration(static_cast<VariableDeclarationNode *>(node));
	case ASTNode::Type::N_EXPRESSION_STATEMENT:
		return analyze(static_cast<ExpressionStatementNode *>(node)->expression.get());
	case ASTNode::Type::N_EXPRESSION:
		return analyze_expression(static_cast<ExpressionNode *>(node));
	case ASTNode::Type::N_OPERAND:
		return analyze_operand(static_cast<OperandNode *>(node));
	case ASTNode::Type::N_FUNCTION_CALL:
		return analyze_function_call(static_cast<FunctionCallNode *>(node));
	case ASTNode::Type::N_IDENTIFIER:
		return analyze_identifier(static_cast<IdentifierNode *>(node));
	case ASTNode::Type::N_LITERAL:
		return analyze_literal(static_cast<LiteralNode *>(node));
	default:
		// For debugging purposes
		globals::error_handler.log_semantic_error({
			"Unknown AST node type",
			node->location,
			node->length
		}, true);
	}

	return { builtins::none_class };
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_variable_declaration(VariableDeclarationNode * variable_declaration)
{
	// Check if the variable name is already defined in the current scope
	if (current_scope->is_variable_defined(variable_declaration->identifier->name))
	{
		// Log a warning instead of an error, to allow overwriting variables
		globals::error_handler.log_warning({
			"Redefinition of symbol '" + std::string(variable_declaration->identifier->name) + "'",
			variable_declaration->identifier->location,
			variable_declaration->identifier->length
		});
	}

	// Analyze the type if it exists
	ClassPtr type = nullptr;
	if (variable_declaration->type)
	{
		type = analyze_type(variable_declaration->type.get()).cls;
	}

	if (!variable_declaration->expression && !type)
	{
		// Log a warning if the type is not defined and the expression is missing
		globals::error_handler.log_semantic_error({
			"Variable declaration without type or expression",
			variable_declaration->identifier->location,
			variable_declaration->identifier->length
		}, true);
	}

	// Analyze the expression
	auto result = analyze(variable_declaration->expression.get());
	if (type)
	{
		// Use the helper to check type compatibility and handle errors/warnings
		auto coercion_path = check_type_compatibility(
			type,
			result.cls,
			variable_declaration->expression.get()
		);

		// If the match level is not exact, store the coercion path index for the compiler
		if (coercion_path->effective_match_level != TypeCoercion::MatchLevel::EXACT)
		{
			variable_declaration->coercion_index = TypeCoercion::instance().get_path_index(result.cls, type);
			variable_declaration->needs_coercion = true;
		}

		// If the match level is exact, the coercion is not needed
	}

	// Add the variable to the current scope
	size_t index;
	if (type)
	{
		index = current_scope->define_variable(variable_declaration->identifier->name, type).first;
	}
	else
	{
		// If the type is not defined, use the default type
		index = current_scope->define_variable(variable_declaration->identifier->name).first;
	}
	variable_declaration->variable_index = index;

	return { builtins::none_class };
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_expression(ExpressionNode * expression)
{
	AnalysisResult left = analyze(expression->left.get());
	AnalysisResult right = analyze(expression->right.get());

	// Get all implementations of the operator
	auto implementations = expression->op->implementations();
	auto signature = FunctionSignature({
		{ "", left },
		{ "", right }
	});

	// Find the most specific implementation of the operator for the given signature
	auto match = resolve_overload(
		implementations,
		expression->op->op, // The operator function inside the OperatorNode
		signature,
		{ expression->left, expression->right } // Operand nodes for error reporting
	);

	if (match->conversion.match_level == TypeCoercion::MatchLevel::INCOMPATIBLE)
	{
		globals::error_handler.log_semantic_error({
			"No operator implementation found for the given operands",
			expression->op->location,
			expression->op->length,
			"Got: '" + left.to_string() + "' and '" + right.to_string() + '\''
		}, true);
	}

	// Store the index of the operator and the match conversions for the compiler
	expression->op->match = match;
	return expression->op->op->get_implementation(match->index)->return_type();
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_operand(OperandNode * operand)
{
	AnalysisResult primary = analyze(operand->primary.get());

	if (!operand->op)
	{
		return primary;
	}

	auto implementations = operand->op->implementations();
	auto signature = FunctionSignature({
		{ "", primary }
	});

	// Find the most specific implementation of the operator for the given signature
	auto match = resolve_overload(
		implementations,
		operand->op->op, // The operator function inside the OperatorNode
		signature,
		{ operand->primary } // Operand node for error reporting
	);

	if (match->conversion.match_level == TypeCoercion::MatchLevel::INCOMPATIBLE)
	{
		globals::error_handler.log_semantic_error({
			"No operator implementation found for the given operand",
			operand->op->location,
			operand->op->length,
			"Got: '" + primary.to_string() + '\''
		}, true);
	}

	// Store the index of the operator and the match conversions for the compiler
	operand->op->match = match;
	return operand->op->op->get_implementation(match->index)->return_type();
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_function_call(FunctionCallNode * function_call)
{
	auto [index, variable] = find_variable_in_current_scope(
		function_call->identifier->name,
		"Symbol '" + std::string(function_call->identifier->name) + "' is not defined",
		function_call->identifier.get() // Identifier node for error reporting
	);
	
	if (!variable->is_function())
	{
		globals::error_handler.log_semantic_error({
			"Symbol '" + std::string(function_call->identifier->name) + "' is not a function",
			function_call->identifier->location,
			function_call->identifier->length
		}, true);
	}

	auto function = variable->value()->as<Function>();
	
	// Analyze the arguments
	FunctionSignature signature;
	for (auto & argument : function_call->arguments)
	{
		auto result = analyze(argument.get());
		signature.parameters.push_back({ "", result });
	}

	// Find the most specific implementation of the function for the given signature
	auto match = resolve_overload(
		function->implementations(),
		function,
		signature,
		function_call->arguments
	);

	if (match->conversion.match_level == TypeCoercion::MatchLevel::INCOMPATIBLE)
	{
		globals::error_handler.log_semantic_error({
			"No function implementation found for the given arguments",
			function_call->location,
			function_call->length,
			"Arguments: " + signature.to_string(true),
			"Candidate implementations:\n" + function->implementations().to_string()
		}, true);
	}

	// Store the index of the function and the match conversions for the compiler
	function_call->function_index = index;
	function_call->match = match;
	return function->get_implementation(match->index)->return_type();
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_identifier(IdentifierNode * identifier)
{
	auto [index, variable] = find_variable_in_current_scope(
		identifier->name,
		"Symbol '" + std::string(identifier->name) + "' is not defined",
		identifier // Identifier node for error reporting
	);

	identifier->variable_index = index;
	return { variable->get_class(), Type::Qualifier::REF };
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_type(TypeNode * type)
{
	auto [index, variable] = find_variable_in_current_scope(
		type->name,
		"Type '" + std::string(type->name) + "' is not defined",
		type // Type node for error reporting
	);

	if (!variable->is_class())
	{
		globals::error_handler.log_semantic_error({
			"Symbol '" + std::string(type->name) + "' is not a type",
			type->location,
			type->length
		}, true);
	}

	auto cls = variable->value()->as<Class>();
	return cls;
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_literal(LiteralNode * literal)
{
	return literal->cls;
}
	
FunctionImplementationRegistry::MatchPtr SemanticAnalyzer::resolve_overload(
	const FunctionImplementationRegistry & implementations,
	const FunctionPtr & function,
	const FunctionSignature & signature,
	const std::vector<std::shared_ptr<ASTNode>> & argument_nodes
)
{
	auto match = implementations.find_best_match(signature);

	if (match->conversion.match_level == TypeCoercion::MatchLevel::LOSSY)
	{
		// Look for the lossy conversions
		for (size_t i = 0; i < match->conversion.conversions.size(); ++i)
		{
			if (match->conversion.conversions[i]->effective_match_level == TypeCoercion::MatchLevel::LOSSY)
			{
				auto & from = signature.parameters[i].second;
				auto & to = function->get_implementation(match->index)->signature().parameters[i].second;
				globals::error_handler.log_warning({
					"Lossy conversion from '" + from.to_string() + "' to '" + to.to_string() + "'",
					argument_nodes[i]->location,
					argument_nodes[i]->length
				});
			}
		}
	}

	return match;
}

TypeCoercion::CoercionPathPtr SemanticAnalyzer::check_type_compatibility(
	const Type & expected,
	const Type & actual,
	const ASTNode * node
)
{
	// Try to find a coercion path
	auto coercion_path = TypeCoercion::instance().find_best_coercion_path(actual, expected);

	// If the coercion path is incompatible, log an error
	if (coercion_path->effective_match_level == TypeCoercion::MatchLevel::INCOMPATIBLE)
	{
		globals::error_handler.log_semantic_error({
			"Cannot convert '" + actual.cls->name() + "' to '" + expected.cls->name() + "'",
			node->location,
			node->length
		}, true);
	}

	// If the coercion is lossy, log a warning
	if (coercion_path->effective_match_level == TypeCoercion::MatchLevel::LOSSY)
	{
		globals::error_handler.log_warning({
			"Lossy conversion from '" + actual.cls->name() + "' to '" + expected.cls->name() + "'",
			node->location,
			node->length
		});
	}

	return coercion_path;
}

std::pair<size_t, VariablePtr> SemanticAnalyzer::find_variable_in_current_scope(
	std::string_view name,
	std::string_view error_message,
	const ASTNode * node
)
{
	auto [index, variable] = current_scope->find_variable(name);
	if (index == -1)
	{
		globals::error_handler.log_semantic_error({
			std::string(error_message),
			node->location,
			node->length
		}, true);
		// The error is fatal, so we won't reach here
	}
	return { index, variable };
}