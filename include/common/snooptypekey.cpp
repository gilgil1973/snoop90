#include <SnoopTypeKey>

#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopMacKey
// ----------------------------------------------------------------------------
bool SnoopMacKey::operator < (const SnoopMacKey& rhs) const
{
  return (mac < rhs.mac);
}

// ----------------------------------------------------------------------------
// SnoopMacFlowKey
// ----------------------------------------------------------------------------
bool SnoopMacFlowKey::operator < (const SnoopMacFlowKey& rhs) const
{
  if (this->srcMac < rhs.srcMac) return true;
  if (this->srcMac > rhs.srcMac) return false;
  if (this->dstMac < rhs.dstMac) return true;
  return false;
}

SnoopMacFlowKey SnoopMacFlowKey::reverse()
{
  SnoopMacFlowKey res;
  res.srcMac = this->dstMac;
  res.dstMac = this->srcMac;
  return res;
}

// ----------------------------------------------------------------------------
// SnoopMacSessionKey
// ----------------------------------------------------------------------------
bool SnoopMacSessionKey::operator < (const SnoopMacSessionKey& rhs) const
{
  if (this->mac1 < rhs.mac1) return true;
  if (this->mac1 > rhs.mac1) return false;
  if (this->mac2 < rhs.mac2) return true;
  return false;
}

// ----------------------------------------------------------------------------
// SnoopIpKey
// ----------------------------------------------------------------------------
bool SnoopIpKey::operator < (const SnoopIpKey& rhs) const
{
  return (this->ip < rhs.ip);
}

// ----------------------------------------------------------------------------
// SnoopIpFlowKey
// ----------------------------------------------------------------------------
bool SnoopIpFlowKey::operator < (const SnoopIpFlowKey& rhs) const
{
  if (this->srcIp < rhs.srcIp) return true;
  if (this->srcIp > rhs.srcIp) return false;
  if (this->dstIp < rhs.dstIp) return true;
  return false;
}

SnoopIpFlowKey SnoopIpFlowKey::reverse()
{
  SnoopIpFlowKey res;
  res.srcIp = this->dstIp;
  res.dstIp = this->srcIp;
  return res;
}

// ----------------------------------------------------------------------------
// SnoopIpSessionKey
// ----------------------------------------------------------------------------
bool SnoopIpSessionKey::operator < (const SnoopIpSessionKey& rhs) const
{
  if (this->ip1 < rhs.ip1) return true;
  if (this->ip1 > rhs.ip1) return false;
  if (this->ip2 < rhs.ip2) return true;
  return false;
}

// ----------------------------------------------------------------------------
// SnoopPortKey
// ----------------------------------------------------------------------------
bool SnoopPortKey::operator < (const SnoopPortKey& rhs) const
{
  return (this->port < rhs.port);
}

// ----------------------------------------------------------------------------
// SnoopPortFlowKey
// ----------------------------------------------------------------------------
bool SnoopPortFlowKey::operator < (const SnoopPortFlowKey& rhs) const
{
  if (this->srcPort < rhs.srcPort) return true;
  if (this->srcPort > rhs.srcPort) return false;
  if (this->dstPort < rhs.dstPort) return true;
  return false;
}

SnoopPortFlowKey SnoopPortFlowKey::reverse()
{
  SnoopPortFlowKey res;
  res.srcPort = this->dstPort;
  res.dstPort = this->srcPort;
  return res;
}

// ----------------------------------------------------------------------------
// SnoopPortSessionKey
// ----------------------------------------------------------------------------
bool SnoopPortSessionKey::operator < (const SnoopPortSessionKey& rhs) const
{
  if (this->port1 < rhs.port1) return true;
  if (this->port1 > rhs.port1) return false;
  if (this->port2 < rhs.port2) return true;
  return false;
}

// ----------------------------------------------------------------------------
// SnoopTransportKey
// ----------------------------------------------------------------------------
bool SnoopTransportKey::operator < (const SnoopTransportKey& rhs) const
{
  if (this->ip   < rhs.ip) return true;
  if (this->ip   > rhs.ip) return false;
  if (this->port < rhs.port) return true;
  return false;
}

// ----------------------------------------------------------------------------
// SnoopTransportFlowKey
// ----------------------------------------------------------------------------
bool SnoopTransportFlowKey::operator < (const SnoopTransportFlowKey& rhs) const
{
  if (this->srcIp   < rhs.srcIp)   return true;
  if (this->srcIp   > rhs.srcIp)   return false;
  if (this->srcPort < rhs.srcPort) return true;
  if (this->srcPort > rhs.srcPort) return false;
  if (this->dstIp   < rhs.dstIp)   return true;
  if (this->dstIp   > rhs.dstIp)   return false;
  if (this->dstPort < rhs.dstPort) return true;
  return false;
}

bool SnoopTransportFlowKey::operator == (const SnoopTransportFlowKey& rhs) const
{
  if (this->srcIp   != rhs.srcIp)   return false;
  if (this->srcIp   != rhs.srcIp)   return false;
  if (this->srcPort != rhs.srcPort) return false;
  if (this->srcPort != rhs.srcPort) return false;
  if (this->dstIp   != rhs.dstIp)   return false;
  if (this->dstIp   != rhs.dstIp)   return false;
  if (this->dstPort != rhs.dstPort) return false;
  return true;
}

SnoopTransportFlowKey SnoopTransportFlowKey::reverse()
{
  SnoopTransportFlowKey res;
  res.srcIp   = this->dstIp;
  res.srcPort = this->dstPort;
  res.dstIp   = this->srcIp;
  res.dstPort = this->srcPort;
  return res;
}

// ----------------------------------------------------------------------------
// SnoopTransportSessionKey
// ----------------------------------------------------------------------------
bool SnoopTransportSessionKey::operator < (const SnoopTransportSessionKey& rhs) const
{
  if (this->ip1   < rhs.ip1)   return true;
  if (this->ip1   > rhs.ip1)   return false;
  if (this->port1 < rhs.port1) return true;
  if (this->port1 > rhs.port1) return false;
  if (this->ip2   < rhs.ip2)   return true;
  if (this->ip2   > rhs.ip2)   return false;
  if (this->port2 < rhs.port2) return true;
  return false;
}

// ----------------------------------------------------------------------------
// SnoopTupleFlowKey
// ----------------------------------------------------------------------------
bool SnoopTupleFlowKey::operator < (const SnoopTupleFlowKey& rhs) const
{
  if (this->proto < rhs.proto) return true;
  if (this->proto > rhs.proto) return false;
  return this->flow < rhs.flow;
}

SnoopTupleFlowKey SnoopTupleFlowKey::reverse()
{
  SnoopTupleFlowKey res;
  res.proto = this->proto;
  res.flow  = this->flow.reverse();
  return res;
}
