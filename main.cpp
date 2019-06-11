#include "mainwindow.h"
#include <QApplication>
#include <myapplication.h>
#include <QtPrintSupport>
#include <QTimer>

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#ifdef Q_OS_WIN
    QApplication a(argc,argv);
#else
    MyApplication a(argc,argv);
#endif
    a.setOrganizationName("Zenmeister");
    a.setOrganizationDomain("Zenmeister");
    a.setApplicationName("Shooty");
    MainWindow w;

#ifdef Q_OS_WIN
    QTimer::singleShot(0,&w,SLOT(fromCommandLine()));
#else
    w.connectOpenWithApp(&a);
#endif

    w.show();

    return a.exec();
}
