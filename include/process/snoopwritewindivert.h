// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_WRITE_WIN_DIVERT_H__
#define __SNOOP_WRITE_WIN_DIVERT_H__

#include <SnoopWinDivert>

// ----------------------------------------------------------------------------
// SnoopWriteWinDivert
// ----------------------------------------------------------------------------
class SnoopWriteWinDivert : public SnoopWinDivert
{
  Q_OBJECT

public:
  SnoopWriteWinDivert(void* owner = NULL);
  virtual ~SnoopWriteWinDivert();

public slots:
  void copy(SnoopPacket* packet);
  void move(SnoopPacket* packet);

signals:
  void copied(SnoopPacket* packet);
  void moved(SnoopPacket* packet);
};

#endif // __SNOOP_WRITE_WIN_DIVERT_H__
