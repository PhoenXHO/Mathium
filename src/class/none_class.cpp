#include "class/builtins.hpp"
#include "class/none_class.hpp"


ObjectPtr NoneClass::instantiate(const std::any & value) const
{
	return Object::none;
}

ObjectPtr NoneClass::default_value(void) const
{
	return Object::none;
}