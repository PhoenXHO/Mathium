#pragma once

#include <memory>
#include <vector>
#include <string_view>
#include <iostream>

#include "operator/operator.hpp"
#include "object/object.hpp"
#include "util/util.hpp"
#include "class/class.hpp"
#include "type/type.hpp"
#include "function/function.hpp"


struct AST;
struct ASTNode;
struct VariableDeclarationNode;
struct ExpressionStatementNode;
struct ExpressionNode;
struct OperandNode;
struct OperatorNode;
struct FunctionCallNode;
struct IdentifierNode;
struct TypeNode;
struct LiteralNode;


// <program> ::= <statement>*
// <statement> ::= <expression-statement> (for now)
struct AST
{
	std::vector<std::shared_ptr<ASTNode>> statements;

	AST() = default;
	~AST() = default;
	
	//* Debugging
	friend std::ostream & operator<<(std::ostream & os, const AST & ast);
};

struct ASTNode
{
	enum class Type
	{
		N_STATEMENT,
		N_VARIABLE_DECLARATION,
		N_EXPRESSION_STATEMENT,
		N_EXPRESSION,
		N_OPERAND,
		N_OPERATOR,
		N_FUNCTION_CALL,
		N_IDENTIFIER,
		N_TYPE,
		N_LITERAL
	};

	const Type type;
	SourceLocation location;
	size_t length;

	ASTNode(Type type) : type(type) {}
	virtual ~ASTNode() = default;

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	virtual void print(int depth = 0) const = 0;
#endif
};

// <variable-declaration> ::= "let" [ <type> ] <identifier> [ ":=" <expression> ] [ ";" ]
struct VariableDeclarationNode : public ASTNode
{
	std::shared_ptr<TypeNode> type;
	std::shared_ptr<IdentifierNode> identifier;
	std::shared_ptr<ASTNode> expression;

	bool needs_coercion = false;
	size_t coercion_index = 0;

	bool print_expression = false;
	size_t variable_index = 0;

	VariableDeclarationNode() : ASTNode(Type::N_VARIABLE_DECLARATION) {}
	~VariableDeclarationNode() = default;

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	void print(int depth = 0) const override;
#endif
};

// <expression-statement> ::= ( <expression> | <operand> ) [ ";" ]
struct ExpressionStatementNode : public ASTNode
{
	std::shared_ptr<ASTNode> expression;
	bool print_expression = false;

	ExpressionStatementNode() : ASTNode(Type::N_EXPRESSION_STATEMENT) {}
	ExpressionStatementNode(std::shared_ptr<ASTNode> expression)
		: ASTNode(Type::N_EXPRESSION_STATEMENT), expression(std::move(expression))
	{}
	~ExpressionStatementNode() = default;

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	void print(int depth = 0) const override;
#endif
};

// <expression> ::= ( <operand> | <expression> ) <operator> ( <operand> | <expression> )
struct ExpressionNode : public ASTNode
{
	std::shared_ptr<ASTNode> left;
	std::shared_ptr<ASTNode> right;
	std::shared_ptr<OperatorNode> op;

	ExpressionNode() : ASTNode(Type::N_EXPRESSION) {}
	ExpressionNode(
		std::shared_ptr<ASTNode> left,
		std::shared_ptr<OperatorNode> op,
		std::shared_ptr<ASTNode> right
	) : ASTNode(Type::N_EXPRESSION), left(std::move(left)), op(std::move(op)), right(std::move(right))
		{}
	~ExpressionNode() = default;

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	void print(int depth = 0) const override;
#endif
};

// <operand> ::= <operator> <primary>
// <primary> ::= <literal> | <operand> | <expression>
struct OperandNode : public ASTNode
{
	std::shared_ptr<OperatorNode> op;
	std::shared_ptr<ASTNode> primary;

	OperandNode() : ASTNode(Type::N_OPERAND) {}
	~OperandNode() = default;

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	void print(int depth = 0) const override;
#endif
};

// <operator> ::= <custom-operator> | <built-in-operator>
struct OperatorNode : public ASTNode
{
	OperatorPtr op;
	FunctionImplementationRegistry::MatchPtr impl; // To store the implementation of the operator for the given operands (for the compiler)
	size_t operator_index = 0;
	FunctionImplementationRegistry::MatchPtr match;

	OperatorNode(OperatorPtr op) : ASTNode(Type::N_OPERATOR), op(op) {}
	~OperatorNode() = default;

	Fixity fixity(void) const
	{ return op->fixity(); }
	Associativity associativity(void) const
	{ return op->associativity(); }
	Precedence precedence(void) const
	{ return op->precedence(); }
	FunctionImplementationRegistry & implementations(void)
	{ return op->implementations(); }

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	void print(int depth = 0) const override;
#endif
};

// <function-call> ::= <identifier> "(" [ <expression> [ "," <expression> ]* ] ")"
struct FunctionCallNode : public ASTNode
{
	std::shared_ptr<IdentifierNode> identifier;
	std::vector<std::shared_ptr<ASTNode>> arguments;
	size_t function_index = 0;
	FunctionImplementationRegistry::MatchPtr match;

	FunctionCallNode() : ASTNode(Type::N_FUNCTION_CALL) {}
	~FunctionCallNode() = default;

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	void print(int depth = 0) const override;
#endif
};

// <identifier> ::= <identifier>
struct IdentifierNode : public ASTNode
{
	std::string_view name;
	size_t variable_index = 0;

	IdentifierNode() : ASTNode(Type::N_IDENTIFIER) {}
	IdentifierNode(std::string_view name) : ASTNode(Type::N_IDENTIFIER), name(name) {}
	~IdentifierNode() = default;

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	void print(int depth = 0) const override;
#endif
};

// <type> ::= <identifier>
struct TypeNode : public ASTNode
{
	std::string_view name;

	TypeNode() : ASTNode(Type::N_TYPE) {}
	TypeNode(std::string_view name) : ASTNode(Type::N_TYPE), name(name) {}
	~TypeNode() = default;

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	void print(int depth = 0) const override;
#endif
};

// <literal> ::= <integer> | <real> (for now)
struct LiteralNode : public ASTNode
{
	ClassPtr cls;
	std::string_view value;

	LiteralNode() : ASTNode(Type::N_LITERAL) {}
	LiteralNode(std::string_view value) : ASTNode(Type::N_LITERAL), value(value) {}
	~LiteralNode() = default;

	//* Debugging
#ifdef MATHIUM_DEV_MODE
	void print(int depth = 0) const override;
#endif
};