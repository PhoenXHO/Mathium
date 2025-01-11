#pragma once

#include <memory>

#include "util/forward.hpp"


namespace builtins
{
	extern ClassPtr mathobj_class; // The base class for all objects
	extern ClassPtr class_class; // The class for class objects
	extern ClassPtr none_class; // The class for the none object

	extern ClassPtr function_class; // The class for function objects
	extern ClassPtr reference_class; // The class for references (for variables)

	extern ClassPtr integer_class; // The class for integer objects
	extern ClassPtr real_class; // The class for real objects

	extern void init_builtin_classes(LookupTable * symtab);
}