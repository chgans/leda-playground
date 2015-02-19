#-------------------------------------------------
#
# Project created by QtCreator 2015-02-16T11:45:56
#
#-------------------------------------------------

QT       += core gui

QT += KItemModels KItemViews

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = item-model-1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    documentmodel.cpp \
    libraryproxymodel.cpp \
    UngroupProxyModel.cpp

HEADERS  += mainwindow.h \
    documentmodel.h \
    documentitems.h \
    libraryproxymodel.h \
    UngroupProxyModel.h

FORMS    += mainwindow.ui
