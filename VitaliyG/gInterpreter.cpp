#include "mainwindow.h"
#include "gInterpreter.h"

//(( gSetWindowSize 800 800 ) ( set dx 2 ) ( set dy 2 ) (set x 10) (set y 100)\
//                        ( defun aintEvent (painter) )\
//                             ( painter setRenderHintAntialiasing true )\
//                             ( set radius 10 )\
//                            ( if ( OR (> (+ x dx) (- (painter with) radius) (< (+ x dx) 0))\
//                                    (set dx (- 0 dx)) )\
//                             ( painter setBrush \"black\" ))

void MainWidget::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    gInterpreter::getInstance().eval("(paintEvent 0)");

}

gInterpreter::gInterpreter()
{
    m_builtInFuncMap["gSetWindowSize"] = new BuiltinFunc( "gSetWindowSize", [](List* expr) -> ISExpr* {
        auto* width = expr->m_car;
        auto* height = expr->m_cdr->m_car;
        if (width->type() != ISExpr::INT_NUMBER){
            LOG_ERR("gSetWindowSize: invalid 1-st parametr type");
            return nullptr;
        }
        if (height->type() != ISExpr::INT_NUMBER){
            LOG_ERR("gSetWindowSize: invalid 2-d parametr type");
            return nullptr;
        }
        int w = width->toIntNumber()->value();
        int h = height->toIntNumber()->value();
        gInterpreter::getInstance().w->resize(w, h);
        return nullptr;
    });
}

int gInterpreter::exec(int argc, char* argv[], std::string code){
    QApplication a(argc, argv);
    w = new MainWindow;

    eval(code);

    widget = new MainWidget;
    w->centralWidget()->setLayout(new QVBoxLayout);
    w->centralWidget()->layout()->addWidget(widget);
    w->show();
    return a.exec();
}
