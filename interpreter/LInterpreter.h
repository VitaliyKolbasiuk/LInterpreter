#pragma once

#include "Scanner.h"
#include "Log.h"

#include <iostream>
#include <map>
#include <functional>
#include <stack>

using BuiltInFunc = std::function <const SExpression(const std::list<SExpression>&)>;

class LInterpreter {
	Scanner m_scanner;
	LInterpreter();
public: 
	std::map<std::string, BuiltInFunc> m_builtInFuncMap;
	std::map<std::string, SExpression> m_userFuncMap;
	std::map<std::string, SExpression> m_variableMap;

	using LocalVariableMap = std::map<std::string, SExpression>;
	std::stack<LocalVariableMap> m_stack;

	static LInterpreter& getInstance() {
		static LInterpreter instance;
		return instance;
	}

	void execute(const std::string& lText) {
		m_scanner.scan(lText);
		const auto& root = m_scanner.root();
		root.print();
		for ( auto& it: root.m_list) {
			execute(it);
		}
	}

	SExpression execute(const SExpression& sExpr) {
		switch (sExpr.m_type) {
		case SExpression::NUMBER:
		case SExpression::ATOM:
			LOG("ATOM");
			if (auto it = m_variableMap.find(sExpr.m_stringValue);
				it != m_variableMap.end())
			{
				return it->second;
			}
			else {
				return sExpr;
			}
		case SExpression::LIST:
			LOG("LIST");
			if (sExpr.m_list.size() == 0) {
				LOG("NULL");
				return SExpression{};
			}
			else {
				const auto& funcName = sExpr.m_list.front();
				LOGVAR(funcName.m_stringValue);
				if (funcName.m_type == SExpression::ATOM) {
					if ( auto it =  m_builtInFuncMap.find(funcName.m_stringValue);
						      it != m_builtInFuncMap.end() ) 
					{
						return it->second(sExpr.m_list);
					}
				}
			}
			break;
		}
		return SExpression{};
	}
};