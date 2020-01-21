#ifndef SESSION_H
#define SESSION_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QList>
#include <centertarget.h>
#include <configdaten.h>
#include <QFile>
#include <QDate>

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = nullptr);

signals:

public slots:
    void writeToFile(QFile *outFile);
    void loadFromFile(QFile *inFile);
    void setSession(QPixmap imageData,
                            QString session, QString schuetze, QString ort,
                            qint32 entfernungIndex, qint32 caliberIndex, qint32 ringDistanceIndex,
                    QDate date, double mm, double pixel, qint32 angle, QPointF centerTarget, QList<QPointF> allShots);

    QPixmap getSessionPixmap();
    QString getSessionSession();
    QString getSessionSchuetze();
    QString getSessionOrt();
    qint32  getEntfernungIndex();
    qint32  getCaliberIndex();
    qint32  getRingDistanceIndex();
    QDate   getDate();
    double  getMM();
    double  getPixel();
    int     getAngle();
    QList<QPointF> getAllShots();
    QPointF getCenterTarget();

private:

    // image
    QPixmap m_imageData;
    QString m_session;
    QString m_schuetze;
    QString m_ort;
    qint32  m_entfernungIndex;
    qint32  m_caliberIndex;
    qint32  m_ringDistanceIndex;
    QDate   m_date;
    double  m_mm;
    double  m_pixel;
    int     m_angle;
    QList<QPointF> m_allShots;
    QPointF m_centerTarget;

};

#endif // SESSION_H
