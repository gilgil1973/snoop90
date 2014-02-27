// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_DUMP_H__
#define __SNOOP_DUMP_H__

#include <SnoopProcess>

// ----------------------------------------------------------------------------
// SnoopDump
// ----------------------------------------------------------------------------
class SnoopDump : public SnoopProcess
{
  Q_OBJECT

public:
  static const char* DEFAULT_DUMP_FILE_NAME;

protected:
  pcap*          m_pcap;
  pcap_dumper_t* m_pcap_dumper;

public:
  SnoopDump(void* owner = NULL);
  virtual ~SnoopDump();

public:
  QString filePath;
  int     linkType;

protected:
  virtual bool doOpen();
  virtual bool doClose();

public slots:
  void dump(SnoopPacket* packet);

signals:
  void dumped(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_DUMP_H__
