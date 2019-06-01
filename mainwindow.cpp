#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <session.h>
#include <serial.h>
#include <qcolor.h>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setGUIValues();
    initSession(true);

    // Konfiguration gilt für die gesamte Applikation
    m_config = new ConfigDaten(m_version);
    loadSettings(); // aus Konfigurationdatei

    // Setzen der Registrierungsdaten
    m_serial = new Serial();
    m_serial->setData(m_config);    // Daten aus Konfiguration übernehmen
    m_isRegistered = m_serial->isOK();
    if(m_isRegistered){
        QString sl = m_statusLabel->text() + " Registriert für " + m_serial->getRegName();
        m_statusLabel->setText(sl);
    }

    connect(ui->resetButton,SIGNAL(clicked()),this,SLOT(resetView()));
    connect(ui->zoomSlider,SIGNAL(valueChanged(int)),this,SLOT(setupMatrix()));
    connect(ui->rotateSlider,SIGNAL(valueChanged(int)),this,SLOT(setupMatrix()));
    connect(ui->rotateLeftIcon,SIGNAL(clicked(bool)),this,SLOT(rotateLeft()));
    connect(ui->rotateRightIcon,SIGNAL(clicked(bool)),this,SLOT(rotateRight()));
    connect(ui->rotateLeft90,SIGNAL(clicked(bool)),this,SLOT(rotateLeft90()));
    connect(ui->rotateRight90,SIGNAL(clicked(bool)),this,SLOT(rotateRight90()));
    connect(ui->graphicsView->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(setResetButtonEnabled()));
    connect(ui->graphicsView->horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(setResetButtonEnabled()));
    connect(ui->rotateSlider,SIGNAL(valueChanged(int)),this,SLOT(displayAngle()));
    connect(ui->checkBoxTrefferMausklick,SIGNAL(toggled(bool)),this,SLOT(toggleShot()));
    connect(ui->graphicsView,SIGNAL(centerTargetSignal(QPointF)),this,SLOT(centerTargetIsSet(QPointF)));
    connect(ui->graphicsView,SIGNAL(shotCount(int)),this,SLOT(shotCountChanged(int)));
    connect(ui->toolButtonEinpassen,SIGNAL(pressed()),this,SLOT(fitInView()));
    connect(ui->graphicsView,SIGNAL(calibSceneReady(QLineF)),this,SLOT(calibration(QLineF)));
    connect(ui->graphicsView,SIGNAL(shots(QList<QPointF>)),this,SLOT(shotChanged(QList<QPointF>)));
    connect(this,SIGNAL(deleteCalibrationLine()),ui->graphicsView,SLOT(deleteQLine()));
    connect(ui->comboBoxRingabstand,SIGNAL(currentTextChanged(QString)),this,SLOT(calc()));
    connect(ui->comboBoxDistance,SIGNAL(currentTextChanged(QString)),this,SLOT(setDistance()));
    connect(ui->comboBox_Kaliber,SIGNAL(currentIndexChanged(int)),this,SLOT(setCaliber()));

}

void MainWindow::setGUIValues(){

    // create objects for the label and progress bar
    m_statusLabel = new QLabel(this);
    QFont f( "Helvetica Neue", 10);
    m_statusLabel->setFont(f);
    m_statusProgressBar = new QProgressBar(this);

    // set text for the label
    QString txt = "Shooty Version " + m_version;
    m_statusLabel->setText(txt + " ©2019  fz@zenmeister.de");

    // make progress bar text invisible
    m_statusProgressBar->setTextVisible(false);

    // add the two controls to the status bar
    ui->statusBar->addPermanentWidget(m_statusLabel);
    ui->statusBar->addPermanentWidget(m_statusProgressBar,1);

    ui->listWidgetReal->setStyleSheet("QListWidget{ background: rgb(236,236,236) ; }");
    setCaliberList();
}

void MainWindow::initSession(bool completeFlag){
    // Session parameter
    // Rechter Header
    if(completeFlag){
        ui->lineEditOrt->setText("");
        ui->lineEditSchuetze->setText("");
        ui->lineEditSession->setText("");
    }
    // Obere Leiste
    ui->checkBoxTrefferMausklick->setChecked(false);
    toggleShot();

    ui->label_Schussziel->setStyleSheet("QLabel { color : red}");
    ui->label_Kalibrieren->setStyleSheet("QLabel { color : red}");
    ui->checkBoxSchussziel->setChecked(false);
    ui->checkBoxKalibrieren->setChecked(false);

    // Bei den Ergebnissen
    ui->label_KalibrierungValue->setText("");
    ui->lineEdit_ShotCount->setText("");

    // Achtung! Bei Einfügen von weiteren Werten!!
    ui->comboBoxDistance->setCurrentIndex(5);
    ui->comboBox_Kaliber->setCurrentIndex(0);
    ui->comboBoxRingabstand->setCurrentIndex(4);

    ui->rotateSlider->setValue(0);
    ui->actionSichern->setEnabled(false);
    resetQLists();
    resetDate();
    setCaliber();    // ui -> membervariable
    setDistance();   // ui -> membervariable
    displayAngle();  // ui -> membervariable
    setSessionVariables();
}

