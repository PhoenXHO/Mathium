#pragma once

#include <memory>

#include "parser/ast.hpp"
#include "object/object.hpp"
#include "scope/scope.hpp"
#include "class/class.hpp"
#include "type/type.hpp"


// This class will be responsible for type checking and other semantic analysis tasks
class SemanticAnalyzer
{
	using AnalysisResult = Type;

	std::shared_ptr<Scope> global_scope;
	std::shared_ptr<Scope> current_scope = global_scope;

public:
	SemanticAnalyzer(std::shared_ptr<Scope> global_scope) : global_scope(global_scope) {}
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
};