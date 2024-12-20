#include "class/function_class.hpp"


ObjectPtr FunctionClass::instantiate(const std::any & value) const
{
	return std::make_shared<FunctionClass>();
}

ObjectPtr FunctionClass::default_value(void) const
{
	return std::make_shared<FunctionClass>();
}

bool FunctionClass::can_cast_to(const ClassPtr & cls) const
{
	return cls->name() == "Function";
}

ObjectPtr FunctionClass::cast(const ObjectPtr & obj) const
{
	return std::make_shared<FunctionClass>();
}