// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_ICMP_H__
#define __SNOOP_ICMP_H__

#include <SnoopIp>

// ----------------------------------------------------------------------------
// SnoopIcmp
// ----------------------------------------------------------------------------
class SnoopIcmp
{
public:
  static bool   isData(IP_HDR* ipHdr, ICMP_HDR* icmpHdr, BYTE** icmpData = NULL, int* icmpDataLen = NULL);
  static UINT16 checksum(IP_HDR* ipHdr, ICMP_HDR* icmpHdr);

public:
  static bool parse(SnoopPacket* packet);
  static bool parseAll(SnoopPacket* packet);
};

#endif // __SNOOP_ICMP_H__
