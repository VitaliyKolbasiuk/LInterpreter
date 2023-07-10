#pragma once

#include <iostream>

class ParserX {
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

inline std::ostream& operator<<(std::ostream& os, ParserX::Token const& token) {
    switch (token.m_type) {
    case ParserX::LEFT_BRACKET: {
        os << "LEFT_BRACKET";
        break;
    }
    case ParserX::RIGHT_BRACKET:
        os << "RIGHT_BRACKET";
        break;
    case ParserX::ATOM:
        os << token.m_atom;
        break;
    case ParserX::END:
        os << "END";
        break;
    }
    return os;
}

    
