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
    serial.h \
    myapplication.h

macx: {
FORMS    += mainwindow.ui \
            einstellungen.ui \
            serial.ui
    message("MacOS Architektur")

ICON = shooty1.icns

QMAKE_INFO_PLIST += myShooty.plist
}

win32: {
FORMS    += mainwindowwin.ui \
            einstellungenwin.ui \
            serialwin.ui
    message("Win32 Architektur")

RC_FILE = shooty.rc
}

RESOURCES += \
    resources.qrc


DISTFILES += \
    shooty.ico \
    shooty.rc \
    myShooty.plist
