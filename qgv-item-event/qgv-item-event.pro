#-------------------------------------------------
#
# Project created by QtCreator 2014-07-09T10:48:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = qgv-item-event
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicsscene.cpp \
    graphicsview.cpp \
    graphicsrectitem.cpp \
    graphicstool.cpp \
    graphicsselecttool.cpp \
    graphicslinetool.cpp \
    graphicslinetooldialog.cpp \
    graphicslineitem.cpp \
    graphicscontrolpoint.cpp \
    graphicsobject.cpp \
    graphicsrecttool.cpp

HEADERS  += mainwindow.h \
    graphicsscene.h \
    graphicsview.h \
    graphicsrectitem.h \
    graphicstool.h \
    graphicsselecttool.h \
    graphicslinetool.h \
    graphicslinetooldialog.h \
    graphicslineitem.h \
    graphicscontrolpoint.h \
    graphicsobject.h \
    graphicsrecttool.h

FORMS    += mainwindow.ui \
    graphicslinetooldialog.ui

DISTFILES += \
    graphicslinetool.svg

RESOURCES += \
    qgv-item-event.qrc
