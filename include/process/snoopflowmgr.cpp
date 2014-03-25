#include <SnoopFlowMgr>
#include <VDebugNew>

#include <SnoopEth>
#include <SnoopIp>
#include <SnoopTcp>
#include <SnoopUdp>

REGISTER_METACLASS(SnoopFlowMgr, SnoopProcess)

// ----------------------------------------------------------------------------
// Snoop_MacFlow_Map
// ----------------------------------------------------------------------------
Snoop_MacFlow_Map::Snoop_MacFlow_Map()
{
  clear();
}

Snoop_MacFlow_Map::~Snoop_MacFlow_Map()
{
  clear();
}

void Snoop_MacFlow_Map::clear()
{
  for (Snoop_MacFlow_Map::iterator it = begin(); it != end(); it++)
  {
    BYTE* totalMem = it.value().totalMem;
    delete[] totalMem;
  }
  QMap<SnoopMacFlowKey, SnoopFlowValue>::clear();
}

Snoop_MacFlow_Map::iterator Snoop_MacFlow_Map::erase(SnoopMacFlowKey& key)
{
  Snoop_MacFlow_Map::iterator it = find(key);
  LOG_ASSERT(it != end());
  BYTE* totalMem = it.value().totalMem;
  delete[] totalMem;
  return QMap<SnoopMacFlowKey, SnoopFlowValue>::erase(it);
}

// ----------------------------------------------------------------------------
// Snoop_IpFlow_Map
// ----------------------------------------------------------------------------
Snoop_IpFlow_Map::Snoop_IpFlow_Map()
{
  clear();
}

Snoop_IpFlow_Map::~Snoop_IpFlow_Map()
{
  clear();
}

void Snoop_IpFlow_Map::clear()
{
  for (Snoop_IpFlow_Map::iterator it = begin(); it != end(); it++)
  {
    BYTE* totalMem = it.value().totalMem;
    delete[] totalMem;
  }
  QMap<SnoopIpFlowKey, SnoopFlowValue>::clear();
}

Snoop_IpFlow_Map::iterator Snoop_IpFlow_Map::erase(SnoopIpFlowKey& key)
{
  Snoop_IpFlow_Map::iterator it = find(key);
  LOG_ASSERT(it != end());
  BYTE* totalMem = it.value().totalMem;
  delete[] totalMem;
  return QMap<SnoopIpFlowKey, SnoopFlowValue>::erase(it);
}

// ----------------------------------------------------------------------------
// Snoop_TcpFlow_Map
// ----------------------------------------------------------------------------
Snoop_TcpFlow_Map::Snoop_TcpFlow_Map()
{
  clear();
}

Snoop_TcpFlow_Map::~Snoop_TcpFlow_Map()
{
  clear();
}

void Snoop_TcpFlow_Map::clear()
{
  for (Snoop_TcpFlow_Map::iterator it = begin(); it != end(); it++)
  {
    BYTE* totalMem = it.value().totalMem;
    delete[] totalMem;
  }
  QMap<SnoopTcpFlowKey, SnoopFlowValue>::clear();
}

Snoop_TcpFlow_Map::iterator Snoop_TcpFlow_Map::erase(SnoopTcpFlowKey& key)
{
  Snoop_TcpFlow_Map::iterator it = find(key);
  LOG_ASSERT(it != end());
  BYTE* totalMem = it.value().totalMem;
  delete[] totalMem;
  return QMap<SnoopTcpFlowKey, SnoopFlowValue>::erase(it);
}

// ----------------------------------------------------------------------------
// Snoop_UdpFlow_Map
// ----------------------------------------------------------------------------
Snoop_UdpFlow_Map::Snoop_UdpFlow_Map()
{
  clear();
}

Snoop_UdpFlow_Map::~Snoop_UdpFlow_Map()
{
  clear();
}

void Snoop_UdpFlow_Map::clear()
{
  for (Snoop_UdpFlow_Map::iterator it = begin(); it != end(); it++)
  {
    BYTE* totalMem = it.value().totalMem;
    delete[] totalMem;
  }
  QMap<SnoopUdpFlowKey, SnoopFlowValue>::clear();
}

