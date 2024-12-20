#pragma once

#include "class/class.hpp"


struct ClassClass : public Class
{
	ClassClass(void) : Class("Class", { Builtins::mathobj_class }) {}

	ObjectPtr instantiate(const std::any & value) const override;
	ObjectPtr default_value(void) const override;
	bool can_cast_to(const ClassPtr & cls) const override;
	ObjectPtr cast(const ObjectPtr & obj) const override;
};