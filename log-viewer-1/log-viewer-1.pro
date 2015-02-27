#-------------------------------------------------
#
# Project created by QtCreator 2015-02-24T15:29:42
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = log-viewer-1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logcat1.cpp \
    logmessage.cpp \
    loggingmodel.cpp \
    loggingsortfilterproxymodel.cpp \
    logcat2.cpp \
    loggingwidget.cpp

HEADERS  += mainwindow.h \
    logcat1.h \
    logmessage.h \
    loggingmodel.h \
    loggingsortfilterproxymodel.h \
    logcat2.h \
    loggingwidget.h

FORMS    += mainwindow.ui \
    loggingwidget.ui

OTHER_FILES += \
    TODO.md
