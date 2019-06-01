#include "serial.h"
#include "ui_serial.h"
#include <configdaten.h>
#include <QCryptographicHash>

Serial::Serial(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Serial)
{
    ui->setupUi(this);
}

Serial::~Serial()
{
    delete ui;
}

void Serial::setData(ConfigDaten *cfg){
    m_regName = cfg->m_regName;
    m_regSerial = cfg->m_regSerial;
    ui->lineEditVorNachname->setText(m_regName);
    ui->lineEditSeriennummer->setText(m_regSerial);
}

void Serial::getData(ConfigDaten *cfg){
    m_regName = ui->lineEditVorNachname->text();
    m_regSerial = ui->lineEditSeriennummer->text();
    cfg->m_regName = m_regName;
    cfg->m_regSerial = m_regSerial;
}

bool Serial::isOK(){

    qDebug("Seriennummer-Erzeugung:  <%s> ergibt <%s>",m_regName.toStdString().c_str(),
           QCryptographicHash::hash(m_regName.toUtf8(),QCryptographicHash::Md5).toHex().toStdString().substr(16).c_str());
    return check();
}

bool Serial::check(){
 bool retVal = false;
 // nur provisorisch
    if(m_regSerial == QCryptographicHash::hash(m_regName.toUtf8(),QCryptographicHash::Md5).toHex().toStdString().substr(16).c_str()){
        retVal = true;
    }
    return retVal;
}

QString Serial::getRegName(){
    return m_regName;
}

