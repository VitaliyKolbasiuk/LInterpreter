#include "mainwindow.h"
#include "gInterpreter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    std::string code = "(( gSetWindowSize 800 800 ) ( set dx 2 ) ( set dy 2 ) (set x 10) (set y 100)\
//                        ( defun aintEvent (widget) )\
//                             ( widget setRenderHintAntialiasing true )\
//                             ( set radius 10 )\
//                            ( if ( OR (> (+ x dx) (- (widget with) radius) (< (+ x dx) 0))\
//                                    (set dx (- 0 dx)) )\
//                             ( widget setBrush \"black\" ))";
//    std::string code = "( gSetWindowSize 400 400 ) (print(- -7.5 5))";
//    auto& interpreter = gInterpreter::getInstance();
//    return interpreter.exec(argc, argv, code);



//    std::string code = "(print(- -7.5 5))";
//    std::string code = "(print(OR ( < 5 10) (< 4 10) )            )";
    std::string code = "(if ( OR ( > 5 10) (< 4 10) )  (print(+ 10 10)) )";
//std::string code = "(print(13123412412))";
    LInterpreter& lInterpreter = LInterpreter::getInstance();
    while( lInterpreter.eval(code) != nullptr ) {
    }
    return 0;
}
