

QT += core gui widgets
TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $$PWD
TARGET = core
CONFIG(debug, debug|release): TARGET = $$join(TARGET,,,d)

include($$PWD/../rsatoolbox/rsatoolbox.pri)
INCLUDEPATH += $$PWD
HEADERS     += settings.h \
               dmmdriver.h \
               dmm.h \
               stagesettings.h \
               stagesettingsmodel.h \
               stagelistwidget.h \
               stagedialog.h \
               dmmcontroller.h \
               stageresult.h \
               measurepae.h \
               traceswidget.h \
    processtrace.h \
    tracesettings.h
SOURCES     += dmmdriver.cpp \
               dmm.cpp \
               stagesettings.cpp \
               stagesettingsmodel.cpp \
               stagelistwidget.cpp \
               stagedialog.cpp \
               dmmcontroller.cpp \
               stageresult.cpp \
               measurepae.cpp \
               traceswidget.cpp \
    processtrace.cpp \
    tracesettings.cpp
FORMS +=       stagelistwidget.ui \
               stagedialog.ui \
               traceswidget.ui
RESOURCES +=   core.qrc

DEFINES     += SOURCE_DIR=\\\"$$PWD/\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT






