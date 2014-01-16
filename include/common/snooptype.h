// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 8.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_TYPE_H__
#define __SNOOP_TYPE_H__

#include <SnoopCommon>

#pragma pack(push, 1)
#include <libnet/config.h>
#include <libnet/libnet-macros.h>
#include <libnet/libnet-headers.h>
#pragma pack(pop)

// ----------------------------------------------------------------------------
// PKT_HDR
// ----------------------------------------------------------------------------
typedef struct pcap_pkthdr             PKT_HDR;

// ----------------------------------------------------------------------------
// Mac
// ----------------------------------------------------------------------------
#pragma pack(push, 1)
class Mac
{
public:
  static const int MAC_SIZE = 6;

protected:
  UINT8 value[MAC_SIZE];

public:
  Mac()                   {                                       } // default ctor
  Mac(const UINT8* value) { memcpy(this->value, value, MAC_SIZE); } // conversion ctor

  operator UINT8*() const { return (UINT8*)value;                 } // cast operator

public:
  Mac(const QString s);
  Mac(const char* s)      { *this = QString(s);                         }

public:
  QString str() const;
  QString qformat(QString format);

  bool operator == (const Mac& rhs) const   { return memcmp(value, rhs.value, MAC_SIZE) == 0; }
  bool operator != (const Mac& rhs) const   { return memcmp(value, rhs.value, MAC_SIZE) != 0; }
  bool operator <  (const Mac& rhs) const   { return memcmp(value, rhs.value, MAC_SIZE) <  0; }
  bool operator >  (const Mac& rhs) const   { return memcmp(value, rhs.value, MAC_SIZE) >  0; }
  bool operator <= (const Mac& rhs) const   { return memcmp(value, rhs.value, MAC_SIZE) <= 0; }
  bool operator >= (const Mac& rhs) const   { return memcmp(value, rhs.value, MAC_SIZE) >= 0; }
  bool operator == (const UINT8* rhs) const { return memcmp(value, rhs,       MAC_SIZE) == 0; }

public:
  void clear()          { *this = cleanMac();               }

public:
  bool isClean()        { return *this == cleanMac();       }
  bool isBroadcast()    { return *this == broadcastMac();   } // FF:FF:FF:FF:FF:FF
  bool isMulticast()    { return value[0] == 0x01 && value[1] == 0x00 && value[2] == 0x5E && (value[3] & 0x80) == 0x00; } // 01:00:5e:0*

  static Mac  randomMac();
  static Mac& cleanMac();
  static Mac& broadcastMac();
};
#pragma pack(pop)

