#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "object/object.hpp"
#include "class/builtins.hpp"


struct Variable;
using VariablePtr = std::shared_ptr<Variable>;


struct Variable : public Object
{
	bool static_type = false;

	Variable(std::string_view name, ObjectPtr value) :
		Object(Builtins::reference_class),
		m_name(name),
		m_value(value)
	{}
	Variable(std::string_view name, ClassPtr cls);
	~Variable() = default;

	std::string to_string(void) const override;

	std::string name(void) const
	{ return m_name; }

	void set(ObjectPtr value)
	{ this->m_value = value; }

	const ObjectPtr & value(void) const
	{ return m_value ? m_value : Object::none; }

	ClassPtr value_class(void) const
	{ return m_value ? m_value->get_class() : Builtins::none_class; }

	ObjectPtr cast_to(const ClassPtr & cls) override
	{ return m_value->cast_to(cls); }

	bool is_reference(void) const
	{ return m_value->get_class() == Builtins::reference_class; }

	bool is_function(void) const
	{ return m_value->get_class() == Builtins::function_class; }

	bool is_class(void) const
	{ return m_value->get_class() == Builtins::class_class; }

private:
	std::string m_name;
	ObjectPtr m_value;
};

#endif // VARIABLE_HPP