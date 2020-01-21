#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyQGraphicsView(QWidget *parent = nullptr);
    void setMouseClickForShots(bool);
signals:
    void centerTargetSignal(QPointF);
    void shotCount(int);
    void calibSceneReady(QLineF);
    void shots(QList<QPointF>);

public slots:
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseDoubleClickEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void deleteQLine();
    QList<QPointF> getShots();
    void setShotsFromSessionOpen(QList<QPointF> liste);
    void setCenterTargetFromSessionOpen(QPointF centerTarget);
    void setCenterTarget(QPointF centerTarget);
    void setShots(QPointF p);
private:
    bool mouseClickForShots=false;
    int countShots=0;
    QGraphicsLineItem *line = new QGraphicsLineItem();
    bool lineDrawing=false;
    QPointF firstPoint;
};

#endif // MYQGRAPHICSVIEW_H
