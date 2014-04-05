// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_TCP_BLOCK_H__
#define __SNOOP_TCP_BLOCK_H__

#include <SnoopBlock>
#include <SnoopCapture>

// ----------------------------------------------------------------------------
// SnoopTcpBlock
// ----------------------------------------------------------------------------
class SnoopTcpBlock : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopTcpBlock(void* owner = NULL);
  virtual ~SnoopTcpBlock();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  static const UINT8 TCP_BLOCK_TOS_NO = 0x44;
  static int sendForwardBlock(SnoopCapture* capture, SnoopPacket* packet, UINT8 flag, QByteArray msg = "");
  static int sendBackwardBlock(SnoopCapture* capture, SnoopPacket* packet, UINT8 flag, QByteArray msg = "");

public slots:
  void tcpBlock(SnoopPacket* packet);

public:
  SnoopCapture* writer;
  bool          forwardRst;
  bool          backwardRst;
  bool          forwardFin;
  QByteArray    forwardFinMsg;
  bool          backwardFin;
  QByteArray    backwardFinMsg;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_TCP_BLOCK_H__
