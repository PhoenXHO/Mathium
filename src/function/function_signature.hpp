#pragma once

#include <vector>
#include <string>
#include <memory>

#include "util/forward.hpp"
#include "class/builtins.hpp"
#include "type/type_coercion.hpp"


/// @brief Signature of a function which consists of a list of parameters and a return type
struct FunctionSignature
{
	std::vector<std::pair<std::string, ClassPtr>> parameters;
	ClassPtr return_type;


	FunctionSignature() : return_type(builtins::none_class) {}
	FunctionSignature(
		const std::vector<std::pair<std::string, ClassPtr>> & parameters,
		const ClassPtr & return_type = builtins::none_class
	) :
		parameters(parameters), return_type(return_type)
	{}
	~FunctionSignature() = default;

	size_t arity(void) const
	{ return parameters.size(); }

	bool operator==(const FunctionSignature & other) const
	{
		if (parameters.size() != other.parameters.size())
			return false;

		for (size_t i = 0; i < parameters.size(); ++i)
		{
			if (parameters[i].second != other.parameters[i].second)
				return false;
		}

		// We don't need to compare the return type because a function signature
		// is uniquely identified by its parameters
		return true;
	}

	TypeCoercion::MatchResult match_arguments(const std::vector<std::pair<std::string, ClassPtr>> & arguments) const;

	std::string to_string(bool parameters_only = false) const;
};


// Hash specialization for FunctionSignature
namespace std
{
}