Snoop_TcpFlow_Map::iterator Snoop_UdpFlow_Map::erase(SnoopUdpFlowKey& key)
{
  Snoop_UdpFlow_Map::iterator it = find(key);
  LOG_ASSERT(it != end());
  BYTE* totalMem = it.value().totalMem;
  delete[] totalMem;
  return QMap<SnoopUdpFlowKey, SnoopFlowValue>::erase(it);
}

// ----------------------------------------------------------------------------
// SnoopFlowRequestItem
// ----------------------------------------------------------------------------
SnoopFlowRequestItem::SnoopFlowRequestItem()
{
  id      = NULL;
  offset  = 0;
  memSize = 0;
};

SnoopFlowRequestItem::~SnoopFlowRequestItem()
{
}

// ----------------------------------------------------------------------------
// SnoopFlowRequestItems
// ----------------------------------------------------------------------------
SnoopFlowRequestItems::SnoopFlowRequestItems()
{
  totalMemSize = 0;
}

SnoopFlowRequestItems::~SnoopFlowRequestItems()
{
  // gilgil temp 2014.02.21
}

// ----------------------------------------------------------------------------
// SnoopBlock
// ----------------------------------------------------------------------------
SnoopFlowMgr::SnoopFlowMgr(void* owner) : SnoopProcess(owner)
{
  lastCheckTick    = 0;
  checkInterval    = 1;
  macFlowTimeout   = 60 * 60; // 1 hour
  ipFlowTimeout    = 60 * 5;  // 1 hour
  tcpFlowTimeout   = 60 * 5;  // 5 minute
  udpFlowTimeout   = 60 * 5;  // 5 minute
}

SnoopFlowMgr::~SnoopFlowMgr()
{
  close();
}

bool SnoopFlowMgr::doOpen()
{
  clearMaps();
  clearItems();
  lastCheckTick = 0;

  return SnoopProcess::doOpen();
}

bool SnoopFlowMgr::doClose()
{
  //
  // MacFlow
  //
  while(macFlow_Map.count() > 0)
  {
    const SnoopMacFlowKey& key = macFlow_Map.begin().key();
    del_MacFlow((SnoopMacFlowKey&)key);
  }

  //
  // IpFlow
  //
  while(ipFlow_Map.count() > 0)
  {
    const SnoopIpFlowKey& key = ipFlow_Map.begin().key();
    del_IpFlow((SnoopIpFlowKey&)key);
  }

  //
  // TcpFlow
  //
  while(tcpFlow_Map.count() > 0)
  {
    const SnoopTcpFlowKey& key = tcpFlow_Map.begin().key();
    del_TcpFlow((SnoopTcpFlowKey&)key);
  }

  //
  // UdpFlow
  //
  while(udpFlow_Map.count() > 0)
  {
    const SnoopUdpFlowKey& key = udpFlow_Map.begin().key();
    del_UdpFlow((SnoopUdpFlowKey&)key);
  }


  clearMaps();
  clearItems();

  return SnoopProcess::doClose();
}

void SnoopFlowMgr::clearMaps()
{
  macFlow_Map.clear();
  tcpFlow_Map.clear();
  udpFlow_Map.clear();
}

void SnoopFlowMgr::deleteOldMaps(struct timeval ts)
{
  //
  // MacFlow
  //
  {
    Snoop_MacFlow_Map::iterator it = macFlow_Map.begin();
    while (it != macFlow_Map.end())
    {
      const SnoopFlowValue& value = it.value();
      long elapsed = ts.tv_sec - value.ts.tv_sec;
      if (elapsed >= macFlowTimeout)
      {
        it = del_MacFlow((SnoopMacFlowKey&)it.key());
        continue;
      }
      it++;
    }
  }

  //
  // IpFlow
  //
  {
    Snoop_IpFlow_Map::iterator it = ipFlow_Map.begin();
    while (it != ipFlow_Map.end())
    {
      const SnoopFlowValue& value = it.value();
      long elapsed = ts.tv_sec - value.ts.tv_sec;
      if (elapsed >= ipFlowTimeout)
      {
        it = del_IpFlow((SnoopIpFlowKey&)it.key());
        continue;
      }
      it++;
    }
  }

  //
  // TcpFlow
  //
  {
    Snoop_TcpFlow_Map::iterator it = tcpFlow_Map.begin();
    while (it != tcpFlow_Map.end())
    {
      const SnoopFlowValue& value = it.value();
      long elapsed = ts.tv_sec - value.ts.tv_sec;
      if (elapsed >= tcpFlowTimeout)
      {
        it = del_TcpFlow((SnoopTcpFlowKey&)it.key());
        continue;
      }
      it++;
    }
  }

  //
  // UdpFlow
  //
  {
    Snoop_UdpFlow_Map::iterator it = udpFlow_Map.begin();
    while (it != udpFlow_Map.end())
    {
      const SnoopFlowValue& value = it.value();
      long elapsed = ts.tv_sec - value.ts.tv_sec;
      if (elapsed >= udpFlowTimeout)
      {
        it = del_UdpFlow((SnoopUdpFlowKey&)it.key());
        continue;
      }
      it++;
    }
  }
}

