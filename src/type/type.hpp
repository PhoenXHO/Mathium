#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

#include "util/hash.hpp"
#include "util/forward.hpp"


struct Type
{
	ClassPtr type;
	bool is_const;
	bool is_ref;
};

class TypeCoercion
{
public:
	TypeCoercion() = default;
	~TypeCoercion() = default;

	/// @brief Match level of a coercion rule
	/// Lower values are worse matches
	enum class MatchLevel
	{
		EXACT = 3,        // Exact match
		LOSSLESS = 2,     // Good match - e.g. `Integer -> Real`
		INHERITANCE = 1,  // Good match
		LOSSY = 0,        // Lossy conversion - e.g. `Real -> Integer`
		INCOMPATIBLE = -1 // No conversion possible
	};

	using CoercionFunction = std::function<ObjectPtr(const ObjectPtr &)>;
	struct Rule
	{
		CoercionFunction converter;
		MatchLevel match_level;
	};

	struct CoercionPath
	{
		std::vector<CoercionFunction> steps;
		MatchLevel effective_match_level; // Worst match level in the path

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

	/// @brief Result of a match between multiple pairs of types
	struct MatchResult
	{
		MatchLevel match_level;
		std::vector<CoercionPathPtr> conversions;
	};


	CoercionPathPtr find_best_coercion_path(const ClassPtr & from, const ClassPtr & to) const;

private:
	static std::shared_ptr<TypeCoercion> m_instance; // Singleton instance

	std::unordered_map<std::pair<ClassPtr, ClassPtr>, Rule> rules;

	mutable std::unordered_map<std::pair<ClassPtr, ClassPtr>, size_t> m_path_index_cache;
	mutable std::vector<CoercionPathPtr> m_path_cache;

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


	void add_rule(ClassPtr from, ClassPtr to, CoercionFunction coercion, MatchLevel match_level);
	ObjectPtr coerce(const ObjectPtr & obj, ClassPtr to) const;
	bool can_coerce(const ClassPtr & from, ClassPtr to) const;
	bool can_coerce(const ObjectPtr & obj, ClassPtr to) const;
	MatchLevel get_match_level(const ClassPtr & from, ClassPtr to) const;

	size_t get_path_index(const ClassPtr & from, const ClassPtr & to) const
	{ return m_path_index_cache.at({ from, to }); }

	CoercionPathPtr get_path(int index) const
	{ return m_path_cache.at(index); }

	size_t get_path_cache_size(void) const
	{ return m_path_cache.size(); }


	size_t cache_path(const ClassPtr & from, const ClassPtr & to, const CoercionPathPtr & path) const;
	size_t cache_path(const CoercionPathPtr & path) const;
};

// Operator to combine match levels (worst match level)
TypeCoercion::MatchLevel operator|(TypeCoercion::MatchLevel a, TypeCoercion::MatchLevel b);