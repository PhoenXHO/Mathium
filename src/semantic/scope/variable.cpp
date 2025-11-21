#include "semantic/scope/variable.hpp"
#include "runtime/core/class/class.hpp"
#include "runtime/core/class/builtin_class_init.hpp"
#include "runtime/core/function/function.hpp"

Variable::Variable(std::string_view name, ClassPtr cls)
	: m_name(name)
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