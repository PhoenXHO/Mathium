#include "class/builtins.hpp"
#include "class/reference_class.hpp"


ObjectPtr ReferenceClass::instantiate(const std::any & value) const
{
	return Object::none;
}

ObjectPtr ReferenceClass::default_value(void) const
{
	return Object::none;
}