#include "Scanner.h"
#include "Parser.h"
#include "LInterpreter.h"

#include <iostream>
#include <string>
#include <vector>

/*
 HomeWork:
 
 size_t width = 120+1;
 size_t height = 60 or 40;
 std::string text( width*height, ' ' );
 for( int i=0; i<height; i++ )
 {
     text[i*width] = '\n';
 }
 //https://symbl.cc/ru/unicode/blocks/box-drawing/
 
 char* cell( int posX, int posY ) { text[posX+posY*width]; };

 text[ posX + posY*width ] = '┌';  <--> cell(posX,posY) = '┌';

 */



using namespace std;

int main() {
    //string input = "(print (a b c))";
    //string input = "(print (print1 (a b c)) (print2 (a b c)) )";
    //string input = "(print (quote (a b c)) (quote (1 2 3)))";
    //string input = "(setvar x (quote a)) (print x)";
    //string input = "(setvar c (quote x)) (defun f(a b) (+ (a b c)) (+ (a b c c))) (print (f x y))";
    //string input = "(print (+ (+ a b)(+ c d)))";
    //string input = "(print (+ (+ a b)(+ c d)))";
    //string input = "(print a) (print b) ";
    
    string input = "(set p1 xxx) (defun test (p1 p2 p3) (print (+ p1 p2 p3)) ) (test a b c) (print p1)";

    LInterpreter& lInterpreter = LInterpreter::getInstance();
    while( lInterpreter.eval(input) != nullptr ) {
    }
    std::cout << "\n\n# LInterpreter ended\n\n";
    return 0;
}
