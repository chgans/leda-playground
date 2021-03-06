#-------------------------------------------------
#
# Project created by QtCreator 2014-06-05T11:11:46
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = grid_snap_1
TEMPLATE = app


SOURCES += main.cpp\
    igrid.cpp \
    igridfactory.cpp \
    igrideditor.cpp \
    grideditordialog.cpp \
    gridmanager.cpp \
    gridmanagerdialog.cpp \
    \
    cartesiangrid.cpp \
    cartesiangrideditor.cpp \
    cartesiangridfactory.cpp \
    \
    polargrideditor.cpp \
    polargrid.cpp \
    polargridfactory.cpp \
    mainwindow.cpp \
    view.cpp

HEADERS  += \
    igrid.h \
    igridfactory.h \
    igrideditor.h \
    grideditordialog.h \
    gridmanager.h \
    gridmanagerdialog.h \
    gridmanagerdialog_p.h \
    \
    cartesiangrid.h \
    cartesiangrideditor.h \
    cartesiangridfactory.h \
    polargrideditor.h \
    \
    polargrid.h \
    polargridfactory.h \
    mainwindow.h \
    view.h

FORMS += \
    cartesiangrideditorwidget.ui \
    polargrideditorwidget.ui \
    form.ui
