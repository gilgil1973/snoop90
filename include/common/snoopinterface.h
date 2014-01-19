// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_ADAPTER_INFO_H__
#define __SNOOP_ADAPTER_INFO_H__

#include <Snoop>

// ----------------------------------------------------------------------------
// SnoopInterface
// ----------------------------------------------------------------------------
class SnoopInterface : public VXmlable /*, private VNonCopyable */ // gilgil temp 2012.06.04
{
  friend class SnoopInterfaces;
  friend class QList<SnoopInterface>;
  // friend class QList<SnoopInterface>::iterator; // gilgil temp 2012.06.05
  friend class SnoopRemote;

public: // protected: // gilgil temp 2012.06.04
  SnoopInterface();
  virtual ~SnoopInterface();

public:
  int          index;
  QString      name;
  QString      description;
  pcap_if_t*   dev;

#ifdef WIN32
public:
  PIP_ADAPTER_INFO adapterInfo;
#endif // WIN32

public:
  bool operator == (SnoopInterface& rhs);
  bool operator != (SnoopInterface& rhs);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// SnoopInterfaces
// ----------------------------------------------------------------------------
class SnoopInterfaces : public QList<SnoopInterface>, public VXmlable , private VNonCopyable // gilgil temp 2012.06.04
{
  friend class SnoopRemote;
  friend class _SnoopInterfaces;

public:
  SnoopInterfaces();
  virtual ~SnoopInterfaces();

public:
  pcap_if_t*       allDevs;

#ifdef WIN32
public:
  PIP_ADAPTER_INFO allAdaptersInfo;
#endif // WIN32

protected:
  void initialize();
  void finalize();

public:
  bool operator == (SnoopInterfaces& rhs);
  bool operator != (SnoopInterfaces& rhs);

public:
  void load(VXml xml);
  void save(VXml xml);

public:
  static int getBestAdapterIndex(QString host = "");

  //
  // Static
  //
public:
  static SnoopInterfaces& instance();
};

#endif // __SNOOP_ADAPTER_INFO_H__
