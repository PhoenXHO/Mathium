#pragma once

#include <memory>
#include <utility> // for `std::pair`

#include "util/forward.hpp"


namespace std
{
	// Hash specialization for std::pair<ClassPtr, ClassPtr>
	template <>
	struct hash<std::pair<ClassPtr, ClassPtr>>
	{
		size_t operator()(const std::pair<ClassPtr, ClassPtr> & pair) const;
	};


	// Hash specialization for Type
	template <>
	struct hash<Type>
	{
		size_t operator()(const Type & type) const;
	};
	

	// Hash specialization for FunctionSignature
	template<>
	struct hash<FunctionSignature>
	{
		size_t operator()(const FunctionSignature & signature) const;
	};
}