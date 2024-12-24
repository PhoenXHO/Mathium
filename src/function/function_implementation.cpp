#include "function/function_implementation.hpp"
#include "class/class.hpp"


TypeCoercion::MatchResult FunctionSignature::match_arguments(const std::vector<std::pair<std::string, ClassPtr>> & arguments) const
{
	if (arguments.size() != parameters.size())
		return {TypeCoercion::MatchLevel::INCOMPATIBLE, {}};

	TypeCoercion::MatchResult result;
	result.match_level = TypeCoercion::MatchLevel::EXACT;

	for (size_t i = 0; i < parameters.size(); ++i)
	{
		auto & from = parameters[i].second;
		auto & to = arguments[i].second;
		auto path = TypeCoercion::instance().find_best_coercion_path(from, to);
		if (path->effective_match_level == TypeCoercion::MatchLevel::INCOMPATIBLE)
			return {TypeCoercion::MatchLevel::INCOMPATIBLE, {}};

		result.match_level = result.match_level | path->effective_match_level;
		result.conversions.push_back(path);
	}

	return result;
}

std::string FunctionSignature::to_string(bool parameters_only) const
{
	std::string str = "(";
	for (size_t i = 0; i < parameters.size(); ++i)
	{
		if (i > 0)
			str += ", ";
		str += parameters[i].second->to_string();
	}
	str += ")";
	if (!parameters_only)
	{
		str += " -> " + return_type->to_string();
	}
	return str;
}