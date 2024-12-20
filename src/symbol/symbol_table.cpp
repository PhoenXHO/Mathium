#include "symbol/symbol_table.hpp"


std::pair<size_t, VariablePtr> SymbolTable::define(std::string_view name, const ObjectPtr & object)
{
	VariablePtr variable;
	if (object->get_class() == Builtins::reference_class)
	{
		variable = std::static_pointer_cast<Variable>(object);
	}
	else
	{
		variable = std::make_shared<Variable>(name, object);
	}
	return symbols.define(name, variable);
}