#pragma once

#include "Parser.h"
#include "Log.h"

#include <iostream>
#include <map>
#include <functional>
#include <stack>

using BuiltInFunc = std::function< SExpr* (SExpr*) >;

class LInterpreter {
    Parser m_parser;
	LInterpreter();

public:
	std::map<std::string, BuiltInFunc>  m_builtInFuncMap;
    std::map<std::string, SExpr*>       m_userFuncMap;
    
    NameToSExprMap              m_globalVariableMap;
	std::stack<NameToSExprMap>  m_stack;

	static LInterpreter& getInstance() {
		static LInterpreter instance;
		return instance;
	}

	void execute(const std::string& lText) {
        auto* exprs = m_parser.parse( lText, m_globalVariableMap );
        exprs->print();
        std::cout << std::endl << std::endl;
        std::cout << "execution: " << std::endl;
        for( auto* it = exprs; it != nullptr; it = it->m_next ) {
			execute(it->m_car);
		}
	}

    SExpr* execute(SExpr* sExpr) {
        switch (sExpr->m_type) {
        case SExpr::ATOM:
			LOG("ATOM");
            if (auto it = m_globalVariableMap.find(sExpr->m_atomName);
				it != m_globalVariableMap.end())
			{
				return it->second;
			}
			else {
				return sExpr;
			}
        case SExpr::LIST:
            if (sExpr->isNil() ) {
				LOG("NULL");
                return sExpr;
			}
			else {
                auto* funcName = sExpr->m_car;
                if ( funcName->m_type == SExpr::ATOM )
                {
                    //LOGVAR(funcName->m_atomName);

                    if ( auto it =  m_builtInFuncMap.find(funcName->m_atomName);
                              it != m_builtInFuncMap.end() )
                    {
                        return it->second( sExpr->m_next );
                    }
                    else
                    {
                        // user function map
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
        return new SExpr{};
	}
};
