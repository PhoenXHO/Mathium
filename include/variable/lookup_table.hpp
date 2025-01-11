#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "variable/variable.hpp"
#include "object/object.hpp"
#include "variable/registry.hpp"
#include "function/function.hpp"
#include "operator/operator.hpp"
#include "object/none_object.hpp"
#include "class/builtins.hpp"


class LookupTable
{
	Registry<VariablePtr> bindings;
	OperatorRegistry operators;

public:
	LookupTable() = default;
	~LookupTable() = default;


#pragma region Bindings
	std::pair<size_t, VariablePtr> define(std::string_view name, const ObjectPtr & object)
	{
		auto [index, binding] = bindings.find(name);
		if (index != -1)
			return { index, binding };

		binding = std::make_shared<Variable>(name, object);
		return { bindings.define(binding->name(), binding).first, binding };
	}

	std::pair<size_t, VariablePtr> define_from_class(std::string_view name, ClassPtr cls);

	/// @brief Find the binding in the binding table with the given name
	/// @return A pair containing the index of the binding in the binding table and the binding itself,
	/// or `{ -1, nullptr }` if the binding is not defined
	std::pair<size_t, VariablePtr> find(std::string_view name) const
	{ return bindings.find(name); }

	VariablePtr get(size_t index) const
	{ return bindings[index]; }


	void init_builtin_functions(void);
	void init_builtin_classes(void)
	{
		builtins::init_builtin_classes(this);
		auto b = bindings.find("Real");

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

	std::pair<size_t, OperatorPtr> find_operator(std::string_view op_symbol, bool is_unary = false) const
	{ return operators.find(op_symbol, is_unary); }

	OperatorPtr get_operator(size_t index, bool is_unary) const
	{ return operators.find(index, is_unary); }
#pragma endregion
};