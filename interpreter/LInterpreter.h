#pragma once

#include "Parser.h"
#include "Log.h"

#include <iostream>
#include <map>
#include <functional>
#include <forward_list>

using BuiltInFunc = std::function< ISExpr* (List*) >;

class LInterpreter {
public:
    Atom*  m_nilAtom = new Atom("nil");

private:
    Parser  m_parser;
    
    LInterpreter();
    void addPseudoTableFuncs();
public:
	std::map<std::string, BuiltInFunc>  m_builtInFuncMap;
    std::map<std::string, List*>        m_userFuncMap;
    
    NameToSExprMap m_globalVariableMap;

	static LInterpreter& getInstance() {
		static LInterpreter instance;
		return instance;
	}

    ISExpr* eval(const std::string& lText) {
        auto* expr = m_parser.parse( lText, m_globalVariableMap );
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
                return ((Atom*)sExpr0)->value();
            }
            case ISExpr::LIST:
            {
                List* sExpr = (List*) sExpr0;
                if (sExpr->isEmptyList() ) {
                    LOG("NULL");
                    return sExpr;
                }
                else {
                    auto* funcName = sExpr->m_car;
                    if ( funcName->type() == ISExpr::ATOM )
                    {
                        //LOGVAR( funcName->m_atomName );

                        if ( auto it =  m_builtInFuncMap.find( ((Atom*)funcName)->name() );
                                  it != m_builtInFuncMap.end() )
                        {
                            // applay lambda function
                            return it->second( sExpr->m_cdr );
                        }
                        else
                        {
                            //LOG( "function '" << ((Atom*)funcName)->name() << "' not defined" );
                            return evalUserDefinedFunc( sExpr );
                        }
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
        auto* funcName = sExpr->m_car;
        auto* parameters = sExpr->m_cdr;
        //parameters->print("\nparameters:");

        auto* funcDefinition = ((Atom*)funcName)->value();
        //funcDefinition->print("\nfuncDefinition:");
        
        if ( funcDefinition->type() != ISExpr::LIST )
        {
            std::cerr << "\nbad definition of user function: ";
            funcDefinition->print( std::cerr );
            std::cerr << "\n";
            return m_nilAtom;
        }

        auto* argList = (List*) ((List*)funcDefinition)->m_car;
        //argList->print("\nargList:");

        auto* funcBody = ((List*)funcDefinition)->m_cdr->m_car;
        //funcBody->print("\nfuncBody:");
        
        // copy
        struct AtomValue { Atom* atom; ISExpr* value; };
        std::forward_list<AtomValue> savedList;
        
        auto* parameterIt = parameters;
        for( auto* it = argList; (it != nullptr); it = it->m_cdr )
        {
//            it->m_car->m_atomValue->print("\nbefore save:");

            savedList.push_front( AtomValue{ (Atom*)it->m_car, ((Atom*)(it->m_car))->value() } );
            
            if ( parameterIt != nullptr )
            {
                // substitute/replace argumement value by parameters
                //parameterIt->m_car->print("\n");
                ((Atom*)(it->m_car))->setValue( parameterIt->m_car );
                parameterIt = parameterIt->m_cdr;
            }
            else
            {
                ((Atom*)(it->m_car))->setValue( m_nilAtom );
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
