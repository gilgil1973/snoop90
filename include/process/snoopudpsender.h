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
#include <SnoopCapture>

// ----------------------------------------------------------------------------
// SnoopUdpSender
// ----------------------------------------------------------------------------
class SnoopUdpSender : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopUdpSender(void* owner = NULL);
  virtual ~SnoopUdpSender();

public:
  SnoopCapture* writer;

protected:
  virtual bool doOpen();
  virtual bool doClose();

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
