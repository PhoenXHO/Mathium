#include "runtime/class/builtins.hpp"
#include "runtime/class/integer_class.hpp"
#include "runtime/object/integer_object.hpp"


ObjectPtr IntegerClass::instantiate(const std::any & value) const
{
	if (value.has_value())
	{
		return std::make_shared<IntegerObj>(std::any_cast<std::string_view>(value));
	}
	return std::make_shared<IntegerObj>();
}

ObjectPtr IntegerClass::default_value(void) const
{
	return std::make_shared<IntegerObj>();
}