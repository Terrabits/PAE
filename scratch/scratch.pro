#-------------------------------------------------
#
# Project created by QtCreator 2017-05-06T20:18:27
#
#-------------------------------------------------

QT        += core

QT        -= gui

include(../core/core.pri)
TARGET     = scratch
CONFIG    += console
CONFIG    -= app_bundle

TEMPLATE   = app

SOURCES   += main.cpp
DISTFILES += log.txt

DEFINES   += SOURCE_DIR=\\\"$$PWD/\\\"
CONFIG(debug, debug|release):DEFINES += DEBUG_MODE


