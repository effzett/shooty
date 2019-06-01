#include "centertarget.h"
#include <QPainter>

CenterTarget::CenterTarget(QGraphicsItem* parent): QGraphicsItem(parent)
{

}

QRectF CenterTarget::boundingRect() const{
    return QRectF(-radius2-1,-radius2-1,2*radius2+2,2*radius2+2);
}

void CenterTarget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QPen blackPen(Qt::black);
    QPen yellowPen(Qt::yellow);
    blackPen.setWidth(4);
    yellowPen.setWidth(5);
    QBrush redBrush(Qt::red);
    QBrush blueBrush(Qt::blue);
    QBrush yellowBrush(Qt::yellow);
    painter->setBrush(yellowBrush);
    painter->setPen(blackPen);
    painter->drawEllipse(-radius2/2,-radius2/2,radius2,radius2);
    painter->setBrush(redBrush);
    painter->drawEllipse(-radius1/2,-radius1/2,radius1,radius1);
    painter->setBrush(yellowBrush);
    painter->setPen(yellowPen);
    painter->drawLine(-radius2,0,+radius2,0);
    painter->drawLine(0,-radius2,0,+radius2);
}
