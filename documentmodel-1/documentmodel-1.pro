#-------------------------------------------------
#
# Project created by QtCreator 2015-02-19T23:51:43
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = documentmodel-1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    documentitem.cpp \
    librarydocumentmodel.cpp

HEADERS  += mainwindow.h \
    documentitem.h \
    librarydocumentmodel.h

FORMS    += mainwindow.ui
