#include "scope/scope.hpp"


std::pair<size_t, SymbolPtr> Scope::find_symbol(std::string_view name) const
{
	auto [index, symbol] = symbols.find(name);
	if (index != -1)
		return { index, symbol };

	// If the symbol is not found in the current scope, check the parent scope
	if (parent)
		return parent->find_symbol(name);

	return { -1, nullptr };
}

std::pair<size_t, SymbolPtr> Scope::define_variable(std::string_view name, ClassPtr cls)
{
	VariablePtr variable = std::make_shared<Variable>(name, cls);
	return symbols.define(name, variable);
}

std::pair<size_t, SymbolPtr> Scope::define_variable(std::string_view name, ObjectPtr value)
{
	VariablePtr variable = std::make_shared<Variable>(name, value);
	return symbols.define(name, variable);
}