#pragma once

#include "Scanner.h"
#include "SExpr.h"
#include "Log.h"

#include <iostream>
#include <map>
#include <functional>


struct ConstCharPtrCompare {
public:
    bool operator() (const char* str1, const char* str2) const
    { return std::strcmp(str1, str2) < 0; }
};
using NameToSExprMap = std::map<const char*, SExpr*, ConstCharPtrCompare>;


//
// Parser
//
class Parser
{
    Scanner     m_scanner;
    std::string m_expression;

    NameToSExprMap* m_globalVariableMap;

    SExpr* getAtom( const char* name )
    {
        if ( auto it = m_globalVariableMap->find( name ); it != m_globalVariableMap->end() )
        {
            return it->second;
        }
        
        auto* atom = new SExpr(name);
        (*m_globalVariableMap)[atom->m_atomName] = atom;
        return atom;
    }

public:
    SExpr* parse( const std::string& expression, NameToSExprMap& globalVariableMap ) {
        m_globalVariableMap = &globalVariableMap;
        m_expression = expression;
        SExpr* expr = parse();
        return expr;
    }
    
    SExpr* parse()
    {
        auto token = m_scanner.parseToken(m_expression);
        switch (token.m_type)
        {
            case Scanner::LEFT_BRACKET:
            {
                return parseList();
            }
            case Scanner::RIGHT_BRACKET:
            {
                std::cerr << "unexpected ')'";
                return nullptr;
            }
            case Scanner::ATOM:
            {
                return  getAtom( token.m_atom.c_str() );
            }
            case Scanner::END:
            {
                return nullptr;
            }
        }
    }
    
    SExpr* parseList()
    {
        SExpr* result = new SExpr{};
        SExpr* back   = result;

        for (;;)
        {
            auto token = m_scanner.parseToken(m_expression);

            switch (token.m_type)
            {
                case Scanner::LEFT_BRACKET:
                {
                    //std::cout << "\n-LB---";
                    SExpr* sExpr = parseList();
                    //sExpr->print("\n-sExpr---");
                    if ( sExpr != nullptr )
                    {
                        if ( result->m_car == nullptr )
                        {
                            result->m_car = sExpr;
                        }
                        else
                        {
                            back->m_cdr = new SExpr( sExpr );
                            back = back->m_cdr;
                        }
                    }
                    //result->print("\n-list-result---");
                    break;
                }
                case Scanner::RIGHT_BRACKET:
                {
                    //result->print("\n--RB-- parser result: ");
                    return result;
                }
                case Scanner::ATOM:
                {
                    //LOGVAR( token.m_atom );
                    
                    auto* atom = getAtom( token.m_atom.c_str() );
                    if ( result->m_car == nullptr )
                    {
                        result->m_car = atom;
                    }
                    else
                    {
                        back->m_cdr = new SExpr( atom );
                        back = back->m_cdr;
                    }
                    //result->print("\n-atom-result---");
                    break;
                }
                case Scanner::END:
                {
                    return nullptr;
                }
            }
        }
    }
};

