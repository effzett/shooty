#-------------------------------------------------
#
# Project created by QtCreator 2019-04-19T20:26:38
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += app_bundle

ICON = shooty1.icns

TARGET = Shooty
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myqgraphicsview.cpp \
    centertarget.cpp \
    statistic.cpp \
    createpdf.cpp \
    einstellungen.cpp \
    configdaten.cpp \
    pdfdaten.cpp \
    session.cpp \
    serial.cpp

HEADERS  += mainwindow.h \
    myqgraphicsview.h \
    centertarget.h \
    statistic.h \
    createpdf.h \
    einstellungen.h \
    configdaten.h \
    pdfdaten.h \
    session.h \
    serial.h

FORMS    += mainwindow.ui \
    einstellungen.ui \
    serial.ui

RESOURCES += \
    resources.qrc
