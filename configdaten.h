#ifndef CONFIGDATEN_H
#define CONFIGDATEN_H
#include <QString>

class ConfigDaten
{
public:
    ConfigDaten(QString version);
    ConfigDaten();
    int m_PositionX;
    int m_PositionY;
    int m_boxSize;

    int m_b1x;
    int m_b1y;
    int m_b2x;
    int m_b2y;
    int m_b3x;
    int m_b3y;

    bool m_drechteck;
    bool m_esline;
    bool m_poaks;
    bool m_coiks;
    bool m_poacoiline;
    bool m_konvex;
    bool m_trefferkoordinaten;

    bool m_rundKonvexCMR;
    int  m_rundSpannung;
    bool m_rundKonvex;
    int  m_rundPrecision;
    int  m_minPDFBox;

    bool m_circleR;
    bool m_circleSigma;
    bool m_circleCEP50;
    bool m_circleCEP90;
    bool m_circleCEP95;
    bool m_circleRinge;

    QString m_regName;
    QString m_regSerial;
    QString m_version;

};

#endif // CONFIGDATEN_H
