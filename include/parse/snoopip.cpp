#include <SnoopIp>

#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopIp
// ----------------------------------------------------------------------------
bool SnoopIp::is(IP_HDR* ipHdr, UINT8 protocol, void** transportHdr)
{
  if (ipHdr->ip_p != protocol)
    return false;
  if (transportHdr != NULL)
    *transportHdr = (void*)((BYTE*)(ipHdr) + ipHdr->ip_hl * sizeof(UINT32));
  return true;
}

bool SnoopIp::isTcp(IP_HDR* ipHdr, TCP_HDR** tcpHdr)
{
  if (ipHdr->ip_p != IPPROTO_TCP)
    return false;
  if (tcpHdr != NULL)
    *tcpHdr = (TCP_HDR*)((BYTE*)(ipHdr) + ipHdr->ip_hl * sizeof(UINT32));
  return true;
}

bool SnoopIp::isUdp(IP_HDR* ipHdr, UDP_HDR** udpHdr)
{
  if (ipHdr->ip_p != IPPROTO_UDP)
    return false;
  if (udpHdr != NULL)
    *udpHdr = (UDP_HDR*)((BYTE*)(ipHdr) + ipHdr->ip_hl * sizeof(UINT32));
  return true;
}

bool SnoopIp::isIcmp(IP_HDR* ipHdr, ICMP_HDR** icmpHdr)
{
  if (ipHdr->ip_p != IPPROTO_ICMP)
    return false;
  if (icmpHdr != NULL)
    *icmpHdr = (ICMP_HDR*)((BYTE*)(ipHdr) + ipHdr->ip_hl * sizeof(UINT32));
  return true;
}

//
// All ipHdr field except ipHdr.ip_sum
//
UINT16 SnoopIp::checksum(IP_HDR* ipHdr)
{
  int i;
  UINT32 sum;
  UINT16 *p;

  sum = 0;

  // Add ipHdr buffer as array of UINT16
  p = (UINT16*)(ipHdr);
  for (i = 0; i < (int)sizeof(IP_HDR) / 2; i++)
  {
    sum += ntohs(*p);
    p++;
  }
  
  // Do not consider padding because ip header length is always multilpe of 2.

  // Decrease checksum from sum
  sum -= ntohs(ipHdr->ip_sum);

  // Recalculate sum
  while(sum >> 16)
  {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }
  sum = ~sum;

  return (UINT16)sum;
}

//
// Calculate Checksum on condition that only one value(UINT16) is changed.
//
UINT16 SnoopIp::recalculateChecksum(UINT16 oldChecksum, UINT16 oldValue, UINT16 newValue)
{
  UINT32 sum;

  sum = oldValue + (~newValue & 0xFFFF);
  sum += oldChecksum;
  sum = (sum & 0xFFFF) + (sum >> 16);
  return (UINT16)(sum + (sum >> 16));
}

UINT16 SnoopIp::recalculateChecksum(UINT16 oldChecksum, UINT32 oldValue, UINT32 newValue)
{
  UINT16 oldValue16;
  UINT16 newValue16;
  UINT16 sum;

  oldValue16 = (oldValue & 0xFFFF0000) >> 16;
  newValue16 = (newValue & 0xFFFF0000) >> 16;
  sum        = recalculateChecksum(oldChecksum, oldValue16, newValue16);

  oldValue16 = oldValue & 0x0000ffff;
  newValue16 = newValue & 0x0000ffff;
  sum        = recalculateChecksum(sum, oldValue16, newValue16);

  return sum;
}

bool SnoopIp::parse(SnoopPacket* packet)
{
  if (!SnoopEth::isIp(packet->ethHdr, &packet->ipHdr)) return false;
  packet->netType = ETHERTYPE_IP;
  return true;
}

bool SnoopIp::parseAll(SnoopPacket* packet)
{
  if (!SnoopEth::parseAll(packet)) return false;
  return parse(packet);
}
