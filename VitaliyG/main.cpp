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
    std::string code = "( gSetWindowSize 400 400 )";
    auto& interpreter = gInterpreter::getInstance();
    return interpreter.exec(argc, argv, code);
}
