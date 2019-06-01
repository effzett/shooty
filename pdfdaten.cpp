#include <configdaten.h>
#include <QObject>
#include <mainwindow.h>
#include <pdfdaten.h>

PdfDaten::PdfDaten(ConfigDaten cfg, QString sessionName, QString shooterName, QString locationName, QString dateString,
                   QString caliber, QString distance, QString ringDist, QList<QPointF> realShots, double caliber2, QString rings, QString anzahl, QString d, QString es, QString maxhoehe, QString maxbreite, QString fom,
//                   QString stdabwx, QString stdabwy, QString rds , QString cep50, QString cep90, QString cep95,
                   double circleR, double circleSigma, double circleCEP50, double circleCEP90,double circleCEP95,
                   QString stdabwxC, QString stdabwyC, QString rdsC, QString mittlRadiusValueC, QString sigmaValueC, QString cep50C, QString cep90C, QString cep95C,
                   QString mittlRadiusValue, QString shotGroupXValue, QString shotGroupYValue, QString shotGroup2Value, // QString sigmaValue,
                   QList<QPointF> d2Points, QList<QPointF> es2Points, QPointF com, QListWidget* koordinaten)
{
    this->cfg = cfg;
    this->sessionName=sessionName;
    this->shooterName=shooterName;
    this->locationName=locationName;
    this->dateString=dateString;
    this->caliber=caliber;
    this->distance=distance;
    this->ringDist=ringDist;
    this->realShots=realShots;
    this->caliber2=caliber2;
    this->rings=rings;
    this->anzahl=anzahl;
    this->d=d;
    this->es=es;
    this->maxhoehe=maxhoehe;
    this->maxbreite=maxbreite;
    this->fom=fom;
//    this->stdabwx=stdabwx;
//    this->stdabwy=stdabwy;
//    this->rds=rds;
    this->mittlRadiusValue=mittlRadiusValue;
    this->shotGroupXValue =shotGroupXValue ;
    this->shotGroupYValue =shotGroupYValue ;
    this->shotGroup2Value =shotGroup2Value ;
//    this->sigmaValue      =sigmaValue      ;
    this->d2Points= d2Points;
    this->es2Points=es2Points;
//    this->cep50 = cep50;
//    this->cep90 = cep90;
//    this->cep95 = cep95;
    this->stdabwxC         =stdabwxC;
    this->stdabwyC         =stdabwyC;
    this->rdsC             =rdsC;
    this->mittlRadiusValueC=mittlRadiusValueC;
    this->sigmaValueC      =sigmaValueC      ;
    this->cep50C            = cep50C;
    this->cep90C            = cep90C;
    this->cep95C            = cep95C;
    this->com               = com;
    this->circleR           = circleR;
    this->circleSigma       = circleSigma;
    this->circleCEP50       = circleCEP50;
    this->circleCEP90       = circleCEP90;
    this->circleCEP95       = circleCEP95;
    this->koordinaten       = koordinaten;


//    foreach(QPointF p, realShots){
//        qDebug("pdfdaten: realshots = %f %f",p.x(),p.y());
//    }
//    qDebug("pdfdaten: Ringe: " + rings.toLatin1());

}
