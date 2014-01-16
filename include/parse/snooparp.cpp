#include <SnoopArp>

#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopArp
// ----------------------------------------------------------------------------
bool SnoopArp::parseAll(SnoopPacket* packet)
{
  if (!SnoopEth::parseAll(packet)) return false;

  if (!SnoopEth::isArp(packet->ethHdr, &packet->arpHdr)) return false;
  packet->netType = ETHERTYPE_ARP;

  return true;
}
