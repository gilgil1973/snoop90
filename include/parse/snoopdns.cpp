#include <SnoopDns>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopDnsQuestion
// ----------------------------------------------------------------------------
QByteArray SnoopDnsQuestion::encode()
{
  QByteArray res;

  res = SnoopDns::encodeName(this->name);

  UINT16 _type = htons(this->type);
  res.append((const char*)&_type, sizeof(UINT16));

  UINT16 __class = htons(this->_class);
  res.append((const char*)&__class, sizeof(UINT16));

  return res;
}

bool SnoopDnsQuestion::decode(BYTE* udpData, int dataLen, int* offset)
{
  this->name = SnoopDns::decodeName(udpData, dataLen, offset);
  if (this->name == "") return false;

  if (*offset + sizeof(UINT16) > dataLen) return false;
  UINT16* _type = (UINT16*)(udpData + *offset);
  this->type = ntohs(*_type);
  *offset += sizeof(UINT16);

  if (*offset + sizeof(UINT16) > dataLen) return false;
  UINT16* __class = (UINT16*)(udpData + *offset);
  this->_class = ntohs(*__class);
  *offset += sizeof(UINT16);

  if (*offset > dataLen) return false;
  return true;
}

// ----------------------------------------------------------------------------
// SnoopDnsQuestions
// ----------------------------------------------------------------------------
QByteArray SnoopDnsQuestions::encode()
{
  QByteArray res;
  foreach (SnoopDnsQuestion question, *this)
  {
    res += question.encode();
  }
  return res;
}

bool SnoopDnsQuestions::decode(BYTE* udpData, int dataLen, int count, int* offset)
{
  for (int i = 0; i < count; i++)
  {
    SnoopDnsQuestion question;
    if (!question.decode(udpData, dataLen, offset)) return false;
    push_back(question);
  }
  return true;
}

// ----------------------------------------------------------------------------
// SnoopDnsResourceRecord
// ----------------------------------------------------------------------------
QByteArray SnoopDnsResourceRecord::encode()
{
  QByteArray res;

  //res = SnoopDns::encodeName(this->name);
  res.append((char)0xC0); // gilgil temp 2014.03.22
  res.append((char)0x0C);


  UINT16 _type = htons(this->type);
  res.append((const char*)&_type, sizeof(UINT16));

  UINT16 __class = htons(this->_class);
  res.append((const char*)&__class, sizeof(UINT16));

  UINT32 _ttl = htonl(this->ttl);
  res.append((const char*)&_ttl, sizeof(UINT32));

  UINT16 _dataLength = htons(this->dataLength);
  res.append((const char*)&_dataLength, sizeof(UINT16));

  res += data;

  return res;
}

bool SnoopDnsResourceRecord::decode(BYTE* udpData, int dataLen, int* offset)
{
  this->name = SnoopDns::decodeName(udpData, dataLen, offset);
  if (this->name == "") return false;

  if (*offset + sizeof(UINT16) > dataLen) return false;
  UINT16* _type = (UINT16*)(udpData + *offset);
  this->type = ntohs(*_type);
  *offset += sizeof(UINT16);

  if (*offset  + sizeof(UINT16) > dataLen) return false;
  UINT16* __class = (UINT16*)(udpData + *offset);
  this->_class = ntohs(*__class);
  *offset += sizeof(UINT16);

  if (*offset  + sizeof(UINT32) > dataLen) return false;
  UINT32* _ttl = (UINT32*)(udpData + *offset);
  this->ttl = ntohl(*_ttl);
  *offset += sizeof(UINT32);

  if (*offset  + sizeof(UINT16) > dataLen) return false;
  UINT16* _dataLength = (UINT16*)(udpData + *offset);
  this->dataLength= ntohs(*_dataLength);
  *offset += sizeof(UINT16);

  if (*offset + this->dataLength > dataLen) return false;
  const char* data = (const char*)(udpData + *offset);
  this->data = QByteArray::fromRawData(data, this->dataLength);
  *offset += this->dataLength;

  return true;
}

