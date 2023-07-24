#include "LInterpreter.h"
/*
 (rect (position 100 20) (size 20 30) (color 0xFFCC00) (fillcolor 0xFFFFFF) (text "Some textz"))
 (rect (position 100 20) (size 20 30) (color 0xFFCC00))
 */

// (defun test (p1 p2) (print (+ p1 p2)) )
LInterpreter::LInterpreter() {

    addPseudoTableFuncs();

    // Add user fuction
	m_builtInFuncMap["defun"] = new BuiltinFunc( "defun", [](List* expr) -> ISExpr* {
        
        // get funcName = 'test'
        auto* funcName = expr->m_car->toAtom();
        
        // 'test' = ((p1 p2 p3) (print (+ p1 p2 p3)))
        funcName->setValue( expr->m_cdr );
        
        return funcName->value();
	});
    
    m_globalVariableMap["nil"] = m_nilAtom;



    //--------------------------------------------------------------------------------
    // (parse "(print (cons (a b c) (1 2 3)))") -> (print (cons (a b c) (1 2 3)))
    //--------------------------------------------------------------------------------
//    m_builtInFuncMap["parse"] = [](List* expr) -> ISExpr* {
//        auto* result = LInterpreter::getInstance().m_parser.parse( expr->m_car->m_atomName, LInterpreter::getInstance().m_globalVariableMap );
//        return result;
//    };



    //--------------------------------------------------------------------------------
    // (eval expr) ->
    //--------------------------------------------------------------------------------
    m_builtInFuncMap["quote"] = new BuiltinFunc( "quote", [](List* expr) -> ISExpr* {
        return expr->m_car;
    });

    // (print (+ a b) (+ d c) )
    m_builtInFuncMap["print"] = new BuiltinFunc( "print", [](List* expr) -> ISExpr* {
        ISExpr* result = LInterpreter::getInstance().m_nilAtom;
        //expr->print("\ndbg: ");

        for( auto* it = expr; it != nullptr; it = it->m_cdr ) {
            result = LInterpreter::getInstance().eval(it->m_car);
            if ( result != nullptr )
            {
                result->print( std::cout );
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
	});

    m_builtInFuncMap["car"] = new BuiltinFunc( "car", [](List* expr) -> ISExpr* {
        if ( expr == nullptr )
        {
            return LInterpreter::getInstance().m_nilAtom;
        }
        return expr->m_car;
    });

    m_builtInFuncMap["cdr"] = new BuiltinFunc( "cdr", [](List* expr) -> ISExpr* {
        if ( expr == nullptr )
        {
            return LInterpreter::getInstance().m_nilAtom;
        }
        return expr->m_cdr;
    });

    m_builtInFuncMap["cons"] = new BuiltinFunc( "cons", [](List* expr) -> ISExpr* {
        if ( expr == nullptr )
        {
            return LInterpreter::getInstance().m_nilAtom;
        }
        expr->print("dbg:cons:");
        std::cout << std::endl;
        
        auto* firstArg = expr->m_car;
        auto* secondArg = expr->m_cdr->m_car->toList();

        return new List( firstArg, secondArg );
    });

   
    // (set x 1) -> 1   
    // (set x) -> nil
    // (set x 1 2 3) -> ?
    // (set x '(1 2 3)) -> (1 2 3)
    m_builtInFuncMap["set"] = new BuiltinFunc( "set", [](List* expr) -> ISExpr*
    {
        if ( expr == nullptr )
        {
            return LInterpreter::getInstance().m_nilAtom;
        }
        auto* var= expr->m_car->toAtom();
        auto* value = (expr->m_cdr==nullptr) ? LInterpreter::getInstance().m_nilAtom
                                                 : LInterpreter::getInstance().eval( expr->m_cdr->m_car );
        var->setValue( value );
        return value;
    });

    // (set x (+ a b c )) -> atom("abc")
    // (+ a (b c) d) -> atom("ad")
    m_builtInFuncMap["+"] = new BuiltinFunc( "+", [](List* expr) -> ISExpr*
    {
        ISExpr* result = nullptr;

        ISExpr::Type returnType = ISExpr::INT_NUMBER;
        for (auto* it = expr; it != nullptr; it = it->m_cdr)
        {
            auto* value = LInterpreter::getInstance().eval( it->m_car );

//            it->toList()->m_car->print("\n? ");
//            LOG( "\ntype: " << it->toList()->m_car->type());
            
            if ( value->type() < returnType )
            {
                returnType = value->type();
            }
        }
        
        switch (returnType)
        {
                // (+ "save x: " 10 567) -> "save x: 10567"
            case ISExpr::ATOM:
            {
                std::string returnValue;

                //expr->print("+ arguments: ");
                for (auto* it = expr; it != nullptr; it = it->m_cdr)
                {
                    //it->m_car->print("\nit->m_car: ");
                    auto* value = LInterpreter::getInstance().eval( it->m_car );
                    //value->print("\nvalue: ");
                    if (value->type() == ISExpr::ATOM) {
                        returnValue += value->toAtom()->name();
                    }
                    else if ( value->type() == ISExpr::DOUBLE ) {
                        std::string str = std::to_string( value->toDouble()->value() );
                        str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
                        str.erase ( str.find_last_not_of('.') + 1, std::string::npos );
                        returnValue += str;
                    }
                    else if ( value->type() == ISExpr::INT_NUMBER ) {
                        returnValue += std::to_string( value->toIntNumber()->value() );
                    }
                }
                return new Atom( returnValue.c_str() );
            }

            case ISExpr::DOUBLE:
            {
                double returnValue = 0;

                //expr->print("+ arguments: ");
                for (auto* it = expr; it != nullptr; it = it->m_cdr)
                {
                    //it->m_car->print("\nit->m_car: ");
                    auto* value = LInterpreter::getInstance().eval( it->m_car );
                    //value->print("\nvalue: ");
                    assert( value->type() != ISExpr::ATOM );
                    if ( value->type() == ISExpr::DOUBLE ) {
                        returnValue += value->toDouble()->value();
                    }
                    else if ( value->type() == ISExpr::INT_NUMBER ) {
                        returnValue += value->toIntNumber()->value();
                    }
                }
                return new Double( returnValue );
            }

            case ISExpr::INT_NUMBER:
            {
                int64_t returnValue = 0;

                //expr->print("+ arguments: ");
                for (auto* it = expr; it != nullptr; it = it->m_cdr)
                {
                    //it->m_car->print("\nit->m_car: ");
                    auto* value = LInterpreter::getInstance().eval( it->m_car );
                    //value->print("\nvalue: ");
                    assert( value->type() != ISExpr::ATOM );
                    assert( value->type() != ISExpr::DOUBLE );
                    if ( value->type() == ISExpr::INT_NUMBER ) {
                        returnValue += value->toIntNumber()->value();
                    }
                }
                return new IntNumber( returnValue );
            }
            default:
                break;
        }
        
    });

}
