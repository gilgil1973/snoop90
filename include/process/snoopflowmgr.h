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
#include <SnoopFlowMgrAccessible>

// ----------------------------------------------------------------------------
// SnoopFlowMgrAccessibleItem
// ----------------------------------------------------------------------------
class SnoopFlowMgrAccessibleItem
{
public:
  SnoopFlowMgrAccessibleItem();

public:
  ISnoopFlowMgrAccessible* accessible;
  int    user;
  size_t offset;
  size_t memSize;
};

// ----------------------------------------------------------------------------
// SnoopFlowMgrAccessibleItems
// ----------------------------------------------------------------------------
class SnoopFlowMgrAccessibleItems : public QList<SnoopFlowMgrAccessibleItem*>
{
public:
  SnoopFlowMgrAccessibleItems();
  virtual ~SnoopFlowMgrAccessibleItems();

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

protected:
  void clearMaps();
  SnoopFlowMgrMap_MacFlow macFlow_map;
  SnoopFlowMgrMap_TcpFlow tcpFlow_map;
  SnoopFlowMgrMap_UdpFlow udpFlow_map;

public:
  SnoopFlowMgrAccessibleItems macFlow_items;
  SnoopFlowMgrAccessibleItems tcpFlow_items;
  SnoopFlowMgrAccessibleItems udpFlow_items;

public:
  void registerAccessible(ISnoopFlowMgrAccessible* accessible, SnoopFlowMgrAccessibleItems& items, int user, size_t memSize);

protected:
  void processMacFlow(SnoopPacket* packet, SnoopMacFlowKey& key);

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
