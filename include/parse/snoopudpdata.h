// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_UDP_DATA_H_
#define __SNOOP_UDP_DATA_H_

#include <SnoopUdp>

// ----------------------------------------------------------------------------
// SnoopUdpData
// ----------------------------------------------------------------------------
class SnoopUdpData
{
public:
  static bool parse(SnoopPacket* packet);
  static bool parseAll(SnoopPacket* packet);
};

#endif // __SNOOP_UDP_DATA_H_
