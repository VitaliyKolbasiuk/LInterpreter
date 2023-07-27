#pragma once

#include "Parser.h"
#include "Log.h"

#include <iostream>
#include <map>
#include <functional>
#include <forward_list>

class LInterpreter {
public:
    Atom*  m_nilAtom = new Atom("nil");

protected:
    Parser  m_parser;
    
    LInterpreter();
    void addPseudoTableFuncs();
public:
    BuiltinFuncMap m_builtInFuncMap;
    
    NameToSExprMap m_globalVariableMap;

	static LInterpreter& getInstance() {
		static LInterpreter instance;
		return instance;
	}

    ISExpr* eval(const std::string& lText) {
        auto* expr = m_parser.parse( lText, m_globalVariableMap, m_builtInFuncMap );
        if ( expr == nullptr )
        {
            return nullptr;
        }
        std::cout << std::endl << std::endl;
        expr->print("\n# evaluation of: ");
        std::cout << std::endl;
        return eval( expr );
	}

    ISExpr* eval(ISExpr* sExpr0)
    {
        switch (sExpr0->type())
        {
            case ISExpr::ATOM:
            {
                return sExpr0->toAtom()->value();
            }
            case ISExpr::DOUBLE:
            case ISExpr::INT_NUMBER:
            {
                return sExpr0;
            }
            case ISExpr::LIST:
            {
                List* sExpr = sExpr0->toList();
                if (sExpr->isEmptyList() ) {
                    LOG("NULL");
                    return sExpr;
                }
                else {
                    auto* funcName = sExpr->m_car;
                    if ( funcName->type() == ISExpr::BUILT_IN_FUNC )
                    {
                        return funcName->toBuiltinFunc()->func()( sExpr->m_cdr );
                    }

                    if ( funcName->type() == ISExpr::ATOM )
                    {
                        //LOGVAR( funcName->m_atomName );
                        //LOG( "function '" << ((Atom*)funcName)->name() << "' not defined" );
                        return evalUserDefinedFunc( sExpr );
                    }
                    else
                    {
                        funcName->print("error: ");
                        LOG("must be a function name!");
                    }
                }
                break;
            }
		}
        return m_nilAtom;
	}
    
    ISExpr* evalUserDefinedFunc( List* sExpr )
    {
        //sExpr->print("sExpr:");
        auto* funcName = sExpr->m_car->toAtom();
        auto* parameters = sExpr->m_cdr;
        //parameters->print("\nparameters:");

        auto* funcDefinition = funcName->value()->toList();
        //funcDefinition->print("\nfuncDefinition:");
        
        if ( funcDefinition->type() != ISExpr::LIST )
        {
            std::cerr << "\nbad definition of user function: ";
            funcDefinition->print( std::cerr );
            std::cerr << "\n";
            return m_nilAtom;
        }

        auto* argList = funcDefinition->m_car->toList();
        //argList->print("\nargList:");

        auto* funcBody = funcDefinition->m_cdr->m_car->toList();
        //funcBody->print("\nfuncBody:");
        
        // copy
        struct AtomValue { Atom* atom; ISExpr* value; };
        std::forward_list<AtomValue> savedList;
        
        auto* parameterIt = parameters;
        for( auto* it = argList; (it != nullptr); it = it->m_cdr )
        {
//            it->m_car->m_atomValue->print("\nbefore save:");

            savedList.push_front( AtomValue{ it->m_car->toAtom(), it->m_car->toAtom()->value() } );
            
            if ( parameterIt != nullptr )
            {
                // substitute/replace argumement value by parameters
                //parameterIt->m_car->print("\n");
                it->m_car->toAtom()->setValue( parameterIt->m_car );
                parameterIt = parameterIt->m_cdr;
            }
            else
            {
                it->m_car->toAtom()->setValue( m_nilAtom );
            }
//            it->m_car->m_atomValue->print("\nafter save:");
        }

        auto retValue = eval( funcBody );

        // restore atom values
        while( !savedList.empty() )
        {
            auto& front = savedList.front();
            auto* atom = front.atom;
            auto* value = front.value;
            
//            atom->m_car->print("\nbefore restore:");
            atom->setValue( value );
//            atom->m_car->print("\nafter restore:");
            
            savedList.pop_front();
        }

        return retValue;
    }
};
