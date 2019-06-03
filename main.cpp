#include "mainwindow.h"
#include <QApplication>
#include <QtPrintSupport>
#include <QTimer>

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication a(argc, argv);
    a.setOrganizationName("Zenmeister");
    a.setOrganizationDomain("Zenmeister");
    a.setApplicationName("Shooty");
    MainWindow w;

#ifdef Q_OS_WIN
    QTimer::singleShot(0,&w,SLOT(fromCommandLine()));
#endif

    w.show();

    return a.exec();
}
