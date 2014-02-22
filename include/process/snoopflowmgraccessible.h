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
// SnoopFlowMgrMap_MacFlow
// ----------------------------------------------------------------------------
class SnoopFlowMgrMap_MacFlow : public QMap<SnoopMacFlowKey /*key*/, BYTE* /*totalMem*/>
{

public:
  SnoopFlowMgrMap_MacFlow();
  virtual ~SnoopFlowMgrMap_MacFlow();
  void clear();
  int  remove(SnoopMacFlowKey& key);
};

// ----------------------------------------------------------------------------
// SnoopFlowMgrMap_TcpFlow
// ----------------------------------------------------------------------------
class SnoopFlowMgrMap_TcpFlow : public QMap<SnoopTcpFlowKey /*key*/, BYTE* /*totalMem*/>
{
public:
  SnoopFlowMgrMap_TcpFlow();
  virtual ~SnoopFlowMgrMap_TcpFlow();
  void clear();
  int  remove(SnoopTcpFlowKey& key);
};

// ----------------------------------------------------------------------------
// SnoopFlowMgrMap_UdpFlow
// ----------------------------------------------------------------------------
class SnoopFlowMgrMap_UdpFlow : public QMap<SnoopUdpFlowKey /*key*/, BYTE* /*totalMem*/>
{
public:
  SnoopFlowMgrMap_UdpFlow();
  virtual ~SnoopFlowMgrMap_UdpFlow();
  void clear();
  int  remove(SnoopUdpFlowKey& key);
};

#endif // __SNOOP_KEY_MGR_ACCESSIBLE_H__
