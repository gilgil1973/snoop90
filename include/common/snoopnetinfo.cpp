#include <SnoopNetInfo>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopNetInfoAdapterIndex
// ----------------------------------------------------------------------------
SnoopNetInfoAdapterIndex::SnoopNetInfoAdapterIndex(SnoopNetInfo* netInfo)
{
  this->netInfo = netInfo;
  m_adapterIndex = snoop::INVALID_ADAPTER_INDEX;
}

SnoopNetInfoAdapterIndex::operator int()
{
  return m_adapterIndex;
}

void SnoopNetInfoAdapterIndex::operator = (const int i)
{
  setAdapterIndex(i);
}

void SnoopNetInfoAdapterIndex::setAdapterIndex(int value)
{
  if (m_adapterIndex == value) return;
  netInfo->clear();
  m_adapterIndex = value;
  const SnoopInterface& intf = SnoopInterfaces::instance().at(m_adapterIndex);
#ifdef WIN32 // gilgil temp 2012.08.10
  PIP_ADAPTER_INFO p = intf.adapterInfo;
  if (p == NULL)
  {
    LOG_DEBUG("snoopAdapterInfo->adapterInfo is NULL (index=%d)", m_adapterIndex);
    return;
  }

  // mac
  netInfo->mac = p->Address;

  // ip
  if (p->IpAddressList.IpAddress.String[0] != '\0')
    netInfo->ip = QString(p->IpAddressList.IpAddress.String); // p->IpAddressList.IpAddress.String;

  // mac
  netInfo->mac = p->Address;

  // subnet
  if (p->IpAddressList.IpMask.String[0] != '\0')
    netInfo->subnet = QString(p->IpAddressList.IpMask.String);

  // gateway
  if (p->GatewayList.IpAddress.String[0] != '\0')
    netInfo->gateway = QString(p->GatewayList.IpAddress.String);
#else // linux
  if (intf.dev != NULL && intf.dev->addresses != NULL)
  {
    for (struct pcap_addr* a = intf.dev->addresses; a != NULL; a = a->next)
    {
      if (a->addr != NULL && a->addr->sa_family == AF_INET)
      {
        if (a->addr != NULL)
          netInfo->ip     = ntohl(((struct sockaddr_in *)a->addr   )->sin_addr.s_addr);
        if (a->netmask != NULL)
          netInfo->subnet = ntohl(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr);
        break;
      }
    }
  }
#endif // WIN32
  // ip_and_subnet
  netInfo->ip_and_subnet = netInfo->ip & netInfo->subnet;
}

// ----------------------------------------------------------------------------
// SnoopNetInfo
// ----------------------------------------------------------------------------
SnoopNetInfo::SnoopNetInfo() : adapterIndex(this)
{
  clear();
}

SnoopNetInfo::~SnoopNetInfo()
{
}

void SnoopNetInfo::clear()
{
  ip            = 0;
  mac           = Mac::cleanMac();
  subnet        = 0;
  gateway       = 0;
  ip_and_subnet = 0;
}

void SnoopNetInfo::load(VXml xml)
{
  //
  // netInfo
  //
  ip      = xml.getStr("ip",      ip.str());
  mac     = xml.getStr("mac",     mac.str());
  subnet  = xml.getStr("subnet",  subnet.str());
  gateway = xml.getStr("gateway", gateway.str());
  ip_and_subnet = ip & subnet;
}

void SnoopNetInfo::save(VXml xml)
{
  //
  // netInfo
  //
  xml.setStr("ip",     ip.str());
  xml.setStr("mac",     mac.str());
  xml.setStr("subnet",  subnet.str());
  xml.setStr("gateway", gateway.str());
}
