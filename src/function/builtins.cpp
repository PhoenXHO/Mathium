#include <iostream>

#include "object/object.hpp" // for `MathObjPtr`
#include "class/builtins.hpp"
#include "function/function_implementation.hpp"
#include "class/builtins.hpp"
#include "function/function.hpp"
#include "variable/lookup_table.hpp"
#include "object/integer_object.hpp"
#include "object/reference_object.hpp"


void LookupTable::init_builtin_functions(void)
{
	FunctionImplementationPtr print0 = std::make_shared<BuiltinFunctionImplentation>(
		FunctionSignature(),
		[](const std::vector<ObjectPtr> & arguments) -> ObjectPtr
		{
			std::cout << "Hello World!" << std::endl;
			return Object::none;
		}
	);
	FunctionImplementationPtr print1 = std::make_shared<BuiltinFunctionImplentation>(
		FunctionSignature({ { "object", builtins::mathobj_class } }),
		[](const std::vector<ObjectPtr> & arguments) -> ObjectPtr
		{
			std::cout << arguments[0]->to_string() << std::endl;
			return Object::none;
		}
	);
	FunctionImplementationPtr print2 = std::make_shared<BuiltinFunctionImplentation>(
		FunctionSignature({ { "object1", builtins::mathobj_class }, { "object2", builtins::mathobj_class } }),
		[](const std::vector<ObjectPtr> & arguments) -> ObjectPtr
		{
			std::cout << arguments[0]->to_string() << ' ' << arguments[1]->to_string() << std::endl;
			return Object::none;
		}
	);

	auto print = std::make_shared<Function>("print");
	print->define(print0);
	print->define(print1);
	print->define(print2);

	define("print", print);



	// Testing reference parameters
	FunctionImplementationPtr func = std::make_shared<BuiltinFunctionImplentation>(
		FunctionSignature({
			{ "x", { builtins::integer_class, Type::Qualifier::REF } }, // Non-const reference of type `Integer`
			{ "y", builtins::integer_class }
		}, builtins::integer_class),
		[](const std::vector<ObjectPtr> & arguments) -> ObjectPtr
		{
			auto x_ref = std::dynamic_pointer_cast<ReferenceObj>(arguments[0]);
			auto x_var = x_ref->get();
			auto x = std::dynamic_pointer_cast<IntegerObj>(x_var->value());
			auto y = std::dynamic_pointer_cast<IntegerObj>(arguments[1]);

			x_var->set(std::make_shared<IntegerObj>(x->value() + y->value()));
			
			return x_var->value();
		}
	);

	auto add = std::make_shared<Function>("add");
	add->define(func);

	define("add", add);
}