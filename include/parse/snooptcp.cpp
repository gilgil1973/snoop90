#include <SnoopTcp>

#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopTcp
// ----------------------------------------------------------------------------
bool SnoopTcp::isData(IP_HDR* ipHdr, TCP_HDR* tcpHdr, BYTE** tcpData, int* tcpDataLen)
{
  int   tcpHdrLen   = tcpHdr->th_off * sizeof(UINT32);
  BYTE* _tcpData    = (BYTE*)(tcpHdr) + tcpHdrLen;
  int   _tcpDataLen = ntohs(ipHdr->ip_len) - sizeof(IP_HDR) - tcpHdrLen;
  
  if (_tcpDataLen > 0)
  {
    if (tcpData != NULL)
      *tcpData = _tcpData;
    if (tcpDataLen != NULL)
      *tcpDataLen = _tcpDataLen;
    return true;
  }
  return false;
}

bool SnoopTcp::isOption(TCP_HDR* tcpHdr, BYTE** tcpOption, int* tcpOptionLen)
{
  int tcpHdrLen = tcpHdr->th_off * sizeof(UINT32);
  int _tcpOptionLen = tcpHdrLen - sizeof(TCP_HDR);
  if (tcpOption != NULL)
    *tcpOption = (BYTE*)tcpHdr + sizeof(TCP_HDR);
  if (tcpOptionLen != NULL)
    *tcpOptionLen = _tcpOptionLen;
  return _tcpOptionLen > 0;
}

//
// All tcpHdr field except tcpHdr.th_sum
// All data buffer(padding)
// ipHdr.ip_src, ipHdr.ip_dst, tcpHdrDataLen and IPPROTO_TCP
//
UINT16 SnoopTcp::checksum(IP_HDR* ipHdr, TCP_HDR* tcpHdr)
{
  int i;
  int tcpHdrDataLen;
  UINT32 src, dst;
  UINT32 sum;
  UINT16 *p;
  
  tcpHdrDataLen = ntohs(ipHdr->ip_len) - sizeof(IP_HDR);
  sum = 0;

  // Add tcpHdr and data buffer as array of UIN16
  p = (UINT16*)tcpHdr;
  for (i = 0; i < tcpHdrDataLen / 2; i++)
  {
    sum += htons(*p);
    p++;
  }

  // If length is odd, add last data(padding)
  if ((tcpHdrDataLen / 2) * 2 != tcpHdrDataLen)
    sum += (htons(*p) & 0xFF00);

  // Decrease checksum from sum
  sum -= ntohs(tcpHdr->th_sum);

  // Add src address
  src = ntohl(ipHdr->ip_src);
  sum += ((src & 0xFFFF0000) >> 16) + (src & 0x0000FFFF);

  // Add dst address
  dst = ntohl(ipHdr->ip_dst);
  sum += ((dst & 0xFFFF0000) >> 16) + (dst & 0x0000FFFF);

  // Add extra information
  sum += (UINT32)(tcpHdrDataLen) + IPPROTO_TCP;

  // Recalculate sum
  while(sum >> 16)
  {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }
  sum = ~sum;

  return (UINT16)sum;
}

bool SnoopTcp::parse(SnoopPacket* packet)
{
  if (!SnoopIp::isTcp(packet->ipHdr, &packet->tcpHdr)) return false;
  packet->proto = IPPROTO_TCP;
  return true;
}

bool SnoopTcp::parseAll(SnoopPacket* packet)
{
  if (!SnoopIp::parseAll(packet)) return false;
  return parse(packet);
}

int SnoopTcp::getOption(
  char*           tcpOption,
  int             tcpOptionLen,
  SnoopTcpOption& snoopTCPOption)
{
  UINT8*      p;
  UINT8       kind;
  UINT8       len;

  p = (UINT8*)tcpOption;

  //
  // Set kind
  //
  if (tcpOptionLen < 1) goto _error;
  kind = *p; p++; tcpOptionLen--;
  snoopTCPOption.kind = kind;

  //
  // Set Length and Value
  //
  if (kind != 0 && kind != 1)
  {
    if (tcpOptionLen < 1) goto _error;
    len = *p; p++; tcpOptionLen--;
    snoopTCPOption.len = len;

    if (tcpOptionLen < len - 2) goto _error;
    snoopTCPOption.value = (BYTE*)p;
    p += len - 2; // p++; tcpOptionLen--; // remove warning
  }

  //
  // Set desc
  //
  char* desc;
  switch (kind)
  {
     case  0:  desc = "EOL";                                 break;
     case  1:  desc = "NOP";                                 break;
     case  2:  desc = "MSS";                                 break;
     case  3:  desc = "Window Scale";                        break;
     case  4:  desc = "SACK Permitted";                      break;
     case  5:  desc = "SACK";                                break;
     case  6:  desc = "Echo (obsoleted by option 8)";        break;
     case  7:  desc = "Echo Reply (obsoleted by option 8";   break;
     case  8:  desc = "TSOPT - Time Stamp Option";           break;
     case  9:  desc = "Partial Order Connection Permitte";   break;
     case  10: desc = "Partial Order Service Profile";       break;
     case  11: desc = "CC";                                  break;
     case  12: desc = "CC.NEW";                              break;
     case  13: desc = "CC.ECHO";                             break;
     case  14: desc = "TCP Alternate Checksum Request";      break;
     case  15: desc = "TCP Alternate Checksum Data";         break;
     case  16: desc = "Skeeter";                             break;
     case  17: desc = "Bubba";                               break;
     case  18: desc = "Trailer Checksum Option";             break;
     case  19: desc = "MD5 Signature Option";                break;
     case  20: desc = "SCPS Capabilities";                   break;
     case  21: desc = "Selective Negative Acknowledgements"; break;
     case  22: desc = "Record Boundaries";                   break;
     case  23: desc = "Corruption experienced";              break;
     case  24: desc = "SNAP";                                break;
     case  25: desc = "Unassigned (released 12/18/00)";      break;
     case  26: desc = "TCP Compression Filter";              break;
     case  27: desc = "Quick-Start Response";                break;
     case 253: desc = "RFC3692-style Experiment 1";          break;
     case 254: desc = "RFC3692-style Experiment 2";          break;
     default : desc = "UNKNOWN";                             break;
  }

  snoopTCPOption.desc = (BYTE*)desc;

  return (int)(p - (UINT8*)tcpOption);
_error:
  return 0;
}

int SnoopTcp::getOptionList(
  char*               tcpOption,
  int                 tcpOptionLen,
  SnoopTcpOptionList& snoopTCPOptionList)
{
  int totalRes = 0;
  while (true)
  {
    SnoopTcpOption snoopTCPOption;
    int res = getOption(tcpOption, tcpOptionLen, snoopTCPOption);
    if (res == 0) 
    {
      break;
    }
    snoopTCPOptionList.push_back(snoopTCPOption);
    tcpOption    += res;
    tcpOptionLen -= res;
    totalRes     += res;
  }
  return totalRes;
}
