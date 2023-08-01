#include "mainwindow.h"
#include "gInterpreter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    std::string code0 = "\
////                            (gSetWindowSize 800 800) \
////                            (set dx 2) \
////                            (set dy 2) \
////                            (set x 10) \
////                            (set y 100) \
//                            (defun paintEvent (painter) \
//                                (gSetRenderHintAntialiasing painter true) \
////                                (set radius 10)\
////                                ( if ( OR (> (+ x dx) (- (widget with) radius) (< (+ x dx) 0))\
////                                    (set dx (- 0 dx)) )\
////                                ( widget setBrush \"black\" )\
//                            ) \
//                        ";
//    std::string code = "( gSetWindowSize 400 400 ) (print(- -7.5 5))";


//    std::string code = "(set x 10) \
//                        (set y 10) \
//                        (set width 800) \
//                        (set height 800) \
//                        \
//                        (defun paintEvent (painter) \
//                            (gSetWindowSize width height) \
//                            (set dx 2) \
//                            (set dy 2) \
//                            (set radius 10) \
//                            \
//                            (if (OR (> (+ x dx) (- width radius) (< (+ x dx) 0)) \
//                                (set dx (- 0 dx)) ) \
//                            \
//                            (if (OR (> (+ y dy) (- height radius) (< (+ y dy) 0)) \
//                                (set dy (- 0 dy)) ) \
//                            \
//                            (set x (+ x dx)) \
//                            (set y (+ y dy)) \
//                            (gSetRenderHintAntialiasing painter true) \
//                            (gSetBrush painter Qt::black) \
//                            (gDrawRect painter 0 0 width height) \
//                            (gSetBrush painter Qt::blue) \
//                            (gDrawEllipse painter 100 200 10 10) \
//                        )";
    //TODO:


        std::string code = "(set x 10) \
                            (set y 10) \
                            (set width 800) \
                            (set height 700) \
                            (set dx 2) \
                            (set dy 2) \
                            (set radius 100)\
                            \
                            (defun paintEvent (painter) \
                                (gSetWindowSize width height) \
                                (gSetRenderHintAntialiasing painter true) \
                                (if ( OR ( > (+ x dx) (- width radius) ) (< (+ x dx) 0) ) \
                                    (set dx (- 0 dx)) ) \
                                (if ( OR ( > (+ y dy) (- height radius) ) (< (+ y dy) 0) ) \
                                    (set dy (- 0 dy)) ) \
                                (set x (+ x dx)) \
                                (set y (+ y dy)) \
                                (gSetBrush painter Qt::black) \
                                (gDrawRect painter 0 0 width height) \
                                (gSetBrush painter Qt::blue) \
                                (gDrawEllipse painter x y radius radius) \
                            )";



//    std::string code = "(set x 11) (print x)";

//        std::string code = "(if ( OR ( > 5 10) (> 4 10) )  (print(+ 10 10)) )";
    auto& interpreter = gInterpreter::getInstance();
    return interpreter.exec(argc, argv, code);


//    std::string code = "(print(- -7.5 5))";
//    std::string code = "(print(OR ( < 5 10) (< 4 10) )            )";
//std::string code = "(print(13123412412))";

//std::string code = "(set x 11) (print x)";
//LInterpreter& lInterpreter = LInterpreter::getInstance();
//while( lInterpreter.eval(code) != nullptr ) {
//}

//    LInterpreter& lInterpreter = LInterpreter::getInstance();
//    while( lInterpreter.eval(code) != nullptr ) {
//    }
    return 0;
}
