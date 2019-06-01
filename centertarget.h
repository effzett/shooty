#ifndef CENTERTARGET_H
#define CENTERTARGET_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QGraphicsItem>

class CenterTarget : public QGraphicsItem
{
public:
    CenterTarget(QGraphicsItem* parent = 0);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

private:
    int radius1 =10;
    int radius2 =20;
};

#endif // CENTERTARGET_H
