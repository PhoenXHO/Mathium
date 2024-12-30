//#ifndef VARIABLE_HPP
//#define VARIABLE_HPP

//#include "object/object.hpp"
//#include "class/builtins.hpp"
//#include "symbol/symbol.hpp"


//struct Variable;
//using VariablePtr = std::shared_ptr<Variable>;


//struct Variable : public Symbol
//{
//	bool static_type = false;

//	Variable(std::string_view name, ObjectPtr value)
//		: Symbol(name)
//		, m_value(value) {}
//	Variable(std::string_view name, ClassPtr cls);
//	~Variable() = default;


//	SymbolType get_symbol_type() const override
//	{ return SymbolType::S_VARIABLE; }

//	ClassPtr get_class() const override
//	{ return m_value ? m_value->get_class() : builtins::none_class; }

//	Type get_type() override
//	{ return Type(m_value->get_class(), is_const ? Type::Qualifier::CONST : Type::Qualifier::REF); }


//	std::string name(void) const
//	{ return m_name; }

//	std::string to_string(void) const;

//	void set(ObjectPtr value)
//	{ this->m_value = value; }

//	const ObjectPtr & value(void) const
//	{ return m_value ? m_value : Object::none; }

//	bool is_function(void) const
//	{ return m_value->get_class() == builtins::function_class; }

//	bool is_class(void) const
//	{ return m_value->get_class() == builtins::class_class; }

//private:
//	bool is_const = false;
//	ObjectPtr m_value;
//};

//#endif // VARIABLE_HPP