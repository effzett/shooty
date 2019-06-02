#include "einstellungen.h"
#ifdef Q_OS_WIN
#include "ui_einstellungenwin.h"
#else
#include "ui_einstellungen.h"
#endif
#include <mainwindow.h>

Einstellungen::Einstellungen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Einstellungen)
{
    ui->setupUi(this);
}

Einstellungen::~Einstellungen()
{
    delete ui;
}

ConfigDaten *Einstellungen::getData()
{
    ConfigDaten *cfg = new ConfigDaten();
    m_x = ui->spinBoxXP->value();
    m_y = ui->spinBoxYP->value();
    m_xysize = ui->spinBoxSize->value();

    m_b1x = ui->spinBoxB1x->value();
    m_b1y = ui->spinBoxB1y->value();
    m_b2x = ui->spinBoxB2x->value();
    m_b2y = ui->spinBoxB2y->value();
    m_b3x = ui->spinBoxB3x->value();
    m_b3y = ui->spinBoxB3y->value();
    
    m_drechteck         = ui->checkBox_DRechteck->isChecked();
    m_esline            = ui->checkBox_ESLine->isChecked();
    m_poaks             = ui->checkBox_POAKS->isChecked();
    m_coiks             = ui->checkBox_COIKS->isChecked();
    m_poacoiline        = ui->checkBox_POACOILine->isChecked();
    m_konvex            = ui->checkBox_Konvex->isChecked();
    m_trefferkoordinaten= ui->checkBox_TrefferKoordinaten->isChecked();

    m_rundKonvexCMR     = ui->checkBox_KonvexCMR->isChecked();
    m_rundSpannung      = ui->spinBoxSpannung->value();
    m_rundKonvex        = ui->checkBox_KonvexRund->isChecked();
    m_rundPrecision     = ui->spinBoxKonvex->value();
    m_minPDFBox         = ui->spinBoxMinPDFBox->value();

    m_circleR           = ui->checkBox_cMittlRadius->isChecked();
    m_circleSigma       = ui->checkBox_cSigma->isChecked();
    m_circleCEP50       = ui->checkBox_cCEP50->isChecked();
    m_circleCEP90       = ui->checkBox_cCEP90->isChecked();
    m_circleCEP95       = ui->checkBox_cCEP95->isChecked();
    m_circleRinge       = ui->checkBox_cRinge->isChecked();

    cfg->m_PositionX=m_x;
    cfg->m_PositionY=m_y;
    cfg->m_boxSize=m_xysize;

    cfg->m_b1x = m_b1x;
    cfg->m_b1y = m_b1y;
    cfg->m_b2x = m_b2x;
    cfg->m_b2y = m_b2y;
    cfg->m_b3x = m_b3x;
    cfg->m_b3y = m_b3y;

    cfg->m_drechteck         =  m_drechteck         ;
    cfg->m_esline            =  m_esline            ;
    cfg->m_poaks             =  m_poaks             ;
    cfg->m_coiks             =  m_coiks             ;
    cfg->m_poacoiline        =  m_poacoiline        ;
    cfg->m_konvex            =  m_konvex            ;
    cfg->m_trefferkoordinaten=  m_trefferkoordinaten;

    cfg->m_rundKonvexCMR     =  m_rundKonvexCMR     ;
    cfg->m_rundSpannung      =  m_rundSpannung      ;
    cfg->m_rundKonvex        =  m_rundKonvex        ;
    cfg->m_rundPrecision     =  m_rundPrecision     ;
    cfg->m_minPDFBox         =  m_minPDFBox         ;

    cfg->m_circleR           = m_circleR            ;
    cfg->m_circleSigma       = m_circleSigma        ;
    cfg->m_circleCEP50       = m_circleCEP50        ;
    cfg->m_circleCEP90       = m_circleCEP90        ;
    cfg->m_circleCEP95       = m_circleCEP95        ;
    cfg->m_circleRinge       = m_circleRinge        ;

    return cfg;
}

