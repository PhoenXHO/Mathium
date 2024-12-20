#include <iostream>

#include "object/object.hpp" // for `MathObjPtr`
#include "class/builtins.hpp"
#include "function/function_implementation.hpp"
#include "class/builtins.hpp"
#include "function/function.hpp"
#include "symbol/symbol_table.hpp"


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
}