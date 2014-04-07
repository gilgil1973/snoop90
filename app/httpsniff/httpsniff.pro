#-------------------------------------------------
#
# Project created by QtCreator 2014-04-06T04:06:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../lib/snoop.pri)

TARGET   = httpsniff
TEMPLATE = app
DESTDIR  = ../../bin

SOURCES += main.cpp\
    dialog.cpp \
    explicit_link.cpp \
    httpsniffconfig.cpp

HEADERS  += dialog.h \
    explicit_link.h \
    httpsniffconfig.h

FORMS    += dialog.ui
