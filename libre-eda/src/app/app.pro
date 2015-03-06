#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T16:00:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core
LIBS += -L$$OUT_PWD/../core/ -llibre-eda-core

INCLUDEPATH += $$PWD/../modules/symboleditor
DEPENDPATH += $$PWD/../modules/symboleditor
LIBS += -L$$OUT_PWD/../modules/symboleditor/ -llibre-eda-symbol-editor
