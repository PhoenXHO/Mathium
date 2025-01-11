#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

#include "interface/callable.hpp"
#include "function/function_signature.hpp"


class FunctionImplementation;
using FunctionImplementationPtr = std::shared_ptr<FunctionImplementation>;


class FunctionImplementation : public ICallable
{
protected:
	FunctionSignature m_signature;

public:
	enum class FunctionType
	{
		F_BUILTIN, // Built-in function
		F_USER_DEFINED // User-defined function
	};

	FunctionImplementation(const FunctionSignature & signature) :
		m_signature(signature)
	{}
	virtual ~FunctionImplementation() = default;

	virtual FunctionType type() const = 0;

	FunctionSignature signature() const
	{ return m_signature; }
	size_t arity() const
	{ return m_signature.arity(); }
	//ClassPtr return_class() const
	//{ return m_signature.return_type; }
	const Type & return_type() const
	{ return m_signature.return_type; }

	ObjectPtr operator()(const std::vector<ObjectPtr> & arguments) const
	{ return call(arguments); }
};

class BuiltinFunctionImplentation : public FunctionImplementation
{
	std::function<ObjectPtr(const std::vector<ObjectPtr> &)> m_function;

public:
	BuiltinFunctionImplentation(
		const FunctionSignature & signature,
		std::function<ObjectPtr(const std::vector<ObjectPtr> &)> function
	) :
		FunctionImplementation(signature),
		m_function(function)
	{}
	~BuiltinFunctionImplentation() = default;

	FunctionType type() const override
	{ return FunctionType::F_BUILTIN; }

	ObjectPtr call(const std::vector<ObjectPtr> & arguments) const override
	{ return m_function(arguments); }
};

class UserDefinedFunctionImplentation : public FunctionImplementation
{
};