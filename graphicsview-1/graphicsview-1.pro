#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T11:03:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphicsview-1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    detailview.cpp \
    objectpreview.cpp \
    overview.cpp \
    mainview.cpp \
    scene.cpp \
    insightlenswidget.cpp \
    insightheadsupwidget.cpp \
    pcbeditorwidget.cpp \
    insightconnectivitywidget.cpp \
    insightpicklistwidget.cpp \
    pcbeditorlayer.cpp \
    pcbpalette.cpp \
    pcbpalettesettingsdialog.cpp \
    pcbpalettemanager.cpp \
    pcbpalettemanagerdialog.cpp \
    pcbphysicallayer.cpp \
    pcbphysicalboard.cpp

HEADERS  += mainwindow.h \
    detailview.h \
    objectpreview.h \
    overview.h \
    mainview.h\
    scene.h \
    insightlenswidget.h \
    insightheadsupwidget.h \
    pcbeditorwidget.h \
    insightconnectivitywidget.h \
    insightpicklistwidget.h \
    pcbeditorlayer.h \
    pcbpalette.h \
    pcbpalettesettingsdialog.h \
    pcbpalettemanager.h \
    pcbpalettemanagerdialog.h \
    pcbphysicallayer.h \
    pcbphysicalboard.h

FORMS    += mainwindow.ui \
    pcbpalettesettingsdialog.ui

OTHER_FILES += \
    test.js \
    test2.js \
    test.json \
    test2.json.js \
    test2.json \
    TODO.txt \
    layers.txt \
    Classic.PCBSysColors \
    PCB1.stackup \
    PCB1.stackup.txt \
    pcb-palette.conf \
    AltiumDefault.LedaPcbPalette \
    vhdl-model.txt

RESOURCES += \
    graphicsview-1.qrc
