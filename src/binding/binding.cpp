#include "binding/binding.hpp"
#include "class/class.hpp"
#include "class/builtins.hpp"
#include "function/function.hpp"

Binding::Binding(std::string_view name, ClassPtr cls)
	: m_name(name)
	, m_value(cls->default_value())
	, static_type(true) {}

std::string Binding::to_string(void) const
{
	std::ostringstream oss;
	oss << '<' << m_name
		<< " (" << get_class()->name() << ") = "
		<< value()->to_string() << '>'
		<< " (" << this << ')';
	return oss.str();
}