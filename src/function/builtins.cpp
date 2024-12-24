#include <iostream>

#include "object/object.hpp" // for `MathObjPtr`
#include "class/builtins.hpp"
#include "function/function_implementation.hpp"
#include "class/builtins.hpp"
#include "function/function.hpp"
#include "symbol/symbol_table.hpp"
#include "object/integer_object.hpp"


void SymbolTable::init_builtin_functions(void)
{
	FunctionImplentationPtr print0 = std::make_shared<BuiltinFunctionImplentation>(
		FunctionSignature(),
		[](const std::vector<ObjectPtr> & arguments) -> ObjectPtr
		{
			std::cout << "Hello World!" << std::endl;
			return Object::none;
		}
	);
	FunctionImplentationPtr print1 = std::make_shared<BuiltinFunctionImplentation>(
		FunctionSignature({ { "object", builtins::mathobj_class } }),
		[](const std::vector<ObjectPtr> & arguments) -> ObjectPtr
		{
			std::cout << arguments[0]->to_string() << std::endl;
			return Object::none;
		}
	);
	FunctionImplentationPtr print2 = std::make_shared<BuiltinFunctionImplentation>(
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



	FunctionImplentationPtr func = std::make_shared<BuiltinFunctionImplentation>(
		FunctionSignature({ { "x", builtins::integer_class }, { "y", builtins::integer_class } }, builtins::integer_class),
		[](const std::vector<ObjectPtr> & arguments) -> ObjectPtr
		{
			auto x = std::dynamic_pointer_cast<IntegerObj>(arguments[0]);
			auto y = std::dynamic_pointer_cast<IntegerObj>(arguments[1]);
			return std::make_shared<IntegerObj>(x->value() + y->value());
		}
	);

	auto add = std::make_shared<Function>("add");
	add->define(func);

	define("add", add);
}