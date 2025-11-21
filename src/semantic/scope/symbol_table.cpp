#include "semantic/scope/symbol_table.hpp"
#include "runtime/core/class/class.hpp"


std::pair<size_t, VariablePtr> LookupTable::define_from_class(std::string_view name, ClassPtr cls)
{
	return define(name, cls->default_value());
}