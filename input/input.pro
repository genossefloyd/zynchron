#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T21:44:45
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = input
TEMPLATE = app


SOURCES += main.cpp\
        maingui.cpp \
    bluetoothconnector.cpp

HEADERS  += maingui.h \
    bluetoothconnector.h

FORMS    += maingui.ui

unix: LIBS += -L$$PWD/../libs/metawear/build-metawear-Desktop_Qt_5_7_0_clang_64bit-Debug/ -lmetawear.1.0.0

INCLUDEPATH += $$PWD/../libs/metawear
DEPENDPATH += $$PWD/../libs/metawear
