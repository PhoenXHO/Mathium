#include "scope/scope.hpp"


std::pair<size_t, VariablePtr> Scope::find_variable(std::string_view name) const
{
	auto [index, binding] = variables.find(name);
	if (index != -1)
		return { index, binding };

	// If the binding is not found in the current scope, check the parent scope
	if (parent)
		return parent->find_variable(name);

	return { -1, nullptr };
}

std::pair<size_t, VariablePtr> Scope::define_variable(std::string_view name, ClassPtr cls)
{
	return variables.define_from_class(name, cls);
}

std::pair<size_t, VariablePtr> Scope::define_variable(std::string_view name, ObjectPtr value)
{
	return variables.define(name, value);
}