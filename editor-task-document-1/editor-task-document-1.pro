#-------------------------------------------------
#
# Project created by QtCreator 2015-02-10T11:52:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = editor-task-document-1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    document.cpp \
    documentitems.cpp \
    documentmodels.cpp \
    schlibrarydockwidget.cpp

HEADERS  += mainwindow.h \
    document.h \
    leda.h \
    documentitems.h \
    documentmodels.h \
    schlibrarydockwidget.h

FORMS    += mainwindow.ui \
    schlibrarydockwidget.ui

DISTFILES += \
    doc1.xml \
    notes.txt
