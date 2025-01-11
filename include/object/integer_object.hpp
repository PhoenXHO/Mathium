#pragma once

#include "interface/numeric.hpp"
#include "object/object.hpp"
#include "class/builtins.hpp"


class IntegerObj : public Object, public INumeric
{
	mpz_int m_value;
	size_t m_size;

public:
	IntegerObj(std::string_view value) :
		Object(builtins::integer_class),
		m_value(value),
		m_size(value.size())
	{}
	IntegerObj(const mpz_int & value) :
		Object(builtins::integer_class),
		m_value(value),
		m_size(value.str().size())
	{}
	IntegerObj() :
		Object(builtins::integer_class),
		m_value(0),
		m_size(1)
	{}
	~IntegerObj() = default;

	mpz_int value(void) const
	{ return m_value; }
	size_t size(void) const
	{ return m_size; }

	std::string to_string(void) const override
	{ return m_value.str(); }
	ObjectPtr add(const ObjectPtr & rhs) const override;
};