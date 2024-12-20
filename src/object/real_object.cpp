#include "class/class.hpp"
#include "class/builtins.hpp"
#include "object/real_object.hpp"
#include "object/integer_object.hpp"


ObjectPtr RealObj::add(const ObjectPtr & rhs) const
{
	auto rhs_real = std::dynamic_pointer_cast<RealObj>(builtins::real_class->cast(rhs));
	if (rhs_real)
	{
		auto result = add_reals(m_value, rhs_real->value());
		return std::make_shared<RealObj>(
			result,
			std::max(m_integer_part, rhs_real->integer_part()),
			std::max(m_decimal_part, rhs_real->decimal_part())
		);
	}
	return Object::none;
}