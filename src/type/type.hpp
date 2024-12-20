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
		// 1 - lossless conversion
		// 2 - lossy conversion
		int cost;
	};

	struct CoercionPath
	{
		std::vector<CoercionFunction> steps;
		int total_cost;

		ObjectPtr apply(const ObjectPtr & obj) const
		{
			ObjectPtr result = obj;
			for (const auto & step : steps)
			{
				result = step(result);
			}
			return result;
		}
	};
	using CoercionPathPtr = std::shared_ptr<CoercionPath>;


	CoercionPathPtr find_best_coercion_path(const ClassPtr & from, const ClassPtr & to) const;

private:
	std::unordered_map<std::pair<ClassPtr, ClassPtr>, Rule> rules;
	static std::shared_ptr<TypeCoercion> m_instance; // Singleton instance

	mutable std::unordered_map<std::pair<ClassPtr, ClassPtr>, size_t> m_path_index_cache;
	mutable std::vector<CoercionPathPtr> m_path_cache;


	void cache_path(const ClassPtr & from, const ClassPtr & to, const CoercionPathPtr & path) const;

public:
	// Singleton instance
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

	size_t get_path_index(const ClassPtr & from, const ClassPtr & to) const
	{ return m_path_index_cache.at({ from, to }); }

	CoercionPathPtr get_path(int index) const
	{ return m_path_cache.at(index); }

	size_t get_path_cache_size(void) const
	{ return m_path_cache.size(); }
};