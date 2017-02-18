

QT += core gui widgets
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD
TARGET = core
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)

include($$PWD/../rsatoolbox/rsatoolbox.pri)

HEADERS     += settings.h \
               keysightdmm.h \
    drainefficiency.h \
    dmmdriver.h \
    dmm.h
INCLUDEPATH += $$PWD
SOURCES     += keysightdmm.cpp \
    drainefficiency.cpp \
    dmmdriver.cpp \
    dmm.cpp
#FORMS       += 
#RESOURCES   += 

DEFINES     += SOURCE_DIR=\\\"$$PWD/\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT


