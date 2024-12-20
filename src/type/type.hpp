#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

#include "util/hash.hpp"
#include "util/forward.hpp"


class TypeCoercion
{
public:
	TypeCoercion() = default;
	~TypeCoercion() = default;

	using CoercionFunction = std::function<ObjectPtr(const ObjectPtr &)>;
	struct Rule
	{
		CoercionFunction converter;
		// The cost of the coercion - lower is more preferred
		// 0 - exact match
		// 1 - implicit conversion
		// 2 - lossy conversion
		int cost;
	};

private:
	std::unordered_map<std::pair<ClassPtr, ClassPtr>, Rule> rules;
	static std::shared_ptr<TypeCoercion> m_instance; // Singleton instance

public:
	static TypeCoercion & instance(void)
	{
		if (!m_instance)
		{
			m_instance = std::make_shared<TypeCoercion>();
		}
		return *m_instance;
	}

	void add_rule(ClassPtr from, ClassPtr to, CoercionFunction coercion, int cost = 1);
	ObjectPtr coerce(const ObjectPtr & obj, ClassPtr to) const;
	bool can_coerce(const ClassPtr & from, ClassPtr to) const;
	bool can_coerce(const ObjectPtr & obj, ClassPtr to) const;
	int get_cost(const ClassPtr & from, ClassPtr to) const;
};