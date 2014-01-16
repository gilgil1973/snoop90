// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 8.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_BP_FILTER_H__
#define __SNOOP_BP_FILTER_H__

#include <SnoopFilter>

// ----------------------------------------------------------------------------
// SnoopBpFilter
// ----------------------------------------------------------------------------
class SnoopBpFilter : public SnoopFilter
{
  Q_OBJECT

public:
  SnoopBpFilter(void* owner = NULL);
  virtual ~SnoopBpFilter();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  pcap_t*      m_pcap;
  bpf_program* m_code;

public:
  QString filter;
  int     linkType;

public:
  bool check(char* pktData, int pktLen);

public slots:
  void check(SnoopPacket* packet);

signals:
  void ack(SnoopPacket* packet);
  void nak(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VShowOption
  virtual void addOptionWidget(QLayout* layout);
  virtual void saveOption(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_BP_FILTER_H__
