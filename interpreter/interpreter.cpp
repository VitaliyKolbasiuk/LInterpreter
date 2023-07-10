#include "Scanner.h"
#include "Parser.h"
#include "LInterpreter.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    //string input = "(print (quote (a b c)) (quote (1 2 3)))";
    //string input = "(setvar x (quote a)) (print x)";
    //string input = "(setvar c (quote x)) (defun f(a b) (+ (a b c)) (+ (a b c c))) (print (f x y))";
    string input = "(print (+ (+ a b)(+ c d)))";
    LInterpreter& lInterpreter = LInterpreter::getInstance();
    lInterpreter.execute(input);
    return 0;
}