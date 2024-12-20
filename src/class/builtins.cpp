#include "class/builtins.hpp"
#include "class/mathobj_class.hpp"
#include "class/class_class.hpp"
#include "class/function_class.hpp"
#include "class/reference_class.hpp"
#include "class/none_class.hpp"
#include "class/integer_class.hpp"
#include "class/real_class.hpp"
#include "symbol/symbol_table.hpp"


namespace Builtins
{
	ClassPtr mathobj_class;
	ClassPtr class_class;
	ClassPtr none_class;

	ClassPtr function_class;
	ClassPtr reference_class;

	ClassPtr integer_class;
	ClassPtr real_class;

	void init_builtin_classes(SymbolTable * symtab)
	{
		mathobj_class = std::make_shared<MathObjClass>();
		class_class = std::make_shared<ClassClass>();
		none_class = std::make_shared<NoneClass>();

		// Late initialization of the classes' properties and methods

		function_class = std::make_shared<FunctionClass>();
		reference_class = std::make_shared<ReferenceClass>();

		integer_class = std::make_shared<IntegerClass>();
		real_class = std::make_shared<RealClass>();

		// Initialize the classes and add them to the symbol table
		mathobj_class->init(); symtab->define("MathObj", mathobj_class);
		class_class->init(); symtab->define("Class", class_class);
		none_class->init(); symtab->define("None", none_class);

		function_class->init(); symtab->define("Function", function_class);
		reference_class->init(); symtab->define("Reference", reference_class);

		integer_class->init(); symtab->define("Integer", integer_class);
		real_class->init(); symtab->define("Real", real_class);
	}
}