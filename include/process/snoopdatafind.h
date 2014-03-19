// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_DATA_FIND_H__
#define __SNOOP_DATA_FIND_H__

#include <SnoopProcess>
#include <VDataFind>

// ----------------------------------------------------------------------------
// SnoopDataFind
// ----------------------------------------------------------------------------
class SnoopDataFind : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopDataFind(void* owner = NULL);
  virtual ~SnoopDataFind();

protected:
  virtual bool doOpen();
  virtual bool doClose();

signals:
  void found(SnoopPacket* packet);
  void unfound(SnoopPacket* packet);

public slots:
  void find(SnoopPacket* packet);

public:
  bool      tcpFind;
  bool      udpFind;
  VDataFind dataFind;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public:
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_DATA_FIND_H__
