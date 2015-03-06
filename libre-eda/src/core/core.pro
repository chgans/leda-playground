#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T16:25:24
#
#-------------------------------------------------

QT       += widgets

CONFIG += c++11

TARGET = libre-eda-core
TEMPLATE = lib

DEFINES += CORE_LIBRARY

SOURCES += core.cpp \
    document.cpp \
    documentfactory.cpp \
    documentcreator.cpp \
    editor.cpp \
    editorcreator.cpp \
    editorfactory.cpp

HEADERS += core.h\
        core_global.h \
    abstractfactory.h \
    singleton.h \
    document.h \
    documentfactory.h \
    documentcreator.h \
    editor.h \
    editorcreator.h \
    editorfactory.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
