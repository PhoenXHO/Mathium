#include "runtime/core/class/builtin_class_init.hpp"
#include "runtime/builtins/classes/none_class.hpp"


ObjectPtr NoneClass::instantiate(const std::any & value) const
{
	return Object::none;
}

ObjectPtr NoneClass::default_value(void) const
{
	return Object::none;
}