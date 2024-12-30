#include "scope/scope.hpp"


std::pair<size_t, BindingPtr> Scope::find_binding(std::string_view name) const
{
	auto [index, binding] = bindings.find(name);
	if (index != -1)
		return { index, binding };

	// If the binding is not found in the current scope, check the parent scope
	if (parent)
		return parent->find_binding(name);

	return { -1, nullptr };
}

std::pair<size_t, BindingPtr> Scope::define_binding(std::string_view name, ClassPtr cls)
{
	return bindings.define_from_class(name, cls);
}

std::pair<size_t, BindingPtr> Scope::define_binding(std::string_view name, ObjectPtr value)
{
	return bindings.define(name, value);
}