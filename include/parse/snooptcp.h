// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_TCP_H__
#define __SNOOP_TCP_H__

#include <SnoopIp>

// ----------------------------------------------------------------------------
// SnoopTcpOption
// ----------------------------------------------------------------------------

#pragma pack(push, 1)
typedef struct
{
  UINT8 kind;
  UINT8 len;
  BYTE* value;
  BYTE* desc;
} SnoopTcpOption;
#pragma pack(pop)

typedef std::list<SnoopTcpOption> SnoopTcpOptionList; // gilgil temp 2011.10.27

// ----------------------------------------------------------------------------
// SnoopTcp
// ----------------------------------------------------------------------------
class SnoopTcp
{
public:
  static bool   isData  (IP_HDR* ipHdr, TCP_HDR* tcpHdr, BYTE** tcpData = NULL, int* tcpDataLen = NULL);
  static bool   isOption(TCP_HDR* tcpHdr, BYTE** tcpOption = NULL, int* tcpOptionLen = NULL);
  static UINT16 checksum(IP_HDR* ipHdr, TCP_HDR* tcpHdr);

public:
  static bool parse(SnoopPacket* packet);
  static bool parseAll(SnoopPacket* packet);

  //
  // Return one snoopTCPOption buffer size
  //
  static int getOption(
    char*           tcpOption,
    int             tcpOptionLen,
    SnoopTcpOption& snoopTCPOption);

  //
  // Return total snoopTCPOption buffer size
  //
  static int getOptionList(
    char*               tcpOption,
    int                 tcpOptionLen,
    SnoopTcpOptionList& snoopTCPOptionList);
};

// ----------------------------------------------------------------------------
/* from : http://www.iana.org/assignments/tcp-parameters

TCP OPTION NUMBERS

(last updated 2007-02-15)

The Transmission Control Protocol (TCP) has provision for optional
header fields identified by an option kind field.  Options 0 and 1 are
exactly one octet which is their kind field.  All other options have
their one octet kind field, followed by a one octet length field,
followed by length-2 octets of option data.

Kind   Length   Meaning                           Reference
----   ------   -------------------------------   ---------
  0        -    End of Option List                 [RFC793]
  1        -    No-Operation                       [RFC793]
  2        4    Maximum Segment Size               [RFC793]
  3        3    WSOPT - Window Scale              [RFC1323]
  4        2    SACK Permitted                    [RFC2018]
  5        N    SACK                              [RFC2018]
  6        6    Echo (obsoleted by option 8)      [RFC1072]
  7        6    Echo Reply (obsoleted by option 8)[RFC1072]
  8       10    TSOPT - Time Stamp Option         [RFC1323]
  9        2    Partial Order Connection Permitted[RFC1693]
 10        3    Partial Order Service Profile     [RFC1693]
 11             CC                                [RFC1644]
 12             CC.NEW                            [RFC1644]
 13             CC.ECHO                           [RFC1644]
 14         3   TCP Alternate Checksum Request    [RFC1146]
 15         N   TCP Alternate Checksum Data       [RFC1146]
 16             Skeeter                           [Knowles]
 17             Bubba                             [Knowles]
 18         3   Trailer Checksum Option    [Subbu & Monroe]
 19        18   MD5 Signature Option              [RFC2385]
 20             SCPS Capabilities                   [Scott]
 21             Selective Negative Acknowledgements [Scott]
 22             Record Boundaries                   [Scott]
 23             Corruption experienced              [Scott]
 24             SNAP         [Sukonnik]
 25             Unassigned (released 12/18/00)
 26             TCP Compression Filter           [Bellovin]
 27          8  Quick-Start Response              [RFC4782]
 28-252         Unassigned
 253         N  RFC3692-style Experiment 1 (*)    [RFC4727]
 254         N  RFC3692-style Experiment 2 (*)    [RFC4727]

(*) It is only appropriate to use these values in explicitly-
    configured experiments; they MUST NOT be shipped as defaults in
    implementations.  See RFC 3692 for details.


TCP ALTERNATE CHECKSUM NUMBERS

Number  Description      Reference
------- ------------------------------- ----------
   0    TCP Checksum                    [RFC-1146]
   1    8-bit Fletchers's algorithm     [RFC-1146]
   2    16-bit Fletchers's algorithm    [RFC-1146]
   3    Redundant Checksum Avoidance    [Kay]

*/
// ----------------------------------------------------------------------------

#endif // __SNOOP_TCP_H__
