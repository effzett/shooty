#ifndef STATISTIC_H
#define STATISTIC_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QList>
#include <QPointF>

class Statistic
{
public:
    Statistic(QList<QPointF> liste);

    double meanArithmetic(bool comFlag);
    double static length(QPointF);
    double rings(double ringDistance, double caliber2);
    QPointF shotGroupCenter();
    QList<QPointF> diagonal();
    QList<QPointF> extremeSpread();
    double sigma(bool comFlag);
    double stdabw(int k, bool comFlag);

private:
    QList<QPointF> pointList;
    QPointF com;

};

#endif // STATISTIC_H
