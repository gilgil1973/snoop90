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
  int       linkType;
  ETH_HDR*  ethHdr;
  FDDI_HDR* fddiHdr;

  ///
  /// network layer
  ///
  UINT16    netType;
  IP_HDR*   ipHdr;
  ARP_HDR*  arpHdr;

  ///
  /// transport layer(protocol)
  ///
  UINT8     proto;
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
  /// windivert
  ///
  WINDIVERT_ADDRESS divertAddr;

  ///
  /// keymgr
  ///
  int       user;
  void*     mem;

public:
  void clear();
  int  write(QByteArray& ba);
};

#endif // __SNOOP_PACKET_H__
