// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_TYPE_KEY_H__
#define __SNOOP_TYPE_KEY_H__

#include <SnoopType>

// ----------------------------------------------------------------------------
// SnoopMacKey
// ----------------------------------------------------------------------------
class SnoopMacKey
{
public:
  Mac mac;

  bool operator < (const SnoopMacKey& rhs) const;
};

// ----------------------------------------------------------------------------
// SnoopMacFlowKey
// ----------------------------------------------------------------------------
class SnoopMacFlowKey
{
public:
  Mac src_mac;
  Mac dst_mac;

  bool operator < (const SnoopMacFlowKey& rhs) const;
};

// ----------------------------------------------------------------------------
// SnoopMacSessionKey
// ----------------------------------------------------------------------------
class SnoopMacSessionKey
{
public:
  Mac mac_1;
  Mac mac_2;

  bool operator < (const SnoopMacSessionKey& rhs) const;
};

// ----------------------------------------------------------------------------
// SnoopIpKey
// ----------------------------------------------------------------------------
class SnoopIpKey
{
public:
  Ip ip;

  bool operator < (const SnoopIpKey& rhs) const;
};

// ----------------------------------------------------------------------------
// SnoopIpFlowKey
// ----------------------------------------------------------------------------
class SnoopIpFlowKey
{
public:
  Ip src_ip;
  Ip dst_ip;

  bool operator < (const SnoopIpFlowKey& rhs) const;
};

// ----------------------------------------------------------------------------
// SnoopIpSessionKey
// ----------------------------------------------------------------------------
class SnoopIpSessionKey
{
public:
  Ip ip_1;
  Ip ip_2;

  bool operator < (const SnoopIpSessionKey& rhs) const;
};

// ----------------------------------------------------------------------------
// SnoopPortKey
// ----------------------------------------------------------------------------
class SnoopPortKey
{
public:
  int port;

  bool operator < (const SnoopPortKey& rhs) const;
};

// ----------------------------------------------------------------------------
// SnoopPortFlowKey
// ----------------------------------------------------------------------------
class SnoopPortFlowKey
{
public:
  int src_port;
  int dst_port;

  bool operator < (const SnoopPortFlowKey& rhs) const;
};

// ----------------------------------------------------------------------------
// SnoopPortSessionKey
// ----------------------------------------------------------------------------
class SnoopPortSessionKey
{
public:
  int port_1;
  int port_2;

  bool operator < (const SnoopPortSessionKey& rhs) const;
};

// ----------------------------------------------------------------------------
// SnoopTransportKey
// ----------------------------------------------------------------------------
class SnoopTransportKey
{
public:
  Ip  ip;
  int port;

  bool operator < (const SnoopTransportKey& rhs) const;
};

typedef SnoopTransportKey SnoopTcpKey;
typedef SnoopTransportKey SnoopUdpKey;

// ----------------------------------------------------------------------------
// SnoopTransportFlowKey
// ----------------------------------------------------------------------------
class SnoopTransportFlowKey
{
public:
  Ip  src_ip;
  int src_port;
  Ip  dst_ip;
  int dst_port;

  bool operator < (const SnoopTransportFlowKey& rhs) const;
};

typedef SnoopTransportFlowKey SnoopTcpFlowKey;
typedef SnoopTransportFlowKey SnoopUdpFlowKey;

// ----------------------------------------------------------------------------
// SnoopTransportSessionKey
// ----------------------------------------------------------------------------
class SnoopTransportSessionKey
{
public:
  Ip  ip_1;
  int port_1;
  Ip  ip_2;
  int port_2;

  bool operator < (const SnoopTransportSessionKey& rhs) const;
};

typedef SnoopTransportSessionKey SnoopTcpSessionKey;
typedef SnoopTransportSessionKey SnoopUdpSessionKey;

// ----------------------------------------------------------------------------
// Snoop5TupleKey
// ----------------------------------------------------------------------------
class Snoop5TupleKey
{
public:
  UINT8                 proto;
  SnoopTransportFlowKey flow;

  bool operator < (const Snoop5TupleKey& rhs) const;
};

#endif // __SNOOP_TYPE_KEY_H__
