// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_UDP_RECEIVER_H__
#define __SNOOP_UDP_RECEIVER_H__

#include <SnoopProcess>
#include <SnoopFlowMgr>
#include <SnoopCapture>
#include <SnoopUdp>

// ----------------------------------------------------------------------------
// SnoopUdpSenderFlowItem
// ----------------------------------------------------------------------------
class SnoopUdpSenderFlowItem
{
public:
  int count;
  QList<QByteArray> bufferList;

public:
  SnoopUdpSenderFlowItem();
  virtual ~SnoopUdpSenderFlowItem();

public:
  void clear();
};

// ----------------------------------------------------------------------------
// SnoopUdpSender
// ----------------------------------------------------------------------------
class SnoopUdpSender : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopUdpSender(void* owner = NULL);
  virtual ~SnoopUdpSender();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  SnoopFlowMgr* flowMgr;
  SnoopCapture* writer;
  QString       discriminator;
  int           maxBufferCount;

protected:
  size_t udpFlowOffset;

protected slots:
  void __udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value);
  void __udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value);

public slots:
  void merge(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_UDP_RECEIVER_H__
