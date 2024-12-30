#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "binding/binding_table.hpp"


struct Scope
{
	Scope(std::shared_ptr<Scope> parent = nullptr) : parent(parent) {}
	~Scope() = default;

	void init_global_scope(void)
	{
		bindings.init_builtin_classes();
		bindings.init_builtin_operators();
		bindings.init_builtin_functions();
	}

	#pragma region bindings
	/// @brief Find the binding in the current scope or any of its parents
	/// @return A pair containing the index of the binding in the bindings table and the binding itself,
	/// or `{ -1, nullptr }` if the binding is not defined
	std::pair<size_t, BindingPtr> find_binding(std::string_view name) const;

	BindingPtr get_binding(size_t index) const
	{ return bindings.get(index); }

	bool is_binding_defined(std::string_view name) const
	{ return bindings.find(name).first != -1; }

	std::pair<size_t, BindingPtr> define_binding(std::string_view name, ClassPtr cls);
	std::pair<size_t, BindingPtr> define_binding(std::string_view name, ObjectPtr value = Object::none);
	#pragma endregion


	#pragma region Operators
	OperatorRegistry & get_operators(void)
	{ return bindings.get_operators(); }
	#pragma endregion

private:
	std::shared_ptr<Scope> parent;
	BindingTable bindings;
};