#pragma once
#include "Scanner.h"
#include <iostream>
#include <map>
#include <functional>

using BuiltInFunc = std::function <const SExpression(const std::list<SExpression>&)>;

class LInterpreter {
	Scanner m_scanner;
	std::map<std::string, BuiltInFunc> m_builtInFuncMap;
public:
	LInterpreter();
	void execute(const std::string& lText) {
		m_scanner.scan(lText);
		const auto& root = m_scanner.root();
		execute(root.m_list.front());
	}
	void execute(const SExpression& sExpr) {
		switch (sExpr.m_type) {
		case SExpression::NUMBER:
			break;
		case SExpression::ATOM:
			break;
		case SExpression::LIST:
			if (sExpr.m_list.size() == 0) {
				//nothing
			}
			else {
				const auto& funcName = sExpr.m_list.front();
				if (funcName.m_type == SExpression::ATOM) {
					if ( auto it =  m_builtInFuncMap.find(funcName.m_stringValue);
						      it != m_builtInFuncMap.end() ) 
					{
						it->second(sExpr.m_list);
					}
				}
			}
			break;
		}
	}
};