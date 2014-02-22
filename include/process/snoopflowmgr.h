// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_FLOW_MGR_H__
#define __SNOOP_FLOW_MGR_H__

#include <SnoopProcess>
#include <SnoopFlowMgrAccessible>
#include <VTick>

// ----------------------------------------------------------------------------
// SnoopFlowMgrRequesterItem
// ----------------------------------------------------------------------------
class SnoopFlowMgrRequesterItem
{
public:
  SnoopFlowMgrRequesterItem();
  virtual ~SnoopFlowMgrRequesterItem();

public:
  void*  requester;
  int    user;
  size_t offset;
  size_t memSize;
};

// ----------------------------------------------------------------------------
// SnoopFlowMgrRequesterItems
// ----------------------------------------------------------------------------
class SnoopFlowMgrRequesterItems : public QList<SnoopFlowMgrRequesterItem>
{
public:
  SnoopFlowMgrRequesterItems();
  virtual ~SnoopFlowMgrRequesterItems();

public:
  size_t totalMemSize;
};

// ----------------------------------------------------------------------------
// SnoopFlowMgr
// ----------------------------------------------------------------------------
class SnoopFlowMgr : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopFlowMgr(void* owner = NULL);
  virtual ~SnoopFlowMgr();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  void clearMaps();
  SnoopFlowMgrMap_MacFlow macFlow_Map;
  SnoopFlowMgrMap_TcpFlow tcpFlow_Map;
  SnoopFlowMgrMap_UdpFlow udpFlow_Map;

public:
  void clearItems();
  SnoopFlowMgrRequesterItems macFlow_Items;
  SnoopFlowMgrRequesterItems tcpFlow_Items;
  SnoopFlowMgrRequesterItems udpFlow_Items;

protected:
  size_t requestMemory(void* requester, SnoopFlowMgrRequesterItems& items, int user, size_t memSize);

public:
  size_t requestMemory_MacFlow(void* requester, int user, size_t memSize);
  void   process_MacFlow(SnoopPacket* packet, SnoopMacFlowKey& key);
  BYTE*  add_MacFlow(SnoopMacFlowKey& key);
  void   del_MacFlow(SnoopMacFlowKey& key);

public slots:
  void process(SnoopPacket* packet);

signals:
  void onNew_MacFlow(SnoopMacFlowKey& key);
  void onDel_MacFlow(SnoopMacFlowKey& key);
  void macFlow_Processed(SnoopPacket* packet);

protected:
  VTick lastCheckTick;

public:
  VDuration checkInterval;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void addOptionWidget(QLayout* layout);
  virtual void saveOptionDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_FLOW_MGR_H__
