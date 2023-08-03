#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QTimer>
#include <QMouseEvent>

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
    QPoint mousePressPos;
    QPoint ellipsePos;

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        int radius = qMin(width(), height()) / 4;
        if (x + dx > width() - radius || x + dx < 0) {
            dx = -dx;
        }
        if (y + dy > height() - radius || y + dy < 0) {
            dy = -dy;
        }
        x += dx;
        y += dy;

        painter.setBrush(Qt::black);
        painter.drawRect(0, 0, width(), height());
        painter.setBrush(Qt::blue);
        painter.drawEllipse(x, y, radius, radius);

        int ellipseRadius = 30;
        painter.setBrush(Qt::red);
        painter.drawEllipse(50, ellipsePos.y(), 30, 100);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            isMousePressed = true;
            mousePressPos = event->pos();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (isMousePressed) {
            ellipsePos = event->pos();
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
