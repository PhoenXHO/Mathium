#pragma once

#include "runtime/core/class/class.hpp"

struct NoneClass : public Class
{
	NoneClass(void) : Class("None", { builtins::mathobj_class }, true) {}

	ObjectPtr instantiate(const std::any & value) const override;
	ObjectPtr default_value(void) const override;
};