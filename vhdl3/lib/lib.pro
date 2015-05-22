#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T09:59:38
#
#-------------------------------------------------

QT       += xml xmlpatterns
QT       -= gui

CONFIG += c++11

TARGET = lib
TEMPLATE = lib

DEFINES += LIB_LIBRARY

SOURCES += document.cpp

HEADERS += document.h\
        lib_global.h \
    architecture.h \
    configuration.h \
    entity.h \
    designunit.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