void SnoopFlowMgr::clearItems()
{
  macFlow_Items.clear();
  ipFlow_Items.clear();
  tcpFlow_Items.clear();
  udpFlow_Items.clear();
}

size_t SnoopFlowMgr::requestMemory(void* id, SnoopFlowRequestItems& items, size_t memSize)
{
  size_t currentOffset = 0;
  int _count = items.count();
  for (int i = 0; i < _count; i++)
  {
    const SnoopFlowRequestItem& item = items.at(i);
    if (item.id == id) return currentOffset;
    currentOffset += item.memSize;
  }

  SnoopFlowRequestItem item;
  item.id      = id;
  item.offset  = currentOffset;
  item.memSize = memSize;

  items.push_back(item);
  items.totalMemSize += memSize;

  return currentOffset;
}

void SnoopFlowMgr::connect(const char* signal, VObject* receiver, const char* slot, Qt::ConnectionType type)
{
  VObjectConnection connection(signal, receiver, slot);
  if (this->connections.indexOf(connection) == -1)
  {
    // ----- by gilgil 2014.03.26 -----
    /*
    LOG_DEBUG("connecting    %s(%s) %s > %s(%s) %s",
      qPrintable(this->name),     qPrintable(this->className()),     signal,
      qPrintable(receiver->name), qPrintable(receiver->className()), slot);
    */
    // --------------------------------
    VObject::connect(this, signal, receiver, slot, type);
  }
}

void SnoopFlowMgr::disconnect(const char* signal, VObject* receiver, const char* slot)
{
  VObjectConnection connection(signal, receiver, slot);
  if (this->connections.indexOf(connection) != -1)
  {
    // ----- by gilgil 2014.03.26 -----
    /*
    LOG_DEBUG("disconnecting %s(%s) %s > %s(%s) %s",
      qPrintable(this->name),     qPrintable(this->className()),     signal,
      qPrintable(receiver->name), qPrintable(receiver->className()), slot);
    */
    // --------------------------------
    VObject::disconnect(this, signal, receiver, slot);
  }
}

size_t SnoopFlowMgr::requestMemory_MacFlow(void* id, size_t memSize)
{
  return requestMemory(id, macFlow_Items, memSize);
}

Snoop_MacFlow_Map::iterator SnoopFlowMgr::add_MacFlow(SnoopMacFlowKey& key, struct timeval ts, bool created)
{
  SnoopFlowValue value;
  value.packets = 0;
  value.bytes   = 0;
  value.ts      = ts;
  value.created = created;
  value.totalMem = new BYTE[macFlow_Items.totalMemSize];
  memset(value.totalMem, 0, macFlow_Items.totalMemSize);
  Snoop_MacFlow_Map::iterator it = macFlow_Map.insert(key, value);
  if (created)
  {
    emit __macFlowCreated((SnoopMacFlowKey*)&it.key(), (SnoopFlowValue*)&it.value());
  }
  return it;
}

Snoop_MacFlow_Map::iterator SnoopFlowMgr::del_MacFlow(SnoopMacFlowKey& key)
{
  Snoop_MacFlow_Map::iterator it = macFlow_Map.find(key);
  if (it == macFlow_Map.end())
  {
    LOG_FATAL("key(%s > %s) is null", qPrintable(key.srcMac.str()), qPrintable(key.dstMac.str()));
    return it;
  }
  emit __macFlowDeleted((SnoopMacFlowKey*)&it.key(), (SnoopFlowValue*)&it.value());
  return macFlow_Map.erase(key);
}

