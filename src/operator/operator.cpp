#include "operator/operator.hpp"
#include "interface/numeric.hpp"
#include "class/builtins.hpp"
#include "class/class.hpp"


const OperatorPtr OperatorRegistry::find(std::string_view symbol, bool is_unary) const
{
	auto & operators = is_unary ? unary_operators : binary_operators;
	return operators[symbol].second;
}

const OperatorPtr OperatorRegistry::find(size_t index, bool is_unary) const
{
	auto & operators = is_unary ? unary_operators : binary_operators;
	return operators[index];
}

OperatorPtr OperatorRegistry::register_binary_operator(const std::string & symbol, Associativity associativity, Precedence precedence)
{
	return binary_operators.define(symbol, std::make_shared<Operator>(symbol, Fixity::F_INFIX, associativity, precedence)).second;
	//return binary_operators.insert({ symbol, std::make_unique<Operator>(symbol, Fixity::F_INFIX, associativity, precedence) }).first->second;
}

OperatorPtr OperatorRegistry::register_unary_operator(const std::string & symbol, Fixity fixity)
{
	return unary_operators.define(symbol, std::make_shared<Operator>(symbol, fixity, Associativity::A_NONE, Precedence::P_UNARY)).second;
	//return unary_operators.insert({ symbol, std::make_unique<Operator>(symbol, fixity, Associativity::A_NONE, Precedence::P_UNARY) }).first->second;
}

void OperatorRegistry::register_builtin_operators(void)
{
	////* Binary operators
	//register_binary_operator("+", Associativity::A_LEFT, Precedence::P_ADDITION);
	//register_binary_operator("-", Associativity::A_LEFT, Precedence::P_ADDITION);
	//register_binary_operator("*", Associativity::A_LEFT, Precedence::P_MULTIPLICATION);
	//register_binary_operator("/", Associativity::A_LEFT, Precedence::P_MULTIPLICATION);
	//register_binary_operator("%", Associativity::A_LEFT, Precedence::P_MULTIPLICATION);
	//register_binary_operator("^", Associativity::A_RIGHT, Precedence::P_EXPONENTIATION);
	//register_binary_operator("=", Associativity::A_RIGHT, Precedence::P_ASSIGNMENT);

	////* Unary operators
	//register_unary_operator("+", Fixity::F_PREFIX);
	//register_unary_operator("-", Fixity::F_PREFIX);
	//register_unary_operator("!", Fixity::F_POSTFIX);

	//* Binary operators
	auto add = register_binary_operator("+", Associativity::A_LEFT, Precedence::P_ADDITION);
	auto sub = register_binary_operator("-", Associativity::A_LEFT, Precedence::P_ADDITION);
	auto mul = register_binary_operator("*", Associativity::A_LEFT, Precedence::P_MULTIPLICATION);
	auto div = register_binary_operator("/", Associativity::A_LEFT, Precedence::P_MULTIPLICATION);
	auto mod = register_binary_operator("%", Associativity::A_LEFT, Precedence::P_MULTIPLICATION);
	auto exp = register_binary_operator("^", Associativity::A_RIGHT, Precedence::P_EXPONENTIATION);
	
	//* Unary operators
	auto uplus = register_unary_operator("+", Fixity::F_PREFIX);
	auto uminus = register_unary_operator("-", Fixity::F_PREFIX);
	auto bang = register_unary_operator("!", Fixity::F_POSTFIX);



	// Implementations
	// add
	auto add_lambda = [](const ObjectPtr & lhs, const ObjectPtr & rhs) -> ObjectPtr
	{
		auto lhs_num = std::dynamic_pointer_cast<INumeric>(lhs);
		return lhs_num->add(rhs);
	};
	add->add_implementation(
		std::make_shared<BuiltinOperatorImplentation>(
			add_lambda,
			std::make_pair(builtins::real_class, builtins::real_class),
			builtins::real_class
		)
	);
	add->add_implementation(
		std::make_shared<BuiltinOperatorImplentation>(
			add_lambda,
			std::make_pair(builtins::integer_class, builtins::integer_class),
			builtins::integer_class
		)
	);
}



//* OperatorImplentationRegistry
OperatorImplentationPtr OperatorImplentationRegistry::define(OperatorImplentationPtr implementation)
{
	auto key = std::make_pair(implementation->lhs_type(), implementation->rhs_type());
	indices[key] = implementations.size();
	implementations.push_back(implementation);
	return implementation;
}

size_t OperatorImplentationRegistry::get_index(const ClassPtr & lhs, const ClassPtr & rhs) const
{
	auto it = indices.find({lhs, rhs});
	if (it != indices.end())
	{
		return it->second;
	}
	return -1;
}

OperatorImplentationPtr OperatorImplentationRegistry::find(const ClassPtr & lhs, const ClassPtr & rhs) const
{
	size_t index = get_index(lhs, rhs);
	if (index != -1)
	{
		return implementations[index];
	}
	return nullptr;
}

OperatorImplentationPtr OperatorImplentationRegistry::find_most_specific(const ClassPtr & lhs, const ClassPtr & rhs) const
{
	OperatorImplentationPtr most_specific = nullptr;
	int highest_specificity = 0;
	for (const auto & [key, index] : indices)
	{
		int specificity = measure_specificity({lhs, rhs}, key);
		if (specificity > highest_specificity)
		{
			highest_specificity = specificity;
			most_specific = implementations[index];
		}
	}
	return most_specific;
}

int OperatorImplentationRegistry::measure_specificity(const std::pair<ClassPtr, ClassPtr> & signature, const std::pair<ClassPtr, ClassPtr> & target) const
{
	int lhs_specificity = signature.first->measure_specificity(target.first);
	int rhs_specificity = signature.second->measure_specificity(target.second);
	return lhs_specificity * rhs_specificity;
}

int OperatorImplentation::measure_specificity(const ClassPtr & lhs, const ClassPtr & rhs) const
{
	int lhs_specificity = lhs->measure_specificity(m_result_type);
	int rhs_specificity = rhs->measure_specificity(m_result_type);
	return lhs_specificity * rhs_specificity;
}