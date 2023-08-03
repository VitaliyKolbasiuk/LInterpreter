#include "mainwindow.h"
#include "gInterpreter.h"
#include <QMouseEvent>


void MainWidget::mouseMoveEvent( QMouseEvent* qEvent )
{
    if ( gInterpreter::getInstance().m_mouseMoveEventAtom->value()->type() != ISExpr::LIST )
    {
        LOG_ERR( "Undefined function 'mouseMoveEvent'" );
        return;
    }
    
    auto qX = qEvent->pos().x();
    auto qY = qEvent->pos().y();

    IntNumber x(qX);
    IntNumber y(qY);
    List yList(&y);
    List arguments( &x, &yList );
    arguments.print("\narguments: ");
    List funcCall( gInterpreter::getInstance().m_mouseMoveEventAtom, nullptr );
    gInterpreter::getInstance().eval( &funcCall );
}

void MainWidget::mousePressEvent( QMouseEvent* qEvent )
{
    if ( gInterpreter::getInstance().m_mousePressEventAtom->value()->type() != ISExpr::LIST )
    {
        LOG_ERR( "Undefined function 'mousePressEvent'" );
        return;
    }
    
    List funcCall( gInterpreter::getInstance().m_mousePressEventAtom, nullptr );
    gInterpreter::getInstance().eval( &funcCall );
}

void MainWidget::mouseReleaseEvent( QMouseEvent* qEvent )
{
    if ( gInterpreter::getInstance().m_mouseReleaseEventAtom->value()->type() != ISExpr::LIST )
    {
        LOG_ERR( "Undefined function 'mouseReleaseEvent'" );
        return;
    }
    
    List funcCall( gInterpreter::getInstance().m_mouseReleaseEventAtom, nullptr );
    gInterpreter::getInstance().eval( &funcCall );
}

void MainWidget::paintEvent(QPaintEvent* event)
{
    // Create QPainter
    QPainter qPainter(this);
    Custom<QPainter> painter( &qPainter );

    if ( gInterpreter::getInstance().m_paintEventAtom->value()->type() != ISExpr::LIST )
    {
        LOG_ERR( "Undefined function 'paintEvent'" );
        return;
    }
    
    List argument(&painter);
    List funcCall( gInterpreter::getInstance().m_paintEventAtom, &argument );
    gInterpreter::getInstance().eval( &funcCall );
}

gInterpreter::gInterpreter()
{
    m_paintEventAtom        = getAtom( "paintEvent" );
    m_mousePressEventAtom   = getAtom( "mousePressEvent" );
    m_mouseMoveEventAtom    = getAtom( "mouseMoveEvent" );
    m_mouseReleaseEventAtom = getAtom( "mouseReleaseEvent" );

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
        int w = width->toIntNumber()->intValue();
        int h = height->toIntNumber()->intValue();
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
        
        expr->m_car->print0("\n");
        gInterpreter::getInstance().eval( expr->m_car )->print0("\n");
        int x = gInterpreter::getInstance().eval( expr->m_car )->toNumberBase()->intValue();
        expr = expr->m_cdr;

        int y = gInterpreter::getInstance().eval( expr->m_car )->toNumberBase()->intValue();
        expr = expr->m_cdr;

        int xRadius = gInterpreter::getInstance().eval( expr->m_car )->toNumberBase()->intValue();
        expr = expr->m_cdr;

        int yRadius = gInterpreter::getInstance().eval( expr->m_car )->toNumberBase()->intValue();
        expr = expr->m_cdr;

        qPainter->drawEllipse( x, y, xRadius, yRadius );
    });

    m_builtInFuncMap["gDrawRect"] = new BuiltinFunc( "gDrawRect", [](List* expr) -> ISExpr*
    {
        auto* painter = to<QPainter>( expr->m_car->toAtom()->value() );
        auto* qPainter = painter->value();
        expr = expr->m_cdr;

        int x = gInterpreter::getInstance().eval( expr->m_car )->toNumberBase()->intValue();
        expr = expr->m_cdr;

        int y = gInterpreter::getInstance().eval( expr->m_car )->toNumberBase()->intValue();
        expr = expr->m_cdr;

        int width = gInterpreter::getInstance().eval( expr->m_car )->toNumberBase()->intValue();
        expr = expr->m_cdr;

        int height = gInterpreter::getInstance().eval( expr->m_car )->toNumberBase()->intValue();
        expr = expr->m_cdr;

        qPainter->drawRect(x , y, width, height);
    });

    m_builtInFuncMap["gSetBrush"] = new BuiltinFunc( "gSetBrush", [](List* expr) -> ISExpr*
    {
        auto* painter = to<QPainter>( expr->m_car->toAtom()->value() );
        auto* qPainter = painter->value();
        expr = expr->m_cdr;
        LOG_VAR( "color: " << expr->m_car->toAtom()->name() )
        QColor color( expr->m_car->toAtom()->name() );
        qPainter->setBrush(color);
    });

}

int gInterpreter::exec(int argc, char* argv[], std::string code){
    QApplication a(argc, argv);
    w = new MainWindow;

    widget = Custom<MainWidget>( new MainWidget );

    while( eval(code) != nullptr ) {
    }

    w->centralWidget()->setLayout(new QVBoxLayout);
    w->centralWidget()->layout()->addWidget( widget.value() );
    w->show();
    return a.exec();
}

int gInterpreter::execFile(int argc, char* argv[], std::string fileName ){
    QApplication a(argc, argv);
    w = new MainWindow;

    widget = Custom<MainWidget>( new MainWidget );

    while( evalFile(fileName) != nullptr ) {
    }

    w->centralWidget()->setLayout(new QVBoxLayout);
    w->centralWidget()->layout()->addWidget( widget.value() );
    w->show();
    return a.exec();
}
