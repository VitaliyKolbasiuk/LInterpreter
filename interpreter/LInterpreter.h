#pragma once

#include "Parser.h"
#include "Log.h"

#include <iostream>
#include <map>
#include <functional>
#include <forward_list>

using BuiltInFunc = std::function< SExpr* (SExpr*) >;

class LInterpreter {
public:
    SExpr*  m_nilAtom = new SExpr("nil");

private:
    Parser  m_parser;
    
    LInterpreter();

public:
	std::map<std::string, BuiltInFunc>  m_builtInFuncMap;
    std::map<std::string, SExpr*>       m_userFuncMap;
    
    NameToSExprMap m_globalVariableMap;

	static LInterpreter& getInstance() {
		static LInterpreter instance;
		return instance;
	}

    SExpr* eval(const std::string& lText) {
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

    SExpr* eval(SExpr* sExpr)
    {
        switch (sExpr->m_type)
        {
            case SExpr::ATOM:
            {
                return sExpr->m_atomValue;
            }
            case SExpr::LIST:
            {
                if (sExpr->isNil() ) {
                    LOG("NULL");
                    return sExpr;
                }
                else {
                    auto* funcName = sExpr->m_car;
                    if ( funcName->m_type == SExpr::ATOM )
                    {
                        //LOGVAR( funcName->m_atomName );

                        if ( auto it =  m_builtInFuncMap.find(funcName->m_atomName);
                                  it != m_builtInFuncMap.end() )
                        {
                            return it->second( sExpr->m_cdr );
                        }
                        else
                        {
                            LOG( "function '" << funcName->m_atomName << "' not defined" );
                            evalUserDefinedFunc( sExpr );
                        }
                    }
                    else
                    {
                        funcName->print();
                        LOG("must be a function name!");
                    }
                }
                break;
            }
		}
        return new SExpr{};
	}
    
    SExpr* evalUserDefinedFunc( SExpr* sExpr )
    {
        sExpr->print("sExpr:");
        auto* funcName = sExpr->m_car;
        auto* parameters = sExpr->m_cdr;
//        parameters->print("\nparameters:");

        auto* funcDefinition = funcName->m_atomValue;
//        funcDefinition->print("\nfuncDefinition:");

        auto* argList = funcDefinition->m_car;
//        argList->print("\nargList:");

        auto* funcBody = funcDefinition->m_cdr->m_car;
  //      funcBody->print("\nfuncBody:");
        
        // copy
        struct AtomValue { SExpr* atom; SExpr* value; };
        std::forward_list<AtomValue> savedList;
        
        auto* parameterIt = parameters;
        for( auto* it = argList; (it != nullptr); it = it->m_cdr )
        {
//            it->m_car->m_atomValue->print("\nbefore save:");

            savedList.push_front( AtomValue{ it->m_car, it->m_car->m_atomValue } );
            
            if ( parameterIt != nullptr )
            {
                // substitute/replace argumement value by parameters
                parameterIt->m_car->print("\n");
                it->m_car->m_atomValue = parameterIt->m_car;
                parameterIt = parameterIt->m_cdr;
            }
            else
            {
                it->m_car->m_atomValue = m_nilAtom;
            }
//            it->m_car->m_atomValue->print("\nafter save:");
        }

        eval( funcBody );

        // restore atom values
        while( !savedList.empty() )
        {
            auto& front = savedList.front();
            auto* atom = front.atom;
            auto* value = front.value;
            
//            atom->m_car->print("\nbefore restore:");
            atom->m_car = value;
//            atom->m_car->print("\nafter restore:");
            
            savedList.pop_front();
        }

        return nullptr;
    }
};
