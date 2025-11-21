#pragma once

#include <memory>

#include "frontend/parser/ast.hpp"
#include "runtime/core/object/object.hpp"
#include "semantic/scope/scope.hpp"
#include "runtime/core/class/class.hpp"
#include "semantic/type/type.hpp"
#include "runtime/core/function/function.hpp"


// This class will be responsible for type checking and other semantic analysis tasks
class SemanticAnalyzer
{
	using AnalysisResult = Type;

	std::shared_ptr<Scope> global_scope;
	std::shared_ptr<Scope> current_scope;

public:
	SemanticAnalyzer(std::shared_ptr<Scope> global_scope) :
		global_scope(global_scope),
		current_scope(global_scope)
	{}
	~SemanticAnalyzer() = default;

	void reset(void)
	{
		current_scope = global_scope;
	}

	void analyze(const AST & ast);

private:
	AnalysisResult analyze                      (ASTNode * node)                                 ;
	AnalysisResult analyze_variable_declaration (VariableDeclarationNode * variable_declaration) ;
	AnalysisResult analyze_expression           (ExpressionNode * expression)                    ;
	AnalysisResult analyze_operand              (OperandNode * operand)                          ;
	AnalysisResult analyze_function_call        (FunctionCallNode * function_call)               ;
	AnalysisResult analyze_identifier           (IdentifierNode * identifier)                    ;
	AnalysisResult analyze_type                 (TypeNode * type)                                ;
	AnalysisResult analyze_literal              (LiteralNode * literal)                          ;

	FunctionImplementationRegistry::MatchPtr resolve_overload(
		const FunctionImplementationRegistry & implementations,
		const FunctionPtr & function,
		const FunctionSignature & signature,
		const std::vector<std::shared_ptr<ASTNode>> & argument_nodes // For error reporting
	);

	TypeCoercion::CoercionPathPtr check_type_compatibility(
		const Type & expected,
		const Type & actual,
		const ASTNode * node // For error reporting
	);

	std::pair<size_t, VariablePtr> find_variable_in_current_scope(
		std::string_view name,
		std::string_view error_message,
		const ASTNode * node
	);
};