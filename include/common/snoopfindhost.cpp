#include <SnoopFindHost>

// ----------------------------------------------------------------------------
// SnoopFindHost
// ----------------------------------------------------------------------------
SnoopFindHost::SnoopFindHost(void* owner) : SnoopAdapter(owner)
{
  filter         = "arp";

  findAllTimeout = snoop::DEFAULT_TIMEOUT;
  scanInterval   = 0;
  sendInterval   = 0;
  hostList.clear();
  lastSendTick   = 0;
}

SnoopFindHost::~SnoopFindHost()
{
  close();
}

bool SnoopFindHost::doOpen()
{
  lastSendTick = tick();
  netInfo.adapterIndex = this->adapterIndex;
  return SnoopAdapter::doOpen();
}

bool SnoopFindHost::doClose()
{
  lastSendTick = 0;
  return SnoopAdapter::doClose();
}

bool SnoopFindHost::findAll()
{
  if (isFoundAll()) return true;

  if (!sendArpRequestAll()) return false;

  //
  // Receive and wait
  //
  VTick startTick = tick();
  VTick nowTick;
  while (true)
  {
    //
    // If found all?
    //
    if (isFoundAll()) return true;

    //
    // Is time elapsed?
    //
    nowTick = tick();
    if (nowTick - startTick > findAllTimeout)
    {
      {
        SET_ERROR(SnoopError, "can not find all host", VERR_CAN_NOT_FIND_ALL_HOST);
      }
      logUnfoundHost();
      return false;
    }

    //
    // Retrieve ARP packet
    //
    Ip  ip;
    Mac mac;
    int res = read(ip, mac);
    if (res == 0) continue;
    if (res < 0) break;
  }
  return false;
}

bool SnoopFindHost::sendArpRequestAll()
{
  for (SnoopHostList::iterator it = hostList.begin(); it != hostList.end(); it++)
  {
    SnoopHost& host = *it;
    if (host.mac.isClean())
    {
      if (!sendArpRequest(host.ip)) return false;
      if (sendInterval != 0)
        Sleep(sendInterval);
    }
  }
  lastSendTick = GetTickCount();
  return true;
}

bool SnoopFindHost::isFoundAll()
{
  for (SnoopHostList::iterator it = hostList.begin(); it != hostList.end(); it++)
  {
    SnoopHost& host = *it;
    if (host.mac.isClean())
      return false;
  }
  return true;
}

void SnoopFindHost::logUnfoundHost(VLog* log)
{
  if (log == NULL) log = VLog::getLog();
  if (log == NULL) return;

  for (SnoopHostList::iterator it = hostList.begin(); it != hostList.end(); it++)
  {
    SnoopHost& host = *it;
    if (host.mac.isClean())
    {
      log->info("[SnoopFindHost.cpp] SnoopFindHost::logUnfoundHost ip=%s", qPrintable(host.ip.str()));
    }
  }
}

int SnoopFindHost::sendArpRequest(Ip ip)
{
  static const int BUF_SIZE = sizeof(ETH_HDR) + sizeof(ARP_HDR);
  BYTE buf[BUF_SIZE];
  ETH_HDR* ethHdr = (ETH_HDR*)buf;
  ARP_HDR* arpHdr = (ARP_HDR*)(buf + sizeof(ETH_HDR));

  //
  // Set Ethernet Hdr
  //
  ethHdr->ether_dhost = Mac::broadcastMac();
  ethHdr->ether_shost = netInfo.mac;
  ethHdr->ether_type  = htons(ETHERTYPE_ARP);

  //
  // Set ARP Hdr
  //
  arpHdr->ar_hrd = htons(ARPHRD_ETHER);
  arpHdr->ar_pro = htons(ETHERTYPE_IP);
  arpHdr->ar_hln = sizeof(Mac);
  arpHdr->ar_pln = sizeof(Ip);
  arpHdr->ar_op  = htons(ARPOP_REQUEST);
  arpHdr->ar_sa  = netInfo.mac;
  arpHdr->ar_si  = htonl(netInfo.ip);
  arpHdr->ar_ta  = Mac::cleanMac();
  arpHdr->ar_ti  = htonl(ip);

  //
  // Send Raw Packet
  //
  return write(buf, BUF_SIZE);
}

int SnoopFindHost::read(Ip& ip, Mac& mac)
{
  SnoopPacket packet;

  int res = SnoopAdapter::read(&packet);
  if (scanInterval != 0)
  {
    VTick nowTick = tick();
    if (nowTick - lastSendTick > (DWORD)scanInterval)
    {
      sendArpRequestAll();
    }
  }
  if (res <= 0) return res;

  if (packet.arpHdr == NULL) return 0;

  bool ok = false;
  if (ntohs(packet.arpHdr->ar_op) == ARPOP_REPLY && packet.ethHdr->ether_dhost == netInfo.mac)
  {
    ip  = htonl(packet.arpHdr->ar_si);
    mac = packet.arpHdr->ar_sa;

    for( SnoopHostList::iterator it = hostList.begin(); it != hostList.end(); it++)
    {
      SnoopHost& host = *it;
      if (host.ip == ip)
      {
        host.mac = mac;
        ok = true;
      }
    }
  }
  return ok ? res : 0;
}

void SnoopFindHost::run()
{
  while (runThread().active())
  {
    Ip ip; Mac mac;
    int res = read(ip, mac);
    if (res == 0) continue;
    if (res < 0) break;
    emit found(ip, mac);
  }
}

void SnoopFindHost::load(VXml xml)
{
  SnoopAdapter::load(xml);

  findAllTimeout = xml.getInt("findAllTimeout", findAllTimeout);
  scanInterval   = xml.getULong("scanInterval", scanInterval);
  sendInterval   = xml.getULong("sendInterval", sendInterval);
  hostList.load(xml.gotoChild("hostList"));
}

void SnoopFindHost::save(VXml xml)
{
  SnoopAdapter::save(xml);

  xml.setInt("findAllTimeout", findAllTimeout);
  xml.setULong("scanInterval", scanInterval);
  xml.setULong("sendInterval", sendInterval);
  hostList.save(xml.gotoChild("hostList"));
}
