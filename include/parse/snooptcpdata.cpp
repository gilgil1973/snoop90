#include <SnoopTcpData>

#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopTcpData
// ----------------------------------------------------------------------------
bool SnoopTcpData::parse(SnoopPacket* packet)
{
  if (!SnoopTcp::isData(packet->ipHdr, packet->tcpHdr, &packet->data, &packet->dataLen)) return false;
  return true;
}

bool SnoopTcpData::parseAll(SnoopPacket* packet)
{
  if (!SnoopTcp::parseAll(packet)) return false;
  return parse(packet);
}
