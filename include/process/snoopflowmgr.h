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
#include <SnoopTypeKey>

// ----------------------------------------------------------------------------
// Snoop_MacFlow_Map
// ----------------------------------------------------------------------------
class Snoop_MacFlow_Map : public QMap<SnoopMacFlowKey, SnoopFlowValue>
{
public:
  Snoop_MacFlow_Map();
  virtual ~Snoop_MacFlow_Map();
  void clear();
  Snoop_MacFlow_Map::iterator erase(SnoopMacFlowKey& key);
};

// ----------------------------------------------------------------------------
// Snoop_TcpFlow_Map
// ----------------------------------------------------------------------------
class Snoop_TcpFlow_Map : public QMap<SnoopTcpFlowKey, SnoopFlowValue>
{
public:
  Snoop_TcpFlow_Map();
  virtual ~Snoop_TcpFlow_Map();
  void clear();
  Snoop_TcpFlow_Map::iterator erase(SnoopTcpFlowKey& key);
};

// ----------------------------------------------------------------------------
// Snoop_UdpFlow_Map
// ----------------------------------------------------------------------------
class Snoop_UdpFlow_Map : public QMap<SnoopUdpFlowKey, SnoopFlowValue>
{
public:
  Snoop_UdpFlow_Map();
  virtual ~Snoop_UdpFlow_Map();
  void clear();
  Snoop_UdpFlow_Map::iterator erase(SnoopUdpFlowKey& key);
};

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
  Snoop_MacFlow_Map macFlow_Map;
  Snoop_TcpFlow_Map tcpFlow_Map;
  Snoop_UdpFlow_Map udpFlow_Map;

  void clearMaps();
  void deleteOldMaps(struct timeval ts);

public:
  SnoopFlowMgrRequesterItems macFlow_Items;
  SnoopFlowMgrRequesterItems tcpFlow_Items;
  SnoopFlowMgrRequesterItems udpFlow_Items;

  void clearItems();

protected:
  size_t requestMemory(void* requester, SnoopFlowMgrRequesterItems& items, size_t memSize);

public:
  //
  // MacFlow
  //
  size_t requestMemory_MacFlow(void* requester, size_t memSize);
  void   process_MacFlow(SnoopPacket* packet, SnoopMacFlowKey& key);
  Snoop_MacFlow_Map::iterator add_MacFlow(SnoopMacFlowKey& key, struct timeval ts);
  Snoop_MacFlow_Map::iterator del_MacFlow(SnoopMacFlowKey& key);

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
