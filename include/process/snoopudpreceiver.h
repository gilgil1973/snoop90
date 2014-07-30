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
#include <SnoopUdpChunk>

// ----------------------------------------------------------------------------
// SnoopUdpReceiverFlowItem
// ----------------------------------------------------------------------------
class SnoopUdpReceiverFlowItem
{
public:
  bool    first;
  quint16 lastId;

public:
  void clear();
};

// ----------------------------------------------------------------------------
// SnoopUdpReceiver
// ----------------------------------------------------------------------------
class SnoopUdpReceiver : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopUdpReceiver(void* owner = NULL);
  virtual ~SnoopUdpReceiver();


protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  SnoopFlowMgr* flowMgr;
  SnoopCapture* writer;
  QByteArray    dscr;
  int           headerSize;
  bool          autoBlockOnSplit;

protected:
  size_t udpFlowOffset;

protected slots:
  void __udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value);
  void __udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value);

protected:
  void doSplit(SnoopUdpChunk& chunk, SnoopPacket* packet);

signals:
  void splitted(SnoopPacket* packet);

public slots:
  void split(SnoopPacket* packet);

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
