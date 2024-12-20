#include <queue>
#include <unordered_set>

#include "type/type.hpp"
#include "object/object.hpp"
#include "class/class.hpp"


void TypeCoercion::add_rule(ClassPtr from, ClassPtr to, CoercionFunction converter, int cost)
{
	rules[{ from, to }] = { converter, cost };
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

int TypeCoercion::get_cost(const ClassPtr & from, ClassPtr to) const
{
	auto rule = rules.find({ from, to });
	if (rule != rules.end())
	{
		return rule->second.cost;
	}

	return -1;
}

TypeCoercion::CoercionPathPtr TypeCoercion::find_best_coercion_path(const ClassPtr & from, const ClassPtr & to) const
{
	// Direct conversion
	if (from == to)
		return std::make_shared<CoercionPath>(CoercionPath{ {}, 0 });

	// Check if the path is already cached
	if (m_path_index_cache.contains({ from, to }))
		return m_path_cache[m_path_index_cache[{ from, to }]];

	// Dijkstra's algorithm
	std::unordered_map<ClassPtr, int> distances;
	std::unordered_map<ClassPtr, std::pair<ClassPtr, CoercionFunction>> previous;
	std::priority_queue<
		std::pair<int, ClassPtr>,
		std::vector<std::pair<int, ClassPtr>>,
		std::greater<>
	> queue;

	// Set the initial distance
	distances[from] = 0;
	queue.push({ 0, from });

	while (!queue.empty())
	{
		// Get the current node
		auto [distance, current] = queue.top();
		queue.pop();

		// Found the target
		if (current == to)
		{
			// Reconstruct the path
			CoercionPath path;
			path.total_cost = distance;
			
			ClassPtr _current = to;
			while (_current != from)
			{
				auto [prev, converter] = previous[_current];
				path.steps.push_back(converter);
				_current = prev;
			}

			std::reverse(path.steps.begin(), path.steps.end());
			// Cache the path
			cache_path(from, to, std::make_shared<CoercionPath>(path));
			return m_path_cache.back();
		}

		// Check the neighbors (possible conversions)
		for (const auto & [pair, rule] : rules)
		{
			auto [cls, next] = pair;

			// Skip if the rule is not related to the current class
			if (cls != current)
				continue;

			int new_distance = distance + rule.cost;
			// Update the distance if it is shorter
			if (!distances.contains(next) || new_distance < distances[next])
			{
				distances[next] = new_distance;
				previous[next] = { current, rule.converter };
				queue.push({ new_distance, next });
			}
		}
	}

	// No path found
	return std::make_shared<CoercionPath>(CoercionPath{ {}, -1 });
}

void TypeCoercion::cache_path(const ClassPtr & from, const ClassPtr & to, const CoercionPathPtr & path) const
{
	m_path_index_cache[{ from, to }] = m_path_cache.size();
	m_path_cache.push_back(path);
}