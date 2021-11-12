#include "mylabel.h"
#include <QPainter>

mylabel::mylabel()
{

}

void mylabel::paintEvent(QPaintEvent * event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::red,2));
    painter.drawLine( QPoint(0,0), QPoint(400,400));
}
