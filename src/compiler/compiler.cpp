#include <any>

#include "compiler/compiler.hpp"
#include "object/integer_object.hpp"
#include "object/real_object.hpp"
#include "util/globals.hpp"
#include "util/config.hpp"
#include "class/builtins.hpp"

void Compiler::compile_source(void)
{
	parser->parse_source();
	semantic_analyzer->analyze(parser->get_ast());

	auto & ast = parser->get_ast();
	for (const auto &statement : ast.statements)
	{
		compile_statement(statement.get());
	}

	if (!config::print_all && last_print != 0)
	{
		chunk.emit_at(last_print, OP_PRINT);
	}

	chunk.emit(OP_RETURN);

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	chunk.disassemble();
#endif
}

void Compiler::compile_statement(const ASTNode * statement_n)
{
	switch (statement_n->type)
	{
	case ASTNode::Type::N_VARIABLE_DECLARATION:
		{
			auto variable_declaration = static_cast<const VariableDeclarationNode *>(statement_n);
			compile_variable_declaration(variable_declaration);
		}
		break;

	case ASTNode::Type::N_EXPRESSION_STATEMENT:
		{
			auto expression_statement = static_cast<const ExpressionStatementNode *>(statement_n);
			compile_expression(expression_statement->expression.get());
			compile_print(expression_statement->print_expression);
		}
		break;

	case ASTNode::Type::N_OPERAND:
		{
			auto operand = static_cast<const OperandNode *>(statement_n);
			compile_operand(operand);
		}
		break;

	case ASTNode::Type::N_IDENTIFIER:
		{
			auto identifier = static_cast<const IdentifierNode *>(statement_n);
			compile_identifier(identifier);
		}
		break;
	
	case ASTNode::Type::N_LITERAL:
		{
			auto literal = static_cast<const LiteralNode *>(statement_n);
			compile_literal(literal);
		}
		break;

	default:
		break;
	}
}

void Compiler::compile_variable_declaration(const VariableDeclarationNode * variable_declaration)
{
	size_t index = variable_declaration->variable_index;
	if (index >= UINT8_MAX)
	{
		// We have reached the maximum number of variables
		globals::error_handler.log_compiletime_error({
			"Maximum number of variables reached",
			variable_declaration->identifier->location,
			variable_declaration->identifier->length
		}, true);
		return;
	}

	// Compile the expression if it exists
	if (variable_declaration->expression)
	{
		compile_expression(variable_declaration->expression.get());

		// Check if the expression needs to be coerced
		if (variable_declaration->needs_coercion)
		{
			chunk.emit(OP_COERCE, variable_declaration->coercion_index);
		}
		
		chunk.emit(OP_SET_VARIABLE, index); // Set the variable in the current scope to the value on the stack and print it
		compile_print(variable_declaration->print_expression);
	}
}

void Compiler::compile_expression(const ASTNode * expression_n)
{
	switch (expression_n->type)
	{
	case ASTNode::Type::N_EXPRESSION:
		{
			auto expression = static_cast<const ExpressionNode *>(expression_n);
			compile_binary(expression);
		}
		break;
	case ASTNode::Type::N_OPERAND:
		{
			auto operand = static_cast<const OperandNode *>(expression_n);
			compile_operand(operand);
		}
		break;
	case ASTNode::Type::N_FUNCTION_CALL:
		{
			auto function_call = static_cast<const FunctionCallNode *>(expression_n);
			compile_function_call(function_call);
		}
		break;
	case ASTNode::Type::N_IDENTIFIER:
		{
			auto identifier = static_cast<const IdentifierNode *>(expression_n);
			compile_identifier(identifier);
		}
		break;
	case ASTNode::Type::N_LITERAL:
		{
			auto literal = static_cast<const LiteralNode *>(expression_n);
			compile_literal(literal);
		}
		break;

	default:
		// For debugging purposes
		globals::error_handler.log_compiletime_error({
			"Unhandled expression type",
			expression_n->location,
			expression_n->length
		}, true);
	}
}

void Compiler::compile_operand(const OperandNode * operand_n)
{
	if (operand_n->op)
	{
		compile_unary(operand_n);
	}
	else
	{
		compile_expression(operand_n->primary.get());
	}
}

