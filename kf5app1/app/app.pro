#-------------------------------------------------
#
# Project created by QtCreator 2015-04-24T15:20:56
#
#-------------------------------------------------

CONFIG += c++11

# Qt modules
QT       += core gui

# KF5 modules
QT       += KXmlGui KService KI18n

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mainwindow.cpp

HEADERS  += widget.h \
    mainwindow.h \
    plugin.h

RESOURCES += \
    app.qrc

OTHER_FILES += \
    appui.rc
