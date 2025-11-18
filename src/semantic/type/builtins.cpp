#include "semantic/type/type_coercion.hpp"
#include "runtime/class/builtins.hpp"
#include "runtime/object/integer_object.hpp"
#include "runtime/object/real_object.hpp"
#include "core/function/function.hpp"


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
			},
			TypeCoercion::MatchLevel::LOSSLESS
		);

		// Real -> Integer
		TypeCoercion::instance().add_rule(
			builtins::real_class, builtins::integer_class,
			[](const ObjectPtr & obj) -> ObjectPtr
			{
				auto real = std::static_pointer_cast<RealObj>(obj);
				return std::make_shared<IntegerObj>(mpz_int(real->value()));
			},
			TypeCoercion::MatchLevel::LOSSY
		);
	}
}