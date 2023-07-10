#pragma once

#include "Scanner.h"

#include <iostream>
#include <list>

struct SExpr {
    enum Type {
        LIST=0,
        ATOM
    };

    SExpr() : m_type(LIST), m_next(nullptr) {} // '()'

    SExpr( const char* atomName ) : m_type(ATOM) {
        m_stringValue = new char[ std::strlen(atomName)+1 ];
        std::strcpy( (char*)m_stringValue, atomName );
    }

    ~SExpr() {
        if ( m_type == ATOM ) {
            if ( m_stringValue == nullptr )
            {
                delete [] m_stringValue;
            }
        }
    }

    Type m_type = LIST;

    union
    {
        const char* m_stringValue;
        SExpr*      m_next;
    };

    bool isNil() const { return m_type==LIST && m_next==nullptr; }

    void push_back( SExpr* sExpr)
    {
        if ( m_next == nullptr)
        {
            m_next = sExpr;
        }
        else
        {
            auto* it = m_next;
            while( it->m_next != nullptr )
                it++;
            it->m_next = sExpr;
        }

//        SExpr** it = &m_next;
//        while( (*it) != nullptr )
//            it = &( (*it)->m_next );

//        (*it) = sExpr;
    }

    void print() const {
        switch (m_type) {
        case LIST:
            if ( m_next == nullptr )
            {
                std::cout << "NIL" << std::flush;
            }

            std::cout << "( " << std::flush;
            for( const auto* it = m_next; it != nullptr; it = it->m_next ) {
                it->print();
            }
            std::cout << " )" << std::flush;
            break;
        case ATOM:
            std::cout << m_stringValue << ' ' << std::flush;
            break;
        }
    }
};


//struct SExpression {
//    enum Type {
//        ATOM,
//        NUMBER,
//        LIST
//    };
//    Type m_type = LIST;
//    int  m_numberValue;
//    std::string             m_stringValue;
//    std::list <SExpression> m_list;

//    SExpression(const std::string& atomString) : m_type(ATOM), m_stringValue(atomString){
//        char* ptrEnd;
//        m_numberValue = strtol(m_stringValue.c_str(), &ptrEnd, 10);
//        if (*ptrEnd == char(0)) {
//            m_type = NUMBER;
//        }
//    }
//    SExpression(): m_type(LIST){}
//    void print() const{
//        switch (m_type) {
//        case LIST:
//            std::cout << "( ";
//            for (const auto &item : m_list)
//                item.print();
//            std::cout << " )";
//            break;
//        case ATOM:
//            std::cout << m_stringValue << ' ';
//            break;
//        case NUMBER:
//            std::cout << m_numberValue << ' ';
//            break;
//        }
//    }
//};

class Parser {
    ParserX m_parser;
    std::string m_expression;
    SExpr m_root;
public:
    const SExpr* root() const { return &m_root; }
    void parse(const std::string& expression) {
        m_expression = expression;
        parse(&m_root);
    }
    void parse(SExpr* parent) {
        SExpr* back = parent;
        for (;;) {
            auto token = m_parser.parseToken(m_expression);
            std::cerr << token.m_atom << std::endl;

            switch (token.m_type) {
            case ParserX::LEFT_BRACKET: {
                SExpr* sExpr = new SExpr();
                parse(sExpr);
                parent->push_back( sExpr );
                parent->print();
                break;
            }
            case ParserX::RIGHT_BRACKET:
                return;
            case ParserX::ATOM:
                parent->push_back( new SExpr( token.m_atom.c_str() ) );
                parent->print();
                break;
            case ParserX::END:
                return;
            }
        }
    }

};

