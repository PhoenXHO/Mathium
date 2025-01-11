#pragma once

#include <memory>
#include <vector>

#include "object/object.hpp"


class ICallable;
using CallablePtr = std::shared_ptr<ICallable>;


class ICallable
{
public:
	ICallable() = default;
	virtual ~ICallable() = default;

	virtual ObjectPtr call(const std::vector<ObjectPtr> & arguments) const = 0;
};