size_t SnoopFlowMgr::requestMemory_IpFlow(void* id, size_t memSize)
{
  return requestMemory(id, ipFlow_Items, memSize);
}

Snoop_IpFlow_Map::iterator SnoopFlowMgr::add_IpFlow(SnoopIpFlowKey& key, struct timeval ts, bool created)
{
  SnoopFlowValue value;
  value.packets = 0;
  value.bytes   = 0;
  value.ts      = ts;
  value.created = created;
  value.totalMem = new BYTE[ipFlow_Items.totalMemSize];
  memset(value.totalMem, 0, ipFlow_Items.totalMemSize);
  Snoop_IpFlow_Map::iterator it = ipFlow_Map.insert(key, value);
  if (created)
  {
    emit __ipFlowCreated((SnoopIpFlowKey*)&it.key(), (SnoopFlowValue*)&it.value());
  }
  return it;
}

Snoop_IpFlow_Map::iterator SnoopFlowMgr::del_IpFlow(SnoopIpFlowKey& key)
{
  Snoop_IpFlow_Map::iterator it = ipFlow_Map.find(key);
  if (it == ipFlow_Map.end())
  {
    LOG_FATAL("key(%s > %s) is null", qPrintable(key.srcIp.str()), qPrintable(key.dstIp.str()));
    return it;
  }
  emit __ipFlowDeleted((SnoopIpFlowKey*)&it.key(), (SnoopFlowValue*)&it.value());
  return ipFlow_Map.erase(key);
}

size_t SnoopFlowMgr::requestMemory_TcpFlow(void* id, size_t memSize)
{
  return requestMemory(id, tcpFlow_Items, memSize);
}

Snoop_TcpFlow_Map::iterator SnoopFlowMgr::add_TcpFlow(SnoopTcpFlowKey& key, struct timeval ts, bool created)
{
  SnoopFlowValue value;
  value.packets = 0;
  value.bytes   = 0;
  value.ts      = ts;
  value.created = created;
  value.totalMem = new BYTE[tcpFlow_Items.totalMemSize];
  memset(value.totalMem, 0, tcpFlow_Items.totalMemSize);
  Snoop_TcpFlow_Map::iterator it = tcpFlow_Map.insert(key, value);
  if (created)
  {
    emit __tcpFlowCreated((SnoopTcpFlowKey*)&it.key(), (SnoopFlowValue*)&it.value());
  }
  return it;
}

Snoop_TcpFlow_Map::iterator SnoopFlowMgr::del_TcpFlow(SnoopTcpFlowKey& key)
{
  Snoop_TcpFlow_Map::iterator it = tcpFlow_Map.find(key);
  if (it == tcpFlow_Map.end())
  {
    LOG_FATAL("key(%s:%d > %s:%d) is null", qPrintable(key.srcIp.str()), key.srcPort, qPrintable(key.dstIp.str()), key.dstPort);
    return it;
  }
  emit __tcpFlowDeleted((SnoopTcpFlowKey*)&it.key(), (SnoopFlowValue*)&it.value());
  return tcpFlow_Map.erase(key);
}

size_t SnoopFlowMgr::requestMemory_UdpFlow(void* id, size_t memSize)
{
  return requestMemory(id, udpFlow_Items, memSize);
}

Snoop_UdpFlow_Map::iterator SnoopFlowMgr::add_UdpFlow(SnoopUdpFlowKey& key, struct timeval ts, bool created)
{
  SnoopFlowValue value;
  value.packets = 0;
  value.bytes   = 0;
  value.ts      = ts;
  value.created = created;
  value.totalMem = new BYTE[udpFlow_Items.totalMemSize];
  memset(value.totalMem, 0, udpFlow_Items.totalMemSize);
  Snoop_UdpFlow_Map::iterator it = udpFlow_Map.insert(key, value);
  if (created)
  {
    emit __udpFlowCreated((SnoopUdpFlowKey*)&it.key(), (SnoopFlowValue*)&it.value());
  }
  return it;
}

