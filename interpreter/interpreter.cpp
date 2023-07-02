#include <iostream>
#include <string>
#include <vector>
#include "Scanner.h"
#include "Parser.h"
#include "LInterpreter.h"
using namespace std;

int main() {
    string input = "(defun function_name(p1 p2 p3 p4)(+ p1 p2 p3 p4))";
    /*Scanner scanner;
    scanner.scan(input);
    scanner.root().print();*/
    LInterpreter lInterpreter;
    lInterpreter.execute(input);
    /*Parser parse;
    vector<Token> tokens = parse.parseToken(input);
    for (auto token : tokens) {
        switch (token.m_type) {
            case LEFT_BRACKET:
                cout << '(' << endl;
                break;
            case RIGHT_BRACKET:
                cout << ')' << endl;
                break;
            case END:
                return 0;
            default:
                cout << token.m_atom << endl;
                break;
        }
    }*/
    return 0;
}