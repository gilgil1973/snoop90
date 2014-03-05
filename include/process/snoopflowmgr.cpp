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
// SnoopFlowMgrRequesterItem
// ----------------------------------------------------------------------------
SnoopFlowMgrRequesterItem::SnoopFlowMgrRequesterItem()
{
  requester = NULL;
  offset    = 0;
  memSize   = 0;
};

SnoopFlowMgrRequesterItem::~SnoopFlowMgrRequesterItem()
{
}

// ----------------------------------------------------------------------------
// SnoopFlowMgrRequesterItems
// ----------------------------------------------------------------------------
SnoopFlowMgrRequesterItems::SnoopFlowMgrRequesterItems()
{
  totalMemSize = 0;
}

SnoopFlowMgrRequesterItems::~SnoopFlowMgrRequesterItems()
{
  // gilgil temp 2014.02.21
}

// ----------------------------------------------------------------------------
// SnoopBlock
// ----------------------------------------------------------------------------
SnoopFlowMgr::SnoopFlowMgr(void* owner) : SnoopProcess(owner)
{
  lastCheckTickInSec   = 0;
  checkIntervalInSec   = 1;
  macFlow_TimeoutInSec = 60 * 60; // 1 hour
}

SnoopFlowMgr::~SnoopFlowMgr()
{
  close();
}

bool SnoopFlowMgr::doOpen()
{
  clearMaps();
  clearItems();
  lastCheckTickInSec = 0;

  return SnoopProcess::doOpen();
}

bool SnoopFlowMgr::doClose()
{
  while(macFlow_Map.count() > 0)
  {
    const SnoopMacFlowKey& key = macFlow_Map.begin().key();
    del_MacFlow((SnoopMacFlowKey&)key);
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
  Snoop_MacFlow_Map::iterator it = macFlow_Map.begin();
  while (it != macFlow_Map.end())
  {
    const SnoopFlowValue& value = it.value();
    long elapsedInSec = ts.tv_sec - value.ts.tv_sec;
    if (elapsedInSec >= macFlow_TimeoutInSec)
    {
      it = del_MacFlow((SnoopMacFlowKey&)it.key());
      continue;
    }
    it++;
  }
}

void SnoopFlowMgr::clearItems()
{
  macFlow_Items.clear();
  tcpFlow_Items.clear();
  udpFlow_Items.clear();
}

size_t SnoopFlowMgr::requestMemory(void* requester, SnoopFlowMgrRequesterItems& items, size_t memSize)
{
  size_t currentOffset = 0;
  int _count = items.count();
  for (int i = 0; i < _count; i++)
  {
    const SnoopFlowMgrRequesterItem& item = items.at(i);
    if (item.requester == requester) return currentOffset;
    currentOffset += item.memSize;
  }

  SnoopFlowMgrRequesterItem item;
  item.requester = requester;
  item.offset    = currentOffset;
  item.memSize   = memSize;

  items.push_back(item);
  items.totalMemSize += memSize;

  return currentOffset;
}

size_t SnoopFlowMgr::requestMemory_MacFlow(void* requester, size_t memSize)
{
  return requestMemory(requester, macFlow_Items, memSize);
}

void SnoopFlowMgr::process_MacFlow(SnoopPacket* packet, SnoopMacFlowKey& key)
{
  Snoop_MacFlow_Map::iterator it = macFlow_Map.find(key);

  if (it == macFlow_Map.end())
    it = add_MacFlow(key, packet->pktHdr->ts);

  SnoopFlowValue& value = it.value();
  value.packets++;
  value.bytes += packet->pktHdr->caplen;
  value.ts = packet->pktHdr->ts;

  packet->flowKey   = &key;
  packet->flowValue = &value;
  emit macFlow_Processed(packet);
}

Snoop_MacFlow_Map::iterator SnoopFlowMgr::add_MacFlow(SnoopMacFlowKey& key, struct timeval ts)
{
  SnoopFlowValue value;
  value.ts       = ts;
  value.totalMem = new BYTE[macFlow_Items.totalMemSize];
  value.packets  = 0;
  value.bytes    = 0;
  Snoop_MacFlow_Map::iterator it = macFlow_Map.insert(key, value);
  emit onNew_MacFlow(&key);
  return it;
}

Snoop_MacFlow_Map::iterator SnoopFlowMgr::del_MacFlow(SnoopMacFlowKey& key)
{
  emit onDel_MacFlow(&key);
  return macFlow_Map.erase(key);
}

void SnoopFlowMgr::process(SnoopPacket* packet)
{
  if (!SnoopEth::parseAll(packet)) return;

  {
    //
    // MacFlow
    //
    SnoopMacFlowKey key;
    key.srcMac = packet->ethHdr->ether_shost;
    key.dstMac = packet->ethHdr->ether_dhost;
    process_MacFlow(packet, key);
  }

  long now = packet->pktHdr->ts.tv_sec;
  if (checkIntervalInSec != 0 && now - lastCheckTickInSec >= checkIntervalInSec)
  {
    deleteOldMaps(packet->pktHdr->ts);
    lastCheckTickInSec = now;
  }
}

void SnoopFlowMgr::load(VXml xml)
{
  SnoopProcess::load(xml);

  checkIntervalInSec   = xml.getInt64("checkIntervalInSec", checkIntervalInSec);
  macFlow_TimeoutInSec = (long)xml.getInt("macFlow_TimeoutInSec", (int)macFlow_TimeoutInSec);
}

void SnoopFlowMgr::save(VXml xml)
{
  SnoopProcess::save(xml);

  xml.setInt64("checkIntervalInSec", checkIntervalInSec);
  xml.setInt("macFlow_TimeoutInSec", (int)macFlow_TimeoutInSec);
}

#ifdef QT_GUI_LIB
void SnoopFlowMgr::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  VOptionable::addLineEdit(layout, "leCheckIntervalInSec",   "Check Interval(sec)",       QString::number(checkIntervalInSec));
  VOptionable::addLineEdit(layout, "leMacFlow_TimeoutInSec", "MacFlow Timeout(sec)", QString::number(macFlow_TimeoutInSec));
}

void SnoopFlowMgr::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  checkIntervalInSec   = dialog->findChild<QLineEdit*>("leCheckIntervalInSec")->text().toLongLong();
  macFlow_TimeoutInSec = dialog->findChild<QLineEdit*>("leMacFlow_TimeoutInSec")->text().toLong();
}
#endif // QT_GUI_LIB
