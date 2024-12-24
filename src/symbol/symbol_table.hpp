#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "symbol/symbol.hpp"
#include "object/object.hpp"
#include "symbol/symbol_registry.hpp"
#include "variable/variable.hpp"
#include "function/function.hpp"
#include "operator/operator.hpp"
#include "object/none_object.hpp"
#include "class/builtins.hpp"


class SymbolTable
{
	//Registry<VariablePtr> symbols;
	Registry<SymbolPtr> symbols;
	OperatorRegistry operators;

public:
	SymbolTable() = default;
	~SymbolTable() = default;


#pragma region Symbols
	template <typename T>
	std::pair<size_t, std::shared_ptr<T>> define(std::string_view name, const std::shared_ptr<T> & object)
	{
		auto [index, symbol] = symbols.find(name);
		if (index != -1)
			return { index, std::static_pointer_cast<T>(symbol) };

		return { symbols.define(name, object).first, object };
	}

	//std::pair<size_t, SymbolPtr> define(std::string_view name, const SymbolPtr & object);

	/// @brief Find the symbol in the symbol table with the given name
	/// @return A pair containing the index of the symbol in the symbol table and the symbol itself,
	/// or `{ -1, nullptr }` if the symbol is not defined
	std::pair<size_t, SymbolPtr> find(std::string_view name) const
	{ return symbols.find(name); }

	SymbolPtr get(size_t index) const
	{ return symbols[index]; }


	void init_builtin_functions(void);
	void init_builtin_classes(void)
	{
		builtins::init_builtin_classes(this);

		Object::none = std::make_shared<NoneObj>();
	}
#pragma endregion


#pragma region Operators
	OperatorRegistry & get_operators(void)
	{ return operators; }

	void init_builtin_operators(void)
	{
		operators.register_builtin_operators();
	}

	OperatorPtr find_operator(std::string_view symbol, bool is_unary = false) const
	{ return operators.find(symbol, is_unary); }
#pragma endregion
};