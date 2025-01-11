#pragma once

#include "class/class.hpp"


struct ReferenceClass : public Class
{
	ReferenceClass(void) : Class("Reference", { builtins::mathobj_class }, true) {}

	ObjectPtr instantiate(const std::any & value) const override;
	ObjectPtr default_value(void) const override;

	std::string to_string(void) const override
	{
		std::ostringstream oss;
		oss << "<Reference>";
		return oss.str();
	}
};