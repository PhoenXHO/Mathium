#pragma once

#include <string>

#include "class/class.hpp"
#include "class/builtins.hpp"


struct Type
{
	ClassPtr cls;
	bool is_const;
	bool is_ref;

	Type(ClassPtr cls, bool is_const = true, bool is_ref = false)
		: cls(cls)
		, is_const(is_const)
		, is_ref(is_ref) {}

	std::string to_string(void) const
	{
		std::string str = cls->name();
		if (cls == builtins::none_class)
			return str;
		if (is_const)
			str = "const " + str;
		if (is_ref)
			str = "ref " + str;
		return str;
	}
};