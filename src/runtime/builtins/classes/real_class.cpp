#include "runtime/core/class/builtin_class_init.hpp"
#include "runtime/builtins/classes/real_class.hpp"
#include "runtime/builtins/objects/real_object.hpp"


ObjectPtr RealClass::instantiate(const std::any & value) const
{
	if (value.has_value())
	{
		return std::make_shared<RealObj>(std::any_cast<std::string_view>(value));
	}
	return std::make_shared<RealObj>();
}

ObjectPtr RealClass::default_value(void) const
{
	return std::make_shared<RealObj>();
}