// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_DELAY_H__
#define __SNOOP_DELAY_H__

#include <SnoopProcess>
#include <SnoopCapture>
#include <VThread>
#include <VTick>

// ----------------------------------------------------------------------------
// SnoopDelayItem
// ----------------------------------------------------------------------------
class SnoopDelayItem
{
public:
  VTick             tick;
  QByteArray        buf;
  WINDIVERT_ADDRESS divertAddr;
};

// ----------------------------------------------------------------------------
// SnoopDelayItemMgr
// ----------------------------------------------------------------------------
class SnoopDelayItemMgr : public VLockable
{
public:
  SnoopDelayItemMgr();
  virtual ~SnoopDelayItemMgr();

public:
  SnoopCapture*         writer; // reference
  QList<SnoopDelayItem> items;

public:
  int flush(VTick now = 0xFFFFFFFFFFFF);
};

// ----------------------------------------------------------------------------
// SnoopDelayThread
// ----------------------------------------------------------------------------
class SnoopDelay;
class SnoopDelayThread : public VThread
{
public:
  SnoopDelayThread(SnoopDelay* delay);
  virtual ~SnoopDelayThread();

public:
  SnoopDelayItemMgr itemMgr;

protected:
  virtual void run();
};

// ----------------------------------------------------------------------------
// SnoopDelay
// ----------------------------------------------------------------------------
class SnoopDelay : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopDelay(void* owner = NULL);
  virtual ~SnoopDelay();

public:
  SnoopCapture* writer;
  VTimeout      timeout;

protected:
  SnoopDelayThread* thread;

protected:
  virtual bool doOpen();
  virtual bool doClose();

public slots:
  void delay(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_DELAY_H__
