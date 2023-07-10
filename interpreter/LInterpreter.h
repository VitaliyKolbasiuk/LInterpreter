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
	std::map<std::string, BuiltInFunc> m_builtInFuncMap;
    std::map<std::string, SExpr*> m_userFuncMap;
    std::map<std::string, SExpr*> m_variableMap;

    using LocalVariableMap = std::map<std::string, SExpr*>;
	std::stack<LocalVariableMap> m_stack;

	static LInterpreter& getInstance() {
		static LInterpreter instance;
		return instance;
	}

	void execute(const std::string& lText) {
        m_parser.parse(lText);
        const auto* root = m_parser.root();
        root->print();
        for( auto* it = root->m_next; it != nullptr; it = it->m_next ) {
			execute(it);
		}
	}

    SExpr* execute(SExpr* sExpr) {
        switch (sExpr->m_type) {
        case SExpr::ATOM:
			LOG("ATOM");
            if (auto it = m_variableMap.find(sExpr->m_stringValue);
				it != m_variableMap.end())
			{
				return it->second;
			}
			else {
				return sExpr;
			}
        case SExpr::LIST:
			LOG("LIST");
            if (sExpr->isNil() ) {
				LOG("NULL");
                return sExpr;
			}
			else {
                auto* funcName = sExpr->m_next;
                if ( funcName->m_type == SExpr::ATOM )
                {
                    LOGVAR(funcName->m_stringValue);

                    if ( auto it =  m_builtInFuncMap.find(funcName->m_stringValue);
                              it != m_builtInFuncMap.end() )
                    {
                        return it->second( sExpr->m_next->m_next );
                    }
                    else
                    {
                        // user function map
                    }
                }
                else
                {
                    LOG("must be a function name!");
                }
			}
			break;
		}
        return new SExpr{};
	}
};
