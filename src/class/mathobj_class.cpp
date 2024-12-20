#include "class/builtins.hpp"
#include "class/mathobj_class.hpp"


ObjectPtr MathObjClass::instantiate(const std::any & value) const
{
	return Object::none;
}

ObjectPtr MathObjClass::default_value(void) const
{
	return Object::none;
}