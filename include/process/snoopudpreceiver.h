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
#include <SnoopCapture>

// ----------------------------------------------------------------------------
// SnoopUdpReceiver
// ----------------------------------------------------------------------------
class SnoopUdpReceiver : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopUdpReceiver(void* owner = NULL);
  virtual ~SnoopUdpReceiver();

public:
  SnoopCapture* writer;
  QString       discriminator;

protected:
  virtual bool doOpen();
  virtual bool doClose();

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

#endif // __SNOOP_UDP_SENDER_H__
