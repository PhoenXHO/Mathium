#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include "variable/lookup_table.hpp"


struct Scope
{
	Scope(std::shared_ptr<Scope> parent = nullptr) : parent(parent) {}
	~Scope() = default;

	void init_global_scope(void)
	{
		lookup_table.init_builtin_classes();
		lookup_table.init_builtin_operators();
		lookup_table.init_builtin_functions();
	}

#pragma region Variables
	/// @brief Find the variable in the current scope or any of its parents
	/// @return A pair containing the index of the variable in the variables table and the variable itself,
	/// or `{ -1, nullptr }` if the variable is not defined
	std::pair<size_t, VariablePtr> find_variable(std::string_view name) const;

	VariablePtr get_variable(size_t index) const
	{ return lookup_table.get(index); }

	bool is_variable_defined(std::string_view name) const
	{ return lookup_table.find(name).first != -1; }

	std::pair<size_t, VariablePtr> define_variable(std::string_view name, ClassPtr cls);
	std::pair<size_t, VariablePtr> define_variable(std::string_view name, ObjectPtr value = Object::none);
#pragma endregion


#pragma region Operators
	/// @brief Find the operator in the current scope or any of its parents
	/// @return A pair containing the index of the operator in the operators table and the operator itself,
	/// or `{ -1, nullptr }` if the operator is not defined
	std::pair<size_t, OperatorPtr> find_operator(std::string_view symbol, bool is_unary) const;

	OperatorPtr get_operator(size_t index, bool is_unary = false) const
	{ return lookup_table.get_operator(index, is_unary); }

	OperatorRegistry & get_operators(void)
	{ return lookup_table.get_operators(); }
#pragma endregion

private:
	std::shared_ptr<Scope> parent;
	LookupTable lookup_table;
};