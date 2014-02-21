// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_TCP_DATA_H__
#define __SNOOP_TCP_DATA_H__

#include <SnoopTcp>

// ----------------------------------------------------------------------------
// SnoopTcpData
// ----------------------------------------------------------------------------
class SnoopTcpData
{
public:
  static bool parse(SnoopPacket* packet);
  static bool parseAll(SnoopPacket* packet);
};

#endif // __SNOOP_TCP_DATA_H__
