#include "variable/variable.hpp"
#include "class/class.hpp"


Variable::Variable(std::string_view name, ClassPtr cls) :
	Object(builtins::reference_class),
	m_name(name),
	m_value(cls->default_value()),
	static_type(true)
{}

std::string Variable::to_string(void) const
{
	std::ostringstream oss;
	oss << '<' << m_name
		<< " (" << value_class()->name() << ") = "
		<< value()->to_string() << '>'
		<< " (" << this << ')';
	return oss.str();
}