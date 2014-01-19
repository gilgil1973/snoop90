// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_VIRTUAL_NAT_H__
#define __SNOOP_VIRTUAL_NAT_H__

#include <SnoopAdapter>
#include <SnoopNetInfo>

// ----------------------------------------------------------------------------
// SnoopVirtualNat
// ----------------------------------------------------------------------------
class SnoopVirtualNat : public SnoopCapture
{
  Q_OBJECT

public:
  SnoopVirtualNat(void* owner = NULL);
  virtual ~SnoopVirtualNat();

protected:
  virtual bool doOpen();
  virtual bool doClose();

protected:
  SnoopAdapter virAdapter;
  SnoopAdapter realAdapter;

  SnoopNetInfo virNetInfo;
  SnoopNetInfo realNetInfo;

  bool changeRouteTable();
  bool recoverRouteTable();

public:
  int virAdapterIndex;
  int realAdapterIndex;

protected slots:
  void myVirtualRecv(SnoopPacket* packet);
  void myRealRecv(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

#endif // __SNOOP_VIRTUAL_NAT_H__
