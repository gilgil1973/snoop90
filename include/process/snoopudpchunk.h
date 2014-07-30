// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_UDP_CHUNK_H__
#define __SNOOP_UDP_CHUNK_H__

#include <VCommon>

// ----------------------------------------------------------------------------
// SnoopUdpChunkInfo
// ----------------------------------------------------------------------------
class SnoopUdpChunkInfo
{
public:
  static const int DSCR_SIZE = 4;

public:
  QByteArray dscr;
  quint16    id;
  quint16    len;
};

// ----------------------------------------------------------------------------
// SnoopUdpChunkPayload
// ----------------------------------------------------------------------------
class SnoopUdpChunkPayload
{
public:
  QByteArray header;
  QByteArray body;
};

// ----------------------------------------------------------------------------
// SnoopUdpChunk
// ----------------------------------------------------------------------------
class SnoopUdpChunk
{
public:
  SnoopUdpChunkInfo    info;
  SnoopUdpChunkPayload payload;

public:
  int encode(QByteArray& ba); // into ba
  int decode(int headerSize, QByteArray& ba); // from ba
};

#endif // __SNOOP_UDP_CHUNK_H__
