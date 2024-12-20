#pragma once

#include "class/class.hpp"


struct RealClass : public Class
{
	RealClass(void) : Class("Real", { builtins::mathobj_class }) {}

	ObjectPtr instantiate(const std::any & value) const override;
	ObjectPtr default_value(void) const override;
};