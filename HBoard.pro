QT -= gui
QT += quick opengl

TEMPLATE = lib
DEFINES += HBOARD_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Common/hcommons.cpp \
    Common/hplanvector.cpp \
    Handles/hhandlearrow.cpp \
    Handles/hhandlebase.cpp \
    Handles/hhandleflyweight.cpp \
    Handles/hhandlemove.cpp \
    Handles/hhandlepaint.cpp \
    Nodes/hfillnode.cpp \
    Nodes/himagenode.cpp \
    Nodes/hnodebase.cpp \
    hboard.cpp \
    hboardmanager.cpp \
    hboarduicontrol.cpp

HEADERS += \
    Common/hcommons.h \
    Common/hplanvector.h \
    HBoard_global.h \
    Handles/hhandlearrow.h \
    Handles/hhandlebase.h \
    Handles/hhandleflyweight.h \
    Handles/hhandlemove.h \
    Handles/hhandlepaint.h \
    Nodes/hfillnode.h \
    Nodes/himagenode.h \
    Nodes/hnodebase.h \
    hboard.h \
    hboardmanager.h \
    hboarduicontrol.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
