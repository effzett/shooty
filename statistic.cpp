#include "statistic.h"
#include <QtMath>
#include <math.h>

Statistic::Statistic(QList<QPointF> liste)
{
    this->pointList = liste;
    this->com = shotGroupCenter();
}

// ************************ Sigma ************************************************************
double Statistic::sigma(bool comFlag)
{
    double sigma=0;
    double sum=0.0;
    double sum1=0.0;

    if(pointList.length()>0){
        // biased
        foreach(QPointF p, pointList)
        {
            if(comFlag){
                sum1 +=  (Statistic::length(p-this->com))*(Statistic::length(p-this->com));
            }
            else{
                sum1 +=  (Statistic::length(p))*(Statistic::length(p));
            }
        }
        // Old: biased
        //sum = sum1 / (2.*pointList.length());
        //sigma = qSqrt(sum)

        // New: unbiased (Closed Form precision, ballistipedia)
        sum = sum1/2.0;
        sigma = qSqrt(sum);
        // tgamma steigt bei n>170 mit overflow aus
        //        sigma = sigma * tgamma(pointList.length()-1)/tgamma(pointList.length()-0.5);
        sigma = sigma * exp(lgamma(pointList.length()-1) - lgamma(pointList.length()-0.5));
    }
    return sigma;
}

double Statistic::length(QPointF p)
{
    QLineF *line = new QLineF(QPointF(),p);
    return line->length();
}

double Statistic::meanArithmetic(bool comFlag)
{
    double meanValue=0;

    double sum=0.0;
    foreach(QPointF p, pointList)
    {
        if(comFlag){
            sum += Statistic::length((p-this->com));
        }
        else{
            sum += Statistic::length(p);
        }
    }
    meanValue = sum / pointList.length();
    return meanValue;
}

double Statistic::stdabw(int k,bool comFlag){
    double stdabw=0;
    double stdabw0=0;
    foreach(QPointF p, pointList){
        switch (k){
            case 1:  // x
                if(comFlag){
                    stdabw0 += (p-this->com).x() * (p-this->com).x();
                }
                else{
                    stdabw0 += p.x() * p.x();
                }
            break;
            case 2:  // y
                if(comFlag){
                    stdabw0 += (p-this->com).y() * (p-this->com).y();
                }
                else{
                    stdabw0 += p.y() * p.y();
                }
            break;
            case 3:  // r
                if(comFlag){
                    stdabw0 += QLineF(this->com,p).length() * QLineF(this->com,p).length();
                }
                else{
                    stdabw0 += QLineF(QPointF(),p).length() * QLineF(QPointF(),p).length();
                }
            break;
            default:
            break;
        }
    }
    stdabw0 /= (pointList.length() - 1);
    stdabw = qSqrt(stdabw0);

    return stdabw;
}

double Statistic::rings(double ringDistance,double caliber2)
{
    double rings=0;
    double radius;
    qDebug("Statistic rings: Anzahl elemente %d",pointList.length());
    foreach(QPointF p, pointList)
    {
        radius = Statistic::length(p);
        for(int i=1; i<11 ; i++){
            if((radius-caliber2)<(i*ringDistance)){
                rings += (11-i);
                break;
            }
        }
    }
    qDebug("Kaliber/2=%f",caliber2);

    return rings;
}

QPointF Statistic::shotGroupCenter()
{
    QPointF *sgc = new QPointF();
    foreach(QPointF p, pointList)
    {
        sgc->operator +=(p);
    }
    return sgc->operator /=(pointList.length());
}

QList<QPointF> Statistic::diagonal()
{
    QList<QPointF> diagonal;
    QPointF p1 = QPointF(+10000,+10000);    // leftbottom
    QPointF p2 = QPointF(-10000,-10000);    // righttop

    foreach(QPointF p, pointList)
    {
        if(p.x()<p1.x()){
            p1.setX(p.x());
        }
        if(p.x()>p2.x()){
            p2.setX(p.x());
        }
        if(p.y()<p1.y()){
            p1.setY(p.y());
        }
        if(p.y()>p2.y()){
            p2.setY(p.y());
        }
    }
    diagonal.append(p1);
    diagonal.append(p2);

   return diagonal;
}

QList<QPointF> Statistic::extremeSpread()
{
    QList<QPointF> es;
    QPointF p1;
    QPointF p2;
    double spread = 0;

    if(pointList.length()>1){
        for(int i=0; i < pointList.length();i++){
            for(int j=i+1; j < pointList.length();j++){
                if(QLineF(pointList.at(i),pointList.at(j)).length() > spread){
                    spread = QLineF(pointList.at(i),pointList.at(j)).length();
                    p1= pointList.at(i);
                    p2= pointList.at(j);
                }
            }
        }
    }

    es.append(p1);
    es.append(p2);
    return es;
}

