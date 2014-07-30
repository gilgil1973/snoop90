// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_BLOCK_H__
#define __SNOOP_BLOCK_H__

#include <SnoopProcess>

// ----------------------------------------------------------------------------
// SnoopBlock
// ----------------------------------------------------------------------------
class SnoopBlock : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopBlock(void* owner = NULL);
  virtual ~SnoopBlock();

public:
  int dropRate; // 0%(all allow) ~ 100%(all drop)

signals:
  void blocked(SnoopPacket* packet);
  void unblocked(SnoopPacket* packet);

public slots:
  void block(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_BLOCK_H__
