#ifndef PDFDATEN_H
#define PDFDATEN_H

#include <configdaten.h>
#include <QObject>
#include <mainwindow.h>

class PdfDaten
{
public:
    PdfDaten(ConfigDaten cfg, QString sessionName, QString shooterName, QString locationName, QString dateString,
             QString caliber, QString distance, QString ringDist, QList<QPointF> realShots, double caliber2, QString rings, QString anzahl, QString d, QString es, QString maxhoehe, QString maxbreite, QString fom,
//             QString stdabwx, QString stdabwy, QString rds , QString cep50, QString cep90, QString cep95,
             double circleR, double circleSigma, double circleCEP50, double circleCEP90,double circleCEP95,
             QString stdabwxC, QString stdabwyC, QString rdsC, QString mittlRadiusValueC, QString sigmaValueC, QString cep50C, QString cep90C, QString cep95C,
             QString mittlRadiusValue, QString shotGroupXValue, QString shotGroupYValue, QString shotGroup2Value, // QString sigmaValue,
             QList<QPointF> d2Points, QList<QPointF> es2Points, QPointF com, QListWidget* koordinaten);
    ConfigDaten cfg;
    QString sessionName;
    QString shooterName;
    QString locationName;
    QString dateString;
    QString caliber;
    QString distance;
    QString ringDist;
    QList<QPointF> realShots;
    double caliber2;
    QString rings;
    QString anzahl;
    QString d;
    QString es;
    QString maxhoehe;
    QString maxbreite;
    QString fom;
//    QString stdabwx;
//    QString stdabwy;
//    QString rds;
    QString mittlRadiusValue;
    QString shotGroupXValue;
    QString shotGroupYValue;
    QString shotGroup2Value;
//    QString sigmaValue;
    QList<QPointF> d2Points;
    QList<QPointF> es2Points;
//    QString cep50;
//    QString cep90;
//    QString cep95;
    QString stdabwxC;
    QString stdabwyC;
    QString rdsC;
    QString mittlRadiusValueC;
    QString sigmaValueC      ;
    QString cep50C;
    QString cep90C;
    QString cep95C;
    QPointF com;
    double circleR;
    double circleSigma;
    double circleCEP50;
    double circleCEP90;
    double circleCEP95;
    QListWidget *koordinaten;

};

#endif // PDFDATEN_H
