// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_UDP_SENDER_H__
#define __SNOOP_UDP_SENDER_H__

#include <SnoopProcess>
#include <SnoopFlowMgr>
#include <SnoopCapture>
#include <SnoopUdp>
#include <SnoopUdpChunk>

// ----------------------------------------------------------------------------
// SnoopUdpSenderFlowItem
// ----------------------------------------------------------------------------
class SnoopUdpSenderFlowItem
{
public:
  quint16 lastId;
  SnoopUdpChunks chunks;

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
  SnoopCapture* writer; // gilgil temp 2014.07.30
  QByteArray    dscr;
  int           headerSize;
  int           addChunkCount;

protected:
  size_t udpFlowOffset;

protected slots:
  void __udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value);
  void __udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value);

signals:
  void merged(SnoopPacket* packet);

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

#endif // __SNOOP_UDP_SENDER_H__
