#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

#include "interface/callable.hpp"
#include "class/class.hpp"
#include "class/builtins.hpp"
#include "type/type.hpp"


class FunctionImplentation;
using FunctionImplentationPtr = std::shared_ptr<FunctionImplentation>;


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

class FunctionImplentation : public ICallable
{
protected:
	FunctionSignature m_signature;

public:
	enum class Type
	{
		F_BUILTIN, // Built-in function
		F_USER_DEFINED // User-defined function
	};

	FunctionImplentation(const FunctionSignature & signature) :
		m_signature(signature)
	{}
	virtual ~FunctionImplentation() = default;

	virtual Type type() const = 0;

	FunctionSignature signature() const
	{ return m_signature; }
	size_t arity() const
	{ return m_signature.arity(); }
	ClassPtr return_class() const
	{ return m_signature.return_type; }

	ObjectPtr operator()(const std::vector<ObjectPtr> & arguments) const
	{ return call(arguments); }
};

class BuiltinFunctionImplentation : public FunctionImplentation
{
	std::function<ObjectPtr(const std::vector<ObjectPtr> &)> m_function;

public:
	BuiltinFunctionImplentation(
		const FunctionSignature & signature,
		std::function<ObjectPtr(const std::vector<ObjectPtr> &)> function
	) :
		FunctionImplentation(signature),
		m_function(function)
	{}
	~BuiltinFunctionImplentation() = default;

	Type type() const override
	{ return Type::F_BUILTIN; }

	ObjectPtr call(const std::vector<ObjectPtr> & arguments) const override
	{ return m_function(arguments); }
};

class UserDefinedFunctionImplentation : public FunctionImplentation
{
};


// Hash specialization for FunctionSignature
namespace std
{
	template<>
	struct hash<FunctionSignature>
	{
		size_t operator()(const FunctionSignature & signature) const
		{
			size_t hash = 0;
			for (const auto & param : signature.parameters)
			{
				// We only need to hash the class pointer
				hash ^= std::hash<ClassPtr>{}(param.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2); // Boost's hash_combine
			}
			// We don't need to hash the return type because a function signature is uniquely identified by its parameters
			// We'll do the lookup in the registry using the parameters only
			return hash;
		}
	};
}