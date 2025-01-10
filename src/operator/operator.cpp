#include "operator/operator.hpp"
#include "class/builtins.hpp"
#include "class/class.hpp"


std::pair<size_t, OperatorPtr> OperatorRegistry::find(std::string_view symbol, bool is_unary) const
{
	auto & operators = is_unary ? unary_operators : binary_operators;
	return operators[symbol];
}

const OperatorPtr OperatorRegistry::find(size_t index, bool is_unary) const
{
	auto & operators = is_unary ? unary_operators : binary_operators;
	return operators[index];
}

OperatorPtr OperatorRegistry::register_binary_operator(
	const std::string & symbol, Associativity associativity, Precedence precedence)
{
	return binary_operators.define(
		symbol,
		std::make_shared<Operator>(
			symbol,
			Fixity::F_INFIX, associativity, precedence
		)
	).second;
}

OperatorPtr OperatorRegistry::register_unary_operator(const std::string & symbol, Fixity fixity)
{
	return unary_operators.define(
		symbol,
		std::make_shared<Operator>(
			symbol,
			fixity, Associativity::A_NONE, Precedence::P_UNARY
		)
	).second;
}