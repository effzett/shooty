#ifndef KALIBRIERUNG_H
#define KALIBRIERUNG_H

#include <QDialog>

namespace Ui {
class Kalibrierung;
}

class Kalibrierung : public QDialog
{
    Q_OBJECT

public:
    explicit Kalibrierung(QWidget *parent = 0);
    ~Kalibrierung();

private:
    Ui::Kalibrierung *ui;
};

#endif // KALIBRIERUNG_H
