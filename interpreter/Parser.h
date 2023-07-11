#pragma once

#include "Scanner.h"
#include "Log.h"

#include <iostream>
#include <map>

//---------------------------------------------------------------
//
// SExpr - symbolic expression
//
//---------------------------------------------------------------
//
//  Example of how implemented s-expression: ( a b )
//
//  !-------------!        !-------------!
//  ! SExpr: LIST !   +--->! SExpr: LIST !
//  !.............!   |    !.............!
//  ! CAR  ! CDR  !---+    ! CAR  ! CDR  !-->(nullptr)
//  !------!------!        !------!------!
//     |                      |
//     V                      V
//  !-----------------!    !-----------------!
//  ! SExpr: ATOM     !    ! SExpr: ATOM     !
//  !.................!    !.................!
//  ! value ! name: a !    ! value ! name: b !
//  !-------!---------!    !-------!---------!
//
//---------------------------------------------------------------
//
struct SExpr
{
    enum Type {
        LIST=0,
        ATOM
    };

    Type    m_type = LIST;

    // CAR
    union {
        SExpr*      m_car;
        SExpr*      m_atomValue;
    };

    // CDR
    union {
        SExpr*      m_next;
        const char* m_atomName;
    };


    // NIL - "()"
    SExpr() : m_type(LIST), m_car(nullptr), m_next(nullptr)
    {}

    // "( carValue )"
    SExpr( SExpr* carValue ) : m_type(LIST), m_car(carValue), m_next(nullptr)
    {}

    // atom
    SExpr( const char* atomName ) : m_type(ATOM), m_atomValue(this),  m_atomName(atomName) {
        m_atomName = new char[ std::strlen(atomName)+1 ];
        std::strcpy( (char*)m_atomName, atomName );
    }

    ~SExpr() {
        if ( m_type == ATOM ) {
            if ( m_atomName == nullptr )
            {
                delete [] m_atomName;
            }
        }
    }

    bool isNil() const { return m_type==LIST && m_car == nullptr && m_next==nullptr; }

    void setValue( SExpr* value ) { m_car = value; }

    void print( const char* prefix = nullptr ) const
    {
        if ( prefix != nullptr )
        {
            std::cout << prefix;
        }
        
        switch (m_type)
        {
            case LIST:
            {
                std::cout << "( " << std::flush;
                for( const auto* it = this; it != nullptr; it = it->m_next )
                {
                    if ( it->m_car == nullptr && m_next != nullptr )
                    {
                        std::cout << "NIL " << std::flush;
                    }
                    else
                    {
                        it->m_car->print();
                    }
                }
                std::cout << " )" << std::flush;
                break;
            }
            case ATOM:
            {
                std::cout << m_atomName << ' ' << std::flush;
                break;
            }
        }
    }
};

struct ConstCharPtrCompare : public std::binary_function<const char*, const char*, bool> {
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
        return parse();
    }
    
    SExpr* parse()
    {
        SExpr* result = nullptr;
        SExpr* back   = nullptr;
        
        for (;;)
        {
            auto token = m_scanner.parseToken(m_expression);

            switch (token.m_type)
            {
                case Scanner::LEFT_BRACKET:
                {
                    SExpr* sExpr = parse();
                    //sExpr->print();
                    if ( sExpr != nullptr )
                    {
                        if ( result == nullptr )
                        {
                            result = new SExpr( sExpr );
                            back   = result;
                        }
                        else
                        {
                            back->m_next = new SExpr( sExpr );
                            back = back->m_next;
                        }
                    }
                    break;
                }
                case Scanner::RIGHT_BRACKET:
                {
                    if ( result == nullptr )
                    {
                        return new SExpr{};
                    }
                    result->print("\n--- scanner result: ");

                    return result;
                }
                case Scanner::ATOM:
                {
                    LOGVAR( token.m_atom );
                    
                    auto* atom = getAtom( token.m_atom.c_str() );
                    if ( result == nullptr )
                    {
                        result = new SExpr( atom );
                        back   = result;
                    }
                    else
                    {
                        back->m_next = new SExpr( atom );
                        back = back->m_next;
                    }
                    break;
                }
                case Scanner::END:
                {
                    if ( result == nullptr )
                    {
                        return new SExpr();
                    }
                    return result;
                }
            }
        }
    }

};

