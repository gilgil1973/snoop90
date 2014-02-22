// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_KEY_MGR_ACCESSIBLE_H__
#define __SNOOP_KEY_MGR_ACCESSIBLE_H__

#include <SnoopTypeKey>

// ----------------------------------------------------------------------------
// SnoopFlowMgrMapValue
// ----------------------------------------------------------------------------
class SnoopFlowMgrMapValue
{
public:
  struct timeval lastTs;
  BYTE*  totalMem;
};

// ----------------------------------------------------------------------------
// SnoopFlowMgrMap_MacFlow
// ----------------------------------------------------------------------------
class SnoopFlowMgrMap_MacFlow : public QMap<SnoopMacFlowKey, SnoopFlowMgrMapValue>
{

public:
  SnoopFlowMgrMap_MacFlow();
  virtual ~SnoopFlowMgrMap_MacFlow();
  void clear();
  SnoopFlowMgrMap_MacFlow::iterator erase(SnoopMacFlowKey& key);
};

// ----------------------------------------------------------------------------
// SnoopFlowMgrMap_TcpFlow
// ----------------------------------------------------------------------------
class SnoopFlowMgrMap_TcpFlow : public QMap<SnoopTcpFlowKey, SnoopFlowMgrMapValue>
{
public:
  SnoopFlowMgrMap_TcpFlow();
  virtual ~SnoopFlowMgrMap_TcpFlow();
  void clear();
  SnoopFlowMgrMap_TcpFlow::iterator erase(SnoopTcpFlowKey& key);
};

// ----------------------------------------------------------------------------
// SnoopFlowMgrMap_UdpFlow
// ----------------------------------------------------------------------------
class SnoopFlowMgrMap_UdpFlow : public QMap<SnoopUdpFlowKey, SnoopFlowMgrMapValue>
{
public:
  SnoopFlowMgrMap_UdpFlow();
  virtual ~SnoopFlowMgrMap_UdpFlow();
  void clear();
  SnoopFlowMgrMap_UdpFlow::iterator erase(SnoopUdpFlowKey& key);
};

#endif // __SNOOP_KEY_MGR_ACCESSIBLE_H__
