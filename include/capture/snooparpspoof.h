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

// ----------------------------------------------------------------------------
// SnoopArpSpoof
// ----------------------------------------------------------------------------
class SnoopArpSpoof : public SnoopAdapter
{
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

public slots:
  void process(SnoopPacket* packet);

signals:
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
