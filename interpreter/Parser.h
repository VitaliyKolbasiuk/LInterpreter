#pragma once

#include <iostream>

class Parser {
    int pos = 0;
public:
    enum TokenType {
        LEFT_BRACKET,
        RIGHT_BRACKET,
        ATOM,
        END
    };

    struct Token {

        Token(TokenType type) : m_type(type) {}
        TokenType m_type;
        std::string    m_atom;
    };


    Token parseToken(const std::string& expression) {
        int len = expression.length();
        while (pos < len) {
            char c = expression[pos];
            switch (c) {
            case '(':
                pos++;
                return Token(LEFT_BRACKET);
            case ')':
                pos++;
                return Token(RIGHT_BRACKET);
            case ' ':
                pos++;
                break; // nothing
            default:
                Token atom(ATOM);
                while (pos < len && expression[pos] != '(' && expression[pos] != ')' && expression[pos] != ' ') {
                    atom.m_atom.push_back(expression[pos]);
                    pos++;
                }
                return atom;
            }
        }
        return Token(END);

    }
};

inline std::ostream& operator<<(std::ostream& os, Parser::Token const& token) {
    switch (token.m_type) {
    case Parser::LEFT_BRACKET: {
        os << "LEFT_BRACKET";
        break;
    }
    case Parser::RIGHT_BRACKET:
        os << "RIGHT_BRACKET";
        break;
    case Parser::ATOM:
        os << token.m_atom;
        break;
    case Parser::END:
        os << "END";
        break;
    }
    return os;
}