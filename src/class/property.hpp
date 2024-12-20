#pragma once

#include <memory>
#include <string_view>


struct Variable;
using VariablePtr = std::shared_ptr<Variable>;
struct Property;
using PropertyPtr = std::shared_ptr<Property>;
class Object;
using ObjectPtr = std::shared_ptr<Object>;


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