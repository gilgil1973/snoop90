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
  if (this->src_mac < rhs.src_mac) return true;
  if (this->src_mac > rhs.src_mac) return false;
  if (this->dst_mac < rhs.dst_mac) return true;
  return false;
}

// ----------------------------------------------------------------------------
// SnoopMacSessionKey
// ----------------------------------------------------------------------------
bool SnoopMacSessionKey::operator < (const SnoopMacSessionKey& rhs) const
{
  if (this->mac_1 < rhs.mac_1) return true;
  if (this->mac_1 > rhs.mac_1) return false;
  if (this->mac_2 < rhs.mac_2) return true;
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
  if (this->src_ip < rhs.src_ip) return true;
  if (this->src_ip > rhs.src_ip) return false;
  if (this->dst_ip < rhs.dst_ip) return true;
  return false;
}

// ----------------------------------------------------------------------------
// SnoopIpSessionKey
// ----------------------------------------------------------------------------
bool SnoopIpSessionKey::operator < (const SnoopIpSessionKey& rhs) const
{
  if (this->ip_1 < rhs.ip_1) return true;
  if (this->ip_1 > rhs.ip_1) return false;
  if (this->ip_2 < rhs.ip_2) return true;
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
  if (this->src_port < rhs.src_port) return true;
  if (this->src_port > rhs.src_port) return false;
  if (this->dst_port < rhs.dst_port) return true;
  return false;
}

// ----------------------------------------------------------------------------
// SnoopPortSessionKey
// ----------------------------------------------------------------------------
bool SnoopPortSessionKey::operator < (const SnoopPortSessionKey& rhs) const
{
  if (this->port_1 < rhs.port_1) return true;
  if (this->port_1 > rhs.port_1) return false;
  if (this->port_2 < rhs.port_2) return true;
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
  if (this->src_ip   < rhs.src_ip)   return true;
  if (this->src_ip   > rhs.src_ip)   return false;
  if (this->src_port < rhs.src_port) return true;
  if (this->src_port > rhs.src_port) return false;
  if (this->dst_ip   < rhs.dst_ip)   return true;
  if (this->dst_ip   > rhs.dst_ip)   return false;
  if (this->dst_port < rhs.dst_port) return true;
  return false;
}

// ----------------------------------------------------------------------------
// SnoopTransportSessionKey
// ----------------------------------------------------------------------------
bool SnoopTransportSessionKey::operator < (const SnoopTransportSessionKey& rhs) const
{
  if (this->ip_1   < rhs.ip_1)   return true;
  if (this->ip_1   > rhs.ip_1)   return false;
  if (this->port_1 < rhs.port_1) return true;
  if (this->port_1 > rhs.port_1) return false;
  if (this->ip_2   < rhs.ip_2)   return true;
  if (this->ip_2   > rhs.ip_2)   return false;
  if (this->port_2 < rhs.port_2) return true;
  return false;
}

// ----------------------------------------------------------------------------
// Snoop5TupleKey
// ----------------------------------------------------------------------------
bool Snoop5TupleKey::operator < (const Snoop5TupleKey& rhs) const
{
  if (this->proto < rhs.proto) return true;
  if (this->proto > rhs.proto) return false;
  return this->flow < rhs.flow;
}
