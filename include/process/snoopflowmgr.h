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
// Snoop_IpFlow_Map
// ----------------------------------------------------------------------------
class Snoop_IpFlow_Map : public QMap<SnoopIpFlowKey, SnoopFlowValue>
{
public:
  Snoop_IpFlow_Map();
  virtual ~Snoop_IpFlow_Map();
  void clear();
  Snoop_IpFlow_Map::iterator erase(SnoopIpFlowKey& key);
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
  Snoop_IpFlow_Map  ipFlow_Map;
  Snoop_TcpFlow_Map tcpFlow_Map;
  Snoop_UdpFlow_Map udpFlow_Map;

  void clearMaps();
  void deleteOldMaps(struct timeval ts);

public:
  SnoopFlowMgrRequesterItems macFlow_Items;
  SnoopFlowMgrRequesterItems ipFlow_Items;
  SnoopFlowMgrRequesterItems tcpFlow_Items;
  SnoopFlowMgrRequesterItems udpFlow_Items;

  void clearItems();

protected:
  size_t requestMemory(void* requester, SnoopFlowMgrRequesterItems& items, size_t memSize);

public:
  void checkConnect(const char* signal, VObject* receiver, const char* slot, bool autoConnect);

public:
  //
  // MacFlow
  //
  size_t requestMemory_MacFlow(void* requester, size_t memSize);
  Snoop_MacFlow_Map::iterator add_MacFlow(SnoopMacFlowKey& key, struct timeval ts, bool created);
  Snoop_MacFlow_Map::iterator del_MacFlow(SnoopMacFlowKey& key);

  //
  // IpFlow
  //
  size_t requestMemory_IpFlow(void* requester, size_t memSize);
  Snoop_IpFlow_Map::iterator add_IpFlow(SnoopIpFlowKey& key, struct timeval ts, bool created);
  Snoop_IpFlow_Map::iterator del_IpFlow(SnoopIpFlowKey& key);

  //
  // TcpFlow
  //
  size_t requestMemory_TcpFlow(void* requester, size_t memSize);
  Snoop_TcpFlow_Map::iterator add_TcpFlow(SnoopTcpFlowKey& key, struct timeval ts, bool created);
  Snoop_TcpFlow_Map::iterator del_TcpFlow(SnoopTcpFlowKey& key);

  //
  // UdpFlow
  //
  size_t requestMemory_UdpFlow(void* requester, size_t memSize);
  Snoop_UdpFlow_Map::iterator add_UdpFlow(SnoopUdpFlowKey& key, struct timeval ts, bool created);
  Snoop_UdpFlow_Map::iterator del_UdpFlow(SnoopUdpFlowKey& key);

public slots:
  void process(SnoopPacket* packet);

protected:
  void process_MacFlow(SnoopPacket* packet, SnoopMacFlowKey& key);
  void process_IpFlow(SnoopPacket* packet, SnoopIpFlowKey& key);
  void process_TcpFlow(SnoopPacket* packet, SnoopTcpFlowKey& key);
  void process_UdpFlow(SnoopPacket* packet, SnoopUdpFlowKey& key);

signals:
  //
  // MacFlow
  //
  void macFlowCreated(SnoopMacFlowKey* key);
  void macFlowDeleted(SnoopMacFlowKey* key);
  void macFlowCaptured(SnoopPacket* packet);

  //
  // IpFlow
  //
  void ipFlowCreated(SnoopIpFlowKey* key);
  void ipFlowDeleted(SnoopIpFlowKey* key);
  void ipFlowCaptured(SnoopPacket* packet);

  //
  // TcpFlow
  //
  void tcpFlowCreated(SnoopTcpFlowKey* key);
  void tcpFlowDeleted(SnoopTcpFlowKey* key);
  void tcpFlowCaptured(SnoopPacket* packet);

  //
  // UdpFlow
  //
  void udpFlowCreated(SnoopUdpFlowKey* key);
  void udpFlowDeleted(SnoopUdpFlowKey* key);
  void udpFlowCaptured(SnoopPacket* packet);

protected:
  long lastCheckTickInSec;

public:
  long checkIntervalInSec;
  long macFlowTimeoutInSec;
  long ipFlowTimeoutInSec;
  long tcpFlowTimeoutInSec;
  long udpFlowTimeoutInSec;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_FLOW_MGR_H__