void MainWindow::setSessionVariables(){
//    m_image        =;
//    *m_imageObject =;
//    *m_scene        ;
    m_realShots.clear();  // Liste mit allen realen Schusskoordinaten in mm bez. auf centerMM
    m_sumShots     = 0;    // Anzahl der Treffer
    m_pixel        = 0;    // Massstab: pixel
    m_mm           = 0;    // Massstab: mm
    m_calibFactor  = 1;    // mm/pixel
    m_centerPixel  = QPointF(); // Position des Schussziels in Scene Koordinaten
    m_fileName     = "";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetView()
{
    ui->zoomSlider->setValue(250);
    ui->rotateSlider->setValue(0);
    setupMatrix();
    ui->graphicsView->ensureVisible(QRectF(0,0,0,0));
    ui->resetButton->setEnabled(false);
}

void MainWindow::setResetButtonEnabled()
{
    ui->resetButton->setEnabled(true);
}

void MainWindow::setupMatrix()
{
    qreal scale = qPow(qreal(2), (ui->zoomSlider->value()-250)/qreal(50));
    QMatrix matrix;
    matrix.scale(scale,scale);
    matrix.rotate(ui->rotateSlider->value());
    ui->graphicsView->setMatrix(matrix);
    setResetButtonEnabled();
}

void MainWindow::rotateLeft()
{
    ui->rotateSlider->setValue(ui->rotateSlider->value()-1);
}

void MainWindow::rotateRight()
{
    ui->rotateSlider->setValue(ui->rotateSlider->value()+1);
}

void MainWindow::rotateLeft90()
{
    ui->rotateSlider->setValue(ui->rotateSlider->value()-90);
}

void MainWindow::rotateRight90()
{
    ui->rotateSlider->setValue(ui->rotateSlider->value()+90);
}

void MainWindow::printView()
{
    QPrinter printer;
    QPrintDialog dialog(&printer,this);
    if(dialog.exec() == QDialog::Accepted){
        QPainter painter(&printer);
        ui->graphicsView->render(&painter);
    }
}

void MainWindow::displayAngle()
{
    ui->angle->setText(QString::number(ui->rotateSlider->value()));
    m_angle = ui->rotateSlider->value();
}

void MainWindow::emptyScene(QGraphicsScene *s)
{
    foreach(QGraphicsItem *item, s->items()){
            s->removeItem(item);
    }
}

void MainWindow::emptySumShots()
{
    this->m_sumShots=0;
    ui->lineEdit_ShotCount->setText(QString::number(this->m_sumShots));
}

void MainWindow::toggleShot()
{
    if(ui->checkBoxTrefferMausklick->isChecked()){
        ui->label_Treffer->setStyleSheet("QLabel { color : green; }");
        ui->label_Verschiebbar->setStyleSheet("QLabel { color : red; }");
        ui->label_Verschiebbar->setVisible(false);
        ui->label_Loeschen->setStyleSheet("QLabel { color : red; }");
        ui->label_Loeschen->setVisible(false);
        ui->graphicsView->setMouseClickForShots(true);
    }
    else{
        ui->label_Treffer->setStyleSheet("QLabel { color : red; }");
        ui->label_Verschiebbar->setStyleSheet("QLabel { color : green; }");
        ui->label_Verschiebbar->setVisible(true);
        ui->label_Loeschen->setStyleSheet("QLabel { color : green; }");
        ui->label_Loeschen->setVisible(true);
        ui->graphicsView->setMouseClickForShots(false);
    }
}

void MainWindow::centerTargetIsSet(QPointF pt)
{
    if(!pt.isNull()){
        ui->label_Schussziel->setStyleSheet("QLabel { color : green}");
        ui->checkBoxSchussziel->setChecked(true);
        this->m_centerPixel=pt;
        qDebug("Set pt=%f %f",pt.x(),pt.y());
    }
    else{
        ui->label_Schussziel->setStyleSheet("QLabel { color : red}");
        ui->checkBoxSchussziel->setChecked(false);
        this->m_centerPixel = QPointF();
        qDebug("Unset pt=%f %f",pt.x(),pt.y());
    }
}

void MainWindow::shotCountChanged(int shots)
{
    m_sumShots+=shots;
    ui->lineEdit_ShotCount->setText(QString::number(m_sumShots));
}

void MainWindow::fitInView()
{
    QRectF bounds = ui->graphicsView->scene()->itemsBoundingRect();
    bounds.setWidth(bounds.width()*0.9);         // to tighten-up margins
    bounds.setHeight(bounds.height()*0.9);       // same as above
//    ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
    ui->graphicsView->fitInView(ui->graphicsView->sceneRect());
    setupMatrix();
}

void MainWindow::calibration(QLineF line)
{
    m_mm = QInputDialog::getInt(this,"Kalibrierung","Strecke in mm",0,1,1000,1);
    if(m_mm > 0 && line.length()>0){
        m_pixel=line.length();
        m_calibFactor=m_mm/m_pixel;
        ui->label_Kalibrieren->setStyleSheet("QLabel { color : green}");
        ui->checkBoxKalibrieren->setChecked(true);
        QString text = QString("%1px=%2mm").arg(m_pixel,0,'f',0,QLatin1Char('0')).arg(m_mm,0,'f',0,QLatin1Char('0'));
        ui->label_KalibrierungValue->setText(text.trimmed());
    }
    else{
        ui->label_Kalibrieren->setStyleSheet("QLabel { color : red}");
        ui->checkBoxKalibrieren->setChecked(false);
        m_mm=0;
        m_pixel=0;
    }
    //QGraphicsLineItem löschen!!
    emit deleteCalibrationLine();
//    qDebug("P1x=%f P1y=%f P2x=%f P2y=%f Länge=%f",line.x1(),line.y1(),line.x2(),line.y2(),line.length());
}

void MainWindow::shotChanged(QList<QPointF> allShots)
{
    fillQLists(allShots);
}

void MainWindow::resetQLists()
{
    ui->listWidgetReal->clear();

}

void MainWindow::fillQLists(QList<QPointF> allShots)
{
    ui->listWidgetReal->clear();
    m_realShots.clear();
    QPointF realPoint = QPointF();
    foreach(QPointF point, allShots){

        // ins reale Koordinatensystem umrechen
        //
        //erforderlich:
        realPoint = transformScene2Real(point);
        QString str;
        str.sprintf("%8.2f  %8.2f     %8.2f   %8.2f°",realPoint.x(),realPoint.y(),sqrt(pow(realPoint.x(), 2) + pow(realPoint.y(), 2)),qRadiansToDegrees(qAtan2(realPoint.y(),realPoint.x())));
        QListWidgetItem *itemReal = new QListWidgetItem();
        itemReal->setText(str);
        ui->listWidgetReal->addItem(itemReal);
        m_realShots.append(realPoint);
    }
    calc();
}

void MainWindow::resetDate()
{
    QDate date = QDate::currentDate();
    ui->dateEdit->setDate(date);
}

QPointF MainWindow::transformScene2Real(QPointF scenePoint)
{
    // Wandelt scene Koordinaten in reale Koordinaten um
    QPointF realPoint = QPointF();

    // Verschieben
    realPoint = scenePoint-m_centerPixel;

    //Spiegeln
    realPoint.setY(-realPoint.y());

    // Drehen
    QPointF pp;
    qreal x = realPoint.x() * qCos(qDegreesToRadians(1.0*m_angle)) + realPoint.y() * qSin(qDegreesToRadians(1.0*m_angle));
    qreal y = -realPoint.x() * qSin(qDegreesToRadians(1.0*m_angle)) + realPoint.y() * qCos(qDegreesToRadians(1.0*m_angle));
    realPoint.setX(x);
    realPoint.setY(y);
    // auf mm transformieren
    realPoint.operator *=(m_calibFactor);
    return realPoint;
}

void MainWindow::calc()
{
    Statistic *values = new Statistic(m_realShots);
    QString text;
    QPointF shotGroupCenter;
    double meanRadius = 0;
    double meanRadiusC = 0;
    double moa = 0;
    double smoa = 0;
    double moaX = 0;
    double smoaX = 0;
    double moaY = 0;
    double smoaY = 0;
    QString strX;
    QString strY;
    QString strR;
    double phi = 0;
    double len = 0;
//    double sigma = 0;
    double sigmaC = 0;
//    double cep50= 0;
//    double cep90= 0;
//    double cep95= 0;
    double cep50C= 0;
    double cep90C= 0;
    double cep95C= 0;
    double lineLen = 0;
    double height = 0;
    double width = 0;
    QList<QPointF> p1p2;
    QLineF line;
    double fom = 0;
//    double stdabwX = 0;
//    double stdabwY = 0;
//    double rds     = 0;
    double stdabwXC = 0;
    double stdabwYC = 0;
    double rdsC     = 0;

    bool cOK = ui->checkBoxSchussziel->isChecked();
    bool kOK = ui->checkBoxKalibrieren->isChecked();
    bool oneOK = (m_realShots.length()>0) ? true: false;
    bool twoOK = (m_realShots.length()>1) ? true: false;

    // Anzahl **************************************************************************
    // Geht auch mit 0 Treffern, ohne Schussziel und Kalibrieren
    text = "";
    double anzahl = 0;
    bool parsingOK=false;
    anzahl = ui->lineEdit_ShotCount->text().toDouble(&parsingOK);
    if(parsingOK){
        text.sprintf("%4.0f   Treffer",anzahl);
    }
    else{
        text.sprintf("%4.0f   Treffer",0.0);
    }

    ui->label_AnzahlValue->setText(text);


    // Ringe ***************************************************************************
    // Center muss gesetzt sein und es muss kalibriert worden sein
    double rings=0;
    if(kOK && cOK){
        parsingOK=false;
        double ringDistance = ui->comboBoxRingabstand->currentText().toDouble(&parsingOK);
        if(parsingOK){
            rings = values->rings(ringDistance,m_caliber2);
        }
    }
    text.sprintf("%4.0f   Ringe",rings);
    ui->label_RingeValue->setText(text);


    // Mean arithmetig radius POA*******************************************************
    // mindestens 1 Treffer und Center und Kalibrieren erforderlich
    meanRadius=0;
    moa=0;
    smoa=0;
    if(oneOK && cOK && kOK){
        meanRadius = values->meanArithmetic(false);
        moa = qRadiansToDegrees(qAtan(meanRadius/m_distance))*60.;
        smoa = moa * 0.955555;
    }
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", meanRadius, moa,smoa);
    ui->label_MittlRadiusValue->setText(text);

    // Mean arithmetig radius COI*******************************************************
    // Nur 1 Treffer + Kalibrieren erforderlich
    meanRadiusC=0;
    moa=0;
    smoa=0;
    if(oneOK && kOK){
        meanRadiusC = values->meanArithmetic(true);
        moa = qRadiansToDegrees(qAtan(meanRadiusC/m_distance))*60.;
        smoa = moa * 0.955555;
    }
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", meanRadiusC, moa,smoa);
    m_circleR = meanRadiusC;
    ui->label_MittlRadiusValueC->setText(text);

    // Schussgruppezentrum oder Offset *************************************************
    // 1 Treffer und Center und Kalibrieren erforderlich
    moaX  = 0;
    smoaX = 0;
    moaY  = 0;
    smoaY = 0;
    phi = 0;
    len = 0;
    if(oneOK && cOK && kOK){
        shotGroupCenter = values->shotGroupCenter();
        phi = qAtan2(shotGroupCenter.y(),shotGroupCenter.x()) * 180/3.141592;
        len = Statistic::length(shotGroupCenter);
        moaX = qRadiansToDegrees(qAtan(shotGroupCenter.x()/m_distance))*60.;
        smoaX = moaX * 0.955555;
        moaY = qRadiansToDegrees(qAtan(shotGroupCenter.y()/m_distance))*60.;
        smoaY = moaY * 0.955555;
        moa = qRadiansToDegrees(qAtan(len/m_distance))*60.;
        smoa = moa * 0.955555;
    }
    strX.sprintf("%6.1f mm       ( %-6.1f/%6.1f )",shotGroupCenter.x(),moaX,smoaX);
    strY.sprintf("%6.1f mm       ( %-6.1f/%6.1f )",shotGroupCenter.y(),moaY,smoaY);
    strR.sprintf("%6.1f mm,%4.0f° ( %-6.1f/%6.1f )",len,phi,moa,smoa);
    ui->label_ShotGroupXValue->setText(strX);
    ui->label_ShotGroupYValue->setText(strY);
    ui->label_ShotGroup2Value->setText(strR);

//    // Sigma (POA) *****************************************************************
//    // 2 Treffer + Kalibrieren + Center
//    sigma=0;
//    moa=0;
//    smoa=0;
//    if(twoOK && cOK && kOK){
//        sigma = values->sigma(false);
//        moa = qRadiansToDegrees(qAtan(sigma/m_distance))*60.;
//        smoa = moa * 0.955555;
//    }
//    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", sigma,moa,smoa);
//    ui->label_SigmaValue->setText(text);

    // SigmaC (COI) ****************************************************************C
    // 2 Treffer + Kalibrieren + Center
    sigmaC = 0;
    moa = 0;
    smoa = 0;
    if(twoOK && cOK && kOK){
        sigmaC = values->sigma(true);
        moa = qRadiansToDegrees(qAtan(sigmaC/m_distance))*60.;
        smoa = moa * 0.955555;
    }
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", sigmaC,moa,smoa);
    m_circleSigma = sigmaC;
    ui->label_SigmaValueC->setText(text);

//    // CEP 50,90,95 (POA)
//    // CEP(F) = sigma * SQRT(-2 * Ln(1-F))
//    // 2 Treffer + Kalibrieren + Center
//    cep50= 0;
//    cep90= 0;
//    cep95= 0;
//    moa = 0;
//    smoa = 0;
//    if(twoOK && cOK && kOK){
//        cep50= sigma *1.177;
//        cep90= sigma *2.146;
//        cep95= sigma *2.448;
//    }
//    moa = qRadiansToDegrees(qAtan(cep50/m_distance))*60.;
//    smoa = moa * 0.955555;
//    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", cep50,moa,smoa);
//    ui->label_CEP50Value->setText(text);
//    moa = qRadiansToDegrees(qAtan(cep90/m_distance))*60.;
//    smoa = moa * 0.955555;
//    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", cep90,moa,smoa);
//    ui->label_CEP90Value->setText(text);
//    moa = qRadiansToDegrees(qAtan(cep95/m_distance))*60.;
//    smoa = moa * 0.955555;
//    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", cep95,moa,smoa);
//    ui->label_CEP95Value->setText(text);

    // CEP 50,90,95 (COI)
    // CEP(F) = sigma * SQRT(-2 * Ln(1-F))
    // 2 Treffer + Kalibrieren + Center
    cep50C= 0;
    cep90C= 0;
    cep95C= 0;
    moa = 0;
    smoa = 0;
    if(twoOK && cOK && kOK){
        cep50C= sigmaC *1.177;
        cep90C= sigmaC *2.146;
        cep95C= sigmaC *2.448;
    }
    moa = qRadiansToDegrees(qAtan(cep50C/m_distance))*60.;
    smoa = moa * 0.955555;
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", cep50C,moa,smoa);
    m_circleCEP50 = cep50C;
    ui->label_CEP50ValueC->setText(text);
    moa = qRadiansToDegrees(qAtan(cep90C/m_distance))*60.;
    smoa = moa * 0.955555;
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", cep90C,moa,smoa);
    m_circleCEP90 = cep90C;
    ui->label_CEP90ValueC->setText(text);
    moa = qRadiansToDegrees(qAtan(cep95C/m_distance))*60.;
    smoa = moa * 0.955555;
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", cep95C,moa,smoa);
    m_circleCEP95 = cep95C;
    ui->label_CEP95ValueC->setText(text);


    // Diagonale ***********************************************************************
    // nur 2 Treffer
    moa = 0;
    smoa = 0;
    lineLen = 0;
    if(twoOK){
        p1p2 = values->diagonal();
        line = QLineF(p1p2.at(0),p1p2.at(1));
        lineLen = line.length();
        moa = qRadiansToDegrees(qAtan(lineLen/m_distance))*60.;
        smoa = moa * 0.955555;
    }
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", lineLen,moa,smoa);
    ui->label_DiagonaleValue->setText(text);

    // Max H Max W FOM *****************************************************************
    // nur 2 Treffer
    moa = 0;
    smoa = 0;
    height = 0;
    width = 0;
    if(twoOK){
        p1p2 = values->diagonal();
        height = p1p2.at(1).y() - p1p2.at(0).y();
        width  = p1p2.at(1).x() - p1p2.at(0).x();

    }
    moa = qRadiansToDegrees(qAtan(height/m_distance))*60.;
    smoa = moa * 0.955555;
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", height,moa,smoa);
    ui->label_MaxHoeheValue->setText(text);
    moa = qRadiansToDegrees(qAtan(width/m_distance))*60.;
    smoa = moa * 0.955555;
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", width,moa,smoa);
    ui->label_MaxBreiteValue->setText(text);
    // FOM ***************************************************************************
    // nur 2 Treffer
    fom = 0;
    moa = 0;
    smoa = 0;
    if(twoOK){
        fom = (height + width)/2;
    }
    moa = qRadiansToDegrees(qAtan(fom/m_distance))*60.;
    smoa = moa * 0.955555;
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", fom,moa,smoa);
    ui->label_FOMValue->setText(text);

    // Extreme spread ******************************************************************
    // nur 2 Treffer
    lineLen = 0;
    moa = 0;
    smoa = 0;
    if(twoOK){
        p1p2 = values->extremeSpread();
        line = QLineF(p1p2.at(0),p1p2.at(1));
        lineLen = line.length();
    }
    moa = qRadiansToDegrees(qAtan(lineLen/m_distance))*60.;
    smoa = moa * 0.955555;
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", lineLen,moa,smoa);
    ui->label_ExtremeSpreadValue->setText(text);


//    // Std Abweichungen  (POA) ******************************************************
//    // X, Y, RDS
//    // zwei Treffer + Kalibrierung und Center
//    stdabwX = 0;
//    stdabwY = 0;
//    rds     = 0;
//    moa = 0;
//    smoa = 0;
//    if(twoOK && cOK && kOK){
//        stdabwX = values->stdabw(1,false);
//        stdabwY = values->stdabw(2,false);
//        rds     = values->stdabw(3,false);
//    }
//    moa = qRadiansToDegrees(qAtan(stdabwX/m_distance))*60.;
//    smoa = moa * 0.955555;
//    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", stdabwX,moa,smoa);
//    ui->label_StdAbwXValue->setText(text);
//    moa = qRadiansToDegrees(qAtan(stdabwY/m_distance))*60.;
//    smoa = moa * 0.955555;
//    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", stdabwY,moa,smoa);
//    ui->label_StdAbwYValue->setText(text);
//    moa = qRadiansToDegrees(qAtan(rds/m_distance))*60.;
//    smoa = moa * 0.955555;
//    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", rds,moa,smoa);
//    ui->label_RDSValue->setText(text);

    // Std Abweichungen  (COI)
    // X, Y, RDS
    // zwei Treffer + Kalibrierung und Center
    stdabwXC = 0;
    stdabwYC = 0;
    rdsC     = 0;
    moa = 0;
    smoa = 0;
    if(twoOK && cOK && kOK){
        stdabwXC = values->stdabw(1,true);
        stdabwYC = values->stdabw(2,true);
        rdsC     = values->stdabw(3,true);
    }
    moa = qRadiansToDegrees(qAtan(stdabwXC/m_distance))*60.;
    smoa = moa * 0.955555;
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", stdabwXC,moa,smoa);
    ui->label_StdAbwXValueC->setText(text);
    moa = qRadiansToDegrees(qAtan(stdabwYC/m_distance))*60.;
    smoa = moa * 0.955555;
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", stdabwYC,moa,smoa);
    ui->label_StdAbwYValueC->setText(text);
    moa = qRadiansToDegrees(qAtan(rdsC/m_distance))*60.;
    smoa = moa * 0.955555;
    text.sprintf("%6.1f mm       ( %-6.1f/%6.1f )", rdsC,moa,smoa);
    ui->label_RDSValueC->setText(text);
}

void MainWindow::on_actionPrint_triggered()
{

}

void MainWindow::on_actionSchootyBild_drucken_triggered()
{
    printView();
}

void MainWindow::on_action_ber_Shooty_triggered()
{
    QString ver = "Version";
    QString txt = "Dies ist Shooty Version " + m_version;
    QMessageBox::about(this,ver,txt);
}

void MainWindow::setDistance()
{
    bool parsingOK=false;
    double val = ui->comboBoxDistance->currentText().toDouble(&parsingOK);
    if(parsingOK){
           this->m_distance = val*1000;
    }
    else{
        m_distance=5000;
        ui->comboBoxDistance->setCurrentIndex(0);
        QMessageBox::information(this,"Fehler Information","Entfernung auf 5m gesetzt");
    }
    qDebug("Distance=%f",m_distance);
    calc();
}

void MainWindow::setCaliberList()
{
    m_caliberList.append(4.52);    //9mm
    m_caliberList.append(4.56);    //.38sp
    m_caliberList.append(4.57);    //.38sp WC
    m_caliberList.append(4.56);    //.356 Mag
}

void MainWindow::setCaliber()
{
    int index = ui->comboBox_Kaliber->currentIndex();
    m_caliber2 = m_caliberList.value(index);
//    qDebug("Index=%d Kaliber/2=%f",index,m_caliber2);
}

void MainWindow::loadSettings(){
    QSettings settings;

    m_config->m_PositionX           = settings.value("spinBoxXP",            35  ).toInt() ;
    m_config->m_PositionY           = settings.value("spinBoxYP",            30  ).toInt() ;
    m_config->m_boxSize             = settings.value("spinBoxSize",         130  ).toInt() ;
    m_config->m_b1x                 = settings.value("spinBoxB1X",           35  ).toInt() ;
    m_config->m_b1y                 = settings.value("spinBoxB1Y",          170  ).toInt() ;
    m_config->m_b2x                 = settings.value("spinBoxB2X",           35  ).toInt() ;
    m_config->m_b2y                 = settings.value("spinBoxB2Y",          205  ).toInt() ;
    m_config->m_b3x                 = settings.value("spinBoxB3X",           35  ).toInt() ;
    m_config->m_b3y                 = settings.value("spinBoxB3Y",          250  ).toInt() ;
    m_config->m_drechteck           = settings.value("cbRechteck",          true ).toBool() ;
    m_config->m_esline              = settings.value("cbLine",              true ).toBool() ;
    m_config->m_poaks               = settings.value("cbPoaks",             true ).toBool() ;
    m_config->m_coiks               = settings.value("cbCoiks",             true ).toBool() ;
    m_config->m_poacoiline          = settings.value("cbPoacoiLine",        true ).toBool() ;
    m_config->m_konvex              = settings.value("cbKonvex",            true ).toBool() ;
    m_config->m_trefferkoordinaten  = settings.value("cbTrefferkoordinaten",false).toBool() ;

    m_config->m_rundKonvexCMR       = settings.value("cbKonvexRundCMR",     false).toBool() ;
    m_config->m_rundSpannung        = settings.value("spinBoxRundSpannung",   0  ).toInt();
    m_config->m_rundKonvex          = settings.value("cbKonvexRund",        false).toBool() ;
    m_config->m_rundPrecision       = settings.value("spinBoxRundPrecision",  9  ).toInt();
    m_config->m_minPDFBox           = settings.value("minPDFBox",            11  ).toInt();

    m_config->m_circleR             = settings.value("circleR",             false).toBool();
    m_config->m_circleSigma         = settings.value("circleSigma",         false).toBool();
    m_config->m_circleCEP50         = settings.value("circleCEP50",         false).toBool();
    m_config->m_circleCEP90         = settings.value("circleCEP90",         false).toBool();
    m_config->m_circleCEP95         = settings.value("circleCEP95",         false).toBool();
    m_config->m_circleRinge         = settings.value("circleRinge",         false).toBool();

    m_config->m_regName             = settings.value("Serial/regName",      "").toString();
    m_config->m_regSerial           = settings.value("Serial/regSerial",    "").toString();
}

void MainWindow::saveSettings(){
    QSettings settings;
    settings.setValue("spinBoxXP",  m_config->m_PositionX);
    settings.setValue("spinBoxYP",  m_config->m_PositionY);
    settings.setValue("spinBoxSize",m_config->m_boxSize);
    settings.setValue("spinBoxB1X", m_config->m_b1x);
    settings.setValue("spinBoxB1Y", m_config->m_b1y);
    settings.setValue("spinBoxB2X", m_config->m_b2x);
    settings.setValue("spinBoxB2Y", m_config->m_b2y);
    settings.setValue("spinBoxB3X", m_config->m_b3x);
    settings.setValue("spinBoxB3Y", m_config->m_b3y);

    settings.setValue("cbRechteck",           m_config->m_drechteck);
    settings.setValue("cbLine",               m_config->m_esline);
    settings.setValue("cbPoaks",              m_config->m_poaks);
    settings.setValue("cbCoiks",              m_config->m_coiks);
    settings.setValue("cbPoacoiLine",         m_config->m_poacoiline);
    settings.setValue("cbKonvex",             m_config->m_konvex);
    settings.setValue("cbTrefferkoordinaten", m_config->m_trefferkoordinaten);

    settings.setValue("cbKonvexRundCMR",      m_config->m_rundKonvexCMR);
    settings.setValue("spinBoxRundSpannung",  m_config->m_rundSpannung);
    settings.setValue("cbKonvexRund",         m_config->m_rundKonvex);
    settings.setValue("spinBoxRundPrecision", m_config->m_rundPrecision);
    settings.setValue("minPDFBox",            m_config->m_minPDFBox);

    settings.setValue("circleR",              m_config->m_circleR);
    settings.setValue("circleSigma",          m_config->m_circleSigma);
    settings.setValue("circleCEP50",          m_config->m_circleCEP50);
    settings.setValue("circleCEP90",          m_config->m_circleCEP90);
    settings.setValue("circleCEP95",          m_config->m_circleCEP95);
    settings.setValue("circleRinge",          m_config->m_circleRinge);

    settings.setValue("Serial/regName",       m_config->m_regName);
    settings.setValue("Serial/regSerial",     m_config->m_regSerial);
}

void MainWindow::checkRegistration(){
    QDate currDate = QDate::currentDate();
    if(currDate> QDate(2019,12,31) && !m_isRegistered){
        QMessageBox msgBox;
        msgBox.setText("Dies ist eine Test-Version von Shooty!!\nWenn Sie dieses Programm verwenden möchten, wenden Sie sich an fz@zenmeister.de für eine kostenlose und neuere Version und Seriennummer von Shooty.");
        msgBox.exec();
        exit(EXIT_FAILURE);
    }
}

void MainWindow::closeEvent(QCloseEvent *event){

    saveSettings();
    event->accept();
}


void MainWindow::on_actionAls_PDF_Sichern_triggered()
{
    checkRegistration();
    if(!(m_realShots.length()>0 && ui->checkBoxSchussziel->isChecked() && ui->checkBoxKalibrieren->isChecked())){
        QMessageBox msgBox;
        msgBox.setText("Es fehlt die Eingabe des Zentrums (rechte Maustaste) oder die Eingabe eines Trefferpunktes (linke Maustaste) oder die Kalibrierung (Shift+linke Maustaste)!");
        msgBox.exec();
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Sichere Shooty PDF-Datei"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("PDF Datei (*.pdf);;All Files (*)"));

    makePDF(fileName);

}

void MainWindow::on_actionEinstellungen_triggered()
{
    Einstellungen einstellungen;
    einstellungen.setData(*m_config);
    einstellungen.setModal(true);
    if(einstellungen.exec() == QDialog::Accepted){
        m_config = einstellungen.getData();
        m_config->m_version = m_version;
    }

}

void MainWindow::on_actionAls_CSV_exportieren_triggered()
{
    if(!(m_realShots.length()>0 && ui->checkBoxSchussziel->isChecked() && ui->checkBoxKalibrieren->isChecked())){
        QMessageBox msgBox;
        msgBox.setText("Es fehlt die Eingabe des Zentrums (rechte Maustaste) oder die Eingabe eines Trefferpunktes (linke Maustaste) oder die Kalibrierung (Shift+linke Maustaste)!");
        msgBox.exec();
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Sichere Shooty CSV-Datei"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("CSV Datei (*.csv);;All Files (*)"));

    QFile csvData(fileName);
    if(csvData.open(QFile::WriteOnly|QFile::Truncate)){
        QTextStream output(&csvData);
        double x;
        double y;
        double r;
        double phi;
        QString str;
        int n=1;
        foreach(QPointF p , m_realShots){
            x = p.x();
            y = p.y();
            r = qSqrt(x*x + y*y);
            phi = qAtan2(y,x)*180 / 3.141592;
            str.sprintf("%4d ; %6.1f ; %6.1f ; %6.1f ; %6.1f\n",n, x,y,r,phi);
            output <<  str;
            n++;
        }
    }
    csvData.close();
}

void MainWindow::on_actionSichern_triggered()
{
    QFile *file;
    if (m_fileName.isEmpty()){
        return;
    }
    else {
        file = new QFile(m_fileName);
        if (!file->open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file->errorString());
            return;
        }
    }

    QList<QPointF> itemList = ui->graphicsView->getShots();
    Session *session = new Session(this);
    session->setSession(m_image,ui->lineEditSession->text(),ui->lineEditSchuetze->text(),ui->lineEditOrt->text(),
                        ui->comboBoxDistance->currentIndex(), ui->comboBox_Kaliber->currentIndex(),ui->comboBoxRingabstand->currentIndex(),
                        ui->dateEdit->date(),m_mm,m_pixel,m_angle,m_centerPixel,itemList);
    session->writeToFile(file);
    file->close();

}
void MainWindow::on_actionSichern_unter_triggered()
{
    QFile *file;
    QString fileName;
    if(m_fileName.isEmpty()){
        QString currDateTime = QDateTime::currentDateTime().toString(Qt::ISODate);
        currDateTime.replace(":","-");   // Für Dateinamen Doppelpunkte vermeiden
        currDateTime.replace("-","");    // Bindestriche löschen
        currDateTime.chop(2);            // Sekunden löschen
        fileName = QFileDialog::getSaveFileName(this,
                                                tr("Sichere Shooty Session-Daten"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+QDir::separator()+"Shooty"+currDateTime,
                                                tr("Shooty Datei (*.shf);;All Files (*)"));
    }else{
        fileName = QFileDialog::getSaveFileName(this,
                                                tr("Sichere Shooty Session-Daten"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+QDir::separator()+m_fileName,
                                                tr("Shooty Datei (*.shf);;All Files (*)"));

    }

    if (fileName.isEmpty()){
        return;
    }
    else {
        file = new QFile(fileName);
        if (!file->open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file->errorString());
            return;
        }
    }
    m_fileName = fileName;
    ui->actionSichern->setEnabled(true);
    QFileInfo fi(m_fileName);
    m_sessionTitle = m_fileName;
    if(ui->lineEditSession->text().isEmpty()){
        ui->lineEditSession->setText(fi.completeBaseName());
    }
    QWidget::setWindowTitle(m_sessionTitle);

    QList<QPointF> itemList = ui->graphicsView->getShots();
    qDebug("Gespeichert:");
    int i=0;
    foreach(QPointF p , itemList){
        QString txt;
        txt.sprintf("%d: %6.1f %6.1f",i++,p.x(),p.y());
        qDebug(txt.toLatin1());
    }

    Session *session = new Session(this);
    session->setSession(m_image,ui->lineEditSession->text(),ui->lineEditSchuetze->text(),ui->lineEditOrt->text(),
                        ui->comboBoxDistance->currentIndex(), ui->comboBox_Kaliber->currentIndex(),ui->comboBoxRingabstand->currentIndex(),
                        ui->dateEdit->date(),m_mm,m_pixel,m_angle,m_centerPixel,itemList);
    session->writeToFile(file);
    file->close();

}

void MainWindow::on_actionOpen_Session_triggered()
{
    checkRegistration();
    QFile *file;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Öffne Shooty Session"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    tr("Shooty Datei (*.shf);;All Files (*)"));

    if (fileName.isEmpty()){
        return;
    }
    else {
        file = new QFile(fileName);
        if (!file->open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file->errorString());
            return;
        }
    }

    initSession(true);
    m_fileName = fileName;
    ui->actionSichern->setEnabled(true);
    Session *session = new Session(this);
    session->loadFromFile(file);
    file->close();
    m_image = session->getSessionPixmap();
    ui->lineEditSession->setText(session->getSessionSession());
    ui->lineEditSchuetze->setText(session->getSessionSchuetze());
    ui->lineEditOrt->setText(session->getSessionOrt());

    ui->comboBoxDistance->setCurrentIndex(session->getEntfernungIndex());
    setDistance();   // ui -> membervariable
    ui->comboBox_Kaliber->setCurrentIndex(session->getCaliberIndex());
    setCaliber();    // ui -> membervariable
    ui->comboBoxRingabstand->setCurrentIndex(session->getRingDistanceIndex());
    ui->dateEdit->setDate(session->getDate());
    m_mm = session->getMM();
    m_pixel = session->getPixel();
    if(m_pixel != 0 && m_mm != 0){
        m_calibFactor=m_mm/m_pixel;
        ui->label_Kalibrieren->setStyleSheet("QLabel { color : green}");
        ui->checkBoxKalibrieren->setChecked(true);
        QString text = QString("%1px=%2mm").arg(m_pixel,0,'f',0,QLatin1Char('0')).arg(m_mm,0,'f',0,QLatin1Char('0'));
        ui->label_KalibrierungValue->setText(text.trimmed());
    }
    // ansonsten initialwerte übernehemen
    m_angle = session->getAngle();
    ui->rotateSlider->setValue(m_angle);
    displayAngle();  // ui -> membervariable
    resetQLists();

    m_centerPixel = session->getCenterTarget();

    QList<QPointF> liste = session->getAllShots();
    qDebug("Eingelesen:");
    int i=0;
    foreach(QPointF p , liste){
        QString txt;
        txt.sprintf("%d: %6.1f %6.1f",i++,p.x(),p.y());
        qDebug(txt.toLatin1());
    }

    m_scene = ui->graphicsView->scene();
    // Delete all items on scene
    emptyScene(m_scene);
    // Scene mit Bild laden
    m_scene->addPixmap(m_image);
    m_scene->setSceneRect(m_image.rect());
    qDebug("Bildgröße: x=%f y=%f wt=%f ht=%f", m_scene->sceneRect().bottomLeft().x(),m_scene->sceneRect().bottomLeft().y(),m_scene->sceneRect().width(),m_scene->sceneRect().height());
    ui->graphicsView->setScene(m_scene);

    ui->graphicsView->setCenterTargetFromSessionOpen(m_centerPixel);
    ui->graphicsView->setShotsFromSessionOpen(liste);
    m_sessionTitle = m_fileName;
    QWidget::setWindowTitle(m_sessionTitle);
}

void MainWindow::on_actionNeue_Session_triggered()
{
    checkRegistration();
    initSession(true);
    bool ok=false;
    QString text="";
    QString currDateTime = QDateTime::currentDateTime().toString(Qt::ISODate);
    currDateTime.replace(":","-");   // Für Dateinamen Doppelpunkte vermeiden
    currDateTime.replace("-","");    // Bindestriche löschen
    currDateTime.chop(2);            // Sekunden löschen
    text = QInputDialog::getText(this,"Neue Session","Name der Session",QLineEdit::Normal,"Shooty"+currDateTime,&ok);
    if(ok && !text.isEmpty()){
        m_sessionTitle = text;
        QWidget::setWindowTitle(m_sessionTitle);
        ui->lineEditSession->setText(m_sessionTitle);   // einmalig für Initialwert
        m_fileName = m_sessionTitle;
    }

    ui->actionSichern->setEnabled(false);
    on_toolButton_LoadImage_clicked();

}

void MainWindow::on_actionBild_laden_triggered()
{
    on_toolButton_LoadImage_clicked();
}

void MainWindow::on_actionShooty_Bild_sichern_triggered()
{
    QString imagePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Shooty Bild sichern"),
                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                    );

     QImage imageObject = m_image.toImage();

    imageObject.save(imagePath);
}

void MainWindow::on_toolButton_LoadImage_clicked()
{

    checkRegistration();
    initSession(false);

    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Wählen Sie ein Bild aus"),
                "",
                tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                );
    if(imagePath.isEmpty()){
        return;
    }

    m_imageObject = new QImage();
    m_imageObject->load(imagePath);

    // ab hier läufts wie session speichern / laden...
    m_image = QPixmap::fromImage(*m_imageObject);
    m_scene = ui->graphicsView->scene();
    // Delete all items on scene
    emptyScene(m_scene);
    // Scene mit Bild laden
    m_scene->addPixmap(m_image);
    m_scene->setSceneRect(m_image.rect());
    qDebug("Bildgröße: x=%f y=%f wt=%f ht=%f", m_scene->sceneRect().bottomLeft().x(),m_scene->sceneRect().bottomLeft().y(),m_scene->sceneRect().width(),m_scene->sceneRect().height());
    ui->graphicsView->setScene(m_scene);
}

