

QT += core gui widgets
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD
TARGET = core
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)

include($$PWD/../rsatoolbox/rsatoolbox.pri)

HEADERS     += settings.h \
    dmmdriver.h \
    dmm.h \
    stagesettings.h \
    stagesettingsmodel.h
INCLUDEPATH += $$PWD
SOURCES     += \
    dmmdriver.cpp \
    dmm.cpp \
    stagesettings.cpp \
    stagesettingsmodel.cpp
#FORMS       += 
#RESOURCES   += 

DEFINES     += SOURCE_DIR=\\\"$$PWD/\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT


