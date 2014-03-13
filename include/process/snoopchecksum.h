// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_CHECKSUM_H__
#define __SNOOP_CHECKSUM_H__

#include <SnoopProcess>

// ----------------------------------------------------------------------------
// SnoopChecksum
// ----------------------------------------------------------------------------
class SnoopChecksum : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopChecksum(void* owner = NULL);
  virtual ~SnoopChecksum();

public:
  bool ipChecksum;
  bool tcpChecksum;
  bool udpChecksum;

public slots:
  void calculate(SnoopPacket* packet);

signals:
  void calculated(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif __SNOOP_CHECKSUM_H__
