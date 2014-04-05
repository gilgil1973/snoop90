// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_ARP_SPOOF_H__
#define __SNOOP_ARP_SPOOF_H__

#include <SnoopAdapter>
#include <VListWidget>
#include <SnoopNetInfo>
#include <SnoopFindHost>
#include <SnoopBpFilter>
#include <SnoopBpFilter>

// ----------------------------------------------------------------------------
// SnoopArpSpoofSession
// ----------------------------------------------------------------------------
class SnoopArpSpoofSession : public VXmlable
{
public:
  Ip  senderIp;
  Mac senderMac;
  Ip  targetIp;
  Mac targetMac;

public:
  SnoopArpSpoofSession();
  virtual ~SnoopArpSpoofSession();

public:
  void clear();
  bool operator ==(const SnoopArpSpoofSession& r);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public:
  static const int SENDER_IP_IDX  = 0;
  static const int SENDER_MAC_IDX = 1;
  static const int TARGET_IP_IDX  = 2;
  static const int TARGET_MAC_IDX = 3;

public:
  static void initialize(QTreeWidget* treeWidget);
#endif // QT_GUI_LIB
};

#ifdef QT_GUI_LIB
void operator << (QTreeWidgetItem& treeWidgetItem, SnoopArpSpoofSession& session);
void operator << (SnoopArpSpoofSession& session, QTreeWidgetItem& treeWidgetItem);
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopArpSpoofSessionList
// ----------------------------------------------------------------------------
class SnoopArpSessionMgrIpMacMap : QMap<Ip, Mac> {};

// ----------------------------------------------------------------------------
// SnoopArpSpoofSessionList
// ----------------------------------------------------------------------------
class SnoopArpSpoof;
class SnoopArpSpoofSessionList :
  public QObject,
  public QList<SnoopArpSpoofSession>,
  public VXmlable,
  public VOptionable,
  public VListWidgetAccessible,
  public VLockable
{
  Q_OBJECT

protected:
  SnoopArpSpoof* arpSpoof;

public:
  SnoopArpSpoofSessionList(SnoopArpSpoof* arpSpoof);
  virtual ~SnoopArpSpoofSessionList();

public:
  SnoopArpSessionMgrIpMacMap ipMacMap;

public:
  void add(SnoopArpSpoofSession& session);
  void del(SnoopArpSpoofSession& session);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);

public: // VListWidgetAccessible
  virtual void  widgetClear()                                                             { clear();                                         }
  virtual int   widgetCount()                                                             { return count();                                  }
  virtual void* widgetAt(int i)                                                           { return  (void*)&at(i);                           }
  virtual void  widgetPushBack(void* item)                                                { push_back(*(const SnoopArpSpoofSession*)item);   }
  virtual void* widgetCeateItem()                                                         { return new SnoopArpSpoofSession;                 }
  virtual void  widgetItemIntoTreeWidgetItem(void* item, QTreeWidgetItem* treeWidgetItem) { *treeWidgetItem << *(SnoopArpSpoofSession*)item; }
  virtual void  widgetTreeWidgetItemIntoItem(QTreeWidgetItem* treeWidgetItem, void* item) { *(SnoopArpSpoofSession*)item << *treeWidgetItem; }
#endif // QT_GUI_LIB
};

// ----------------------------------------------------------------------------
// SnoopArpSpoofInfectThread
// ----------------------------------------------------------------------------
class SnoopArpSpoofInfectThread : public VThread
{
protected:
  SnoopArpSpoof* arpSpoof;
  VEvent         event;

public:
  SnoopArpSpoofInfectThread(SnoopArpSpoof* arpSpoof);
  virtual ~SnoopArpSpoofInfectThread();

public:
  virtual bool open();
  virtual bool close();

protected:
  virtual void run();
};

// ----------------------------------------------------------------------------
// SnoopArpSpoof
// ----------------------------------------------------------------------------
class SnoopArpSpoof : public SnoopAdapter
{
  Q_OBJECT

  friend class SnoopArpSpoofInfectThread;

  typedef enum
  {
    ipNone,
    ipSender,      // sender       ip packet (emit captured)
    ipRelay,       // relay        ip packet (emit capturedSpoof) - including self spoofed relay packet
    ipSelfSpoofed, // self spoofed ip packet (emit capturedOther)
    ipOther        // other        ip packet (emit capturedOther)
  } IpPacketType;

public:
  SnoopArpSpoof(void* owner = NULL);
  virtual ~SnoopArpSpoof();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  virtual int read(SnoopPacket* packet);
  virtual int write(SnoopPacket* packet);
  virtual int write(u_char* buf, int size, WINDIVERT_ADDRESS* divertAddr = NULL);

public:
  virtual SnoopCaptureType captureType() { return SnoopCaptureType::InPath; }
  // virtual int              dataLink()    { return m_dataLink; }
  virtual bool             relay(SnoopPacket* packet);

public:
  Mac                        virtualMac;
  bool                       selfRelay;
  bool                       disableAutoRouting;
  VTimeout                   infectInterval;
  SnoopArpSpoofSessionList   sessionList;

protected:
  Mac                        realVirtualMac;
  SnoopNetInfo               netInfo;
  SnoopFindHost              findHost;
  SnoopBpFilter              bpFilter;
  SnoopArpSpoofInfectThread* infectThread;

protected:
  bool retrieveUnknownMacHostList();

protected:
  bool sendArpInfect(SnoopArpSpoofSession& session);
  bool sendArpInfectAll();
  bool sendArpRecover(SnoopArpSpoofSession& session);
  bool sendArpRecoverAll();

protected:
  void         preventArpRecover(ETH_HDR* ethHdr, ARP_HDR* arpHdr);
  IpPacketType findSessionByIpPacket(SnoopPacket* packet, SnoopArpSpoofSession** _session);

signals:
  void capturedSpoof(SnoopPacket* packet);
  void capturedOther(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_ARP_SPOOF_H__
