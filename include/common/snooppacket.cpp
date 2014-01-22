#include <SnoopPacket>

// ----------------------------------------------------------------------------
// SnoopPacket
// ----------------------------------------------------------------------------
void SnoopPacket::clear()
{
  memset(this, 0, sizeof(SnoopPacket));
}

int SnoopPacket::write(QByteArray& ba)
{
  int capLen = (int)pktHdr->caplen;
  ba.resize(capLen);
  memcpy(ba.data(), pktData, (size_t)capLen);
  return capLen;
}
