// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_COMMON_H__
#define __SNOOP_COMMON_H__

#include <VCommon>
#include <VNet>
#include <VDesign>
#include <VError>
#include <VLog>
#include <VXml>

#ifdef WIN32
  #define  WPCAP
  #define  HAVE_REMOTE
  #include <Iphlpapi.h>
  #include <pcap.h>
#endif // WIN32
#ifdef linux
  #include <pcap.h>
#endif // linux

// ----------------------------------------------------------------------------
// Link Library
// ----------------------------------------------------------------------------
// ----- gilgil temp 2014.03.28 -----
/*
#ifdef _MSC_VER
  #ifdef _DEBUG
    #ifdef QT_GUI_LIB
      #pragma comment(lib, "snoopd_gui.lib")
    #else
      #pragma comment(lib, "snoopd.lib")
    #endif // QT_GUI_LIB
  #else  _DEBUG
    #ifdef QT_GUI_LIB
      #pragma comment(lib, "snoop_gui.lib")
    #else
      #pragma comment(lib, "snoop.lib")
    #endif // QT_GUI_LIB
  #endif // _DEBUG
  #pragma comment(lib, "wpcap.lib")
  #pragma comment(lib, "Iphlpapi.lib")
#endif // _MSC_VER
*/
// ----------------------------------

// ----------------------------------------------------------------------------
// Snoop Version
// ----------------------------------------------------------------------------
extern const char* SNOOP_VERSION;

// ----------------------------------------------------------------------------
// Constant
// ----------------------------------------------------------------------------
namespace snoop
{
  static const int DEFAULT_READTIMEOUT    =  1;
  static const int DEFAULT_SNAPLEN        =  1600;
  static const int DEFAULT_TIMEOUT        =  5000;
  static const int INVALID_ADAPTER_INDEX  =  -1;
  static const int DEFAULT_ADAPTER_INDEX  =  0;
};

// ----------------------------------------------------------------------------
// SnoopCaptureType
// ----------------------------------------------------------------------------
class SnoopCaptureType
{
public:
  enum _SnoopCaptureType
  {
    None,
    InPath,
    OutOfPath
  };

protected:
  _SnoopCaptureType value;

public:
  SnoopCaptureType()                              {                      } // default ctor
  SnoopCaptureType(const _SnoopCaptureType value) { this->value = value; } // conversion ctor
  operator _SnoopCaptureType() const              { return value;        } // cast operator

public:
  SnoopCaptureType(const QString s);
  QString str() const;
};

// ----------------------------------------------------------------------------
// SnoopError
// ----------------------------------------------------------------------------
VDECLARE_ERROR_CLASS(SnoopError)

// ----------------------------------------------------------------------------
// SnoopError Code
// ----------------------------------------------------------------------------
static const int VERR_INVALID_ADAPTER_INDEX         = VERR_CATEGORY_SNOOP + 0;
static const int VERR_IN_PCAP_OPEN                  = VERR_CATEGORY_SNOOP + 1;
static const int VERR_IN_PCAP_COMPILE               = VERR_CATEGORY_SNOOP + 2;
static const int VERR_IN_PCAP_SETFILTER             = VERR_CATEGORY_SNOOP + 3;
static const int VERR_IN_PCAP_NEXT_EX               = VERR_CATEGORY_SNOOP + 4;
static const int VERR_IN_PCAP_FINDALLDEVS_EX        = VERR_CATEGORY_SNOOP + 5;
static const int VERR_CANCELED_BY_USER              = VERR_CATEGORY_SNOOP + 6;
static const int VERR_IN_PCAP_OPEN_DEAD             = VERR_CATEGORY_SNOOP + 7;
static const int VERR_HOST_NOT_SPECIFIED            = VERR_CATEGORY_SNOOP + 8;
static const int VERR_CAN_NOT_FIND_ALL_HOST         = VERR_CATEGORY_SNOOP + 9;
static const int VERR_SESSION_COUNT_IS_ZERO         = VERR_CATEGORY_SNOOP + 10;
static const int VERR_THE_SAME_SOURCE_AND_TARGET_IP = VERR_CATEGORY_SNOOP + 11;
static const int VERR_CAN_NOT_FIND_HOST             = VERR_CATEGORY_SNOOP + 12;
static const int VERR_THE_SAME_REAL_AND_TARGET_MAC  = VERR_CATEGORY_SNOOP + 13;
static const int VERR_CAN_NOT_OPEN_INFECT_THREAD    = VERR_CATEGORY_SNOOP + 14;
static const int VERR_IN_PCAP_DUMP_OPEN             = VERR_CATEGORY_SNOOP + 15;
static const int VERR_CAN_NOT_SPOOF_MYSELF          = VERR_CATEGORY_SNOOP + 16;

#endif // __SNOOP_COMMON_H__

