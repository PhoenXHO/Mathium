#include "function/function.hpp"


FunctionImplentationPtr FunctionImplentationRegistry::define(const FunctionImplentationPtr & implementation)
{
	implementation_indices[implementation->signature()] = implementations.size();
	implementations.push_back(implementation);
	return implementation;
}

FunctionImplentationRegistry::ImplementationMatchPtr FunctionImplentationRegistry::find_best_match(const FunctionSignature & signature)
{
	ImplementationMatchPtr best_match = std::make_shared<ImplementationMatch>(ImplementationMatch{
		0, {TypeCoercion::MatchLevel::INCOMPATIBLE, {}}
	});

	for (size_t i = 0; i < implementations.size(); ++i)
	{
		const auto & impl = implementations[i];
		auto conversion = signature.match_arguments(impl->signature().parameters);
		if (conversion.match_level > best_match->conversion.match_level)
		{
			best_match->index = i;
			best_match->conversion = conversion;
		}
	}

	return best_match;
}