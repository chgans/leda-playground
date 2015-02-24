#-------------------------------------------------
#
# Project created by QtCreator 2015-02-24T15:29:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = log-viewer-1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logcat1.cpp \
    messagelogviewer.cpp

HEADERS  += mainwindow.h \
    logcat1.h \
    messagelogviewer.h

FORMS    += mainwindow.ui \
    messagelogviewer.ui
