// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_PACKET_H__
#define __SNOOP_PACKET_H__

#include <SnoopType>
#include <windivert/windivert.h>

// ----------------------------------------------------------------------------
// SnoopFlowValue
// ----------------------------------------------------------------------------
class SnoopFlowValue
{
public:
  size_t         packets;
  size_t         bytes;
  struct timeval ts;
  bool           created;
  BYTE*          totalMem;
};

// ----------------------------------------------------------------------------
// SnoopPacket
// ----------------------------------------------------------------------------
class SnoopPacket
{
public:
  ///
  /// packet
  ///
  PKT_HDR*  pktHdr;
  u_char*   pktData;

  ///
  /// datalink layer
  ///
  int       linkType; // DLT_EN10MB, ...
  ETH_HDR*  ethHdr;

  ///
  /// network layer
  ///
  UINT16    netType; // ETHERTYPE_IP, ETHERTYPE_ARP, ...
  IP_HDR*   ipHdr;
  ARP_HDR*  arpHdr;

  ///
  /// transport layer(protocol)
  ///
  UINT8     proto; // IPPROTO_TCP, IPPROTO_UDP, IPPROTO_ICMP, ...
  TCP_HDR*  tcpHdr;
  UDP_HDR*  udpHdr;
  ICMP_HDR* icmpHdr;

  ///
  /// data
  ///
  char*     data;
  int       dataLen;

  ///
  /// control
  ///
  bool      drop;
  bool      ipChanged;
  bool      tcpChanged;
  bool      udpChanged;

  ///
  /// flow
  ///
  void*           flowKey;  // SnoopMacFlowKey, ... // gilgil temp 2014.02.24
  SnoopFlowValue* flowValue;

  ///
  /// windivert
  ///
  WINDIVERT_ADDRESS divertAddr;

public:
  void clear();
  int  write(QByteArray& ba);
};

#endif // __SNOOP_PACKET_H__
