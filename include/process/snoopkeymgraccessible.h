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
class ISnoopKeyMgrAccessible_Mac : public ISnoopKeyMgrAccessible
{
public:
  virtual void onNew_Mac(SnoopMacKey* key, int user, void* mem) = 0;
  virtual void onDel_Mac(SnoopMacKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopMacKey /*key*/, void* /*totalMem*/> SnoopKeyMgr_Mac_Map;

// ----------------------------------------------------------------------------
// SnoopKeyMgrAccessibleItem_TcpFlow
// ----------------------------------------------------------------------------
class SnoopKeyMgrAccessibleItem_TcpFlow : public ISnoopKeyMgrAccessible
{
public:
  virtual void onNew_TcpFlow(SnoopTcpFlowKey* key, int user, void* mem) = 0;
  virtual void onDel_TcpFlow(SnoopTcpFlowKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopTcpFlowKey /*key*/, void* /*totalMem*/> SnoopKeyMgr_TcpFlow_Map;

// ----------------------------------------------------------------------------
// ISnoopKeyMgrAccessible_UdpFlow
// ----------------------------------------------------------------------------
class ISnoopKeyMgrAccessible_UdpFlow : public ISnoopKeyMgrAccessible
{
public:
  virtual void onNew_UdpFlow(SnoopUdpFlowKey* key, int user, void* mem) = 0;
  virtual void onDel_UdpFlow(SnoopUdpFlowKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopUdpFlowKey /*key*/, void* /*totalMem*/> SnoopKeyMgr_UdpFlow_Map;

#endif // __SNOOP_KEY_MGR_ACCESSIBLE_H__