// ----------------------------------------------------------------------------
// Link Layer Type
// ----------------------------------------------------------------------------
#ifndef DLT_NULL
#define DLT_NULL                       0
#endif
#ifndef DLT_EN10MB
#define DLT_EN10MB                     1
#endif
#ifndef DLT_AX25
#define DLT_AX25                       3
#endif
#ifndef DLT_IEEE802
#define DLT_IEEE802                    6
#endif
#ifndef DLT_ARCNET
#define DLT_ARCNET                     7
#endif
#ifndef DLT_SLIP
#define DLT_SLIP                       8
#endif
#ifndef DLT_PPP
#define DLT_PPP                        9
#endif
#ifndef DLT_FDDI
#define DLT_FDDI                       10
#endif
#ifndef DLT_PPP_SERIAL
#define DLT_PPP_SERIAL                 50
#endif
#ifndef DLT_PPP_ETHER
#define DLT_PPP_ETHER                  51
#endif
#ifndef DLT_ATM_RFC1483
#define DLT_ATM_RFC1483                100
#endif
#ifndef DLT_RAW
#define DLT_RAW                        101
#endif
#ifndef DLT_C_HDLC
#define DLT_C_HDLC                     104
#endif
#ifndef DLT_IEEE802_11
#define DLT_IEEE802_11                 105
#endif
#ifndef DLT_FRELAY
#define DLT_FRELAY                     107
#endif
#ifndef DLT_LOOP
#define DLT_LOOP                       108
#endif
#ifndef DLT_LINUX_SLL
#define DLT_LINUX_SLL                  113
#endif
#ifndef DLT_LTALK
#define DLT_LTALK                      114
#endif
#ifndef DLT_PFLOG
#define DLT_PFLOG                      117
#endif
#ifndef DLT_PRISM_HEADER
#define DLT_PRISM_HEADER               119
#endif
#ifndef DLT_IP_OVER_FC
#define DLT_IP_OVER_FC                 122
#endif
#ifndef DLT_SUNATM
#define DLT_SUNATM                     123
#endif
#ifndef DLT_IEEE802_11_RADIO
#define DLT_IEEE802_11_RADIO           127
#endif
#ifndef DLT_ARCNET_LINUX
#define DLT_ARCNET_LINUX               129
#endif
#ifndef DLT_APPLE_IP_OVER_IEEE1394
#define DLT_APPLE_IP_OVER_IEEE1394     138
#endif
#ifndef DLT_MTP2_WITH_PHDR
#define DLT_MTP2_WITH_PHDR             139
#endif
#ifndef DLT_MTP2
#define DLT_MTP2                       140
#endif
#ifndef DLT_MTP3
#define DLT_MTP3                       141
#endif
#ifndef DLT_SCCP
#define DLT_SCCP                       142
#endif
#ifndef DLT_DOCSIS
#define DLT_DOCSIS                     143
#endif
#ifndef DLT_LINUX_IRDA
#define DLT_LINUX_IRDA                 144
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER0
#define LINKTYPE_USER0_LINKTYPE_USER0  147
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER1
#define LINKTYPE_USER0_LINKTYPE_USER1  148
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER2
#define LINKTYPE_USER0_LINKTYPE_USER2  149
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER3
#define LINKTYPE_USER0_LINKTYPE_USER3  150
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER4
#define LINKTYPE_USER0_LINKTYPE_USER4  151
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER5
#define LINKTYPE_USER0_LINKTYPE_USER5  152
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER6
#define LINKTYPE_USER0_LINKTYPE_USER6  153
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER7
#define LINKTYPE_USER0_LINKTYPE_USER7  154
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER8
#define LINKTYPE_USER0_LINKTYPE_USER8  155
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER9
#define LINKTYPE_USER0_LINKTYPE_USER9  156
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER10
#define LINKTYPE_USER0_LINKTYPE_USER10 157
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER11
#define LINKTYPE_USER0_LINKTYPE_USER11 158
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER12
#define LINKTYPE_USER0_LINKTYPE_USER12 159
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER13
#define LINKTYPE_USER0_LINKTYPE_USER13 160
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER14
#define LINKTYPE_USER0_LINKTYPE_USER14 161
#endif
#ifndef LINKTYPE_USER0_LINKTYPE_USER15
#define LINKTYPE_USER0_LINKTYPE_USER15 162
#endif
#ifndef DLT_IEEE802_11_RADIO_AVS
#define DLT_IEEE802_11_RADIO_AVS       163
#endif
#ifndef DLT_BACNET_MS_TP
#define DLT_BACNET_MS_TP               165
#endif
#ifndef DLT_PPP_PPPD
#define DLT_PPP_PPPD                   166
#endif
#ifndef DLT_GPRS_LLC
#define DLT_GPRS_LLC                   169
#endif
#ifndef DLT_LINUX_LAPD
#define DLT_LINUX_LAPD                 177
#endif
#ifndef DLT_BLUETOOTH_HCI_H4
#define DLT_BLUETOOTH_HCI_H4           187
#endif
#ifndef DLT_USB_LINUX
#define DLT_USB_LINUX                  189
#endif
#ifndef DLT_PPI
#define DLT_PPI                        192
#endif
#ifndef DLT_IEEE802_15_4
#define DLT_IEEE802_15_4               195
#endif
#ifndef DLT_SITA
#define DLT_SITA                       196
#endif
#ifndef DLT_ERF
#define DLT_ERF                        197
#endif
#ifndef DLT_BLUETOOTH_HCI_H4_WITH_PHDR
#define DLT_BLUETOOTH_HCI_H4_WITH_PHDR 201
#endif
#ifndef DLT_AX25_KISS
#define DLT_AX25_KISS                  202
#endif
#ifndef DLT_LAPD
#define DLT_LAPD                       203
#endif
#ifndef DLT_PPP_WITH_DIR
#define DLT_PPP_WITH_DIR               204
#endif
#ifndef DLT_C_HDLC_WITH_DIR
#define DLT_C_HDLC_WITH_DIR            205
#endif
#ifndef DLT_FRELAY_WITH_DIR
#define DLT_FRELAY_WITH_DIR            206
#endif
#ifndef DLT_IPMB_LINUX
#define DLT_IPMB_LINUX                 209
#endif
#ifndef DLT_IEEE802_15_4_NONASK_PHY
#define DLT_IEEE802_15_4_NONASK_PHY    215
#endif
#ifndef DLT_USB_LINUX_MMAPPED
#define DLT_USB_LINUX_MMAPPED          220
#endif
#ifndef DLT_FC_2
#define DLT_FC_2                       224
#endif
#ifndef DLT_FC_2_WITH_FRAME_DELIMS
#define DLT_FC_2_WITH_FRAME_DELIMS     225
#endif
#ifndef DLT_IPNET
#define DLT_IPNET                      226
#endif
#ifndef DLT_CAN_SOCKETCAN
#define DLT_CAN_SOCKETCAN              227
#endif
#ifndef DLT_IPV4
#define DLT_IPV4                       228
#endif
#ifndef DLT_IPV6
#define DLT_IPV6                       229
#endif
#ifndef DLT_IEEE802_15_4_NOFCS
#define DLT_IEEE802_15_4_NOFCS         230
#endif
#ifndef DLT_DBUS
#define DLT_DBUS                       231
#endif
#ifndef DLT_DVB_CI
#define DLT_DVB_CI                     235
#endif
#ifndef DLT_MUX27010
#define DLT_MUX27010                   236
#endif
#ifndef DLT_STANAG_5066_D_PDU
#define DLT_STANAG_5066_D_PDU          237
#endif
#ifndef DLT_NFLOG
#define DLT_NFLOG                      239
#endif
#ifndef DLT_NETANALYZER
#define DLT_NETANALYZER                240
#endif
#ifndef DLT_NETANALYZER_TRANSPARENT
#define DLT_NETANALYZER_TRANSPARENT    241
#endif
#ifndef DLT_IPOIB
#define DLT_IPOIB                      242
#endif
#ifndef DLT_MPEG_2_TS
#define DLT_MPEG_2_TS                  243
#endif
#ifndef DLT_NG40
#define DLT_NG40                       244
#endif
#ifndef DLT_NFC_LLCP
#define DLT_NFC_LLCP                   245
#endif

