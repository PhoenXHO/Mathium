#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <utility> // for `std::pair`

//#include "object/object.hpp" // for hash specialization of std::pair<MathObj::Type, MathObj::Type>
#include "class/class.hpp"
#include "symbol/symbol_registry.hpp"


class Operator;
using OperatorPtr = std::shared_ptr<Operator>;
class OperatorImplentation;
using OperatorImplentationPtr = std::shared_ptr<OperatorImplentation>;


// Fixity of an operator
enum class Fixity
{
	F_PREFIX, // Prefix operator (e.g. -1)
	F_INFIX, // Infix operator (e.g. 1 + 1)
	F_POSTFIX // Postfix operator (e.g. 1!)
};

// Associativity of an operator
enum class Associativity
{
	A_LEFT, // Left-associative operator (e.g. +)
	A_RIGHT, // Right-associative operator (e.g. ^)
	A_NONE // Non-associative operator (e.g. ==)
};

// Precedence of an operator
enum Precedence
{
	P_MIN = 0, // Minimum precedence
	P_ASSIGNMENT,
	P_ADDITION,
	P_MULTIPLICATION,
	P_EXPONENTIATION,
	P_UNARY, // Unary operators
};

// Operator function which takes two MathObjPtrs and returns a MathObjPtr
using OperatorFunction = std::function<MathObjPtr(const MathObjPtr & lhs, const MathObjPtr & rhs)>;

// Base class for different implementations of an operator (to allow for operator overloading)
class OperatorImplentation
{
public:
	enum class Type
	{
		O_BUILTIN, // Built-in operator
		O_USER_DEFINED // User-defined operator
	};

protected:
	Type m_type; // Type of the operator implementation
	//MathObj::Type m_result_type; // Result type of the operator
	ClassPtr m_result_class; // Class of the operator

public:
	//OperatorImplentation(Type type, MathObj::Type result_type) :
	//	m_type(type), m_result_type(result_type)
	//{}
	OperatorImplentation(Type type, ClassPtr result_class) :
		m_type(type), m_result_class(result_class)
	{}
	~OperatorImplentation() = default;

	Type type() const
	{ return m_type; }
	//MathObj::Type result_type() const
	//{ return m_result_type; }
	const ClassPtr & result_class() const
	{ return m_result_class; }

	virtual MathObjPtr execute(const MathObjPtr & lhs, const MathObjPtr & rhs) const = 0;
	MathObjPtr operator()(const MathObjPtr & lhs, const MathObjPtr & rhs) const
	{ return execute(lhs, rhs); }

	int measure_specificity(const ClassPtr & lhs, const ClassPtr & rhs) const
	{
		int lhs_specificity = lhs->measure_specificity(m_result_class);
		int rhs_specificity = rhs->measure_specificity(m_result_class);
		return lhs_specificity * rhs_specificity;
	}
};

// Built-in operator implementation
class BuiltinOperatorImplentation : public OperatorImplentation
{
	OperatorFunction function; // Function to execute

public:
	//BuiltinOperatorImplentation(OperatorFunction function, MathObj::Type result_type) :
	//	OperatorImplentation(Type::O_BUILTIN, result_type), function(function)
	//{}
	BuiltinOperatorImplentation(OperatorFunction function, ClassPtr result_class) :
		OperatorImplentation(Type::O_BUILTIN, result_class), function(function)
	{}
	~BuiltinOperatorImplentation() = default;

	MathObjPtr execute(const MathObjPtr & lhs, const MathObjPtr & rhs) const override
	{ return function(lhs, rhs); }
};

// User-defined operator implementation
// class UserDefinedOperatorImplentation : public OperatorImplentation

//using OperatorImplentationMap = std::unordered_map<std::pair<MathObj::Type, MathObj::Type>, std::shared_ptr<OperatorImplentation>>;
//using OperatorImplentationMap = std::unordered_map<std::pair<ClassPtr, ClassPtr>, OperatorImplentationPtr>;

class OperatorImplentationRegistry
{
	std::unordered_map<std::pair<ClassPtr, ClassPtr>, size_t> indices;
	std::vector<OperatorImplentationPtr> implementations;

public:
	OperatorImplentationRegistry() = default;
	~OperatorImplentationRegistry() = default;

	OperatorImplentationPtr define(const ClassPtr & lhs, const ClassPtr & rhs, OperatorImplentationPtr implementation);

	/// @brief Get the index of the operator implementation for the given operands
	/// @return The index of the operator implementation or -1 if no implementation is found
	size_t get_index(const ClassPtr & lhs, const ClassPtr & rhs) const;
	OperatorImplentationPtr find(const ClassPtr & lhs, const ClassPtr & rhs) const;

	size_t size() const
	{ return implementations.size(); }

	const OperatorImplentationPtr operator[](size_t index) const
	{ return implementations[index]; }

	const OperatorImplentationPtr operator[](std::pair<const ClassPtr &, const ClassPtr &> & impl) const
	{ return find(impl.first, impl.second); }

	/// @brief Find the most specific operator implementation for the given operands
	/// @return The most specific operator implementation or `nullptr` if no implementation is found
	OperatorImplentationPtr find_most_specific(const ClassPtr & lhs, const ClassPtr & rhs) const;
};

class Operator
{
	std::string symbol; // Symbol of operator
	Fixity fixity; // Fixity of operator
	Associativity associativity; // Associativity of operator
	Precedence precedence; // Precedence of operator

	//OperatorImplentationMap implementations; // Implementations of the operator
	OperatorImplentationRegistry implementations;

public:
	Operator(const std::string &symbol, Fixity fixity, Associativity associativity, Precedence precedence) :
		symbol(symbol), fixity(fixity), associativity(associativity), precedence(precedence)
	{}
	~Operator() = default;

	const std::string & get_symbol() const
	{ return symbol; }
	Fixity get_fixity() const
	{ return fixity; }
	Associativity get_associativity() const
	{ return associativity; }
	Precedence get_precedence() const
	{ return precedence; }
	//OperatorImplentationMap & get_implementations()
	//{ return implementations; }
	OperatorImplentationRegistry & get_implementations()
	{ return implementations; }

	void add_implementation(const ClassPtr & lhs, const ClassPtr & rhs, const OperatorImplentationPtr & implementation)
	//{ implementations[{lhs, rhs}] = implementation; }
	{ implementations.define(lhs, rhs, implementation); }
};

using OperatorMap = std::unordered_map<std::string, OperatorPtr>;
class OperatorRegistry
{
	//OperatorMap binary_operators, unary_operators;
	Registry<OperatorPtr> binary_operators, unary_operators;

public:
	OperatorRegistry() = default;
	~OperatorRegistry() = default;

	const OperatorPtr find(std::string_view symbol, bool is_unary = false) const;
	const OperatorPtr find(size_t index, bool is_unary = false) const;
	OperatorPtr register_binary_operator(const std::string & symbol, Associativity associativity, Precedence precedence);
	OperatorPtr register_unary_operator(const std::string & symbol, Fixity fixity);	
	
	void register_builtin_operators(void);
};