#include "semantic_analyzer/semantic_analyzer.hpp"
#include "util/globals.hpp"
#include "class/builtins.hpp"

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
	if (current_scope->is_binding_defined(variable_declaration->identifier->name))
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
		// Try to find a coercion path
		auto coercion_path = TypeCoercion::instance().find_best_coercion_path(result.cls, type);

		// If the coercion path is incompatible, log an error
		if (coercion_path->effective_match_level == TypeCoercion::MatchLevel::INCOMPATIBLE)
		{
			globals::error_handler.log_semantic_error({
				"Cannot convert '" + result.cls->name() + "' to '" + type->name() + "'",
				variable_declaration->expression->location,
				variable_declaration->expression->length
			}, true);
		}

		// If the coercion is lossy, log a warning
		if (coercion_path->effective_match_level == TypeCoercion::MatchLevel::LOSSY)
		{
			globals::error_handler.log_warning({
				"Lossy conversion from '" + result.cls->name() + "' to '" + type->name() + "'",
				variable_declaration->expression->location,
				variable_declaration->expression->length
			});
		}

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
		index = current_scope->define_binding(variable_declaration->identifier->name, type).first;
	}
	else
	{
		// If the type is not defined, use the default type
		index = current_scope->define_binding(variable_declaration->identifier->name).first;
	}
	variable_declaration->variable_index = index;

	return { builtins::none_class };
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_expression(ExpressionNode * expression)
{
	AnalysisResult left = analyze(expression->left.get());
	AnalysisResult right = analyze(expression->right.get());

	//TODO: Put this in a separate method
	// Find the candidate operator implementation for the given operands
	// If a candidate is found, return the result of the operation
	// If no candidate is found, log an error
	auto implentations = expression->op->implementations();
	auto impl = implentations.find_most_specific(left.cls, right.cls);
	if (impl)
	{
		// Store the implementation for the compiler
		expression->op->implementation = impl;
		return impl->result_class();
	}
	else
	{
		globals::error_handler.log_semantic_error({
			"No operator implementation found for the given operands",
			expression->op->location,
			expression->op->length,
			"Got: '" + left.to_string() + "' and '" + right.to_string() + '\''
		}, true);
	}

	return { builtins::none_class };
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_operand(OperandNode * operand)
{
	AnalysisResult primary = analyze(operand->primary.get());

	if (!operand->op)
	{
		return primary;
	}

	//TODO: Put this in a separate method
	// Find the candidate operator implementation for the given operands
	// If a candidate is found, return the result of the operation
	// If no candidate is found, log an error
	auto implentations = operand->op->implementations();
	auto impl = implentations.find_most_specific(primary.cls, builtins::none_class);
	if (impl)
	{
		// Store the implementation for the compiler
		operand->op->implementation = impl;
		return impl->result_class();
	}
	else
	{
		globals::error_handler.log_semantic_error({
			"No operator implementation found for the given operand",
			operand->op->location,
			operand->op->length,
			"Got: '" + primary.to_string() + '\''
		}, true);
	}

	return { builtins::none_class };
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_function_call(FunctionCallNode * function_call)
{
	auto [index, binding] = current_scope->find_binding(function_call->identifier->name);
	if (index == -1)
	{
		globals::error_handler.log_semantic_error({
			"Symbol '" + std::string(function_call->identifier->name) + "' is not defined",
			function_call->identifier->location,
			function_call->identifier->length
		}, true);
	}
	else if (!binding->is_function())
	{
		globals::error_handler.log_semantic_error({
			"Symbol '" + std::string(function_call->identifier->name) + "' is not a function",
			function_call->identifier->location,
			function_call->identifier->length
		}, true);
	}

	auto function = binding->value()->as<Function>();
	
	// Analyze the arguments
	FunctionSignature signature;
	for (auto & argument : function_call->arguments)
	{
		auto result = analyze(argument.get());
		signature.parameters.push_back({ "", result });
	}

	// Find the most specific implementation of the function for the given signature
	auto match = function->find_best_match(signature);
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
					function_call->arguments[i]->location,
					function_call->arguments[i]->length
				});
			}
		}
	}

	// Store the index of the function and the match conversions for the compiler
	function_call->function_index = index;
	function_call->match = match;
	return function->get_implementation(match->index)->return_type();
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_identifier(IdentifierNode * identifier)
{
	auto [index, binding] = current_scope->find_binding(identifier->name);
	if (index == -1)
	{
		globals::error_handler.log_semantic_error({
			"Symbol '" + std::string(identifier->name) + "' is not defined",
			identifier->location,
			identifier->length
		}, true);
	}

	identifier->binding_index = index;
	return { binding->get_class(), Type::Qualifier::REF };
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_type(TypeNode * type)
{
	auto [index, binding] = current_scope->find_binding(type->name);
	if (index == -1)
	{
		globals::error_handler.log_semantic_error({
			"Type '" + std::string(type->name) + "' is not defined",
			type->location,
			type->length
		}, true);
	}

	if (!binding->is_class())
	{
		globals::error_handler.log_semantic_error({
			"Symbol '" + std::string(type->name) + "' is not a type",
			type->location,
			type->length
		}, true);
	}

	auto cls = binding->value()->as<Class>();
	return cls;
}

SemanticAnalyzer::AnalysisResult SemanticAnalyzer::analyze_literal(LiteralNode * literal)
{
	return literal->cls;
}