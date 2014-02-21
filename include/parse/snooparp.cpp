#include <SnoopArp>

#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopArp
// ----------------------------------------------------------------------------
bool SnoopArp::parse(SnoopPacket* packet)
{
  if (!SnoopEth::isArp(packet->ethHdr, &packet->arpHdr)) return false;
  packet->netType = ETHERTYPE_ARP;
  return true;
}

bool SnoopArp::parseAll(SnoopPacket* packet)
{
  if (!SnoopEth::parseAll(packet)) return false;
  return parse(packet);
}
