#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T16:14:56
#
#-------------------------------------------------

QT       += widgets

TARGET = libre-eda-symbol-editor
TEMPLATE = lib

DEFINES += SYMBOLEDITOR_LIBRARY

SOURCES += symboleditor.cpp

HEADERS += symboleditor.h\
        symboleditor_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L$$OUT_PWD/../../core/ -llibre-eda-core

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core
