#include <SnoopFlowMgr>
#include <VDebugNew>

#include <SnoopEth>
#include <SnoopIp>
#include <SnoopTcp>
#include <SnoopUdp>

REGISTER_METACLASS(SnoopFlowMgr, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopFlowMgrRequesterItem
// ----------------------------------------------------------------------------
SnoopFlowMgrRequesterItem::SnoopFlowMgrRequesterItem()
{
  requester = NULL;
  user      = 0;
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
  lastCheckTick = 0;
  checkInterval = 1000;
}

SnoopFlowMgr::~SnoopFlowMgr()
{
  close();
}

bool SnoopFlowMgr::doOpen()
{
  clearMaps();
  clearItems();
  lastCheckTick = tick();

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

void SnoopFlowMgr::clearItems()
{
  macFlow_Items.clear();
  tcpFlow_Items.clear();
  udpFlow_Items.clear();
}

size_t SnoopFlowMgr::requestMemory(void* requester, SnoopFlowMgrRequesterItems& items, int user, size_t memSize)
{
  size_t currentOffset = 0;
  int _count = items.count();
  for (int i = 0; i < _count; i++)
  {
    const SnoopFlowMgrRequesterItem& item = items.at(i);
    if (item.requester == requester && item.user == user) return currentOffset;
    currentOffset += item.memSize;
  }

  SnoopFlowMgrRequesterItem item;
  item.requester = requester;
  item.user      = user;
  item.offset    = currentOffset;
  item.memSize   = memSize;

  items.push_back(item);
  items.totalMemSize += memSize;

  return currentOffset;
}

size_t SnoopFlowMgr::requestMemory_MacFlow(void* requester, int user, size_t memSize)
{
  return requestMemory(requester, macFlow_Items, user, memSize);
}

void SnoopFlowMgr::process_MacFlow(SnoopPacket* packet, SnoopMacFlowKey& key)
{
  SnoopFlowMgrMap_MacFlow::iterator it = macFlow_Map.find(key);

  BYTE* totalMem;
  if (it != macFlow_Map.end()) totalMem = it.value();
  else totalMem = add_MacFlow(key);
  LOG_ASSERT(totalMem != NULL);
  packet->totalMem = totalMem;
  emit macFlow_Processed(packet);
}

BYTE* SnoopFlowMgr::add_MacFlow(SnoopMacFlowKey& key)
{
  BYTE* totalMem = new BYTE[macFlow_Items.totalMemSize];
  macFlow_Map.insert(key, totalMem);
  emit onNew_MacFlow(key);
  return totalMem;
}

void SnoopFlowMgr::del_MacFlow(SnoopMacFlowKey& key)
{
  emit onDel_MacFlow(key);
  macFlow_Map.remove(key);
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
}

void SnoopFlowMgr::load(VXml xml)
{
  SnoopProcess::load(xml);

  checkInterval = xml.getInt64("checkInterval", checkInterval);
}

void SnoopFlowMgr::save(VXml xml)
{
  SnoopProcess::save(xml);

  xml.setInt64("checkInterval", checkInterval);
}

#ifdef QT_GUI_LIB
void SnoopFlowMgr::addOptionWidget(QLayout* layout)
{
  SnoopProcess::addOptionWidget(layout);

  VOptionable::addLineEdit(layout, "leCheckInterval", "Check Interval", QString::number(checkInterval));
}

void SnoopFlowMgr::saveOptionDlg(QDialog* dialog)
{
  SnoopProcess::saveOptionDlg(dialog);

  checkInterval = dialog->findChild<QLineEdit*>("leCheckInterval")->text().toLongLong();
}
#endif // QT_GUI_LIB
