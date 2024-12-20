#include "type/type.hpp"
#include "object/object.hpp"
#include "class/class.hpp"


void TypeCoercion::add_rule(ClassPtr from, ClassPtr to, CoercionFunction converter, int cost)
{
	rules[{ from, to }] = { converter, cost };
}

ObjectPtr TypeCoercion::coerce(const ObjectPtr & obj, ClassPtr to) const
{
	auto rule = rules.find({ obj->get_class(), to });
	if (rule != rules.end())
	{
		return rule->second.converter(obj);
	}

	return nullptr;
}

bool TypeCoercion::can_coerce(const ClassPtr & from, ClassPtr to) const
{
	return rules.find({ from, to }) != rules.end();
}

bool TypeCoercion::can_coerce(const ObjectPtr & obj, ClassPtr to) const
{
	return can_coerce(obj->get_class(), to);
}

int TypeCoercion::get_cost(const ClassPtr & from, ClassPtr to) const
{
	auto rule = rules.find({ from, to });
	if (rule != rules.end())
	{
		return rule->second.cost;
	}

	return -1;
}