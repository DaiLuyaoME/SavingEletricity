#-------------------------------------------------
#
# Project created by QtCreator 2016-07-31T12:48:21
#
#-------------------------------------------------

QT       += core gui sql printsupport
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SavingElectricity
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    realtimedatawindow.cpp \
    historydatawindow.cpp \
    testwindow.cpp \
    configwindow.cpp \
    ammeter.cpp \
    dataprocessor.cpp \
    regulator.cpp \
    database.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    datapoint.h \
    realtimedatawindow.h \
    historydatawindow.h \
    testwindow.h \
    configwindow.h \
    ammeter.h \
    dataprocessor.h \
    regulator.h \
    database.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    realtimedatawindow.ui \
    historydatawindow.ui \
    testwindow.ui \
    configwindow.ui

RESOURCES += \
    resource.qrc
