#include <SnoopUdpData>

#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopUdpData
// ----------------------------------------------------------------------------
bool SnoopUdpData::parseAll(SnoopPacket* packet)
{
  if (!SnoopUdp::parseAll(packet)) return false;

  if (!SnoopUdp::isData(packet->ipHdr, packet->udpHdr, &packet->data, &packet->dataLen)) return false;

  return true;
}
