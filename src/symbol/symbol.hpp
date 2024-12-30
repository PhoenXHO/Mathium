//#pragma once

//#include <memory>
//#include <vector>
//#include <string>

//#include "util/forward.hpp"
//#include "type/type.hpp"


//class Symbol;
//using SymbolPtr = std::shared_ptr<Symbol>;


//class Symbol
//{
//protected:
//	std::string m_name;

//public:
//	Symbol(std::string_view name) : m_name(name) {}
//	~Symbol() = default;

//	enum class SymbolType
//	{
//		S_CLASS,
//		S_FUNCTION,
//		S_VARIABLE
//	};

//	virtual SymbolType get_symbol_type() const = 0;
//	virtual std::string to_string() const = 0;
//	std::string get_name() const { return m_name; }

//	virtual ClassPtr get_class() const = 0;
//	virtual Type get_type() = 0;
	
//	static ObjectPtr get_symbol_value(SymbolPtr symbol);
//};