Snoop_UdpFlow_Map::iterator SnoopFlowMgr::del_UdpFlow(SnoopUdpFlowKey& key)
{
  Snoop_UdpFlow_Map::iterator it = udpFlow_Map.find(key);
  if (it == udpFlow_Map.end())
  {
    LOG_FATAL("key(%s:%d > %s:%d) is null", qPrintable(key.srcIp.str()), key.srcIp, qPrintable(key.dstIp.str()), key.dstPort);
    return it;
  }
  emit __udpFlowDeleted((SnoopUdpFlowKey*)&it.key(), (SnoopFlowValue*)&it.value());
  return udpFlow_Map.erase(key);
}

void SnoopFlowMgr::process(SnoopPacket* packet)
{
  long now = packet->pktHdr->ts.tv_sec;
  if (checkInterval != 0 && now - lastCheckTick >= checkInterval)
  {
    deleteOldMaps(packet->pktHdr->ts);
    lastCheckTick = now;
  }

  //
  // MacFlow
  //
  if (packet->ethHdr != NULL)
  {
    if (macFlow_Items.count() > 0)
    {
      Mac srcMac = packet->ethHdr->ether_shost;
      Mac dstMac = packet->ethHdr->ether_dhost;

      SnoopMacFlowKey key;
      key.srcMac = srcMac;
      key.dstMac = dstMac;
      process_MacFlow(packet, key);
    }

    //
    // IpFlow
    //
    if (packet->ipHdr != NULL)
    {
      Ip srcIp = ntohl(packet->ipHdr->ip_src);
      Ip dstIp = ntohl(packet->ipHdr->ip_dst);

      if (ipFlow_Items.count() > 0)
      {
        SnoopIpFlowKey key;
        key.srcIp = srcIp;
        key.dstIp = dstIp;
        process_IpFlow(packet, key);
      }

      //
      // TcpFlow
      //
      if (packet->tcpHdr != NULL)
      {
        if (tcpFlow_Items.count() > 0)
        {
          UINT16 srcPort = ntohs(packet->tcpHdr->th_sport);
          UINT16 dstPort = ntohs(packet->tcpHdr->th_dport);

          SnoopTcpFlowKey key;
          key.srcIp   = srcIp;
          key.srcPort = srcPort;
          key.dstIp   = dstIp;
          key.dstPort = dstPort;
          process_TcpFlow(packet, key);
        }
      }

      //
      // UdpFlow
      //
      if (packet->udpHdr != NULL)
      {
        if (udpFlow_Items.count() > 0)
        {
          UINT16 srcPort = ntohs(packet->udpHdr->uh_sport);
          UINT16 dstPort = ntohs(packet->udpHdr->uh_dport);

          SnoopUdpFlowKey key;
          key.srcIp   = srcIp;
          key.srcPort = srcPort;
          key.dstIp   = dstIp;
          key.dstPort = dstPort;
          process_UdpFlow(packet, key);
        }
      }
    }
  }

  emit processed(packet);
}

void SnoopFlowMgr::process_MacFlow(SnoopPacket* packet, SnoopMacFlowKey& key)
{
  Snoop_MacFlow_Map::iterator it = macFlow_Map.find(key);
  if (it == macFlow_Map.end())
    it = add_MacFlow(key, packet->pktHdr->ts, true);
  SnoopFlowValue& value = it.value();
  if (!value.created)
  {
    value.created = true;
    SnoopMacFlowKey* key = (SnoopMacFlowKey*)&it.key();
    emit __macFlowCreated(key, &value);
  }

  value.packets++;
  value.bytes += packet->pktHdr->caplen;
  value.ts = packet->pktHdr->ts;

  packet->flowKey   = &key;
  packet->flowValue = &value;
  emit __macCaptured(packet);
}

void SnoopFlowMgr::process_IpFlow(SnoopPacket* packet, SnoopIpFlowKey& key)
{
  Snoop_IpFlow_Map::iterator it = ipFlow_Map.find(key);
  if (it == ipFlow_Map.end())
    it = add_IpFlow(key, packet->pktHdr->ts, true);
  SnoopFlowValue& value = it.value();
  if (!value.created)
  {
    value.created = true;
    SnoopIpFlowKey* key = (SnoopIpFlowKey*)&it.key();
    emit __ipFlowCreated(key, &value);
  }
  value.packets++;
  value.bytes += packet->pktHdr->caplen;
  value.ts = packet->pktHdr->ts;

  packet->flowKey   = &key;
  packet->flowValue = &value;
  emit __ipCaptured(packet);
}

