#pragma once

#include <memory>

#include "object/object.hpp"


class INumeric;
using NumericPtr = std::shared_ptr<INumeric>;

class INumeric
{
public:
	INumeric() = default;
	virtual ~INumeric() = default;

	virtual ObjectPtr add(const ObjectPtr & rhs) const = 0;
	//virtual ObjectPtr subtract(const ObjectPtr & rhs) const = 0;
	//virtual ObjectPtr multiply(const ObjectPtr & rhs) const = 0;
	//virtual ObjectPtr divide(const ObjectPtr & rhs) const = 0;
	//virtual ObjectPtr power(const ObjectPtr & rhs) const = 0;
	//virtual ObjectPtr negate(void) const = 0;
};