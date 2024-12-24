#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "symbol/symbol_table.hpp"


struct Scope
{
	Scope(std::shared_ptr<Scope> parent = nullptr) : parent(parent) {}
	~Scope() = default;

	void init_global_scope(void)
	{
		symbols.init_builtin_classes();
		symbols.init_builtin_operators();
		symbols.init_builtin_functions();
	}

	#pragma region Symbols
	/// @brief Find the symbol in the current scope or any of its parents
	/// @return A pair containing the index of the symbol in the symbol table and the symbol itself,
	/// or `{ -1, nullptr }` if the symbol is not defined
	std::pair<size_t, SymbolPtr> find_symbol(std::string_view name) const;

	SymbolPtr get_symbol(size_t index) const
	{ return symbols.get(index); }

	bool is_symbol_defined(std::string_view name) const
	{ return symbols.find(name).first != -1; }

	std::pair<size_t, SymbolPtr> define_variable(std::string_view name, ClassPtr cls);
	std::pair<size_t, SymbolPtr> define_variable(std::string_view name, ObjectPtr value = Object::none);
	#pragma endregion


	#pragma region Operators
	OperatorRegistry & get_operators(void)
	{ return symbols.get_operators(); }
	#pragma endregion

private:
	std::shared_ptr<Scope> parent;
	SymbolTable symbols;
};