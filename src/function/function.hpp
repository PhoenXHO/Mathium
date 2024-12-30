#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

#include "function/function_implementation.hpp"
#include "interface/callable.hpp"
#include "object/object.hpp"
#include "class/builtins.hpp"
#include "type/type.hpp"
#include "util/hash.hpp"


class Function;
using FunctionPtr = std::shared_ptr<Function>;


struct FunctionImplentationRegistry
{
	struct ImplementationMatch
	{
		size_t index; // Index of the implementation
		TypeCoercion::MatchResult conversion; // Conversion result
	};
	using ImplementationMatchPtr = std::shared_ptr<ImplementationMatch>;

	std::unordered_map<FunctionSignature, size_t> implementation_indices;
	std::vector<FunctionImplentationPtr> implementations;

	FunctionImplentationRegistry() = default;
	~FunctionImplentationRegistry() = default;

	FunctionImplentationPtr define(const FunctionImplentationPtr & implementation);
	ImplementationMatchPtr find_best_match(const FunctionSignature & signature);

	FunctionImplentationPtr operator[](size_t index) const
	{ return implementations[index]; }

	std::string to_string() const
	{
		std::string str = "{\n";
		for (const auto & impl : implementations)
		{
			str += "    " + impl->signature().to_string() + "\n";
		}
		str += "}";
		return str;
	}
};

class Function : public Object
{
	FunctionImplentationRegistry m_implementations;

protected:
	std::string m_name;

public:
	Function(std::string_view name)
		: Object(builtins::function_class)
		, m_name(name) {}
	~Function() = default;


	FunctionImplentationRegistry & implementations()
	{ return m_implementations; }
	
	FunctionImplentationPtr define(const FunctionImplentationPtr & implementation)
	{ return m_implementations.define(implementation); }

	FunctionImplentationRegistry::ImplementationMatchPtr find_best_match(const FunctionSignature & signature)
	{ return m_implementations.find_best_match(signature); }

	FunctionImplentationPtr get_implementation(size_t index) const
	{ return m_implementations[index]; }

	std::string to_string() const
	{ return "<function " + m_name + ">"; }
};