#-------------------------------------------------
#
# Project created by QtCreator 2015-02-11T16:42:52
#
#-------------------------------------------------

QT       += core gui testlib

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hierachical-vhdl-1
TEMPLATE = app

SUBDIRS = tests

SOURCES += main.cpp\
        mainwindow.cpp \
    entity.cpp

HEADERS  += mainwindow.h \
    entity.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    notes.txt
