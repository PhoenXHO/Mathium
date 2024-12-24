#include "class/class.hpp"
#include "class/property.hpp"
#include "type/type_coercion.hpp"


bool Class::can_cast_to(const ClassPtr & target)
{
	ClassPtr cls = std::dynamic_pointer_cast<Class>(shared_from_this());

	// Check if the class is the same
	if (cls == target)
		return true;

	// Check if the class is a subclass
	if (is_sub_class(target))
		return true;

	// Check if the class can be coerced to the target class
	return TypeCoercion::instance().can_coerce(cls, target);
}

ObjectPtr Class::cast(const ObjectPtr & obj)
{
	ClassPtr target = std::dynamic_pointer_cast<Class>(shared_from_this());

	// Same type - no conversion needed
	if (obj->get_class() == target)
		return obj;

	// Check if the object can be coerced to the target class
	if (TypeCoercion::instance().can_coerce(obj->get_class(), target))
		return TypeCoercion::instance().coerce(obj, target);

	return Object::none;
}

bool Class::is_sub_class(const ClassPtr & cls, bool strict) const
{
	// Check if the class is the same
	if (!strict && shared_from_this() == cls)
		return true;

	// Check if the class is a subclass
	for (const auto & base : bases)
	{
		if (base->can_cast_to(cls))
			return true;
	}

	return false;
}

int Class::measure_specificity(const ClassPtr & target)
{
	// Check if the current class is the same as the given class
	if (shared_from_this() == target)
		return 2;

	// Check if one class is a reference to the other
	//if 

	// Check if this class is castable to the given class
	if (can_cast_to(target))
		return 1;

	return 0;
}

std::string_view Property::name(void) const
{ return variable->name(); }

void Property::set(const ObjectPtr & value)
{
	// For now, only allow setting the value of the property
	variable->set(value);
}