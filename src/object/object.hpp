#pragma once

#include <string>
#include <memory>
#include <functional>
#include <utility> // for `std::pair`
#include <sstream> // for `std::ostringstream`
#include <boost/multiprecision/gmp.hpp> // for `mpz_int`
#include <boost/multiprecision/mpfr.hpp> // for `mpfr_float`

#include "util/globals.hpp" // for `globals::error_handler`
#include "binding/registry.hpp"
#include "class/property.hpp"
#include "util/forward.hpp"


using namespace boost::multiprecision;


class Object : public std::enable_shared_from_this<Object>
{
protected:
	ClassPtr m_class;
	Registry<PropertyPtr> properties;

public:
	static ObjectPtr none;

	Object(ClassPtr cls) : m_class(cls) { init_properties(); }
	virtual ~Object() = default;


#pragma region Core Interface
	virtual std::string to_string(void) const
	{ return ""; }
	std::ostream & operator<<(std::ostream & os) const
	{ return os << to_string(); }
	template <typename T>
	std::shared_ptr<T> as(void)
	{ return std::dynamic_pointer_cast<T>(shared_from_this()); }
#pragma endregion


#pragma region Property System
	void init_properties(void);
	PropertyPtr get_property(std::string_view name) const
	{ return properties[name].second; }
	void set_property(std::string_view name, const ObjectPtr & value);
#pragma endregion
	
	
#pragma region Type System
	ClassPtr get_class(void) const { return m_class; }
	bool is_instance_of(ClassPtr cls) const;
#pragma endregion
};

mpfr_float add_reals(const mpfr_float & lhs, const mpfr_float & rhs);