// ----------------------------------------------------------------------------
// IP
// ----------------------------------------------------------------------------
// IP is already declared in VNet

// ----------------------------------------------------------------------------
// Header
// ----------------------------------------------------------------------------
#pragma pack(push, 1)

// ----------------------------------------------------------------------------
// ETH_HDR
// ----------------------------------------------------------------------------
typedef struct // libnet_ethernet_hdr
{
    Mac ether_dhost;      /* destination ethernet address */
    Mac ether_shost;      /* source ethernet address */
    u_int16_t ether_type; /* protocol */
} ETH_HDR;

// ----------------------------------------------------------------------------
// FDDI_HDR
// ----------------------------------------------------------------------------
typedef struct libnet_fddi_hdr FDDI_HDR;

// ----------------------------------------------------------------------------
// IP_HDR
// ----------------------------------------------------------------------------
typedef struct IP_HDR // libnet_ipv4_hdr
{
#if (LIBNET_LIL_ENDIAN)
    u_int8_t ip_hl:4,      /* header length */
           ip_v:4;         /* version */
#endif
#if (LIBNET_BIG_ENDIAN)
    u_int8_t ip_v:4,       /* version */
           ip_hl:4;        /* header length */
#endif
    u_int8_t ip_tos;       /* type of service */
#ifndef IPTOS_LOWDELAY
#define IPTOS_LOWDELAY      0x10
#endif
#ifndef IPTOS_THROUGHPUT
#define IPTOS_THROUGHPUT    0x08
#endif
#ifndef IPTOS_RELIABILITY
#define IPTOS_RELIABILITY   0x04
#endif
#ifndef IPTOS_LOWCOST
#define IPTOS_LOWCOST       0x02
#endif
    u_int16_t ip_len;         /* total length */
    u_int16_t ip_id;          /* identification */
    u_int16_t ip_off;
#ifndef IP_RF
#define IP_RF 0x8000          /* reserved fragment flag */
#endif
#ifndef IP_DF
#define IP_DF 0x4000          /* dont fragment flag */
#endif
#ifndef IP_MF
#define IP_MF 0x2000          /* more fragments flag */
#endif 
#ifndef IP_OFFMASK
#define IP_OFFMASK 0x1fff     /* mask for fragmenting bits */
#endif
    u_int8_t ip_ttl;          /* time to live */
    u_int8_t ip_p;            /* protocol */
    u_int16_t ip_sum;         /* checksum */
    u_int32_t ip_src, ip_dst;  /* source and dest address */
} IP_HDR;

// ----------------------------------------------------------------------------
// ARP_HDR
// ----------------------------------------------------------------------------
typedef struct libnet_arp_hdr _ARP_HDR;
typedef struct ARP_HDR : _ARP_HDR
{
  Mac ar_sa;
  Ip  ar_si;
  Mac ar_ta;
  Ip  ar_ti;
} ARP_HDR;

// ----------------------------------------------------------------------------
// TCP_HDR
// ----------------------------------------------------------------------------
typedef struct libnet_tcp_hdr TCP_HDR;

// ----------------------------------------------------------------------------
// UDP_HDR
// ----------------------------------------------------------------------------
typedef struct libnet_udp_hdr UDP_HDR;

// ----------------------------------------------------------------------------
// ICMP_HDR
// ----------------------------------------------------------------------------
typedef struct libnet_icmpv4_hdr ICMP_HDR;

// ----------------------------------------------------------------------------
// DNS_HDR
// ----------------------------------------------------------------------------
typedef struct DNS_HDR
{
    u_int16_t id;             /* DNS packet ID */
    u_int16_t flags;          /* DNS flags */
    u_int16_t num_q;          /* Number of questions */
    u_int16_t num_answ_rr;    /* Number of answer resource records */
    u_int16_t num_auth_rr;    /* Number of authority resource records */
    u_int16_t num_addi_rr;    /* Number of additional resource records */
} DNS_HDR;

#pragma pack (pop)

#endif // __SNOOP_TYPE_H__
