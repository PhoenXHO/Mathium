#include <queue>
#include <unordered_set>

#include "type/type_coercion.hpp"
#include "object/object.hpp"
#include "class/class.hpp"
#include "object/reference_object.hpp"


void TypeCoercion::add_rule(ClassPtr from, ClassPtr to, CoercionFunction converter, MatchLevel match_level)
{
	rules[{ from, to }] = { converter, match_level };
}

ObjectPtr TypeCoercion::coerce(const ObjectPtr & obj, ClassPtr to) const
{
	auto rule = rules.find({ obj->get_class(), to });
	if (rule != rules.end())
	{
		return rule->second.converter(obj);
	}
	return nullptr;
}

bool TypeCoercion::can_coerce(const ClassPtr & from, ClassPtr to) const
{
	return rules.find({ from, to }) != rules.end();
}

bool TypeCoercion::can_coerce(const ObjectPtr & obj, ClassPtr to) const
{
	return can_coerce(obj->get_class(), to);
}

TypeCoercion::MatchLevel TypeCoercion::get_match_level(const ClassPtr & from, ClassPtr to) const
{
	auto rule = rules.find({ from, to });
	if (rule != rules.end())
	{
		return rule->second.match_level;
	}
	return MatchLevel::INCOMPATIBLE;
}

TypeCoercion::CoercionPathPtr TypeCoercion::find_best_coercion_path(const Type & from, const Type & to) const
{
	if (from.qualifier == Type::Qualifier::REF && to.qualifier == Type::Qualifier::REF)
	{
		// Direct reference conversion
		if (from == to)
			return std::make_shared<CoercionPath>(CoercionPath{ {}, MatchLevel::REF });

		// Inheritance reference conversion
		if (from.cls->is_sub_class(to.cls))
			return std::make_shared<CoercionPath>(CoercionPath{ {}, MatchLevel::REF_INHERITANCE });
	}
	
	// Direct conversion
	if (from == to)
		return std::make_shared<CoercionPath>(CoercionPath{ {}, MatchLevel::EXACT });

	auto from_cls = from.cls;
	auto to_cls = to.cls;

	// Check if the path is already cached
	if (m_path_index_cache.contains({ from_cls, to_cls }))
		return m_path_cache[m_path_index_cache[{ from_cls, to_cls }]];

	// Check if there is an inheritance match
	if (from_cls->is_sub_class(to_cls))
	{
		if (from.qualifier == Type::Qualifier::REF && to.qualifier == Type::Qualifier::REF)
			return std::make_shared<CoercionPath>(CoercionPath{ {}, MatchLevel::REF_INHERITANCE });
		return std::make_shared<CoercionPath>(CoercionPath{ {}, MatchLevel::INHERITANCE });
	}

	// Dijkstra's algorithm
	std::unordered_map<ClassPtr, MatchLevel> best_matches;
	std::unordered_map<ClassPtr, std::pair<ClassPtr, CoercionFunction>> previous;
	std::priority_queue<
		std::pair<MatchLevel, ClassPtr>,
		std::vector<std::pair<MatchLevel, ClassPtr>>,
		std::less<> // Order is reversed (best match level last)
	> queue;

	// Set the initial distance
	best_matches[from_cls] = MatchLevel::EXACT;
	queue.push({ MatchLevel::EXACT, from_cls });

	while (!queue.empty())
	{
		// Get the current node
		auto [current_level, current] = queue.top();
		queue.pop();

		// Found the target
		if (current == to)
		{
			// Reconstruct the path
			CoercionPath path;
			path.effective_match_level = current_level;
			
			ClassPtr _current = to_cls;
			while (_current != from)
			{
				auto [prev, converter] = previous[_current];
				path.steps.push_back(converter);
				_current = prev;
			}

			std::reverse(path.steps.begin(), path.steps.end());
			// Cache the path
			cache_path(from_cls, to_cls, std::make_shared<CoercionPath>(path));
			// Return the last element (the one with the highest match level)
			return m_path_cache.back();
		}

		// Check the neighbors (possible conversions)
		for (const auto & [pair, rule] : rules)
		{
			auto [cls, next] = pair;

			// Skip if the rule is not related to the current class
			if (cls != current)
				continue;

			// Combine the match levels
			MatchLevel new_level = current_level | rule.match_level;
			// Update the match level if it is better
			if (!best_matches.contains(next) || new_level > best_matches[next])
			{
				best_matches[next] = new_level;
				previous[next] = { current, rule.converter };
				queue.push({ new_level, next });
			}
		}
	}

	// No path found
	return std::make_shared<CoercionPath>(CoercionPath{ {}, MatchLevel::INCOMPATIBLE });
}

size_t TypeCoercion::cache_path(const ClassPtr & from, const ClassPtr & to, const CoercionPathPtr & path) const
{
	m_path_index_cache[{ from, to }] = m_path_cache.size();
	m_path_cache.push_back(path);
	return m_path_cache.size() - 1;
}

size_t TypeCoercion::cache_path(const CoercionPathPtr & path) const
{
	m_path_cache.push_back(path);
	return m_path_cache.size() - 1;
}

TypeCoercion::MatchLevel operator|(TypeCoercion::MatchLevel a, TypeCoercion::MatchLevel b)
{
	return static_cast<TypeCoercion::MatchLevel>(
		std::min(static_cast<int>(a), static_cast<int>(b))
	);
}