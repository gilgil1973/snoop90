// ----------------------------------------------------------------------------
//
// SnoopPcap Component Suite version 5.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_SOURCE_PCAP_H__
#define __SNOOP_SOURCE_PCAP_H__

#include <SnoopPcap>

// ----------------------------------------------------------------------------
// SnoopSourcePcap
// ----------------------------------------------------------------------------
class SnoopSourcePcap : public SnoopPcap
{
public:
  SnoopSourcePcap(void* owner = NULL);
  virtual ~SnoopSourcePcap();

public:
  QString source;

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VShowOption
  virtual void addOptionWidget(QLayout* layout);
  virtual void saveOption(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_SOURCE_PCAP_H__
