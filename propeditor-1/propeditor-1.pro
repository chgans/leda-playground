#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T14:40:33
#
#-------------------------------------------------

QT       += core gui widgets svg

include(3rdParty/qtpropertybrowser/qtpropertybrowser.pri)
include(3rdParty/qtgradienteditor/qtgradienteditor.pri)
include(3rdParty/misc/shared.pri)

TARGET = propeditor-1
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    propertymanager.cpp \
    graphicsobject.cpp \
    labelitem.cpp \
    textframeitem.cpp \
    svgitem.cpp \
    textframeitemeditor.cpp

HEADERS  += mainwindow.h \
    GraphicsObject.h \
    propertymanager.h \
    labelitem.h \
    textframeitem.h \
    svgitem.h \
    textframeitemeditor.h

FORMS    += mainwindow.ui

RESOURCES += \
    propeditor-1.qrc
