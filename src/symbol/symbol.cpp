#include "symbol/symbol.hpp"
#include "object/object.hpp"
#include "class/class.hpp"
#include "variable/variable.hpp"
#include "function/function.hpp"


ObjectPtr Symbol::get_symbol_value(SymbolPtr symbol)
{
	switch (symbol->get_symbol_type())
	{
	case Symbol::Type::S_CLASS:
		return std::dynamic_pointer_cast<Class>(symbol);
	case Symbol::Type::S_FUNCTION:
		return std::dynamic_pointer_cast<Function>(symbol);
	case Symbol::Type::S_VARIABLE:
		return std::dynamic_pointer_cast<Variable>(symbol)->value();
	default:
		return Object::none;
	}
}