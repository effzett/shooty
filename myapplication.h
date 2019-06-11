#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H
#include <QApplication>
#include <QFileOpenEvent>
#include <QEvent>


class MyApplication : public QApplication
{
    Q_OBJECT

public:
    QString fileName;

    MyApplication(int &argc, char **argv) : QApplication(argc, argv)
    {
    }


signals:

    void fileReady(QString);

protected:

    bool event(QEvent *event) override
    {
        if(event->type() == QEvent::FileOpen){
            QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
            fileName =  openEvent->file();
            emit fileReady(fileName);
        }

        return QApplication::event(event);
    }
};
#endif // MYAPPLICATION_H
