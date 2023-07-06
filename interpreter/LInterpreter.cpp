#include "LInterpreter.h"

LInterpreter::LInterpreter() {
	m_builtInFuncMap["defun"] = [](const std::list<SExpression>& exprList) -> SExpression {
		for (const auto& item : exprList) {
			std::cerr << "# ";
			item.print();
			std::cerr << std::endl;
		}
		return exprList.front();
	};

	m_builtInFuncMap["quote"] = [](const std::list<SExpression>& exprList) -> SExpression {
		SExpression temp;
		auto it = exprList.cbegin();
		it++;
		if (it->m_type == SExpression::LIST) {
			temp.m_list.push_back(*it);
		}
		else {
			temp = *it;
		}
		return temp;
	};

	m_builtInFuncMap["print"] = [](const std::list<SExpression>& exprList) -> SExpression {
		auto it = exprList.cbegin();
		it++;
		
		while (it != exprList.cend()) {
			SExpression result = LInterpreter::getInstance().execute(*it);
			result.print();
			it++;
			std::cout << '_';
		}
		return SExpression{};
	};

	m_builtInFuncMap["setvar"] = [](const std::list<SExpression>& exprList) -> SExpression {
		auto it = exprList.cbegin();
		it++;
		if (it->m_type == SExpression::ATOM) {
			const auto& varName = it->m_stringValue;
			it++;
			LInterpreter::getInstance().m_variableMap[varName] = LInterpreter::getInstance().execute(*it);
			return LInterpreter::getInstance().m_variableMap[varName];
		}
		else {
			std::cerr << "setvar: expected ATOM" << std::endl;
		}
		return SExpression{};
	};

}