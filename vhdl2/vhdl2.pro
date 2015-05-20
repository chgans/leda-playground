#-------------------------------------------------
#
# Project created by QtCreator 2015-05-03T19:40:21
#
#-------------------------------------------------

QT       += core gui qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vhdl2
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp

HEADERS  += mainwindow.h \
    architecture.h \
    entity.h \
    configuration.h

FORMS    += mainwindow.ui

DISTFILES += \
    test1.qml \
    TtForm.ui.qml \
    Tt.qml \
    entities.xml \
    symbols.xml \
    design.xml
