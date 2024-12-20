#pragma once

#include "class/class.hpp"

struct MathObjClass : public Class
{
	MathObjClass(void) : Class("MathObj") {}

	ObjectPtr instantiate(const std::any & value) const override;
	ObjectPtr default_value(void) const override;
};