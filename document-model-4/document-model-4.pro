#-------------------------------------------------
#
# Project created by QtCreator 2015-02-20T20:13:00
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG   += c++11

include(3rdParty/qtpropertybrowser/qtpropertybrowser.pri)
include(3rdParty/qtgradienteditor/qtgradienteditor.pri)
include(3rdParty/misc/shared.pri)

TARGET = document-model-4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    document.cpp \
    documentmodel.cpp \
    documentobject.cpp \
    documentstupidobject.cpp \
    documentmodelexplorerwidget.cpp \
    documentobjectpropertybrowserwidget.cpp \
    documentcommand.cpp

HEADERS  += mainwindow.h \
    document.h \
    documentmodel.h \
    documentobject.h \
    documentstupidobject.h \
    documentmodelexplorerwidget.h \
    documentobjectpropertybrowserwidget.h \
    documentcommand.h

OTHER_FILES += \
    README.md
