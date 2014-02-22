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
  SnoopFlowMgrMap_MacFlow macFlow_Map;
  SnoopFlowMgrMap_TcpFlow tcpFlow_Map;
  SnoopFlowMgrMap_UdpFlow udpFlow_Map;
  void clearMaps();
  void deleteOldMaps(struct timeval ts);

public:
  SnoopFlowMgrRequesterItems macFlow_Items;
  SnoopFlowMgrRequesterItems tcpFlow_Items;
  SnoopFlowMgrRequesterItems udpFlow_Items;
  void clearItems();

protected:
  size_t requestMemory(void* requester, SnoopFlowMgrRequesterItems& items, int user, size_t memSize);

public:
  //
  // MacFlow
  //
  size_t requestMemory_MacFlow(void* requester, int user, size_t memSize);
  void   process_MacFlow(SnoopPacket* packet, SnoopMacFlowKey& key);
  SnoopFlowMgrMap_MacFlow::iterator add_MacFlow(SnoopMacFlowKey& key, struct timeval ts);
  SnoopFlowMgrMap_MacFlow::iterator del_MacFlow(SnoopMacFlowKey& key);

public slots:
  void process(SnoopPacket* packet);

signals:
  void onNew_MacFlow(SnoopMacFlowKey& key);
  void onDel_MacFlow(SnoopMacFlowKey& key);
  void macFlow_Processed(SnoopPacket* packet);

protected:
  long lastCheckTickInSec;

public:
  long checkIntervalInSec;
  long macFlow_TimeoutInSec;

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
