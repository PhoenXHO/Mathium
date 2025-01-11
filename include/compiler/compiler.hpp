#pragma once

#include <string_view>
#include <memory>
#include <vector>

#include "parser/parser.hpp"
#include "semantic_analyzer/semantic_analyzer.hpp"
#include "compiler/chunk.hpp"
#include "parser/ast.hpp"
#include "scope/scope.hpp"

class Compiler
{
	std::unique_ptr<Parser> parser;
	std::unique_ptr<SemanticAnalyzer> semantic_analyzer;
	Chunk & chunk; // Reference to the chunk in the VM
	ConstantPool & constant_pool; // Reference to the constant pool in the VM
	std::shared_ptr<Scope> global_scope;
	std::shared_ptr<Scope> current_scope = global_scope;
	std::vector<std::shared_ptr<OperatorImplentation>> operator_stack;
	size_t last_print = 0;
	bool get_ref = false; // Flag to indicate that the next `OP_GET_SYMBOL` instruction should get a reference

	void compile_statement            (const ASTNode *)                                  ;
	void compile_variable_declaration (const VariableDeclarationNode *)                  ;
	void compile_expression           (const ASTNode *)                                  ;
	void compile_operand              (const OperandNode *)                              ;
	void compile_operator             (const OperatorNode *, bool is_unary = false)      ;
	void compile_unary                (const OperandNode *)                              ;
	void compile_binary               (const ExpressionNode *)                           ;
	void compile_function_call        (const FunctionCallNode *)                         ;
	void compile_arguments			  (const std::vector<std::shared_ptr<ASTNode>> &,
	                                   const FunctionImplementationRegistry::MatchPtr &) ;
	void compile_identifier           (const IdentifierNode *)                           ;
	void compile_literal              (const LiteralNode *)                              ;
	void compile_print                (bool print)                                       ;

public:
	Compiler(Chunk & chunk, ConstantPool & constant_pool, std::shared_ptr<Scope> global_scope) :
		chunk(chunk),
		parser(std::make_unique<Parser>(global_scope->get_operators())),
		constant_pool(constant_pool),
		global_scope(global_scope),
		semantic_analyzer(std::make_unique<SemanticAnalyzer>(global_scope))
	{}
	~Compiler() = default;

	void reset(void)
	{
		parser->reset();
		chunk.clear_code();
		semantic_analyzer->reset();
		last_print = 0;
	}

	void compile_source(void);

	const std::shared_ptr<OperatorImplentation> & get_operator(size_t index) const
	{ return operator_stack.at(index); }
};