#pragma once

#include <memory>


class Class;
using ClassPtr = std::shared_ptr<Class>;
class Object;
using ObjectPtr = std::shared_ptr<Object>;
struct Property;
using PropertyPtr = std::shared_ptr<Property>;
class Binding;
using BindingPtr = std::shared_ptr<Binding>;

class Operator;
using OperatorPtr = std::shared_ptr<Operator>;
class OperatorImplentation;
using OperatorImplentationPtr = std::shared_ptr<OperatorImplentation>;

class BindingTable;
struct Type;
struct FunctionSignature;