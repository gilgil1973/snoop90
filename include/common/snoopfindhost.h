// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_FIND_HOST_H__
#define __SNOOP_FIND_HOST_H__

#include <SnoopAdapter>
#include <SnoopNetInfo>
#include <SnoopHostList>
#include <VTick>

// ----------------------------------------------------------------------------
// SnoopFindHost
// ----------------------------------------------------------------------------
class SnoopFindHost : public SnoopAdapter
{
  Q_OBJECT

public:
  SnoopFindHost(void* owner = NULL);
  virtual ~SnoopFindHost();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  VTimeout      findAllTimeout;
  VTimeout      scanInterval;
  VTimeout      sendInterval;
  SnoopHostList hostList;

public:
  bool findAll();

protected:
  VTick        lastSendTick;
  SnoopNetInfo netInfo;

  bool sendArpRequestAll();
  bool isFoundAll();
  void logUnfoundHost(VLog* log = NULL);
  int  sendArpRequest(Ip ip);
  int  read(Ip& ip, Mac& mac);

protected:
  virtual void run();

signals:
  void found(Ip ip, Mac mac);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

#endif // __SNOOP_FIND_HOST_H__
