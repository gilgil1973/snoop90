#include <SnoopTcpData>

#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopTcpData
// ----------------------------------------------------------------------------
bool SnoopTcpData::parseAll(SnoopPacket* packet)
{
  if (!SnoopTcp::parseAll(packet)) return false;

  if (!SnoopTcp::isData(packet->ipHdr, packet->tcpHdr, &packet->data, &packet->dataLen)) return false;

  return true;
}
