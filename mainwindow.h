#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QGraphicsScene>
#include <qmath.h>
#include <QtPrintSupport>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QListWidget>
#include <QList>
#include <QtMath>
#include <QMessageBox>
#include <QVector>
#include <QLabel>
#include <QProgressBar>
#include <QStandardPaths>
#include <createpdf.h>
#include <einstellungen.h>
#include <configdaten.h>
#include <session.h>
#include <statistic.h>
#include <serial.h>
#include <configdaten.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

signals:
    void deleteCalibrationLine();

private slots:

    void on_toolButton_LoadImage_clicked();
    void on_actionBild_laden_triggered();
    void on_actionShooty_Bild_sichern_triggered();
    void on_actionSichern_unter_triggered();
    void on_actionAls_PDF_Sichern_triggered();
    void on_actionEinstellungen_triggered();
    void on_actionAls_CSV_exportieren_triggered();
    void on_actionOpen_Session_triggered();
    void on_actionNeue_Session_triggered();
    void on_actionSeriennummer_triggered();
    void on_actionPrint_triggered();
    void on_actionSchootyBild_drucken_triggered();
    void on_action_ber_Shooty_triggered();

    void resetView();
    void setupMatrix();
    void setResetButtonEnabled();
    void rotateLeft();
    void rotateRight();
    void rotateLeft90();
    void rotateRight90();
    void printView();
    void displayAngle();
    void emptyScene(QGraphicsScene*);
    void emptySumShots();
    void toggleShot();
    void centerTargetIsSet(QPointF);
    void shotCountChanged(int);
    void fitInView();
    void calibration(QLineF);
    void fillQLists(QList<QPointF> shotList);
    void shotChanged(QList<QPointF>);
    void resetQLists();
    void resetDate();
    QPointF transformScene2Real(QPointF scenePoint);
    void calc();
    void setDistance();
    void setCaliberList();
    void setCaliber();
    void checkRegistration();
    void setGUIValues();
    void initSession(bool completeFlag);
    void setSessionVariables();
    void on_actionSichern_triggered();
    void makePDF(QString fn);
    void on_toolButtonViewPDF_clicked();
    void on_actionDisplay_PDF_triggered();
    void openSession(QString fileName);

#ifdef Q_OS_WIN
    void on_actionBeenden_triggered();
    void fromCommandLine();
#endif


private:
    void loadSettings();
    void saveSettings();

    Ui::MainWindow *ui;
    // for app
    QLabel         *m_statusLabel;
    QProgressBar   *m_statusProgressBar;
    bool            m_isRegistered=false;
    ConfigDaten    *m_config;
    Serial         *m_serial;
    QVector<double> m_caliberList;


    // for session
    QPixmap         m_image;
    QImage         *m_imageObject;
    QGraphicsScene *m_scene;
    int             m_sumShots=0;    // Anzahl der Treffer
    double          m_pixel=0;       // Massstab: pixel
    double          m_mm=0;          // Massstab: mm
    double          m_calibFactor=1; // mm/pixel
    QPointF         m_centerPixel = QPointF();   // Position des Schussziels in Scene Koordinaten
    int             m_angle;         // aktueller drehwinkel
    QList<QPointF>  m_realShots;     // Liste mit allen realen Schusskoordinaten in mm bez. auf centerMM
    double          m_distance=25000;
    double          m_caliber2 =4.5; // half of caliber in mm
    QString         m_fileName = "";
    double          m_circleR;       // mittlerer Radius
    double          m_circleSigma;       // Sigma
    double          m_circleCEP50;       // CEPs
    double          m_circleCEP90;       // CEPs
    double          m_circleCEP95;       // CEPs


    QString         m_sessionTitle;     // Name der Session
    QString         m_version= "1.4";  // Nur hier wird die Version bestimmt !!!!!


};

#endif // MAINWINDOW_H
