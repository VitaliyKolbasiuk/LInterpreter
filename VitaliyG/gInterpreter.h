#ifndef GINTERPRETER_H
#define GINTERPRETER_H

#include "LInterpreter.h"
#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QTimer>

class MainWidget: public QWidget{
public:
    MainWidget(QWidget *parent = nullptr) : QWidget(parent) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWidget::update));
        timer->start(10);
    }
private:
    double x = 0, y = 0;
    double dx = 2;
    double dy = 2;
    QTimer* timer;
protected:
    void paintEvent(QPaintEvent* event); /*override{
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        int radius = qMin(width(), height()) / 4;
        if (x + dx > width() - radius || x + dx < 0){
            dx = -dx;
        }
        if (y + dy > height() - radius || y + dy < 0){
            dy = -dy;
        }
        x += dx;
        y += dy;

        painter.setBrush(Qt::black);
        painter.drawRect(0, 0, width(), height());
        painter.setBrush(Qt::blue);
        painter.drawEllipse(x, y, radius, radius);
    }*/
};

class gInterpreter:public LInterpreter
{
    MainWindow* w;
public:
    MainWidget* widget;
    QPainter* painter;
public:
    gInterpreter();
    static gInterpreter& getInstance() {
        static gInterpreter instance;
        return instance;
    }
    int exec(int argc, char *argv[], std::string code);
    void paintEvent(){
        painter = new QPainter;
        eval("( paintEvent 0 )");
        delete painter;
    }
};

#endif // GINTERPRETER_H
