#include <SnoopType>

#include <VDebugNew>

// ----------------------------------------------------------------------------
// Mac
// ----------------------------------------------------------------------------
Mac::Mac(const QString s)
{
  int i;
  UINT8* p;
  UINT8 ch1, ch2;

  QByteArray arr = s.toLatin1();
  p = (UINT8*)arr.constData();
  for (i = 0 ; i < MAC_SIZE; i++)
  {
    ch1 = *p++;
    if (ch1 >= 'a')
      ch1 = ch1 - 'a' + 10;
    else
    if (ch1 >= 'A')
      ch1 = ch1 - 'A' + 10;
    else
      ch1 = ch1 - '0';
    ch2 = *p++;
    if (ch2 >= 'a')
      ch2 = ch2 - 'a' + 10;
    else if (ch2 >= 'A')
      ch2 = ch2 - 'A' + 10;
    else
      ch2 = ch2 - '0';
    value[i] = (ch1 << 4) + ch2;
    while (*p == '-' || *p == ':') p++;
  }
}

QString Mac::str() const
{
  UINT8 ch1, ch2;
  int i, index;
  char buf[MAC_SIZE * 3]; // enough size

  index = 0;
  for (i = 0; i < MAC_SIZE; i++)
  {
    ch1 = value[i] & 0xF0;
    ch1 = ch1 >> 4;
    if (ch1 > 9)
      ch1 = ch1 + 'A' - 10;
    else
      ch1 = ch1 + '0';
    ch2 = value[i] & 0x0F;
    if (ch2 > 9)
      ch2 = ch2 + 'A' - 10;
    else
      ch2 = ch2 + '0';
    buf[index++] = ch1;
    buf[index++] = ch2;
    if (i == 2)
      buf[index++] = '-';
  }
  buf[index] = '\0';
  return (QString(buf));
}

QString Mac::qformat(QString format)
{
  return ::qformat(qPrintable(format), value[0], value[1], value[2], value[3], value[4], value[5]);
}

Mac Mac::randomMac()
{
  Mac res;
  for (int i = 0; i < MAC_SIZE; i++)
    res.value[i] = rand() % 256;
  res.value[0] &= 0x7F;
  return res;
}

Mac& Mac::cleanMac()
{
  static UINT8 _value[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  static Mac res(_value);
  return res;
}

Mac& Mac::broadcastMac()
{
  static UINT8 _value[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  static Mac res(_value);
  return res;
}







