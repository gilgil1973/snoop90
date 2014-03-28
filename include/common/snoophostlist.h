// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_HOST_LIST_H__
#define __SNOOP_HOST_LIST_H__

#include <Snoop>

// ----------------------------------------------------------------------------
// SnoopHost
// ----------------------------------------------------------------------------
class SnoopHost : public VXmlable
{
public:
  Ip      ip;
  Mac     mac;
  QString name;

public:
  SnoopHost(Ip ip = 0, Mac mac = Mac::cleanMac(), QString name = "");
  virtual ~SnoopHost();

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// SnoopHostList
// ----------------------------------------------------------------------------
class SnoopHostList : public QList<SnoopHost>, public VXmlable
{
public:
  SnoopHost* findByIp(Ip ip);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

#endif // __SNOOP_HOST_LIST_H__
