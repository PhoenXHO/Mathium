#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

#include "interface/callable.hpp"
#include "function/function_signature.hpp"


class FunctionImplentation;
using FunctionImplentationPtr = std::shared_ptr<FunctionImplentation>;


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