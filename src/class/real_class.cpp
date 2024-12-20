#include "class/builtins.hpp"
#include "class/real_class.hpp"
#include "object/real_object.hpp"


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