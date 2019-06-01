#include "session.h"
#include <QIODevice>

Session::Session(QObject *parent) : QObject(parent)
{

}

void Session::writeToFile(QFile *outFile){
    outFile->open(QIODevice::WriteOnly);
    QDataStream out(outFile);
    out << (QPixmap)m_imageData;
    out << (QString)m_session;
    out << (QString)m_schuetze;
    out << (QString)m_ort ;
    out << (qint32)m_entfernungIndex;
    out << (qint32)m_caliberIndex;
    out << (qint32)m_ringDistanceIndex;
    out << (QDate)m_date;
    out << (double)m_mm;
    out << (double)m_pixel;
    out << (qint32)m_angle;
    out << (qreal)m_centerTarget.x();
    out << (qreal)m_centerTarget.y();
    out << (qint32)m_allShots.size();
    foreach(QPointF p , m_allShots){
        out << (qreal)p.x();
        out << (qreal)p.y();
    }
}

void Session::loadFromFile(QFile *inFile){
    int itemListSize = 0;
    qreal x;
    qreal y;
    qreal xCenter;
    qreal yCenter;
    inFile->open(QIODevice::ReadOnly);
    QDataStream in(inFile);
    in >> m_imageData >> m_session >> m_schuetze >> m_ort
            >> m_entfernungIndex >> m_caliberIndex >> m_ringDistanceIndex
            >> m_date >> m_mm >> m_pixel >> m_angle ;
    in >> xCenter;
    in >> yCenter;
    in >> itemListSize;
    m_allShots.clear();
    for(int i=0; i<itemListSize;i++){
        in >> x;
        in >> y;
        QPointF p = QPoint(x,y);
        m_allShots.append(p);
    }
    m_centerTarget = QPointF(xCenter,yCenter);
}

void Session::setSession(QPixmap imageData,
                               QString session, QString schuetze, QString ort,
                                qint32 entfernungIndex, qint32 caliberIndex, qint32 ringDistanceIndex,
                         QDate date, double mm, double pixel, qint32 angle, QPointF centerTarget, QList<QPointF> allShots){
    this->m_imageData         = imageData;
    this->m_session           = session;
    this->m_schuetze          = schuetze;
    this->m_ort               = ort;
    this->m_entfernungIndex   = entfernungIndex;
    this->m_caliberIndex      = caliberIndex;
    this->m_ringDistanceIndex = ringDistanceIndex;
    this->m_date              = date;
    this->m_mm                = mm;
    this->m_pixel             = pixel;
    this->m_angle             = angle;
    this->m_centerTarget      = centerTarget;
    this->m_allShots          = allShots;
}

QPixmap Session::getSessionPixmap(){
    return this->m_imageData;
}

QString Session::getSessionSession(){
    return this->m_session;
}

QString Session::getSessionSchuetze(){
    return this->m_schuetze;
}

QString Session::getSessionOrt(){
    return this->m_ort;
}
qint32 Session::getEntfernungIndex(){
    return this->m_entfernungIndex;
}

qint32 Session::getCaliberIndex(){
    return this->m_caliberIndex;
}

qint32 Session::getRingDistanceIndex(){
    return this->m_ringDistanceIndex;
}

QDate Session::getDate(){
    return this->m_date;
}

double Session::getMM(){
    return this->m_mm;
}

double Session::getPixel(){
    return this->m_pixel;
}

qint32 Session::getAngle(){
    return this->m_angle;
}

QList<QPointF> Session::getAllShots(){
    return this->m_allShots;
}

QPointF Session::getCenterTarget(){
    return this->m_centerTarget;
}
