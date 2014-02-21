// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_ETH_H__
#define __SNOOP_ETH_H__

#include <Snoop>

// ----------------------------------------------------------------------------
// SnoopEth
// ----------------------------------------------------------------------------
class SnoopEth
{
public:
  static bool is   (ETH_HDR* ethHdr, UINT16 etherType, void** networkHdr);
  static bool isIp (ETH_HDR* ethHdr, IP_HDR** ipHdr);
  static bool isArp(ETH_HDR* ethHdr, ARP_HDR** arpHdr);
  
public:
  static bool parse(SnoopPacket* packet);
  static bool parseAll(SnoopPacket* packet);
};

#endif // __SNOOP_ETH_H__
