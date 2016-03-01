#-------------------------------------------------
#
# Project created by QtCreator 2016-02-26T19:48:38
#
#-------------------------------------------------

CONFIG += c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphicsview-track-routing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicsview.cpp \
    ipathrouter.cpp \
    tests.cpp \
    routetracktask.cpp \
    igraphicstask.cpp \
    linetrackelement.cpp \
    tracknode.cpp

HEADERS  += mainwindow.h \
    graphicsview.h \
    ipathrouter.h \
    tests.h \
    routetracktask.h \
    igraphicstask.h \
    linetrackelement.h \
    tracknode.h

FORMS    += mainwindow.ui
