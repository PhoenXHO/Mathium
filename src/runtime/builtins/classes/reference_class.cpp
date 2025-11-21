#include "runtime/core/class/builtin_class_init.hpp"
#include "runtime/builtins/classes/reference_class.hpp"


ObjectPtr ReferenceClass::instantiate(const std::any & value) const
{
	return Object::none;
}

ObjectPtr ReferenceClass::default_value(void) const
{
	return Object::none;
}