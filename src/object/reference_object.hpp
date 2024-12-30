#pragma once

#include "object/object.hpp"
#include "binding/binding.hpp"


class ReferenceObj : public Object
{
public:
	ReferenceObj(BindingPtr reference)
		: Object(reference->get_class())
		, m_reference(reference) {}
	~ReferenceObj() = default;

	ObjectPtr value() const { return m_reference->value(); }
	BindingPtr get() const { return m_reference; }

	std::string to_string(void) const override { return m_reference->to_string(); }

private:
	BindingPtr m_reference;
};