#-------------------------------------------------
#
# Project created by QtCreator 2014-08-05T14:51:06
#
#-------------------------------------------------

QT       += core gui script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = action-items
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    actionhandler.cpp \
    menuitem.cpp

HEADERS  += mainwindow.h \
    actionhandler.h \
    menuitem.h

FORMS    += mainwindow.ui
