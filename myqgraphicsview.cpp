#include "myqgraphicsview.h"
#include <QPointF>
#include <QGraphicsScene>
#include <centertarget.h>
#include <QApplication>
#include <QColor>
#include <QList>

MyQGraphicsView::MyQGraphicsView(QWidget *parent):QGraphicsView(parent)
{
    this->setScene(new QGraphicsScene());
}

void MyQGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton && lineDrawing==true){
        QLineF *newLine = new QLineF(firstPoint,mapToScene(e->pos()));
        line->setLine(*newLine);
    }
    QGraphicsView::mouseMoveEvent(e);
}

void MyQGraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton && lineDrawing==true){
        QPointF currPos = mapToScene(e->pos());
        QLineF *newLine = new QLineF(firstPoint,currPos);
        line->setLine(*newLine);
        lineDrawing=false;
        // Sending Signal
        if(newLine->length()>0){        // sonst fällt der calibFactor in den Sumpf 0/0
            emit calibSceneReady(*newLine);
        }
    }
    emit shots(getShots());
    QGraphicsView::mouseReleaseEvent(e);
}

void MyQGraphicsView::mouseDoubleClickEvent(QMouseEvent *e)
{
    QGraphicsScene *s = this->scene();
    QPointF pt = mapToScene(e->pos());

    foreach(QGraphicsItem *item, s->items()){
        // Dont delete Pixmap
        QGraphicsPixmapItem *pm = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(pm){
            // do nothing
        }else{
            // Dont delete CenterTarget
            CenterTarget *ct = dynamic_cast<CenterTarget*>(item);
            if(ct){
                //do nothing
            }else{
                if(item->contains(item->mapFromScene(pt))){
                    s->removeItem(item);
                    emit shotCount(-1);
                    //Liste aller Treffer weitergeben
                    emit shots(getShots());
                }
            }
        }
    }
    QGraphicsView::mouseDoubleClickEvent(e);
}

void MyQGraphicsView::mousePressEvent(QMouseEvent * e)
{
    if(e->button() == Qt::LeftButton && Qt::ShiftModifier == QApplication::keyboardModifiers() && this->scene()->items().count()!=0){
                    QGraphicsScene *s = this->scene();
    // zum Aufziehen einer Linie: Startpunkt
        if(e->button() == Qt::LeftButton){
            QPointF currPos = mapToScene(e->pos());
            firstPoint = currPos;
            line = new QGraphicsLineItem(QLineF(currPos,currPos));
            QPen pen;
            pen.setColor(Qt::red);
            pen.setWidth(6);
            line->setPen(pen);
            s->addItem(line);
            lineDrawing = true;
            qDebug("mousePressEvent: Aufziehen einer Linie");
        }
    }


    if(mouseClickForShots){
        // Treffer
        if(e->button() ==Qt::LeftButton && e->buttons() != Qt::RightButton && this->scene()->items().count()!=0){
            QPointF pt = mapToScene(e->pos());
            setShots(pt);
        }
    }

    // CenterTarget *************************************************
    if(e->button() == Qt::RightButton && e->button() != Qt::LeftButton && this->scene()->items().count()!=0){
        // Schussziel
        QPointF pt = mapToScene(e->pos());
        setCenterTarget(pt);
    }
    emit shots(getShots());    // ggfs mehrmals ausgeführt !??

    QGraphicsView::mousePressEvent(e);
}

void MyQGraphicsView::setMouseClickForShots(bool b)
{
    this->mouseClickForShots = b;
}

void MyQGraphicsView::deleteQLine()
{
    QGraphicsScene *s = this->scene();
    foreach(QGraphicsItem *item, s->items()){
        QGraphicsLineItem *li = dynamic_cast<QGraphicsLineItem*>(item);
        if(li){
            s->removeItem(li);
        }
    }
}

QList<QPointF> MyQGraphicsView::getShots()
{
    QGraphicsScene *s = this->scene();
    QList<QPointF> allShots;
    foreach(QGraphicsItem *item, s->items()){
        // Dont gather Pixmap
        QGraphicsPixmapItem *pm = dynamic_cast<QGraphicsPixmapItem*>(item);
        if(pm){
            // do nothing
        }else{
            // Dont gather CenterTarget
            CenterTarget *ct = dynamic_cast<CenterTarget*>(item);
            if(ct){
                //do nothing
            }else{
                // fuelle Liste
//                qDebug("ItemPoint:ScenePos: %f %f Items in scene: %d",item->scenePos().x(),item->scenePos().y(),s->items().count());

                allShots.append(item->scenePos());
            }
        }
    }

    return allShots;
}

void MyQGraphicsView::setShotsFromSessionOpen(QList<QPointF> liste){
    foreach(QPointF p, liste){
        setShots(p);
    }
}

void MyQGraphicsView::setCenterTargetFromSessionOpen(QPointF centerTarget){
    setCenterTarget(centerTarget);
}

void MyQGraphicsView::setCenterTarget(QPointF centerTarget){
    // Schussziel
    QGraphicsScene *s = this->scene();
    //Alten Zielpunkt aus Scene löschen
    foreach(QGraphicsItem *item, s->items()){
        CenterTarget *ct = dynamic_cast<CenterTarget*>(item);
        if(ct){
            s->removeItem(ct);
        }
    }
    CenterTarget *ct = new CenterTarget();
    s->addItem(ct);
    ct->setPos(centerTarget.x(),centerTarget.y());
    // Schussziel Label ändern
    emit centerTargetSignal(centerTarget);
    emit shots(getShots());
}

void MyQGraphicsView::setShots(QPointF p){
        QGraphicsScene *s = this->scene();
        double rad = 10;
        double penwidth=5;
        //QPointF pt = mapToScene(e->pos());
        QBrush redBrush(Qt::red);
        QPen blackPen(Qt::black);
        QPen yellowPen(Qt::yellow);
        blackPen.setWidth(penwidth);
        yellowPen.setWidth(penwidth);
        QGraphicsEllipseItem *e = new QGraphicsEllipseItem(0, 0, rad*2.0, rad*2.0);
        e->setPen(yellowPen);
        e->setBrush(redBrush);
        e->setRect(QRect(-rad-2.5,-rad-2.5,2*rad+5,2*rad+5));
        s->addItem(e);
        e->setPos(p.x(), p.y());
        e->setFlags(QGraphicsItem::ItemIsMovable);
        //            qDebug("Treffer: %f %f",e->scenePos().x(),e->scenePos().y());
        emit shotCount(+1);
        emit shots(getShots());
}
