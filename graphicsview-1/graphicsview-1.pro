#-------------------------------------------------
#
# Project created by QtCreator 2014-05-15T11:03:01
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

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
    pcbpalette.cpp \
    pcbpalettesettingsdialog.cpp \
    pcbpalettemanager.cpp \
    pcbphysicallayer.cpp \
    pcbphysicalboard.cpp \
    designlayer.cpp \
    primitive.cpp \
    designlayermanager.cpp \
    widgets/colorpreview.cpp \
    widgets/colorwheel.cpp \
    widgets/gradientslider.cpp \
    widgets/hueslider.cpp \
    widgets/colorwidget.cpp \
    colorprofileeditor.cpp \
    colorprofilecombobox.cpp \
    colorprofiletoolbutton.cpp \
    designlayerset.cpp \
    layerbar.cpp \
    insightdockwidget.cpp

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
    pcbpalette.h \
    pcbpalettesettingsdialog.h \
    pcbpalettemanager.h \
    pcbphysicallayer.h \
    pcbphysicalboard.h \
    designlayer.h \
    primitive.h \
    designlayermanager.h \
    widgets/colorpreview.h \
    widgets/colorwheel.h \
    widgets/gradientslider.h \
    widgets/hueslider.h \
    widgets/paintborder.h \
    widgets/colorwidget.h \
    colorprofileeditor.h \
    colorprofilecombobox.h \
    colorprofiletoolbutton.h \
    designlayerset.h \
    layerbar.h \
    insightdockwidget.h

FORMS    += \
    pcbpalettesettingsdialog.ui \
    colorprofileeditor.ui

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
