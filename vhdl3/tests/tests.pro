#-------------------------------------------------
#
# Project created by QtCreator 2015-05-19T12:03:49
#
#-------------------------------------------------

QT       += xml testlib

QT       -= gui

TARGET = tst_xmltest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= c++11

TEMPLATE = app


SOURCES += tst_xmltest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lib/release/ -llib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lib/debug/ -llib
else:unix: LIBS += -L$$OUT_PWD/../lib/ -llib

INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib
