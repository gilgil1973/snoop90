#-------------------------------------------------
#
# Project created by QtCreator 2013-03-07T12:01:29
#
#-------------------------------------------------

QT -= gui

include(../../lib/snoop.pri)

TARGET   = sscon
CONFIG  += console
CONFIG  -= app_bundle
TEMPLATE = app
DESTDIR  = ../../bin

SOURCES += \
    explicit_link.cpp \
    main.cpp \
    param.cpp

HEADERS += \
    explicit_link.h \
    main.h \
    param.h
