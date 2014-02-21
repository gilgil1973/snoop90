// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_ARP_H__
#define __SNOOP_ARP_H__

#include <SnoopEth>

// ----------------------------------------------------------------------------
// SnoopArp
// ----------------------------------------------------------------------------
class SnoopArp
{
public:
  static bool parse(SnoopPacket* packet);
  static bool parseAll(SnoopPacket* packet);
};

#endif // __SNOOP_ARP_H__
