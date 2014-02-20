#-------------------------------------------------
#
# Project created by QtCreator 2012-06-04T09:44:29
#
#-------------------------------------------------
CONFIG += SNOOP_LIB_BUILD
#QT    -= gui
#QT    += gui widgets

include (snoop.pri)

TARGET   = $${SNOOP_LIB_NAME}
TEMPLATE = lib
CONFIG  += staticlib
DESTDIR  = ../lib

SOURCES += \
    ../include/capture/snoopvirtualnat.cpp \
    ../include/capture/snoopadapter.cpp \
    ../include/capture/snoopcapture.cpp \
    ../include/capture/snoopcapturefactory.cpp \
    ../include/capture/snoopfile.cpp \
    ../include/capture/snooppcap.cpp \
    ../include/capture/snoopremote.cpp \
    ../include/capture/snoopsourcepcap.cpp \
    ../include/capture/snoopwindivert.cpp \
    ../include/common/snoopautodetectadapter.cpp \
    ../include/common/snoopcommon.cpp \
    ../include/common/snoopinterface.cpp \
    ../include/common/snoopnetinfo.cpp \
    ../include/common/snoopnetstat.cpp \
    ../include/common/snooppacket.cpp \
    ../include/common/snooprtm.cpp \
    ../include/common/snooptype.cpp \
    ../include/common/snooptypekey.cpp \
    ../include/filter/snoopbpfilter.cpp \
    ../include/filter/snoopfilter.cpp \
    ../include/filter/snoopfilterfactory.cpp \
    ../include/filter/snoopprocessfilter.cpp \
    ../include/parse/snooparp.cpp \
    ../include/parse/snoopeth.cpp \
    ../include/parse/snoopicmp.cpp \
    ../include/parse/snoopip.cpp \
    ../include/parse/snooptcp.cpp \
    ../include/parse/snooptcpdata.cpp \
    ../include/parse/snoopudp.cpp \
    ../include/parse/snoopudpdata.cpp \
    ../include/process/snoopdump.cpp \
    ../include/process/snoopkeymgr.cpp \
    ../include/process/snoopkeymgrtest.cpp \
    ../include/process/snoopprocess.cpp \
    ../include/process/snoopprocessfactory.cpp \
    ../include/process/snoopblock.cpp \
    ../include/process/snoopdatachange.cpp \
    ../include/process/snoopdelay.cpp \
    ../include/process/snooptcpblock.cpp \
    ../include/process/snoopwriteadapter.cpp

HEADERS += \
    ../include/capture/snoopadapter.h \
    ../include/capture/snoopcapture.h \
    ../include/capture/snoopcapturefactory.h \
    ../include/capture/snoopfile.h \
    ../include/capture/snooppcap.h \
    ../include/capture/snoopremote.h \
    ../include/capture/snoopsourcepcap.h \
    ../include/capture/snoopvirtualnat.h \
    ../include/capture/snoopwindivert.h \
    ../include/common/snoop.h \
    ../include/common/snoopautodetectadapter.h \
    ../include/common/snoopcommon.h \
    ../include/common/snoopinterface.h \
    ../include/common/snoopnetinfo.h \
    ../include/common/snoopnetstat.h \
    ../include/common/snooppacket.h \
    ../include/common/snooprtm.h \
    ../include/common/snooptype.h \
    ../include/common/snooptypekey.h \
    ../include/filter/snoopbpfilter.h \
    ../include/filter/snoopfilter.h \
    ../include/filter/snoopfilterfactory.h \
    ../include/filter/snoopprocessfilter.h \
    ../include/libnet/config.h \
    ../include/libnet/libnet-asn1.h \
    ../include/libnet/libnet-functions.h \
    ../include/libnet/libnet-headers.h \
    ../include/libnet/libnet-macros.h \
    ../include/libnet/libnet-structures.h \
    ../include/libnet/libnet-types.h \
    ../include/parse/snooparp.h \
    ../include/parse/snoopeth.h \
    ../include/parse/snoopicmp.h \
    ../include/parse/snoopip.h \
    ../include/parse/snooptcp.h \
    ../include/parse/snooptcpdata.h \
    ../include/parse/snoopudp.h \
    ../include/parse/snoopudpdata.h \
    ../include/process/snoopdump.h \
    ../include/process/snoopkeymgr.h \
    ../include/process/snoopkeymgrtest.h \
    ../include/process/snoopprocess.h \
    ../include/process/snoopprocessfactory.h \
    ../include/process/snoopblock.h \
    ../include/process/snoopdatachange.h \
    ../include/process/snoopdelay.h \
    ../include/process/snooptcpblock.h \
    ../include/process/snoopwriteadapter.h \
    ../include/windivert/windivert.h;
