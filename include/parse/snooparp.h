// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 8.0
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
  static bool parseAll(SnoopPacket* packet);
};

#endif // __SNOOP_ARP_H__
