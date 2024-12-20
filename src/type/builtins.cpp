#include "type/type.hpp"
#include "class/builtins.hpp"
#include "object/integer_object.hpp"
#include "object/real_object.hpp"


std::shared_ptr<TypeCoercion> TypeCoercion::m_instance = nullptr;


namespace builtins
{
	void init_builtin_type_coercions(void)
	{
		// Integer -> Real
		TypeCoercion::instance().add_rule(
			builtins::integer_class, builtins::real_class,
			[](const ObjectPtr & obj) -> ObjectPtr
			{
				auto integer = std::static_pointer_cast<IntegerObj>(obj);
				return std::make_shared<RealObj>(
					mpfr_float(integer->value()),
					integer->size(),
					0
				);
			}
		);
	}
}