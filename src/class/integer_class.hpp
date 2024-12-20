#pragma once

#include "class/class.hpp"

struct IntegerClass : public Class
{
	IntegerClass(void) : Class("Integer", { builtins::mathobj_class }) {}

	ObjectPtr instantiate(const std::any & value) const override;
	ObjectPtr default_value(void) const override;
};