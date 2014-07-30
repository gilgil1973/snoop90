#include <SnoopUdpChunk>

// ----------------------------------------------------------------------------
// SnoopUdpChunk
// ----------------------------------------------------------------------------
int SnoopUdpChunk::encode(QByteArray& ba)
{
  int size = ba.size();
  quint16 temp;

  ba.append(payload.header);
  ba.append(info.dscr);
  temp = htons(info.id);  ba.append((const char*)&temp, sizeof(info.id));
  temp = htons(info.len); ba.append((const char*)&temp, sizeof(info.len));
  ba.append(payload.body);

  return ba.size() - size;
}

int SnoopUdpChunk::decode(int headerSize, QByteArray& ba)
{
  int size = ba.size();
  if (size < headerSize + SnoopUdpChunkInfo::INFO_SIZE) return -1;

  payload.header = ba.left(headerSize);                   ba.remove(0, headerSize);
  info.dscr      = ba.left(SnoopUdpChunkInfo::DSCR_SIZE); ba.remove(0, SnoopUdpChunkInfo::DSCR_SIZE);
  info.id        = ntohs(*((quint16*)ba.data()));         ba.remove(0, sizeof(info.id));
  info.len       = ntohs(*((quint16*)ba.data()));         ba.remove(0, sizeof(info.len));
  payload.body   = ba.left(info.len);                     ba.remove(0, info.len);

  return size - ba.size();
}
