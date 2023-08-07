#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QTimer>
#include <QMouseEvent>

#include <iostream>

class CircleWidget : public QWidget {
public:
    CircleWidget(QWidget *parent = nullptr) : QWidget(parent) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&CircleWidget::update));
        timer->start(10);
    }

private:
    double x = 0, y = 0;
    double dx = 3;
    double dy = 3;
    QTimer *timer;
    bool isMousePressed;
    QPoint ellipsePos;

    int radius = 15;
    int ellipseRadius = 50;
    
    bool isIntersected = false;

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        auto halfSum = (radius-ellipseRadius);
        
        auto realX = x + radius;
        auto realY = y + radius;
        std::cout << "x:" << realX << " y:" << realY << "\n";

        auto realX2 = ellipsePos.x() + ellipseRadius;
        auto realY2 = ellipsePos.y() + ellipseRadius;
        std::cout << "x:" << realX << " y:" << realY << "\n";

        if ( (realX-realX2) * (realX-realX2) + (realY-realY2) * (realY-realY2) > (radius+ellipseRadius)*(radius+ellipseRadius) )
        {
            if (x + dx > width() - radius || x + dx < 0) {
                dx = -dx;
            }
            if (y + dy > height() - radius || y + dy < 0) {
                dy = -dy;
            }
            
            isIntersected = false;
        }
        else if ( ! isIntersected )
        {
//            auto intersectX = ellipsePos.x() + ((x-ellipsePos.x()) * ellipseRadius) / radius;
//            auto intersectY = ellipsePos.y() + ((y-ellipsePos.y()) * ellipseRadius)) / radius;
            dx = -dx;
            dy = -dy;
            isIntersected = true;
        }
        else
        {
            isIntersected = true;
        }

        x += dx;
        if (x<0) x=0;
        y += dy;
        if (y<0) y=0;

        painter.setBrush(Qt::lightGray);
        painter.drawRect(0, 0, width(), height());
        painter.setBrush(Qt::blue);
        painter.drawEllipse(x, y, radius*2, radius*2);

        painter.setBrush(Qt::red);
        painter.drawEllipse(ellipsePos.x(), ellipsePos.y(), ellipseRadius*2, ellipseRadius*2 );


    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            isMousePressed = true;
//            ellipsePos.setX( event->pos().x() - ellipseRadius );
//            ellipsePos.setY( event->pos().y() - ellipseRadius );
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (isMousePressed) {
            ellipsePos.setX( event->pos().x() - ellipseRadius );
            ellipsePos.setY( event->pos().y() - ellipseRadius );
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            isMousePressed = false;
        }
    }
};

// ( gSetWindowSize 800 800 ) ( set dx 2 ) ( set dy 2 ) (set x 10) (set y 100)
// ( defun aintEvent (widget) )
//      ( widget setRenderHintAntialiasing true )
//      ( set radius 10 )
//      ( if ( OR (> (+ x dx) (- (widget with) radius) (< (+ x dx) 0))
//              (set dx (- 0 dx)) )
//      ...
//      ( widget setBrush "black" )

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(800, 800);
    CircleWidget circleWidget;
    w.centralWidget()->setLayout(new QVBoxLayout);
    w.centralWidget()->layout()->addWidget(&circleWidget);

    w.show();
    return a.exec();
}
