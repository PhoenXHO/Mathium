#include "util/hash.hpp"
#include "type/type.hpp"
#include "function/function_signature.hpp"


namespace std
{
	// Hash specialization for std::pair<ClassPtr, ClassPtr>
	size_t hash<std::pair<ClassPtr, ClassPtr>>::operator()(const std::pair<ClassPtr, ClassPtr> & pair) const
	{
		auto hash1 = std::hash<ClassPtr>()(pair.first);
		auto hash2 = std::hash<ClassPtr>()(pair.second);
		return hash1 ^ (hash2 << 1);
	}


	// Hash specialization for Type
	size_t hash<Type>::operator()(const Type & type) const
	{
		size_t h1 = std::hash<std::shared_ptr<Class>>{}(type.cls);
		size_t h2 = std::hash<bool>()(type.is_const);
		size_t h3 = std::hash<bool>()(type.is_ref);

		// Combine the hashes
		size_t seed = h1;
		seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}


	// Hash specialization for FunctionSignature
	size_t hash<FunctionSignature>::operator()(const FunctionSignature & signature) const
	{
		size_t hash = 0;
		for (const auto & param : signature.parameters)
		{
			// We only need to hash the class pointer
			hash ^= std::hash<Type>{}(param.second) + 0x9e3779b9 + (hash << 6) + (hash >> 2); // Boost's hash_combine
		}
		// We don't need to hash the return type because a function signature is uniquely identified by its parameters
		// We'll do the lookup in the registry using the parameters only
		return hash;
	}
}