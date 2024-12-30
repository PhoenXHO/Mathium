#pragma once

#include "class/class.hpp"


struct FunctionClass : public Class
{
	FunctionClass(void) : Class("Function", { builtins::mathobj_class }, true) {}

	ObjectPtr instantiate(const std::any & value) const override;
	ObjectPtr default_value(void) const override;
};