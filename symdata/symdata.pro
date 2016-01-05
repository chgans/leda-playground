#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T10:17:51
#
#-------------------------------------------------

QT       += core
CONFIG += c++11

TARGET = symdata
TEMPLATE = app

SOURCES += main.cpp\
    document.cpp \
    item.cpp \
    editor.cpp \
    command.cpp

HEADERS  +=  \
    document.h \
    item.h \
    editor.h \
    command.h

OTHER_FILES += \
    xsd.make \
    leda-xdl-symbol.xsd \
    leda-xdl-symbol.map

DISTFILES += \
    symdata.qmodel \
    file-format.txt
