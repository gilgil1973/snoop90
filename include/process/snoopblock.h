// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 8.0
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

public slots:
  void block(SnoopPacket* packet);

public:
  bool          relay;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VShowOption
  virtual void addOptionWidget(QLayout* layout);
  virtual void saveOption(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_BLOCK_H__
