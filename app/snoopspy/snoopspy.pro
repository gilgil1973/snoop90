#-------------------------------------------------
#
# Project created by QtCreator 2012-06-25T02:03:36
#
#-------------------------------------------------

QT += gui
QT += widgets

include(../../lib/snoop.pri)

TARGET        = snoopspy
TEMPLATE      = app
DESTDIR       = ../../bin
win32:RC_FILE = snoopspy.rc

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    node.cpp \
    arrow.cpp \
    scene.cpp \
    view.cpp \
    explicit_link.cpp \
    signalslotform.cpp \
    dommodel.cpp \
    domitem.cpp \
    aboutdlg.cpp

HEADERS += \
    mainwindow.h \
    node.h \
    arrow.h \
    scene.h \
    view.h \
    explicit_link.h \
    signalslotform.h \
    dommodel.h \
    domitem.h \
    aboutdlg.h

FORMS += \
    mainwindow.ui \
    signalslotform.ui \
    aboutdlg.ui

RESOURCES += \
    snoopspy.qrc
