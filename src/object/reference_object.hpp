#pragma once

#include "object/object.hpp"
#include "variable/variable.hpp"


class ReferenceObj : public Object
{
public:
	ReferenceObj(VariablePtr reference)
		: Object(reference->get_class())
		, m_reference(reference) {}
	~ReferenceObj() = default;

	ObjectPtr value() const { return m_reference->value(); }
	VariablePtr get() const { return m_reference; }

	std::string to_string(void) const override { return m_reference->to_string(); }

private:
	VariablePtr m_reference;
};