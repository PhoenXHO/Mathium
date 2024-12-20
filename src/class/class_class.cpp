#include "class/class_class.hpp"


ObjectPtr ClassClass::instantiate(const std::any & value) const
{
	return std::make_shared<ClassClass>();
}

ObjectPtr ClassClass::default_value(void) const
{
	return std::make_shared<ClassClass>();
}

bool ClassClass::can_cast_to(const ClassPtr & cls) const
{
	return cls->name() == "Class";
}

ObjectPtr ClassClass::cast(const ObjectPtr & obj) const
{
	return std::make_shared<ClassClass>();
}