void Compiler::compile_operator(const OperatorNode * op, bool is_unary)
{
	// The implementation of this function will be changed:
	// The left and right operands will be compiled first, and we'll check if coercion is needed
	// Then we'll compile the operator and emit the OP_COERCE instruction if needed

	size_t index = op->operator_index;
	if (index >= UINT8_MAX)
	{
		// We have reached the maximum number of operators
		globals::error_handler.log_compiletime_error({
			"Maximum number of operators reached",
			op->location,
			op->length
		}, true);
		return;
	}
	size_t impl_index = op->match->index;
	if (impl_index >= UINT8_MAX)
	{
		// We have reached the maximum number of operator implementations
		globals::error_handler.log_compiletime_error({
			"Maximum number of operator implementations reached",
			op->location,
			op->length
		}, true);
		return;
	}
	
	chunk.emit(is_unary ? OP_CALL_UNARY : OP_CALL_BINARY, index, impl_index);
}

void Compiler::compile_unary(const OperandNode * operand_n)
{
	std::vector<std::shared_ptr<ASTNode>> operands;
	operands.push_back(std::move(operand_n->primary));

	compile_arguments(operands, operand_n->op->match);
	compile_operator(operand_n->op.get(), true);
}

void Compiler::compile_binary(const ExpressionNode * expression_n)
{
	std::vector<std::shared_ptr<ASTNode>> operands;
	operands.push_back(std::move(expression_n->left));
	operands.push_back(std::move(expression_n->right));

	compile_arguments(operands, expression_n->op->match);
	compile_operator(expression_n->op.get());
}

void Compiler::compile_function_call(const FunctionCallNode * function_call_n)
{
	size_t index = function_call_n->function_index;
	if (index >= UINT8_MAX)
	{
		// We have reached the maximum number of functions
		globals::error_handler.log_compiletime_error({
			"Maximum number of functions reached",
			function_call_n->location,
			function_call_n->length
		}, true);
		return;
	}
	size_t impl_index = function_call_n->match->index;
	if (impl_index >= UINT8_MAX)
	{
		// We have reached the maximum number of function implementations
		globals::error_handler.log_compiletime_error({
			"Maximum number of function implementations reached",
			function_call_n->location,
			function_call_n->length
		}, true);
		return;
	}

	auto & match = function_call_n->match;
	compile_arguments(function_call_n->arguments, match);
	chunk.emit(OP_CALL_FUNCTION, index, impl_index);
}

void Compiler::compile_arguments(const std::vector<std::shared_ptr<ASTNode>> & arguments_n,
								 const FunctionImplementationRegistry::MatchPtr & match)
{
	for (int i = arguments_n.size() - 1; i >= 0; --i)
	{
		auto coercion = match->conversion.conversions[i];
		auto effective_match_level = coercion->effective_match_level;
		// Check if the argument needs to be coerced
		get_ref = effective_match_level == TypeCoercion::MatchLevel::REF ||
				  effective_match_level == TypeCoercion::MatchLevel::REF_INHERITANCE;

		compile_expression(arguments_n[i].get());
		// No else statement because we want to coerce the argument even if it is a reference
		if (effective_match_level != TypeCoercion::MatchLevel::EXACT &&
			effective_match_level != TypeCoercion::MatchLevel::REF)
		{
			size_t coercion_index = TypeCoercion::instance().cache_path(coercion);
			chunk.emit(OP_COERCE, coercion_index);
		}
	}
}

void Compiler::compile_identifier(const IdentifierNode * identifier_n)
{
	size_t index = identifier_n->variable_index;
	if (index >= UINT8_MAX)
	{
		// We have reached the maximum number of variables
		globals::error_handler.log_compiletime_error({
			"Maximum number of variables reached",
			identifier_n->location,
			identifier_n->length
		}, true);
		return;
	}

	if (get_ref)
	{
		chunk.emit(OP_GET_REF, index);
		get_ref = false;
	}
	else
	{
		chunk.emit(OP_GET_VARIABLE, index);
	}
}

void Compiler::compile_literal(const LiteralNode * literal_n)
{
	if (constant_pool.size() >= UINT8_MAX)
	{
		// We have reached the maximum number of constants
		globals::error_handler.log_compiletime_error({
			"Maximum number of constants reached",
			literal_n->location,
			literal_n->length
		}, true);
		return;
	}

	try
	{
		auto value = literal_n->cls->instantiate(std::any(literal_n->value));
		constant_pool.add_constant(value);
		chunk.emit_constant(constant_pool.size() - 1);
	}
	catch (const std::exception & e)
	{
		// For debugging purposes
		globals::error_handler.log_compiletime_error({
			"Exception occurred while compiling literal: " + std::string(e.what()),
			literal_n->location,
			literal_n->length
		}, true);
	}
}

void Compiler::compile_print(bool print)
{
	if (print)
	{
		if (config::print_all)
		{
			chunk.emit(OP_PRINT);
		}
		else
		{
			// Print only the last expression if the print_all flag is not set
			last_print = chunk.code.size();
			chunk.emit(OP_POP);
		}
	}
	else
	{
		chunk.emit(OP_POP);
	}
}