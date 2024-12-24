#include "symbol/symbol_table.hpp"


//std::pair<size_t, SymbolPtr> SymbolTable::define(std::string_view name, const SymbolPtr & object)
//{
//	auto [index, symbol] = symbols.find(name);
//	if (index != -1)
//		return { index, symbol };
		
//	return symbols.define(name, object);
//}

//template <typename T>
//std::pair<size_t, std::shared_ptr<T>> SymbolTable::define(std::string_view name, const std::shared_ptr<T> & object)
//{
//	auto [index, symbol] = symbols.find(name);
//	if (index != -1)
//		return { index, std::static_pointer_cast<T>(symbol) };

//	return { symbols.define(name, object).first, object };
//}