#include <SnoopWriteWinDivert>

REGISTER_METACLASS(SnoopWriteWinDivert, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopWriteWinDivert
// ----------------------------------------------------------------------------
SnoopWriteWinDivert::SnoopWriteWinDivert(void* owner) : SnoopWinDivert(owner)
{
  autoRead = false;
  flags    = WINDIVERT_FLAG_SNIFF;
}

SnoopWriteWinDivert::~SnoopWriteWinDivert()
{
  close();
}

void SnoopWriteWinDivert::copy(SnoopPacket* packet)
{
  SnoopWinDivert::write(packet->pktData, packet->pktHdr->caplen);
  emit copied(packet);
}

void SnoopWriteWinDivert::move(SnoopPacket* packet)
{
  SnoopWinDivert::write(packet->pktData, packet->pktHdr->caplen);
  packet->drop = true;
  emit moved(packet);
}
