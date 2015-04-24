#-------------------------------------------------
#
# Project created by QtCreator 2015-04-24T16:17:58
#
#-------------------------------------------------

CONFIG   += c++11
QT       += widgets
QT       += KXmlGui KTextWidgets KI18n

DEFINES += KDEPLUGIN1_LIBRARY
TARGET = texteditor
TEMPLATE = lib

INCLUDEPATH += ../app

SOURCES += kdeplugin1.cpp

HEADERS += kdeplugin1.h\
        kdeplugin1_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    texteditor.json \
    texteditor.desktop
