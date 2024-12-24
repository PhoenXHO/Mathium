#pragma once

#include "util/forward.hpp"


struct Type
{
	ClassPtr cls;
	bool is_const;
	bool is_ref;

	Type(ClassPtr cls, bool is_const = true, bool is_ref = false)
		: cls(cls)
		, is_const(is_const)
		, is_ref(is_ref) {}
};