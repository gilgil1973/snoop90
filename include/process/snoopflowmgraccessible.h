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
// ISnoopKeyMgrAccessible
// ----------------------------------------------------------------------------
class ISnoopKeyMgrAccessible
{
private:
  virtual void foo() {}
};

// ----------------------------------------------------------------------------
// ISnoopKeyMgrAccessible_Mac
// ----------------------------------------------------------------------------
class ISnoopKeyMgrAccessible_MacFlow : public ISnoopKeyMgrAccessible
{
public:
  virtual void onNew_MacFlow(SnoopMacFlowKey* key, int user, void* mem) = 0;
  virtual void onDel_MacFlow(SnoopMacFlowKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopMacFlowKey /*key*/, void* /*totalMem*/> SnoopKeyMgrMap_MacFlow;

// ----------------------------------------------------------------------------
// SnoopKeyMgrAccessibleItem_TcpFlow
// ----------------------------------------------------------------------------
class SnoopKeyMgrAccessibleItem_TcpFlow : public ISnoopKeyMgrAccessible
{
public:
  virtual void onNew_TcpFlow(SnoopTcpFlowKey* key, int user, void* mem) = 0;
  virtual void onDel_TcpFlow(SnoopTcpFlowKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopTcpFlowKey /*key*/, void* /*totalMem*/> SnoopKeyMgrMap_TcpFlow;

// ----------------------------------------------------------------------------
// ISnoopKeyMgrAccessible_UdpFlow
// ----------------------------------------------------------------------------
class ISnoopKeyMgrAccessible_UdpFlow : public ISnoopKeyMgrAccessible
{
public:
  virtual void onNew_UdpFlow(SnoopUdpFlowKey* key, int user, void* mem) = 0;
  virtual void onDel_UdpFlow(SnoopUdpFlowKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopUdpFlowKey /*key*/, void* /*totalMem*/> SnoopKeyMgrMap_UdpFlow;

#endif // __SNOOP_KEY_MGR_ACCESSIBLE_H__
