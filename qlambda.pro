#-------------------------------------------------
#
# Project created by QtCreator 2015-12-07T20:25:59
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qlambda
TEMPLATE = app

linux {
    LIBS += -lhidapi-hidraw
}

SOURCES += main.cpp\
        mainwindow.cpp \
    serialportsmodel.cpp \
    dilambda.cpp \
    usbhiddevicesmodel.cpp

HEADERS  += mainwindow.h \
    serialportsmodel.h \
    dilambda.h \
    usbhiddevicesmodel.h

FORMS    += mainwindow.ui
