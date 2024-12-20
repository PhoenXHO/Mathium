#pragma once

#include "class/class.hpp"


struct FunctionClass : public Class
{
	FunctionClass(void) : Class("Function", { Builtins::mathobj_class }) {}

	ObjectPtr instantiate(const std::any & value) const override;
	ObjectPtr default_value(void) const override;
	bool can_cast_to(const ClassPtr & cls) const override;
	ObjectPtr cast(const ObjectPtr & obj) const override;
};