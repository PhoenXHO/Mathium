#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

template <typename T>
class Registry
{
	static_assert(
		std::is_pointer<T>::value ||
		std::is_same<T, std::shared_ptr<typename T::element_type>>::value ||
		std::is_same<T, std::shared_ptr<T>>::value,
		"Registry can only store pointers or shared pointers"
	);

	std::unordered_map<std::string, size_t> indices;
	std::vector<T> objects;

public:
	Registry() = default;
	~Registry() = default;

	T define(std::string_view name, T object)
	{
		indices[std::string(name)] = objects.size();
		objects.push_back(object);
		return object;
	}

	size_t get_index(std::string_view name) const
	{
		auto it = indices.find(std::string(name));
		if (it != indices.end())
		{
			return it->second;
		}
		return -1;
	}
	const T find(std::string_view name) const
	{
		size_t index = get_index(name);
		if (index != -1)
		{
			return objects[index];
		}
		return nullptr;
	}

	size_t size() const
	{ return objects.size(); }

	const T operator[](size_t index) const
	{ return objects[index]; }

	const T operator[](std::string_view name) const
	{ return find(name); }
};