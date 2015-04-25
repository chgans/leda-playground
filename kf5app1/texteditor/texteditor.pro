#-------------------------------------------------
#
# Project created by QtCreator 2015-04-24T16:17:58
#
#-------------------------------------------------

CONFIG   += c++11
QT       += widgets
QT       += KTextWidgets KCoreAddons

DEFINES += KDEPLUGIN1_LIBRARY
TARGET = texteditor
TEMPLATE = lib

INCLUDEPATH += ..
LIBS += -L$$OUT_PWD/../core -lcore

SOURCES += kdeplugin1.cpp \
    pluginfactory.cpp

HEADERS += kdeplugin1.h\
        kdeplugin1_global.h \
    pluginfactory.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    texteditor.json \
    texteditor.desktop
