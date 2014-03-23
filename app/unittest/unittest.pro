#-------------------------------------------------
#
# Project created by QtCreator 2014-03-23T13:04:08
#
#-------------------------------------------------

QT       += core
QT       -= gui

include (../../../../other/googletest/unittest.pri)
include (../../lib/snoop.pri)

TARGET    = unittest
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE  = app
DESTDIR   = ../../bin

SOURCES += main.cpp \
    ../../include/common/snoopautodetectadapter.cpp
