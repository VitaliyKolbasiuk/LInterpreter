#include "Scanner.h"
#include "Parser.h"
#include "LInterpreter.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    //string input = "(print (a b c))";
    //string input = "(print (print1 (a b c)) (print2 (a b c)) )";
    //string input = "(print (quote (a b c)) (quote (1 2 3)))";
    //string input = "(setvar x (quote a)) (print x)";
    //string input = "(setvar c (quote x)) (defun f(a b) (+ (a b c)) (+ (a b c c))) (print (f x y))";
    //string input = "(print (+ (+ a b)(+ c d)))";
    
    
    string input = "(print a) (print b) ";
    
    LInterpreter& lInterpreter = LInterpreter::getInstance();
    lInterpreter.eval(input);
    std::cout << "\n\nLInterpreter ended\n";
    return 0;
}
