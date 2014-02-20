// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_KEY_MGR_H__
#define __SNOOP_KEY_MGR_H__

#include <SnoopProcess>
#include <SnoopTypeKey>

#include <SnoopEth>
#include <SnoopIp>
#include <SnoopTcp>
#include <SnoopUdp>


// ----------------------------------------------------------------------------
// ISnoopKeyMgr_Accessible
// ----------------------------------------------------------------------------
class ISnoopKeyMgr_Accessible
{
private:
  virtual void foo() {}
};

// ----------------------------------------------------------------------------
// SnoopKeyMgrAccessibleAux
// ----------------------------------------------------------------------------
class SnoopKeyMgrAccessibleItem
{
public:
  ISnoopKeyMgr_Accessible*  accessible;
  int    user;
  size_t offset;
  size_t memSize;
};

// ----------------------------------------------------------------------------
// ISnoopKeyMgr_Mac_Accessible
// ----------------------------------------------------------------------------
class ISnoopKeyMgr_Mac_Accessible : public ISnoopKeyMgr_Accessible
{
public:
  virtual void onNew_Mac(SnoopMacKey* key, int user, void* mem) = 0;
  virtual void onDel_Mac(SnoopMacKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopMacKey /*key*/, void* /*totalMem*/> SnoopKeyMgr_Mac_Map;

// ----------------------------------------------------------------------------
// ISnoopKeyMgr_TcpFlow_Accessible
// ----------------------------------------------------------------------------
class ISnoopKeyMgr_TcpFlow_Accessible : public ISnoopKeyMgr_Accessible
{
public:
  virtual void onNew_TcpFlow(SnoopTcpFlowKey* key, int user, void* mem) = 0;
  virtual void onDel_TcpFlow(SnoopTcpFlowKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopTcpFlowKey /*key*/, void* /*totalMem*/> SnoopKeyMgr_TcpFlow_Map;

// ----------------------------------------------------------------------------
// ISnoopKeyMgr_UdpFlow_Accessible
// ----------------------------------------------------------------------------
class ISnoopKeyMgr_UdpFlow_Accessible : public ISnoopKeyMgr_Accessible
{
public:
  virtual void onNew_UdpFlow(SnoopUdpFlowKey* key, int user, void* mem) = 0;
  virtual void onDel_UdpFlow(SnoopUdpFlowKey* key, int user, void* mem) = 0;
};
typedef QMap<SnoopUdpFlowKey /*key*/, void* /*totalMem*/> SnoopKeyMgr_UdpFlow_Map;

// ----------------------------------------------------------------------------
// SnoopKeyMgr
// ----------------------------------------------------------------------------
class SnoopKeyMgr : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopKeyMgr(void* owner = NULL);
  virtual ~SnoopKeyMgr();

protected:
  virtual bool doOpen();
  virtual bool doClose();

protected:
  void clearMaps();
  SnoopKeyMgr_Mac_Map     mac_map;
  SnoopKeyMgr_UdpFlow_Map tcpFlow_map;
  SnoopKeyMgr_UdpFlow_Map udpFlow_map;

public:
  QList<SnoopKeyMgrAccessibleItem*> mac_items;
  QList<SnoopKeyMgrAccessibleItem*> tcpFlow_items;
  QList<SnoopKeyMgrAccessibleItem*> udpFlow_items;

protected:
  size_t total_mac_userSize;
  size_t total_tcpFlow_userSize;
  size_t total_udpFlow_userSize;

public:
  void registerAccessible(ISnoopKeyMgr_Mac_Accessible* accessible, QList<SnoopKeyMgrAccessibleItem*>& items, int user, size_t memSize);

protected:
  void processMac(SnoopPacket* packet, Mac* mac);

public slots:
  void process(SnoopPacket* packet);

signals:
  void processed(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void addOptionWidget(QLayout* layout);
  virtual void saveOptionDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_KEY_MGR_H__

