#-------------------------------------------------
#
# Project created by QtCreator 2015-05-26T15:46:17
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xsd1
TEMPLATE = app
CONFIG += c++11
LIBS += -lexpat
SOURCES += main.cpp\
        mainwindow.cpp \
        symbol.cpp \
        leda-xdl-symbol-pskel.cxx \
    symbolparser.cpp \
    leda-xdl-symbol-pimpl.cpp

HEADERS  += mainwindow.h \
        symbol.h \
        leda-xdl-symbol-pskel.hxx \
    symbolparser.h \
    leda-xdl-symbol-pimpl.h

FORMS    += mainwindow.ui

DEFINES += SYMBOL_DIR=\\\"$$PWD/qucs-symbols\\\"

DISTFILES += \
    leda-xdl-symbol.xsd \
    leda-xdl-symbol.map \
    xsd.make
