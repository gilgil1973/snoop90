#include <SnoopFlowMgr>
#include <VDebugNew>

#include <SnoopEth>
#include <SnoopIp>
#include <SnoopTcp>
#include <SnoopUdp>

REGISTER_METACLASS(SnoopFlowMgr, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopFlowMgrAccessibleItem
// ----------------------------------------------------------------------------
SnoopFlowMgrAccessibleItem::SnoopFlowMgrAccessibleItem()
{
  accessible = NULL;
  user       = 0;
  offset     = 0;
  memSize    = 0;
};

SnoopFlowMgrAccessibleItem::~SnoopFlowMgrAccessibleItem()
{
}

// ----------------------------------------------------------------------------
// SnoopFlowMgrAccessibleItems
// ----------------------------------------------------------------------------
SnoopFlowMgrAccessibleItems::SnoopFlowMgrAccessibleItems()
{
  totalMemSize = 0;
}

SnoopFlowMgrAccessibleItems::~SnoopFlowMgrAccessibleItems()
{
  // gilgil temp 2014.02.21
}

// ----------------------------------------------------------------------------
// SnoopBlock
// ----------------------------------------------------------------------------
SnoopFlowMgr::SnoopFlowMgr(void* owner) : SnoopProcess(owner)
{
  // gilgil temp 2014.02.19
}

SnoopFlowMgr::~SnoopFlowMgr()
{
  close();
}

bool SnoopFlowMgr::doOpen()
{
  clearMaps();

  return SnoopProcess::doOpen();
}

bool SnoopFlowMgr::doClose()
{
  clearMaps();

  return SnoopProcess::doClose();
}

void SnoopFlowMgr::clearMaps()
{
  macFlow_map.clear();
  tcpFlow_map.clear();
  udpFlow_map.clear();
}

void SnoopFlowMgr::registerAccessible(ISnoopFlowMgrAccessible* accessible, SnoopFlowMgrAccessibleItems& items, int user, size_t memSize)
{
  size_t currentOffset = 0;
  int _count = items.count();
  for (int i = 0; i < _count; i++)
  {
    const SnoopFlowMgrAccessibleItem& item = items.at(i);
    if (item.accessible == accessible) return;
    if (item.user       == user) return;
    currentOffset += item.memSize;
  }

  SnoopFlowMgrAccessibleItem item;
  item.accessible = accessible;
  item.user       = user;
  item.offset     = currentOffset;
  item.memSize    = memSize;
  items.push_back(item);

  items.totalMemSize += memSize;
}


void SnoopFlowMgr::registerAccessible_MacFlow(ISnoopFlowMgrAccessible* accessible, int user, size_t memSize)
{
  registerAccessible(accessible, macFlow_items, user, memSize);
}

void SnoopFlowMgr::fireAllOnNew_MacFlow(SnoopMacFlowKey& key, void* totalMem)
{
  foreach (const SnoopFlowMgrAccessibleItem& item, macFlow_items)
  {
    ISnoopFlowMgrAccessible_MacFlow* accessible = dynamic_cast<ISnoopFlowMgrAccessible_MacFlow*>(item.accessible);
    LOG_ASSERT(accessible != NULL);
    int user  = item.user;
    void* mem = (void*)((char*)totalMem + item.offset);
    accessible->onNew_MacFlow(key, user, mem);
  }
}

void SnoopFlowMgr::fireAllOnDel_MacFlow(SnoopMacFlowKey& key, void* totalMem)
{
  foreach (const SnoopFlowMgrAccessibleItem& item, macFlow_items)
  {
    ISnoopFlowMgrAccessible_MacFlow* accessible = dynamic_cast<ISnoopFlowMgrAccessible_MacFlow*>(item.accessible);
    LOG_ASSERT(accessible != NULL);
    int user  = item.user;
    void* mem = (void*)((char*)totalMem + item.offset);
    accessible->onDel_MacFlow(key, user, mem);
  }
}

void SnoopFlowMgr::process_MacFlow(SnoopPacket* packet, SnoopMacFlowKey& key)
{
  SnoopFlowMgrMap_MacFlow::iterator it = macFlow_map.find(key);

  void* totalMem;
  if (it != macFlow_map.end())
  {
    totalMem = it.value();
  } else
  {
    totalMem = new char[macFlow_items.totalMemSize];
    it = macFlow_map.insert(key, totalMem);
    fireAllOnNew_MacFlow(key, totalMem);
  }
  LOG_ASSERT(totalMem != NULL);

  foreach (const SnoopFlowMgrAccessibleItem& item, macFlow_items)
  {
    ISnoopFlowMgrAccessible* accessible = item.accessible;
    LOG_ASSERT(accessible != NULL);
    packet->user = item.user;
    packet->mem  = (void*)((char*)totalMem + item.offset);
    emit processed(packet);
  }
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

  //drop = xml.getBool("drop", drop); // gilgil temp 2014.02.19
}

void SnoopFlowMgr::save(VXml xml)
{
  SnoopProcess::save(xml);

  //xml.setBool("drop", drop); // gilgil temp 2014.02.19
}

#ifdef QT_GUI_LIB
void SnoopFlowMgr::addOptionWidget(QLayout* layout)
{
  SnoopProcess::addOptionWidget(layout);

  // VOptionable::addCheckBox(layout, "chkDrop", "Drop", drop); // gilgil temp 2014.02.19
}

void SnoopFlowMgr::saveOptionDlg(QDialog* dialog)
{
  SnoopProcess::saveOptionDlg(dialog);

  // drop = dialog->findChild<QCheckBox*>("chkDrop")->checkState() == Qt::Checked; // gilgil temp 2014.02.19
}
#endif // QT_GUI_LIB
