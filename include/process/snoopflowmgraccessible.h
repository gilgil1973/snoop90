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
// ISnoopFlowMgrAccessible
// ----------------------------------------------------------------------------
class ISnoopFlowMgrAccessible
{
private:
  virtual void foo() {}
};

// ----------------------------------------------------------------------------
// ISnoopFlowMgrAccessible_Mac
// ----------------------------------------------------------------------------
class ISnoopFlowMgrAccessible_MacFlow : public ISnoopFlowMgrAccessible
{
public:
  virtual void onNew_MacFlow(SnoopMacFlowKey* key, int user, void* mem) = 0;
  virtual void onDel_MacFlow(SnoopMacFlowKey* key, int user, void* mem) = 0;
};

// ----------------------------------------------------------------------------
// SnoopFlowMgrMap_MacFlow
// ----------------------------------------------------------------------------
class SnoopFlowMgrMap_MacFlow : public QMap<SnoopMacFlowKey /*key*/, void* /*totalMem*/>
{

public:
  SnoopFlowMgrMap_MacFlow()          { clear(); }
  virtual ~SnoopFlowMgrMap_MacFlow() { clear(); }
  void clear();
};

// ----------------------------------------------------------------------------
// SnoopFlowMgrAccessibleItem_TcpFlow
// ----------------------------------------------------------------------------
class SnoopFlowMgrAccessibleItem_TcpFlow : public ISnoopFlowMgrAccessible
{
public:
  virtual void onNew_TcpFlow(SnoopTcpFlowKey* key, int user, void* mem) = 0;
  virtual void onDel_TcpFlow(SnoopTcpFlowKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopTcpFlowKey /*key*/, void* /*totalMem*/> SnoopFlowMgrMap_TcpFlow;

// ----------------------------------------------------------------------------
// ISnoopFlowMgrAccessible_UdpFlow
// ----------------------------------------------------------------------------
class ISnoopFlowMgrAccessible_UdpFlow : public ISnoopFlowMgrAccessible
{
public:
  virtual void onNew_UdpFlow(SnoopUdpFlowKey* key, int user, void* mem) = 0;
  virtual void onDel_UdpFlow(SnoopUdpFlowKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopUdpFlowKey /*key*/, void* /*totalMem*/> SnoopFlowMgrMap_UdpFlow;

#endif // __SNOOP_KEY_MGR_ACCESSIBLE_H__
