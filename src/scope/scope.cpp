#include "scope/scope.hpp"


std::pair<size_t, VariablePtr> Scope::find_variable(std::string_view name) const
{
	auto [index, binding] = lookup_table.find(name);
	if (index != -1)
		return { index, binding };

	// If the binding is not found in the current scope, check the parent scope
	if (parent)
		return parent->find_variable(name);

	return { -1, nullptr };
}

std::pair<size_t, VariablePtr> Scope::define_variable(std::string_view name, ClassPtr cls)
{
	return lookup_table.define_from_class(name, cls);
}

std::pair<size_t, VariablePtr> Scope::define_variable(std::string_view name, ObjectPtr value)
{
	return lookup_table.define(name, value);
}


std::pair<size_t, OperatorPtr> Scope::find_operator(std::string_view symbol, bool is_unary) const
{
	auto [index, op] = lookup_table.find_operator(symbol);
	if (index != -1)
		return { index, op };

	// If the binding is not found in the current scope, check the parent scope
	if (parent)
		return parent->find_operator(symbol, is_unary);

	return { -1, nullptr };
}