void SnoopFlowMgr::process_TcpFlow(SnoopPacket* packet, SnoopTcpFlowKey& key)
{
  Snoop_TcpFlow_Map::iterator it = tcpFlow_Map.find(key);
  if (it == tcpFlow_Map.end())
    it = add_TcpFlow(key, packet->pktHdr->ts, true);
  SnoopFlowValue& value = it.value();
  if (!value.created)
  {
    value.created = true;
    SnoopTcpFlowKey* key = (SnoopTcpFlowKey*)&it.key();
    emit __tcpFlowCreated(key, &value);
  }
  value.packets++;
  value.bytes += packet->pktHdr->caplen;
  value.ts = packet->pktHdr->ts;

  packet->flowKey   = &key;
  packet->flowValue = &value;
  emit __tcpCaptured(packet);
}

void SnoopFlowMgr::process_UdpFlow(SnoopPacket* packet, SnoopUdpFlowKey& key)
{
  Snoop_UdpFlow_Map::iterator it = udpFlow_Map.find(key);
  if (it == udpFlow_Map.end())
    it = add_UdpFlow(key, packet->pktHdr->ts, true);
  SnoopFlowValue& value = it.value();
  if (!value.created)
  {
    value.created = true;
    SnoopUdpFlowKey* key = (SnoopUdpFlowKey*)&it.key();
    emit __udpFlowCreated(key, &value);
  }
  value.packets++;
  value.bytes += packet->pktHdr->caplen;
  value.ts = packet->pktHdr->ts;

  packet->flowKey   = &key;
  packet->flowValue = &value;
  emit __udpCaptured(packet);
}

void SnoopFlowMgr::load(VXml xml)
{
  SnoopProcess::load(xml);

  checkInterval  = xml.getInt64("checkInterval", checkInterval);
  macFlowTimeout = (long)xml.getInt("macFlowTimeout", (int)macFlowTimeout);
  ipFlowTimeout  = (long)xml.getInt("ipFlowTimeout",  (int)ipFlowTimeout);
  tcpFlowTimeout = (long)xml.getInt("tcpFlowTimeout", (int)tcpFlowTimeout);
  udpFlowTimeout = (long)xml.getInt("udpFlowTimeout", (int)udpFlowTimeout);
}

void SnoopFlowMgr::save(VXml xml)
{
  SnoopProcess::save(xml);

  xml.setInt64("checkInterval", checkInterval);
  xml.setInt("macFlowTimeout", (int)macFlowTimeout);
  xml.setInt("ipFlowTimeout",  (int)ipFlowTimeout);
  xml.setInt("tcpFlowTimeout", (int)tcpFlowTimeout);
  xml.setInt("udpFlowTimeout", (int)udpFlowTimeout);
}

#ifdef QT_GUI_LIB
void SnoopFlowMgr::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  VOptionable::addLineEdit(layout, "leCheckInterval",    "Check Interval(sec)",     QString::number(checkInterval));
  VOptionable::addLineEdit(layout, "leMacFlowTimeout",   "Mac Flow Timeout(sec)",   QString::number(macFlowTimeout));
  VOptionable::addLineEdit(layout, "leIpFlowTimeout",    "IP Flow Timeout(sec)",    QString::number(ipFlowTimeout));
  VOptionable::addLineEdit(layout, "leTcpFlowTimeout",   "TCP Flow Timeout(sec)",   QString::number(tcpFlowTimeout));
  VOptionable::addLineEdit(layout, "leUdpFlowTimeout",   "UDP Flow Timeout(sec)",   QString::number(udpFlowTimeout));
}

void SnoopFlowMgr::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  checkInterval    = dialog->findChild<QLineEdit*>("leCheckInterval")->text().toLongLong();
  macFlowTimeout   = dialog->findChild<QLineEdit*>("leMacFlowTimeout")->text().toLong();
  ipFlowTimeout    = dialog->findChild<QLineEdit*>("leIpFlowTimeout")->text().toLong();
  tcpFlowTimeout   = dialog->findChild<QLineEdit*>("leTcpFlowTimeout")->text().toLong();
  udpFlowTimeout   = dialog->findChild<QLineEdit*>("leUdpFlowTimeout")->text().toLong();
}
#endif // QT_GUI_LIB
