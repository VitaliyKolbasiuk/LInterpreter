#include "mainwindow.h"
#include "gInterpreter.h"

//(( gSetWindowSize 800 800 ) ( set dx 2 ) ( set dy 2 ) (set x 10) (set y 100)\
//                        ( defun paintEvent (painter) )\
//                             ( painter setRenderHintAntialiasing true )\
//                             ( set radius 10 )\
//                            ( if ( OR (> (+ x dx) (- (painter with) radius) (< (+ x dx) 0))\
//                                    (set dx (- 0 dx)) )\
//                             ( painter setBrush \"black\" ))

void MainWidget::paintEvent(QPaintEvent* event)
{
    // Create QPainter
    QPainter qPainter(this);
    Custom<QPainter> painter( &qPainter );

    // Get 'paintEvent' function definition
    auto* paintEventAtom = gInterpreter::getInstance().getAtom( "paintEvent" );
    if ( paintEventAtom == nullptr )
    {
        LOG_ERR( "Undefined function 'paintEvent'" );
        return;
    }
    
    // !!! evaluate: '(paintEvent painter)'
    List argument(&painter);
    List funcCall( paintEventAtom, &argument );
    gInterpreter::getInstance().eval( &funcCall );
}

gInterpreter::gInterpreter()
{
    m_builtInFuncMap["gSetWindowSize"] = new BuiltinFunc( "gSetWindowSize", [](List* expr) -> ISExpr*
    {
        auto* width = gInterpreter::getInstance().eval( expr->m_car );
        auto* height = gInterpreter::getInstance().eval( expr->m_cdr->m_car );
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
    
    // gSetRenderHintAntialiasing
    m_builtInFuncMap["gSetRenderHintAntialiasing"] = new BuiltinFunc( "gSetRenderHintAntialiasing", [](List* expr) -> ISExpr*
    {
        auto* painter = to<QPainter>( expr->m_car->toAtom()->value() );
        auto* qPainter = painter->value();
        bool  booleanValue = expr->m_cdr->m_car != nullptr;
        qPainter->setRenderHint( QPainter::Antialiasing, booleanValue );
    });
    
    // gDrawEllipse
    m_builtInFuncMap["gDrawEllipse"] = new BuiltinFunc( "gDrawEllipse", [](List* expr) -> ISExpr*
    {
        auto* painter = to<QPainter>( expr->m_car->toAtom()->value() );
        auto* qPainter = painter->value();
        expr = expr->m_cdr;
        
        int x = gInterpreter::getInstance().eval( expr->m_car )->toIntNumber()->value();
        expr = expr->m_cdr;

        int y = gInterpreter::getInstance().eval( expr->m_car )->toIntNumber()->value();
        expr = expr->m_cdr;

        int xRadius = gInterpreter::getInstance().eval( expr->m_car )->toIntNumber()->value();
        expr = expr->m_cdr;

        int yRadius = gInterpreter::getInstance().eval( expr->m_car )->toIntNumber()->value();
        expr = expr->m_cdr;

        qPainter->drawEllipse( x, y, xRadius, yRadius );
    });

}

int gInterpreter::exec(int argc, char* argv[], std::string code){
    QApplication a(argc, argv);
    w = new MainWindow;

    widget = Custom<MainWidget>( new MainWidget );
//    widgetAtom = getAtom( "MainWidget" );
//    widgetAtom.setValue( &widget );
    
    eval(code);

    w->centralWidget()->setLayout(new QVBoxLayout);
    w->centralWidget()->layout()->addWidget( widget.value() );
    w->show();
    return a.exec();
}