// ----------------------------------------------------------------------------
// SnoopDnsResourceRecords
// ----------------------------------------------------------------------------
QByteArray SnoopDnsResourceRecords::encode()
{
  QByteArray res;
  foreach (SnoopDnsResourceRecord record, *this)
  {
    res += record.encode();
  }
  return res;
}

bool SnoopDnsResourceRecords::decode(BYTE* udpData, int dataLen, int count, int* offset)
{
  for (int i = 0; i < count; i++)
  {
    SnoopDnsResourceRecord record;
    if (!record.decode(udpData, dataLen, offset)) return false;
    push_back(record);
  }
  return true;
}

// ----------------------------------------------------------------------------
// SnoopDns
// ----------------------------------------------------------------------------
QByteArray SnoopDns::encode()
{
  QByteArray res;
  res.append((const char*)&dnsHdr, sizeof(DNS_HDR));

  // ----- gilgil temp 2014.03.22 -----
  /*
  UINT16 _id = htons(dnsHdr.id);
  res.append((const char*)&_id, sizeof(UINT16));

  UINT16 _flags = htons(dnsHdr.flags);
  res.append((const char*)&_flags, sizeof(UINT16));

  UINT16 _num_q = htons(dnsHdr.num_q);
  res.append((const char*)&_num_q, sizeof(UINT16));

  UINT16 _num_answ_rr = htons(dnsHdr.num_answ_rr);
  res.append((const char*)&_num_answ_rr, sizeof(UINT16));

  UINT16 _num_auth_rr = htons(dnsHdr.num_auth_rr);
  res.append((const char*)&_num_auth_rr, sizeof(UINT16));

  UINT16 _num_addi_rr = htons(dnsHdr.num_addi_rr);
  res.append((const char*)&_num_addi_rr, sizeof(UINT16));
  */
  // ----------------------------------

  res += questions.encode();
  res += answers.encode();
  res += authorities.encode();
  res += additionals.encode();

  return res;
}

bool SnoopDns::decode(BYTE* udpData, int dataLen, int* offset)
{
  if (*offset + sizeof(DNS_HDR) > dataLen) return false;
  memcpy(&this->dnsHdr, udpData, sizeof(DNS_HDR));
  *offset += sizeof(DNS_HDR);

  if (!questions.decode(udpData,   dataLen, ntohs(dnsHdr.num_q),       offset)) return false;
  if (!answers.decode(udpData,     dataLen, ntohs(dnsHdr.num_answ_rr), offset)) return false;
  if (!authorities.decode(udpData, dataLen, ntohs(dnsHdr.num_auth_rr), offset)) return false;
  if (!additionals.decode(udpData, dataLen, ntohs(dnsHdr.num_addi_rr), offset)) return false;

  return true;
}

QByteArray SnoopDns::encodeName(QString name)
{
  QStringList labels = name.split('.');
  int count = labels.count();
  if (count == 0)
  {
    LOG_WARN("label count is zero(%s)", qPrintable(name));
    return "";
  }

  QByteArray res;
  for (int i = 0; i < count; i++)
  {
    QString label = labels.at(i);
    BYTE size = label.size();
    res.append((const char*)&size, sizeof(BYTE));
    res += label;
  }
  res.append((char)0x00);

  return res;
}

QString SnoopDns::decodeName(BYTE* udpData, int dataLen, int* offset)
{
  BYTE* p = (BYTE*)(udpData + *offset);
  QString res;
  bool first = true;
  while (true)
  {
    if (p - udpData > dataLen) return false;
    BYTE count = *p++;
    if (count == 0) break;

    if (count == 0xC0)
    {
      if (p - udpData > dataLen) return false;
      int tempOffset = *p++;
      res = decodeName(udpData, dataLen, &tempOffset);
      *offset += 2;
      return res;
    }
    if (p - udpData + count > dataLen) return false;
    QByteArray label((const char*)p, (int)count);
    p += count;

    if (first)
    {
      res += label;
      first = false;
    } else
    {
      res += ".";
      res += label;
    }
  }
  *offset = p - udpData;
  return res;
}
