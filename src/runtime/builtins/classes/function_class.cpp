#include "runtime/builtins/classes/function_class.hpp"


ObjectPtr FunctionClass::instantiate(const std::any & value) const
{
	return std::make_shared<FunctionClass>();
}

ObjectPtr FunctionClass::default_value(void) const
{
	return std::make_shared<FunctionClass>();
}