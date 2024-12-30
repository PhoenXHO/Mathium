#include "binding/binding_table.hpp"
#include "class/class.hpp"


std::pair<size_t, BindingPtr> BindingTable::define_from_class(std::string_view name, ClassPtr cls)
{
	return define(name, cls->default_value());
}