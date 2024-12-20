#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <any>
#include <functional>
#include <utility> // for `std::pair`

#include "class/property.hpp"
#include "variable/variable.hpp"
#include "symbol/symbol_registry.hpp"
#include "class/builtins.hpp"


struct Class;
using ClassPtr = std::shared_ptr<Class>;


struct Class : public Object
{	
	Class(std::string name, std::vector<ClassPtr> bases) :
		Object(Builtins::class_class),
		m_name(name),
		bases(bases)
	{}
	//! Temporary fix
	Class(std::string name) :
		Object(nullptr),
		m_name(name)
	{}
	~Class() = default;


	#pragma region Core Interface
	virtual void init(void)
	{}
	virtual std::string to_string(void) const
	{
		std::ostringstream oss;
		oss << "<" << m_name << ">";
		return oss.str();
	}
	const std::string & name(void) const
	{ return m_name; }
	#pragma endregion


	#pragma region Property System
	Registry<PropertyPtr> & properties(void)
	{ return m_properties; }

	PropertyPtr get_property(std::string_view name) const
	{ return m_properties[name].second; }

	void add_property(const PropertyPtr & property)
	{
		m_properties.define(property->variable->name(), property);
	}
	#pragma endregion


	#pragma region Inheritance
	bool is_sub_class(const ClassPtr & cls, bool strict = false) const;
	/// @brief Compare the specificity of two classes.
	/// The specificity is a measure of how well a class matches another class.
	/// The higher the specificity, the better the match.
	/// @return `2` if the classes are the same, `1` if this class is castable to `cls`, and `0` otherwise
	int measure_specificity(const ClassPtr & cls) const;
	#pragma endregion


	#pragma region Type System
	/// @brief Instantiate a new object of this class
	virtual ObjectPtr instantiate(const std::any & value) const = 0;
	/// @brief Get the default value of this class
	virtual ObjectPtr default_value(void) const = 0;
	virtual bool can_cast_to(const ClassPtr & cls) const = 0;
	/// @brief Cast an object to this class
	virtual ObjectPtr cast(const ObjectPtr & obj) const = 0;
	#pragma endregion

protected:
	std::string m_name;
	Registry<PropertyPtr> m_properties; // Properties of the class
	std::vector<ClassPtr> bases; // Inheritance
};


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