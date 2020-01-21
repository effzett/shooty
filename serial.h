#ifndef SERIAL_H
#define SERIAL_H
#include <configdaten.h>
#include <QDialog>

namespace Ui {
class Serial;
}

class Serial : public QDialog
{
    Q_OBJECT

public:
    explicit Serial(QWidget *parent = nullptr);
    ~Serial();
    void setData(ConfigDaten *cfg);
    void getData(ConfigDaten *cfg);
    bool isOK();
    QString getRegName();

private:
    Ui::Serial *ui;
    QString m_regName;
    QString m_regSerial;
    bool check();
};

#endif // SERIAL_H
