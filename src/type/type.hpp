#pragma once

#include <string>

#include "util/forward.hpp"
#include "class/builtins.hpp"


struct Type
{
	enum class Qualifier
	{
		CONST,
		REF,
		CONST_REF,
		NONE
	};

	ClassPtr cls;
	Qualifier qualifier;

	Type(ClassPtr cls, Qualifier qualifier = Qualifier::NONE)
		: cls(cls)
		, qualifier(qualifier) {}

	std::string to_string(void) const;


	static Qualifier bool_to_qualifier(bool is_const)
	{
		return is_const ? Qualifier::CONST : Qualifier::NONE;
	}

	bool operator==(const Type & other) const
	{
		return cls == other.cls && qualifier == other.qualifier;
	}
};