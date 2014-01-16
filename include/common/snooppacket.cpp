#include <SnoopPacket>

// ----------------------------------------------------------------------------
// SnoopPacket
// ----------------------------------------------------------------------------
void SnoopPacket::clear()
{
  memset(this, 0, sizeof(SnoopPacket));
}
