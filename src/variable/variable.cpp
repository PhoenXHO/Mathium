#include "variable/variable.hpp"
#include "class/class.hpp"
#include "class/builtins.hpp"
#include "function/function.hpp"


Variable::Variable(std::string_view name, ClassPtr cls)
	: Symbol(name)
	, m_value(cls->default_value())
	, static_type(true) {}


std::string Variable::to_string(void) const
{
	std::ostringstream oss;
	oss << '<' << m_name
		<< " (" << get_class()->name() << ") = "
		<< value()->to_string() << '>'
		<< " (" << this << ')';
	return oss.str();
}