#include "LInterpreter.h"

LInterpreter::LInterpreter() {
//	m_builtInFuncMap["defun"] = [](const std::list<SExpression>& exprList) -> SExpression {
//		for (const auto& item : exprList) {
//			std::cerr << "# ";
//			item.print();
//			std::cerr << std::endl;
//		}
//		return exprList.front();
//	};

    m_builtInFuncMap["quote"] = [](SExpr* expr) -> SExpr* {
        return expr;
	};

    m_builtInFuncMap["print"] = [](SExpr* expr) -> SExpr* {

        SExpr* result = nullptr;

        for( auto* it = expr->m_next; it != nullptr; it = it->m_next ) {
            SExpr* result = LInterpreter::getInstance().execute(it);
            result->print();
			std::cout << '_';
		}

        return new SExpr;
	};

//	m_builtInFuncMap["setvar"] = [](const std::list<SExpression>& exprList) -> SExpression {
//		auto it = exprList.cbegin();
//		it++;
//		if (it->m_type == SExpression::ATOM) {
//			const auto& varName = it->m_stringValue;
//			it++;
//			LInterpreter::getInstance().m_variableMap[varName] = LInterpreter::getInstance().execute(*it);
//			return LInterpreter::getInstance().m_variableMap[varName];
//		}
//		else {
//			std::cerr << "setvar: expected ATOM" << std::endl;
//		}
//		return SExpression{};
//	};

//	m_builtInFuncMap["+"] = [](const std::list<SExpression>& exprList) -> SExpression {
//		auto it = exprList.cbegin();
//		it++;
//		SExpression result = LInterpreter::getInstance().execute(*it);
//		result.m_numberValue = 0;
//		result.m_stringValue = "";
//		while (it != exprList.cend()) {
//			SExpression temp = LInterpreter::getInstance().execute(*it);
//			if (temp.m_type == SExpression::NUMBER) {
//				result.m_numberValue += temp.m_numberValue;
//			}
//			else {
//				result.m_stringValue += temp.m_stringValue;
//			}
//			it++;
//		}
//		return result;
//	};
}
