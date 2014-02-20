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
#include <SnoopKeyMgrAccessible>

// ----------------------------------------------------------------------------
// SnoopKeyMgrAccessibleItem
// ----------------------------------------------------------------------------
class SnoopKeyMgrAccessibleItem
{
public:
  SnoopKeyMgrAccessibleItem();

public:
  ISnoopKeyMgrAccessible* accessible;
  int    user;
  size_t offset;
  size_t memSize;
};

// ----------------------------------------------------------------------------
// SnoopKeyMgrAccessibleItems
// ----------------------------------------------------------------------------
class SnoopKeyMgrAccessibleItems : public QList<SnoopKeyMgrAccessibleItem*>
{
public:
  SnoopKeyMgrAccessibleItems();
  virtual ~SnoopKeyMgrAccessibleItems();

public:
  size_t totalMemSize;
};

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
  SnoopKeyMgrMap_MacFlow macFlow_map;
  SnoopKeyMgrMap_TcpFlow tcpFlow_map;
  SnoopKeyMgrMap_UdpFlow udpFlow_map;

public:
  SnoopKeyMgrAccessibleItems macFlow_items;
  SnoopKeyMgrAccessibleItems tcpFlow_items;
  SnoopKeyMgrAccessibleItems udpFlow_items;

public:
  void registerAccessible(ISnoopKeyMgrAccessible* accessible, SnoopKeyMgrAccessibleItems& items, int user, size_t memSize);

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
