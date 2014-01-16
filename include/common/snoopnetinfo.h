// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 8.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_NET_INFO_H__
#define __SNOOP_NET_INFO_H__

#include <SnoopInterface>

// ----------------------------------------------------------------------------
// SnoopNetInfoAdapterIndex
// ----------------------------------------------------------------------------
class SnoopNetInfo;
class SnoopNetInfoAdapterIndex
{
protected:
  SnoopNetInfo* netInfo;
  int           m_adapterIndex;

public:
  SnoopNetInfoAdapterIndex(SnoopNetInfo* netInfo);
  operator int();
  void operator = (const int i);

protected:
  void setAdapterIndex(int value);
};

// ----------------------------------------------------------------------------
// SnoopNetInfo
// ----------------------------------------------------------------------------
class SnoopNetInfo : public VXmlable
{
  friend class SnoopNetInfoAdapterIndex;
  friend class SnoopInterface;

public:
  SnoopNetInfo();
  virtual ~SnoopNetInfo();

public:
  void clear();

public:
  SnoopNetInfoAdapterIndex adapterIndex;

public:
  Mac mac;
  Ip  ip;
  Ip  subnet;
  Ip  gateway;

protected:
  Ip   ip_and_subnet; // for isSameLanIP

public:
  bool isSameLanIP(Ip ip) { return (ip_and_subnet) == (ip & subnet); }
  Ip   getAdjIP(Ip ip)    { return isSameLanIP(ip) ? ip : gateway;   }
  Ip   getStartIP()       { return (ip & subnet) + 1;                }
  Ip   getEndIP()         { return (ip | ~subnet);                   }

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

#endif // __SNOOP_NET_INFO_H__
