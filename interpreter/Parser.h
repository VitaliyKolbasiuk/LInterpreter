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
using NameToSExprMap = std::map<const char*, Atom*, ConstCharPtrCompare>;


//
// Parser
//
class Parser
{
    Scanner     m_scanner;
    std::string m_expression;

    NameToSExprMap* m_globalVariableMap;

    Atom* getAtom( const char* name )
    {
        if ( auto it = m_globalVariableMap->find( name ); it != m_globalVariableMap->end() )
        {
            return it->second;
        }
        
        auto* atom = new Atom(name);
        (*m_globalVariableMap)[atom->name()] = atom;
        return atom;
    }

public:
    ISExpr* parse( const std::string& expression, NameToSExprMap& globalVariableMap ) {
        m_globalVariableMap = &globalVariableMap;
        m_expression = expression;
        ISExpr* expr = parse();
        return expr;
    }
    
    ISExpr* parse()
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
                // не число ли это -> new IntNumber() or DoubleNumber()
                return  getAtom( token.m_atom.c_str() );
            }
            case Scanner::STRING:
            {
                return  getAtom( token.m_atom.c_str() );
            }
            case Scanner::END:
            {
                return nullptr;
            }
        }
    }
    
    List* parseList()
    {
        List* result = new List();
        List* back   = result;

        for (;;)
        {
            auto token = m_scanner.parseToken(m_expression);

            switch (token.m_type)
            {
                case Scanner::LEFT_BRACKET:
                {
                    //std::cout << "\n-LB---";
                    List* sExpr = parseList();
                    //sExpr->print("\n-sExpr---");
                    if ( sExpr != nullptr )
                    {
                        if ( result->m_car == nullptr )
                        {
                            result->m_car = sExpr;
                        }
                        else
                        {
                            back->m_cdr = new List( sExpr );
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
                        back->m_cdr = new List( atom );
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

