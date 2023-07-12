#include "LInterpreter.h"

/*
 (rect (position 100 20) (size 20 30) (color 0xFFCC00) (fillcolor 0xFFFFFF) (text "Some textz"))
 (rect (position 100 20) (size 20 30) (color 0xFFCC00))
 */

LInterpreter::LInterpreter() {
//	m_builtInFuncMap["defun"] = [](const std::list<SExpression>& exprList) -> SExpression {
//		for (const auto& item : exprList) {
//			std::cerr << "# ";
//			item.print();
//			std::cerr << std::endl;
//		}
//		return exprList.front();
//	};
    
    m_globalVariableMap["nil"] = m_nilAtom;

    m_builtInFuncMap["test"] = [](SExpr* expr) -> SExpr* {
//        auto it* = expr;
//        it->m_car;
//        return result;
    };


    //--------------------------------------------------------------------------------
    // (parse "(print (cons (a b c) (1 2 3)))") -> (print (cons (a b c) (1 2 3)))
    //--------------------------------------------------------------------------------
    m_builtInFuncMap["parse"] = [](SExpr* expr) -> SExpr* {
        auto* result = LInterpreter::getInstance().m_parser.parse( expr->m_car->m_stringValue, LInterpreter::getInstance().m_globalVariableMap );
        return result;
    };

    //--------------------------------------------------------------------------------
    // (eval expr) ->
    //--------------------------------------------------------------------------------
    m_builtInFuncMap["quote"] = [](SExpr* expr) -> SExpr* {
        return LInterpreter::getInstance().eval( expr );
	};

    m_builtInFuncMap["print"] = [](SExpr* expr) -> SExpr* {

        SExpr* result = nullptr;
        //expr->print("\ndbg: ");

        for( auto* it = expr; it != nullptr; it = it->m_cdr ) {
            SExpr* result = LInterpreter::getInstance().eval(it->m_car);
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

        return new SExpr{};
	};

    m_builtInFuncMap["car"] = [](SExpr* expr) -> SExpr* {
        if ( expr == nullptr )
        {
            return LInterpreter::getInstance().m_nilAtom;
        }
        return expr->m_car;
    };

    m_builtInFuncMap["cdr"] = [](SExpr* expr) -> SExpr* {
        if ( expr == nullptr )
        {
            return LInterpreter::getInstance().m_nilAtom;
        }
        return expr->m_cdr;
    };

    m_builtInFuncMap["cons"] = [](SExpr* expr) -> SExpr* {
        if ( expr == nullptr )
        {
            return LInterpreter::getInstance().m_nilAtom;
        }
        expr->print("dbg:cons:");
        std::cout << std::endl;
        
        auto* firstArg = expr->m_car;
        auto* secondArg = expr->m_cdr->m_car;

        return new SExpr{ firstArg, secondArg };
    };

    // (set x 1) -> 1
    // (set x) -> nil
    // (set x 1 2 3) -> ?
    // (set x '(1 2 3)) -> (1 2 3)
    m_builtInFuncMap["set"] = [](SExpr* expr) -> SExpr* {
        if ( expr == nullptr )
        {
            return LInterpreter::getInstance().m_nilAtom;
        }
        auto* var= expr->m_car;
        auto* value = (expr->m_cdr==nullptr) ? LInterpreter::getInstance().m_nilAtom
                                                 : expr->m_cdr->m_car;
        return new SExpr{ var, value };
    };

    // (+ a b c ) -> atom("abc")
    // (+ a (b c) d) -> atom("ad")
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
