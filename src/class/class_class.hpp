#pragma once

#include "class/class.hpp"


struct ClassClass : public Class
{
	ClassClass(void) : Class("Class", { builtins::mathobj_class }) {}

	ObjectPtr instantiate(const std::any & value) const override;
	ObjectPtr default_value(void) const override;
};