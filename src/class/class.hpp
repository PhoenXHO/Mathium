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
#include "type/type.hpp"
#include "object/object.hpp"
#include "symbol/symbol.hpp"


struct Class : public Object, public Symbol
{	
	Class(std::string name, std::vector<ClassPtr> bases)
		: Object(builtins::class_class)
		, Symbol(name)
		, bases(bases) {}
	Class(std::string name)
		: Object(nullptr)
		, Symbol(name) {}
	~Class() = default;


	Symbol::Type get_symbol_type() const override
	{ return Symbol::Type::S_CLASS; }

	ClassPtr get_class() const override
	{ return builtins::class_class; }


#pragma region Core Interface
	virtual void init(void)
	{}
	virtual std::string to_string(void) const
	{
		std::ostringstream oss;
		oss << "<class " << m_name << ">";
		return oss.str();
	}
	const std::string & name(void) const
	{ return m_name; }

	void set_class(const ClassPtr & cls)
	{ m_class = cls; }
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
	int measure_specificity(const ClassPtr & target);
#pragma endregion


#pragma region Type System
	/// @brief Instantiate a new object of this class
	virtual ObjectPtr instantiate(const std::any & value) const = 0;
	/// @brief Get the default value of this class
	virtual ObjectPtr default_value(void) const = 0;
	bool can_cast_to(const ClassPtr & target);
	/// @brief Cast an object to this class
	ObjectPtr cast(const ObjectPtr & obj);
#pragma endregion

protected:
	Registry<PropertyPtr> m_properties; // Properties of the class

private:
	std::vector<ClassPtr> bases; // Inheritance
};