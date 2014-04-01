#include <SnoopWriteWinDivert>

REGISTER_METACLASS(SnoopWriteWinDivert, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopWriteWinDivert
// ----------------------------------------------------------------------------
SnoopWriteWinDivert::SnoopWriteWinDivert(void* owner) : SnoopWinDivert(owner)
{
  autoRead = false;
}

SnoopWriteWinDivert::~SnoopWriteWinDivert()
{
  close();
}

void SnoopWriteWinDivert::output(SnoopPacket* packet)
{
  SnoopWinDivert::write(packet);
  emit captured(packet);
}
