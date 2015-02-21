
INCLUDEPATH += $$PWD

FORMS += $$PWD/addlinkdialog.ui

HEADERS += \
    $$PWD/shared_global_p.h \
    $$PWD/shared_enums_p.h \
    $$PWD/richtexteditor_p.h \
    $$PWD/plaintexteditor_p.h \
    $$PWD/textpropertyeditor_p.h \
    $$PWD/propertylineedit_p.h \
    $$PWD/htmlhighlighter_p.h \

SOURCES += \
    $$PWD/richtexteditor.cpp \
    $$PWD/plaintexteditor.cpp \
    $$PWD/textpropertyeditor.cpp \
    $$PWD/propertylineedit.cpp \
    $$PWD/htmlhighlighter.cpp \

RESOURCES += \
    $$PWD/shared.qrc

