#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QTimer>

class CircleWidget: public QWidget{
public:
    CircleWidget(QWidget *parent = nullptr) : QWidget(parent) {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, QOverload<>::of(&CircleWidget::update));
        timer->start(10);
    }
private:
    double x = 0, y = 0;
    double dx = 1;
    double dy = 1;
    QTimer* timer;
protected:
    void paintEvent(QPaintEvent* event) override{
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
    }
};

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
