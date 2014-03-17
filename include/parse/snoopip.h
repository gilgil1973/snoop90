// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_IP_H__
#define __SNOOP_IP_H__

#include <SnoopEth>

// ----------------------------------------------------------------------------
// SnoopIp
// ----------------------------------------------------------------------------
class SnoopIp
{
public:
  static bool   is    (IP_HDR* ipHdr, UINT8 protocol, void** transportHdr);
  static bool   isTcp (IP_HDR* ipHdr, TCP_HDR**  tcpHdr);
  static bool   isUdp (IP_HDR* ipHdr, UDP_HDR**  udpHdr);
  static bool   isIcmp(IP_HDR* ipHdr, ICMP_HDR** icmpHdr);
  static UINT16 checksum(IP_HDR* ipHdr);
  static UINT16 recalculateChecksum(UINT16 oldChecksum, UINT16 oldValue, UINT16 newValue);
  static UINT16 recalculateChecksum(UINT16 oldChecksum, UINT32 oldValue, UINT32 newValue);

public:
  static bool parse(SnoopPacket* packet);
  static bool parseAll(SnoopPacket* packet);
};

#endif // __SNOOP_IP_H__
