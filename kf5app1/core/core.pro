#-------------------------------------------------
#
# Project created by QtCreator 2015-04-25T12:46:46
#
#-------------------------------------------------

CONFIG += c++11

QT       -= gui

TARGET = core
TEMPLATE = lib

DEFINES += CORE_LIBRARY

SOURCES += core.cpp

HEADERS += core.h\
        core_global.h \
    plugin.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
