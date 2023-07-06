#pragma once
#include <iostream>
#include <list>
#include "Parser.h"

struct SExpression {
    enum Type {
        ATOM,
        NUMBER,
        LIST
    };
    Type m_type = LIST;
    int  m_numberValue;
    std::string             m_stringValue;
    std::list <SExpression> m_list;

    SExpression(const std::string& atomString) : m_type(ATOM), m_stringValue(atomString){
        char* ptrEnd;
        m_numberValue = strtol(m_stringValue.c_str(), &ptrEnd, 10);
        if (*ptrEnd == char(0)) {
            m_type = NUMBER;
        }
    }
    SExpression(): m_type(LIST){}
    void print() const{
        switch (m_type) {
        case LIST:
            std::cout << "( ";
            for (const auto &item : m_list)
                item.print();
            std::cout << " )";
            break;
        case ATOM:
            std::cout << m_stringValue << ' ';
            break;
        case NUMBER:
            std::cout << m_numberValue << ' ';
            break;
        }
    }
};

class Scanner {
    Parser m_parser;
    std::string m_expression;
    SExpression m_root;
public:
    const SExpression& root() const { return m_root; }
    void scan(const std::string& expression) {
        m_expression = expression;
        scan(m_root);
    }
    void scan(SExpression& parent) {
        for (;;) {
            auto token = m_parser.parseToken(m_expression);
            std::cerr << token.m_atom << std::endl;
            switch (token.m_type) {
            case Parser::LEFT_BRACKET: {
                SExpression sExpression;
                scan(sExpression);
                parent.m_list.push_back(sExpression);
                break;
            }
            case Parser::RIGHT_BRACKET:
                return;
            case Parser::ATOM:
                parent.m_list.push_back(token.m_atom);
                break;
            case Parser::END:
                return;
            }
        }
    }

};

    