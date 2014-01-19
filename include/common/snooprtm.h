// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_RTM_H__
#define __SNOOP_RTM_H__

#include <Snoop>

// ----------------------------------------------------------------------------
// SnoopRtmEntry
// ----------------------------------------------------------------------------
class SnoopRtmEntry : public VXmlable
{
public:
  SnoopRtmEntry();
  ~SnoopRtmEntry();
  void clear();

public:
  Ip      dst;
  Ip      mask;
  Ip      gateway;
  QString intf;
  int     metric;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// SnoopRtmEntryList
// ----------------------------------------------------------------------------
class SnoopRtmEntryList : public QList<SnoopRtmEntry>, public VXmlable
{
public:
  int find(Ip dst, Ip mask);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// SnoopRtm
// ----------------------------------------------------------------------------
class SnoopRtm : public VXmlable
{
  friend class SnoopRtmInstance;

public:
  static const QString DEFAULT_RTM_FILE_NAME;

private: // sigleton
  SnoopRtm();
  virtual ~SnoopRtm();

public:
  VError error;
  SnoopRtmEntryList items;

public:
  bool loadFromSystem();
  bool recoverSystem();
  SnoopRtmEntry* getBestEntry(Ip ip);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

public:
  static SnoopRtm& instance();
};

#endif // __SNOOP_RTM_H__
