#pragma once

#include <memory>


class Class;
using ClassPtr = std::shared_ptr<Class>;
class Object;
using ObjectPtr = std::shared_ptr<Object>;
class Variable;
using VariablePtr = std::shared_ptr<Variable>;

class SymbolTable;
struct Type;
struct FunctionSignature;