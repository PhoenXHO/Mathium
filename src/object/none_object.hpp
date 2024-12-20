#pragma once

#include "object/object.hpp"
#include "class/builtins.hpp"


class NoneObj : public Object
{
public:
	NoneObj() : Object(builtins::none_class) {}
	~NoneObj() = default;

	std::string to_string(void) const override
	{ return "none"; }
};