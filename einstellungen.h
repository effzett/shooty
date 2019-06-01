#ifndef EINSTELLUNGEN_H
#define EINSTELLUNGEN_H

#include <QDialog>
#include <configdaten.h>

namespace Ui {
class Einstellungen;
}

class Einstellungen : public QDialog
{
    Q_OBJECT

public:
    explicit Einstellungen(QWidget *parent = 0);
    ~Einstellungen();

    ConfigDaten *getData();
    void setData(ConfigDaten cfg);

private slots:
    void on_checkBox_TrefferKoordinaten_clicked();

    void on_pushButtonReset_clicked();

private:
    Ui::Einstellungen *ui;

    double m_x;
    double m_y;
    double m_xysize;

    double m_b1x;
    double m_b1y;
    double m_b2x;
    double m_b2y;
    double m_b3x;
    double m_b3y;

    bool m_drechteck;
    bool m_esline;
    bool m_poaks;
    bool m_coiks;
    bool m_poacoiline;
    bool m_konvex;
    bool m_trefferkoordinaten;

    bool m_rundKonvexCMR;
    int m_rundSpannung;
    bool m_rundKonvex;
    int m_rundPrecision;
    int m_minPDFBox;

    bool m_circleR;
    bool m_circleSigma;
    bool m_circleCEP50;
    bool m_circleCEP90;
    bool m_circleCEP95;
    bool m_circleRinge;
};

#endif // EINSTELLUNGEN_H
