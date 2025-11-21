#pragma once

#include <memory>
#include <string_view>

#include "util/forward.hpp"


struct Property
{
	VariablePtr variable;
	bool is_static;
	//bool is_private;
	//bool is_protected;

	std::string_view name(void) const;
	void set(const ObjectPtr & value);
};
using PropertyPtr = std::shared_ptr<Property>;