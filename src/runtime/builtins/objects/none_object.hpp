#pragma once

#include "runtime/core/object/object.hpp"
#include "runtime/core/class/builtin_class_init.hpp"


class NoneObj : public Object
{
public:
	NoneObj() : Object(builtins::none_class) {}
	~NoneObj() = default;

	std::string to_string(void) const override
	{ return "none"; }
};