#include "LInterpreter.h"

/*
 (rect (position 100 20) (size 20 30) (color 0xFFCC00) (fillcolor 0xFFFFFF) (text "Some textz"))
 (rect (position 100 20) (size 20 30) (color 0xFFCC00))
 */

// (defun test (p1 p2) (print (+ p1 p2)) )
LInterpreter::LInterpreter() {
	m_builtInFuncMap["defun"] = [](SExpr* expr) -> SExpr* {
        
        // get funcName = 'test'
        auto* funcName = expr->m_car;
        
        // 'test' = ((p1 p2 p3) (print (+ p1 p2 p3)))
        funcName->m_atomValue = expr->m_cdr;
        
        return funcName->m_atomValue;
	};
    
    m_globalVariableMap["nil"] = m_nilAtom;



    //--------------------------------------------------------------------------------
    // (parse "(print (cons (a b c) (1 2 3)))") -> (print (cons (a b c) (1 2 3)))
    //--------------------------------------------------------------------------------
    m_builtInFuncMap["parse"] = [](SExpr* expr) -> SExpr* {
        auto* result = LInterpreter::getInstance().m_parser.parse( expr->m_car->m_atomName, LInterpreter::getInstance().m_globalVariableMap );
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
            result = LInterpreter::getInstance().eval(it->m_car);
            if ( result != nullptr )
            {
                result->print();
            }
            else
            {
                std::cout << "NIL";
            }
            if ( it->m_cdr != nullptr )
            {
                std::cout << '_';
            }
		}

        return result;
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
    m_builtInFuncMap["set"] = [](SExpr* expr) -> SExpr*
    {
        if ( expr == nullptr )
        {
            return LInterpreter::getInstance().m_nilAtom;
        }
        auto* var= expr->m_car;
        auto* value = (expr->m_cdr==nullptr) ? LInterpreter::getInstance().m_nilAtom
                                                 : expr->m_cdr->m_car;
        var->m_atomValue = value;
        return value;
    };

    // (+ a b c ) -> atom("abc")
    // (+ a (b c) d) -> atom("ad")
    m_builtInFuncMap["+"] = [](SExpr* expr) -> SExpr*
    {
        SExpr* result = nullptr;
        std::string resturnValue;
        
        expr->print("expr: ");
        for (auto* it = expr; it != nullptr; it = it->m_cdr)
        {
            //it->m_car->print("\nit->m_car: ");
            auto* value = LInterpreter::getInstance().eval( it->m_car );
            //value->print("\nvalue: ");
            if (value->m_car->m_type == SExpr::ATOM) {
                resturnValue += value->m_atomName;
            }
            else {
//                auto x = LInterpreter::getInstance().eval(value->m_car);
//                if (x->m_type == SExpr::ATOM) {
//                    resturnValue += x->m_car->m_stringValue;
//                }
            }
        }
        return new SExpr{resturnValue.c_str()};
    };

}
