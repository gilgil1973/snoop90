// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_DNS_H__
#define __SNOOP_DNS_H__

#include <SnoopUdp>

// ----------------------------------------------------------------------------
// SnoopDnsQuestion
// ----------------------------------------------------------------------------
class SnoopDnsQuestion
{
public:
  QString name;
  UINT16  type;
  UINT16  _class;

public:
  QByteArray encode();
  bool       decode(BYTE* udpData, int dataLen, int* offset);
};

class SnoopDnsQuestions : public QList<SnoopDnsQuestion>
{
public:
  QByteArray encode();
  bool       decode(BYTE* udpData, int dataLen, int count, int* offset);
};

// ----------------------------------------------------------------------------
// SnoopDnsResourceRecord
// ----------------------------------------------------------------------------
class SnoopDnsResourceRecord
{
public:
  QString    name;
  UINT16     type;
  UINT16     _class;
  UINT32     ttl;
  UINT16     dataLength;
  QByteArray data;

public:
  QByteArray encode();
  bool       decode(BYTE* udpData, int dataLen, int* offset);
};

// ----------------------------------------------------------------------------
// SnoopDnsResourceRecords
// ----------------------------------------------------------------------------
class SnoopDnsResourceRecords : public QList<SnoopDnsResourceRecord>
{
public:
  QByteArray encode();
  bool       decode(BYTE* udpData, int dataLen, int count, int* offset);
};

// ----------------------------------------------------------------------------
// SnoopDns
// ----------------------------------------------------------------------------
class SnoopDns
{
public:
  DNS_HDR dnsHdr;
  SnoopDnsQuestions questions;
  SnoopDnsResourceRecords answers;
  SnoopDnsResourceRecords authorities;
  SnoopDnsResourceRecords additionals;

public:
  QByteArray encode();
  bool       decode(BYTE* udpData, int dataLen, int* offset);

public:
  static QByteArray encodeName(QString name);
  static QString    decodeName(BYTE* udpData, int dataLen, int* offset);
};

#endif // __SNOOP_DNS_H__
