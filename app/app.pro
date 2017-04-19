#-------------------------------------------------
# 
# Project  : PAE
# App name : R&S PAE
# 
# RsaToolbox Application
# 
# (C) Rohde & Schwarz (North) America
# 
#-------------------------------------------------


QT      += core gui widgets

TARGET = PAE
TEMPLATE = app

include(../core/core.pri)
HEADERS     += mainwindow.h
INCLUDEPATH += $$PWD
SOURCES     += main.cpp \
               mainwindow.cpp
FORMS       += mainwindow.ui
RESOURCES   += resources.qrc
OTHER_FILES += vs2010resources.rc \
               DEBUG_LOGFILE.txt
win32:      RC_FILE = vs2010resources.rc

DEFINES += SOURCE_DIR=\\\"$$PWD\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE
