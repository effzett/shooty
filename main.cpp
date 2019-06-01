#include "mainwindow.h"
#include <QApplication>
#include <QtPrintSupport>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Zenmeister");
    a.setOrganizationDomain("Zenmeister");
    a.setApplicationName("Shooty");
    MainWindow w;

//    QTimer::singleShot(0,&w,SLOT(fromCommandLine()));

    w.show();

    return a.exec();
}
