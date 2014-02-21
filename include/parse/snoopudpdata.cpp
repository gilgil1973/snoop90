#include <SnoopUdpData>

#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopUdpData
// ----------------------------------------------------------------------------
bool SnoopUdpData::parse(SnoopPacket* packet)
{
  if (!SnoopUdp::isData(packet->ipHdr, packet->udpHdr, &packet->data, &packet->dataLen)) return false;
  return true;
}

bool SnoopUdpData::parseAll(SnoopPacket* packet)
{
  if (!SnoopUdp::parseAll(packet)) return false;
  return parse(packet);
}
