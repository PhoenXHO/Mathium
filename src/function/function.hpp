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


struct FunctionImplementationRegistry
{
	struct Match
	{
		size_t index; // Index of the implementation
		TypeCoercion::MatchResult conversion; // Conversion result
	};
	using MatchPtr = std::shared_ptr<Match>;

	std::unordered_map<FunctionSignature, size_t> implementation_indices;
	std::vector<FunctionImplementationPtr> implementations;

	FunctionImplementationRegistry() = default;
	~FunctionImplementationRegistry() = default;

	FunctionImplementationPtr define(const FunctionImplementationPtr & implementation);
	MatchPtr find_best_match(const FunctionSignature & signature);

	FunctionImplementationPtr operator[](size_t index) const
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
protected:
	std::string m_name;
	FunctionImplementationRegistry m_implementations;

public:
	Function(std::string_view name)
		: Object(builtins::function_class)
		, m_name(name) {}
	~Function() = default;


	FunctionImplementationRegistry & implementations()
	{ return m_implementations; }
	
	FunctionImplementationPtr define(const FunctionImplementationPtr & implementation)
	{ return m_implementations.define(implementation); }

	FunctionImplementationRegistry::MatchPtr find_best_match(const FunctionSignature & signature)
	{ return m_implementations.find_best_match(signature); }

	FunctionImplementationPtr get_implementation(size_t index) const
	{ return m_implementations[index]; }

	std::string to_string() const
	{ return "<function " + m_name + ">"; }
};