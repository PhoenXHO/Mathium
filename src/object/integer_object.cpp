#include "object/integer_object.hpp"
#include "object/real_object.hpp"


ObjectPtr IntegerObj::add(const ObjectPtr & rhs) const
{
	if (rhs->is_instance_of(builtins::integer_class))
	{
		auto rhs_int = std::dynamic_pointer_cast<IntegerObj>(rhs);
		return std::make_shared<IntegerObj>(m_value + rhs_int->value());
	}
	else if (rhs->is_instance_of(builtins::real_class))
	{
		auto rhs_real = std::dynamic_pointer_cast<RealObj>(rhs);
		return std::make_shared<RealObj>(
			add_reals(m_value, rhs_real->value()),
			std::max(m_size, rhs_real->integer_part()),
			std::max(m_size, rhs_real->decimal_part())
		);
	}
	return Object::none;
}