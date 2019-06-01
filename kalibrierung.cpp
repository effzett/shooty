#include "kalibrierung.h"
#include "ui_kalibrierung.h"

Kalibrierung::Kalibrierung(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kalibrierung)
{
    ui->setupUi(this);
}

Kalibrierung::~Kalibrierung()
{
    delete ui;
}
