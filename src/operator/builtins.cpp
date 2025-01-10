#include "operator/operator.hpp"
#include "interface/numeric.hpp"


#define BUILTIN_BINARY_IMPL(op, function) \
	op->add_implementation( \
		std::make_shared<BuiltinFunctionImplentation>( \
			FunctionSignature({ \
				{ "x", builtins::real_class }, \
				{ "y", builtins::real_class } \
			}, builtins::real_class), \
			function \
		) \
	); \
	op->add_implementation( \
		std::make_shared<BuiltinFunctionImplentation>( \
			FunctionSignature({ \
				{ "x", builtins::integer_class }, \
				{ "y", builtins::integer_class } \
			}, builtins::integer_class), \
			function \
		) \
	)
#define BUILTIN_UNARY_IMPL(op, function) \
	op->add_implementation( \
		std::make_shared<BuiltinFunctionImplentation>( \
			FunctionSignature({ \
				{ "x", builtins::real_class } \
			}, builtins::real_class), \
			function \
		) \
	); \
	op->add_implementation( \
		std::make_shared<BuiltinFunctionImplentation>( \
			FunctionSignature({ \
				{ "x", builtins::integer_class } \
			}, builtins::integer_class), \
			function \
		) \
	)

void OperatorRegistry::register_builtin_operators(void)
{
	//* Binary operators
	auto add = register_binary_operator("+", Associativity::A_LEFT, Precedence::P_ADDITION);
	auto sub = register_binary_operator("-", Associativity::A_LEFT, Precedence::P_ADDITION);
	auto mul = register_binary_operator("*", Associativity::A_LEFT, Precedence::P_MULTIPLICATION);
	auto div = register_binary_operator("/", Associativity::A_LEFT, Precedence::P_MULTIPLICATION);
	auto mod = register_binary_operator("%", Associativity::A_LEFT, Precedence::P_MULTIPLICATION);
	auto exp = register_binary_operator("^", Associativity::A_RIGHT, Precedence::P_EXPONENTIATION);
	
	//* Unary operators
	auto plus  = register_unary_operator("+", Fixity::F_PREFIX);
	auto minus = register_unary_operator("-", Fixity::F_PREFIX);
	auto bang  = register_unary_operator("!", Fixity::F_POSTFIX);



	//* Binary Implementations
	// add
	auto add_lambda = [](const std::vector<ObjectPtr> & arguments) -> ObjectPtr
	{
		auto lhs_num = std::dynamic_pointer_cast<INumeric>(arguments[0]);
		return lhs_num->add(arguments[1]);
	};
	BUILTIN_BINARY_IMPL(add, add_lambda);
	

	//* Unary Implementations
	// plus
	auto plus_lambda = [](const std::vector<ObjectPtr> & arguments) -> ObjectPtr
	{
		return arguments[0];
	};
	BUILTIN_UNARY_IMPL(plus, plus_lambda);
}