void MainWindow::on_actionSeriennummer_triggered()
{
    m_serial->setData(m_config);
    m_serial->setModal(true);
    if(m_serial->exec() == QDialog::Accepted){
        m_serial->getData(m_config);
        m_isRegistered = m_serial->isOK();
    }
}


void MainWindow::makePDF(QString fn){
    const QPoint imageCoordinates(0,0);

    QPdfWriter pdfWriter(fn);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&pdfWriter);

    Statistic *values = new Statistic(m_realShots);
    QList<QPointF> d = values->diagonal();
    QList<QPointF> es = values->extremeSpread();
    PdfDaten *pdfdaten = new PdfDaten(*m_config, ui->lineEditSession->text(),
                                      ui->lineEditSchuetze->text(),
                                      ui->lineEditOrt->text(),
                                      ui->dateEdit->text(),
                                      ui->comboBox_Kaliber->currentText(),
                                      ui->comboBoxDistance->currentText(),
                                      ui->comboBoxRingabstand->currentText(),
                                      m_realShots,m_caliber2,ui->label_RingeValue->text(),
                                      ui->label_AnzahlValue->text(),
                                      ui->label_DiagonaleValue->text(),
                                      ui->label_ExtremeSpreadValue->text(),
                                      ui->label_MaxHoeheValue->text(),
                                      ui->label_MaxBreiteValue->text(),
                                      ui->label_FOMValue->text(),
//                                      ui->label_StdAbwXValue->text(),
//                                      ui->label_StdAbwYValue->text(),
//                                      ui->label_RDSValue->text(),
//                                      ui->label_CEP50Value->text(),
//                                      ui->label_CEP90Value->text(),
//                                      ui->label_CEP95Value->text(),
                                      m_circleR,
                                      m_circleSigma,
                                      m_circleCEP50,
                                      m_circleCEP90,
                                      m_circleCEP95,
                                      ui->label_StdAbwXValueC->text(),
                                      ui->label_StdAbwYValueC->text(),
                                      ui->label_RDSValueC->text(),
                                      ui->label_MittlRadiusValueC->text(),
                                      ui->label_SigmaValueC->text(),
                                      ui->label_CEP50ValueC->text(),
                                      ui->label_CEP90ValueC->text(),
                                      ui->label_CEP95ValueC->text(),
                                      ui->label_MittlRadiusValue->text(),
                                      ui->label_ShotGroupXValue->text(),
                                      ui->label_ShotGroupYValue->text(),
                                      ui->label_ShotGroup2Value->text(),
//                                      ui->label_SigmaValue->text(),
                                      d,es,values->shotGroupCenter(),
                                      ui->listWidgetReal);
    CreatePDF *pdf = new CreatePDF(&painter,pdfdaten, this);

}

void MainWindow::on_toolButtonViewPDF_clicked()
{
    checkRegistration();
    if(!(m_realShots.length()>0 && ui->checkBoxSchussziel->isChecked() && ui->checkBoxKalibrieren->isChecked())){
        QMessageBox msgBox;
        msgBox.setText("Es fehlt die Eingabe des Zentrums (rechte Maustaste) oder die Eingabe eines Trefferpunktes (linke Maustaste) oder die Kalibrierung (Shift+linke Maustaste)!");
        msgBox.exec();
        return;
    }

    QTemporaryFile fn;
    fn.setAutoRemove(false);
    fn.setFileTemplate(QDir::tempPath() + "XXXXXX.pdf");
    if(fn.open()){
        makePDF(fn.fileName());
#ifdef Q_OS_WIN
        QUrl url(fn.fileName() ,QUrl::TolerantMode);
#else
        QUrl url("file://" + fn.fileName() ,QUrl::TolerantMode);
#endif
        qDebug() << url.toString();
        QDesktopServices::openUrl(url);
    }
}

#ifdef Q_OS_WIN
void MainWindow::on_actionBeenden_triggered()
{
    saveSettings();
    QApplication::quit();
}
#endif

