// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_WRITE_ADAPTER_H__
#define __SNOOP_WRITE_ADAPTER_H__

#include <SnoopAdapter>

// ----------------------------------------------------------------------------
// SnoopWriteAdapter
// ----------------------------------------------------------------------------
class SnoopWriteAdapter : public SnoopAdapter
{
  Q_OBJECT

public:
  SnoopWriteAdapter(void* owner = NULL);
  virtual ~SnoopWriteAdapter();

public:
  Mac srcMac;
  Mac dstMac;

public slots:
  void copy(SnoopPacket* packet);
  void move(SnoopPacket* packet);

signals:
  void copied(SnoopPacket* packet);
  void moved(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_WRITE_ADAPTER_H__
