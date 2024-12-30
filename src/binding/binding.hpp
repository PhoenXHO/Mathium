#ifndef BINDING_HPP
#define BINDING_HPP

#include "object/object.hpp"
#include "class/builtins.hpp"


struct Binding;
using BindingPtr = std::shared_ptr<Binding>;


struct Binding
{
	bool static_type = false;

	Binding(std::string_view name, ObjectPtr value)
		: m_name(name)
		, m_value(value) {}
	Binding(std::string_view name, ClassPtr cls);
	~Binding() = default;


	std::string name(void) const
	{ return m_name; }

	std::string to_string(void) const;

	void set(ObjectPtr value)
	{ this->m_value = value; }

	const ObjectPtr & value(void) const
	{ return m_value ? m_value : Object::none; }

	bool is_function(void) const
	{ return m_value->get_class() == builtins::function_class; }

	bool is_class(void) const
	{ return m_value->get_class() == builtins::class_class; }

	ClassPtr get_class() const
	{ return m_value ? m_value->get_class() : builtins::none_class; }

protected:
	std::string m_name;

private:
	ObjectPtr m_value;
	bool is_const = false;
};

#endif // BINDING_HPP