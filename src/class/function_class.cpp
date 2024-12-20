#include "class/function_class.hpp"


ObjectPtr FunctionClass::instantiate(const std::any & value) const
{
	return std::make_shared<FunctionClass>();
}

ObjectPtr FunctionClass::default_value(void) const
{
	return std::make_shared<FunctionClass>();
}