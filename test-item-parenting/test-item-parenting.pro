#-------------------------------------------------
#
# Project created by QtCreator 2014-07-03T15:55:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test-item-parenting
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dockwidget.cpp

HEADERS  += mainwindow.h \
    dockwidget.h

FORMS    += mainwindow.ui \
    dockwidget.ui

RESOURCES += \
    res.qrc
