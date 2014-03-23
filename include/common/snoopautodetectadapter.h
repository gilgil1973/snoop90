// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_AUTO_DETECT_ADAPTER_H__
#define __SNOOP_AUTO_DETECT_ADAPTER_H__

// ----------------------------------------------------------------------------
// This directive is used to determine whi detect mechanism is used
//#define SNOOP_AUTO_DETECT_USE_GETBESTINTERFACE
#define SNOOP_AUTO_DETECT_USE_RTM
//#define SNOOP_AUTO_DETECT_USE_ADAPTER
// ----------------------------------------------------------------------------

#if defined SNOOP_AUTO_DETECT_USE_GETBESTINTERFACE || defined SNOOP_AUTO_DETECT_USE_RTM

#include <QObject>
#include <QString>

// ----------------------------------------------------------------------------
// SnoopAutoDetectAdapter
// ----------------------------------------------------------------------------
class SnoopAutoDetectAdapter
{
private: // singleton
  SnoopAutoDetectAdapter()          {}
  virtual ~SnoopAutoDetectAdapter() {}

public:
  virtual int detect(QString host);

public:
  static SnoopAutoDetectAdapter& instance()
  {
    static SnoopAutoDetectAdapter g_instance;
    return g_instance;
  }
};

#endif // defined SNOOP_AUTO_DETECT_USE_GETBESTINTERFACE || defined SNOOP_AUTO_DETECT_USE_RTM

#ifdef SNOOP_AUTO_DETECT_USE_ADAPTER

#include <VUDPClient>
#include <SnoopAdapter>

// ----------------------------------------------------------------------------
// SnoopAutoDetectAdapterItem
// ----------------------------------------------------------------------------
class SnoopAutoDetectAdapterItem : public VObject
{
  Q_OBJECT

public:
  SnoopAutoDetectAdapterItem(void* owner);
  virtual ~SnoopAutoDetectAdapterItem();

public:
  SnoopAdapter adapter;

protected:
  virtual bool doOpen();
  virtual bool doClose();

public slots:
  void recv(SnoopPacket* packet);
};

// ----------------------------------------------------------------------------
// SnoopAutoDetectAdapter
// ----------------------------------------------------------------------------
class SnoopAutoDetectAdapter : public QObject
{
  Q_OBJECT

  friend class SnoopAutoDetectAdapterItem;

protected:
  int    m_adapterIndex;
  VEvent m_event;

private: // singleton
  SnoopAutoDetectAdapter();
  virtual ~SnoopAutoDetectAdapter();

public:
  virtual int detect(QString& host);

public slots:
  void recv(SnoopPacket* packet);

  static SnoopAutoDetectAdapter& instance()
  {
    static SnoopAutoDetectAdapter g_instance;
    return g_instance;
  }
};

#endif // SNOOP_AUTO_DETECT_USE_ADAPTER

#endif // __SNOOP_AUTO_DETECT_ADAPTER_H__
