// ----------------------------------------------------------------------------
//
// SnoopPcap Component Suite version 5.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_PCAPH_H__
#define __SNOOP_PCAPH_H__

#include <SnoopCapture>

#ifdef linux
typedef void pcap_rmtauth;
#endif // linux

// ----------------------------------------------------------------------------
// SnoopPcap
// ----------------------------------------------------------------------------
class SnoopPcap : public SnoopCapture
{
public:
  SnoopPcap(void* owner = NULL);
  virtual ~SnoopPcap();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  virtual int read(SnoopPacket* packet);
  virtual int write(SnoopPacket* packet);
  virtual int write(u_char* buf, int size, WINDIVERT_ADDRESS* divertAddr = NULL);

public:
  virtual SnoopCaptureType captureType() { return SnoopCaptureType::OutOfPath; }
  virtual int              dataLink()    { return m_dataLink; }
  virtual bool             relay(SnoopPacket* packet);

  //
  // Properties
  //
public:
  QString  filter;
  int      snapLen;
  int      flags;
  int      readTimeout;

public:
  pcap*    m_pcap;
protected:
  int      m_dataLink;
private:
  QString  m_source;

public:

public:
  QString source()     { return m_source;   }

protected:
  bool pcapOpen(char* source, pcap_rmtauth* auth, pcap_if_t* dev);
  bool pcapProcessFilter(pcap_if_t* dev);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_PCAPH_H__
