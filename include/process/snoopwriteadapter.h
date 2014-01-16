// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 8.0
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

public slots:
  void output(SnoopPacket* packet);
};

#endif // __SNOOP_WRITE_ADAPTER_H__
