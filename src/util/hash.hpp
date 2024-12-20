#pragma once

#include <memory>
#include <utility> // for `std::pair`


struct Class;
using ClassPtr = std::shared_ptr<Class>;


// Hash specialization for std::pair<ClassPtr, ClassPtr>
namespace std
{
	template <>
	struct hash<std::pair<ClassPtr, ClassPtr>>
	{
		size_t operator()(const std::pair<ClassPtr, ClassPtr> & pair) const
		{
			auto hash1 = std::hash<ClassPtr>()(pair.first);
			auto hash2 = std::hash<ClassPtr>()(pair.second);
			return hash1 ^ (hash2 << 1);
		}
	};
}