

QT        += core testlib
TEMPLATE   = app
TARGET     = CoreTest
CONFIG    -= app_bundle
CONFIG    += console \
             testcase

include(../core/core.pri)
SOURCES     += main.cpp \
               dmmdrivertest.cpp \
               dmmtest.cpp \
               dmmcontrollertest.cpp \
    measurepaetest.cpp
HEADERS     += dmmdrivertest.h \
               dmmtest.h \
               dmmcontrollertest.h \
    measurepaetest.h

DEFINES     += SOURCE_DIR=\\\"$$PWD/\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
