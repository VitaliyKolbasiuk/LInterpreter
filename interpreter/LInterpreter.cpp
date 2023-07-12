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
        //expr->print("\ndbg: ");
        return expr->m_car;
	};

    m_builtInFuncMap["print"] = [](SExpr* expr) -> SExpr* {

        SExpr* result = nullptr;
        //expr->print("\ndbg: ");

        for( auto* it = expr; it != nullptr; it = it->m_next ) {
            SExpr* result = LInterpreter::getInstance().execute(it->m_car);
            if ( result != nullptr )
            {
                result->print();
            }
            else
            {
                std::cout << "NIL";
            }
			std::cout << '_';
		}

        if ( result == nullptr )
        {
            return result;
        }

        return new SExpr;
	};

//	m_builtInFuncMap["setvar"] = [](const std::list<SExpression>& exprList) -> SExpression {
//		auto it = exprList.cbegin();
//		it++;
//		if (it->m_type == SExpression::ATOM) {
//			const auto& varName = it->m_atomName;
//			it++;
//			LInterpreter::getInstance().m_globalVariableMap[varName] = LInterpreter::getInstance().execute(*it);
//			return LInterpreter::getInstance().m_globalVariableMap[varName];
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
//		result.m_atomName = "";
//		while (it != exprList.cend()) {
//			SExpression temp = LInterpreter::getInstance().execute(*it);
//			if (temp.m_type == SExpression::NUMBER) {
//				result.m_numberValue += temp.m_numberValue;
//			}
//			else {
//				result.m_atomName += temp.m_atomName;
//			}
//			it++;
//		}
//		return result;
//	};
}
