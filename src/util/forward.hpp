#pragma once

#include <memory>


class Class;
using ClassPtr = std::shared_ptr<Class>;
class Object;
using ObjectPtr = std::shared_ptr<Object>;
struct Property;
using PropertyPtr = std::shared_ptr<Property>;
class Variable;
using VariablePtr = std::shared_ptr<Variable>;

class Operator;
using OperatorPtr = std::shared_ptr<Operator>;
class OperatorImplentation;
using OperatorImplentationPtr = std::shared_ptr<OperatorImplentation>;

class LookupTable;
struct Type;
struct FunctionSignature;