void Einstellungen::setData(ConfigDaten cfg)
{
    m_x = cfg.m_PositionX;
    m_y = cfg.m_PositionY;
    m_xysize = cfg.m_boxSize;
    
    m_b1x = cfg.m_b1x;
    m_b1y = cfg.m_b1y;
    m_b2x = cfg.m_b2x;
    m_b2y = cfg.m_b2y;
    m_b3x = cfg.m_b3x;
    m_b3y = cfg.m_b3y;
    
    m_drechteck          = cfg.m_drechteck         ;
    m_esline             = cfg.m_esline            ;
    m_poaks              = cfg.m_poaks             ;
    m_coiks              = cfg.m_coiks             ;
    m_poacoiline         = cfg.m_poacoiline        ;
    m_konvex             = cfg.m_konvex            ;
    m_trefferkoordinaten = cfg.m_trefferkoordinaten;

    m_rundKonvexCMR      = cfg.m_rundKonvexCMR     ;
    m_rundSpannung       = cfg.m_rundSpannung      ;
    m_rundKonvex         = cfg.m_rundKonvex        ;
    m_rundPrecision      = cfg.m_rundPrecision     ;
    m_minPDFBox          = cfg.m_minPDFBox         ;

    m_circleR            = cfg.m_circleR          ;
    m_circleSigma        = cfg.m_circleSigma      ;
    m_circleCEP50        = cfg.m_circleCEP50      ;
    m_circleCEP90        = cfg.m_circleCEP90      ;
    m_circleCEP95        = cfg.m_circleCEP95      ;
    m_circleRinge        = cfg.m_circleRinge      ;

    ui->spinBoxXP->setValue(m_x);
    ui->spinBoxYP->setValue(m_y);
    ui->spinBoxSize->setValue(m_xysize);
    
    ui->spinBoxB1x->setValue(m_b1x);
    ui->spinBoxB1y->setValue(m_b1y);
    ui->spinBoxB2x->setValue(m_b2x);
    ui->spinBoxB2y->setValue(m_b2y);
    ui->spinBoxB3x->setValue(m_b3x);
    ui->spinBoxB3y->setValue(m_b3y);

    ui->checkBox_DRechteck->setChecked(m_drechteck);
    ui->checkBox_ESLine->setChecked(m_esline);
    ui->checkBox_POAKS->setChecked(m_poaks);
    ui->checkBox_COIKS->setChecked(m_coiks);
    ui->checkBox_POACOILine->setChecked(m_poacoiline);
    ui->checkBox_Konvex->setChecked(m_konvex);
    ui->checkBox_TrefferKoordinaten->setChecked(m_trefferkoordinaten);

    ui->checkBox_KonvexCMR->setChecked(m_rundKonvexCMR);
    ui->spinBoxSpannung->setValue(m_rundSpannung);
    ui->checkBox_KonvexRund->setChecked(m_rundKonvex);
    ui->spinBoxKonvex->setValue(m_rundPrecision);
    ui->spinBoxMinPDFBox->setValue(m_minPDFBox);

    ui->checkBox_cMittlRadius->setChecked(m_circleR);
    ui->checkBox_cSigma->setChecked(m_circleSigma);
    ui->checkBox_cCEP50->setChecked(m_circleCEP50);
    ui->checkBox_cCEP90->setChecked(m_circleCEP90);
    ui->checkBox_cCEP95->setChecked(m_circleCEP95);
    ui->checkBox_cRinge->setChecked(m_circleRinge);
}

void Einstellungen::on_checkBox_TrefferKoordinaten_clicked()
{
    QMessageBox::StandardButton reply;

    bool isChecked = ui->checkBox_TrefferKoordinaten->isChecked();
    int dx=0;
    int newValue=0;
    int width= 130;
    if(isChecked){
        int cl = ui->spinBoxSize->value() + ui->spinBoxB1x->value();
        if(cl>width){
                dx = cl - width ;
        }
        newValue = ui->spinBoxSize->value() - dx;
        reply = QMessageBox::question(this,"Platzproblem", "Die Zeichnungsbox muss reduziert werden. Möchten Sie das?",QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            ui->spinBoxSize->setValue(newValue);
        }
    }
}

void Einstellungen::on_pushButtonReset_clicked()
{
    ui->spinBoxXP->setValue(35);
    ui->spinBoxYP->setValue(30);
    ui->spinBoxSize->setValue(130);
    ui->spinBoxB1x->setValue(35);
    ui->spinBoxB1y->setValue(170);
    ui->spinBoxB2x->setValue(35);
    ui->spinBoxB2y->setValue(205);
    ui->spinBoxB3x->setValue(35);
    ui->spinBoxB3y->setValue(230);
    ui->checkBox_COIKS->setChecked(true);
    ui->checkBox_DRechteck->setChecked(true);
    ui->checkBox_ESLine->setChecked(true);
    ui->checkBox_POACOILine->setChecked(true);
    ui->checkBox_POAKS->setChecked(true);
    ui->checkBox_Konvex->setChecked(false);
    ui->checkBox_KonvexCMR->setChecked(true);
    ui->checkBox_KonvexRund->setChecked(false);
    ui->checkBox_TrefferKoordinaten->setChecked(false);
    ui->spinBoxSpannung->setValue(0);
    ui->spinBoxKonvex->setValue(1);
    ui->spinBoxMinPDFBox->setValue(11);

    ui->checkBox_cMittlRadius->setChecked(false);
    ui->checkBox_cSigma->setChecked(false);
    ui->checkBox_cCEP50->setChecked(false);
    ui->checkBox_cCEP90->setChecked(false);
    ui->checkBox_cCEP95->setChecked(false);
    ui->checkBox_cRinge->setChecked(false);
}
