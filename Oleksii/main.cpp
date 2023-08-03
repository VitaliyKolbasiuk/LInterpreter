#include "mainwindow.h"
#include "gInterpreter.h"

#include <QApplication>
#include <filesystem>

#define TO_CSTRING(x) #x

int main(int argc, char *argv[])
{
//    auto* i  = new IntNumber(11);
//    auto* ii = new IntNumber(11);
//    ii->setDoubleValue(12);
//    auto* d  = new Double(12);
//
//    new(ii) Atom("atom");
//    ii->print0("ii:");
//
//    LOG( "\n::: " << i->type() << " " << ii->type() << " " << d->type() );
//    return;

    #define STRING(x) x
    #define XSTRING(x) STRING(x)
//    std::string testFileName = std::string(XSTRING(PROJECT_DIR)) + "/../test.l";
    std::string testFileName = std::string(XSTRING(PROJECT_DIR)) + "/test.l";
    //LOG( testFileName );

    auto& interpreter = gInterpreter::getInstance();
    if ( std::filesystem::exists( testFileName ) )
    {
        return interpreter.execFile( argc, argv, testFileName );
    }
    else
    {
        LOG_ERR( "test file does not exist:" << testFileName );
        exit(1);
    }


//    std::string code = "(set x 11) (print x)";

//        std::string code = "(if ( OR ( > 5 10) (> 4 10) )  (print(+ 10 10)) )";
//    auto& interpreter = gInterpreter::getInstance();
//    return interpreter.exec(argc, argv, code);


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
