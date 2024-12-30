#include "type/type.hpp"
#include "class/class.hpp"


std::string Type::to_string(void) const
{
	std::string str = cls->name();
	if (cls == builtins::none_class)
		return str;
	if (qualifier == Qualifier::CONST)
		str = "const " + str;
	if (qualifier == Qualifier::REF)
		str = "ref " + str;
	return str;
}