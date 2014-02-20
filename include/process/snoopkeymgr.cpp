#include <SnoopKeyMgr>
#include <VDebugNew>

#include <SnoopEth>
#include <SnoopIp>
#include <SnoopTcp>
#include <SnoopUdp>

REGISTER_METACLASS(SnoopKeyMgr, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopKeyMgrAccessibleItem
// ----------------------------------------------------------------------------
SnoopKeyMgrAccessibleItem::SnoopKeyMgrAccessibleItem()
{
  accessible = NULL;
  user       = 0;
  offset     = 0;
  memSize    = 0;
};

// ----------------------------------------------------------------------------
// SnoopKeyMgrAccessibleItems
// ----------------------------------------------------------------------------
SnoopKeyMgrAccessibleItems::SnoopKeyMgrAccessibleItems()
{
  totalMemSize = 0;
}

SnoopKeyMgrAccessibleItems::~SnoopKeyMgrAccessibleItems()
{
  // gilgil temp 2014.02.21
}

// ----------------------------------------------------------------------------
// SnoopBlock
// ----------------------------------------------------------------------------
SnoopKeyMgr::SnoopKeyMgr(void* owner) : SnoopProcess(owner)
{
  // gilgil temp 2014.02.19
}

SnoopKeyMgr::~SnoopKeyMgr()
{
  close();
}

bool SnoopKeyMgr::doOpen()
{
  clearMaps();

  return SnoopProcess::doOpen();
}

bool SnoopKeyMgr::doClose()
{
  clearMaps();

  return SnoopProcess::doClose();
}

void SnoopKeyMgr::clearMaps()
{
  mac_map.clear();
  tcpFlow_map.clear();
  udpFlow_map.clear();
}

void SnoopKeyMgr::registerAccessible(ISnoopKeyMgrAccessible* accessible, SnoopKeyMgrAccessibleItems& items, int user, size_t memSize)
{
  size_t currentOffset = 0;
  int _count = items.count();
  for (int i = 0; i < _count; i++)
  {
    SnoopKeyMgrAccessibleItem* item = items.at(i);
    if (item->accessible == accessible) return;
    if (item->user       == user) return;
    currentOffset += item->memSize;
  }

  SnoopKeyMgrAccessibleItem* item = new SnoopKeyMgrAccessibleItem;
  item->accessible = accessible;
  item->user       = user;
  item->offset     = currentOffset;
  item->memSize    = memSize;
  items.push_back(item);

  items.totalMemSize += memSize;
}

void SnoopKeyMgr::processMac(SnoopPacket* packet, Mac* mac)
{
  SnoopMacKey key; key.mac = *mac;
  SnoopKeyMgr_Mac_Map::iterator it = mac_map.find(key);

  void* totalMem;
  if (it != mac_map.end())
  {
    totalMem = it.value();
  } else
  {
    totalMem = new char[mac_items.totalMemSize];
    it = mac_map.insert(key, totalMem);

    foreach (SnoopKeyMgrAccessibleItem* item, mac_items)
    {
      ISnoopKeyMgrAccessible_Mac* mac_accessible = dynamic_cast<ISnoopKeyMgrAccessible_Mac*>(item->accessible);
      LOG_ASSERT(mac_accessible != NULL);
      int user  = item->user;
      void* mem = (void*)((char*)totalMem + item->offset);
      mac_accessible->onNew_Mac(&key, user, mem);
    }
  }
  LOG_ASSERT(totalMem != NULL);

  foreach (SnoopKeyMgrAccessibleItem* item, mac_items)
  {
    ISnoopKeyMgrAccessible_Mac* mac_accessible = dynamic_cast<ISnoopKeyMgrAccessible_Mac*>(item->accessible);
    LOG_ASSERT(mac_accessible != NULL);
    packet->user = item->user;
    packet->mem  = (void*)((char*)totalMem + item->offset);
    emit processed(packet);
  }
}

void SnoopKeyMgr::process(SnoopPacket* packet)
{
  LOG_DEBUG(""); // gilgil temp 2014.02.19
  if (!SnoopEth::parseAll(packet)) return;

  //
  // Mac
  //
  Mac mac;
  mac = packet->ethHdr->ether_dhost;
  processMac(packet, &mac);
  mac = packet->ethHdr->ether_shost;
  processMac(packet, &mac);
}

void SnoopKeyMgr::load(VXml xml)
{
  SnoopProcess::load(xml);

  //drop = xml.getBool("drop", drop); // gilgil temp 2014.02.19
}

void SnoopKeyMgr::save(VXml xml)
{
  SnoopProcess::save(xml);

  //xml.setBool("drop", drop); // gilgil temp 2014.02.19
}

#ifdef QT_GUI_LIB
void SnoopKeyMgr::addOptionWidget(QLayout* layout)
{
  SnoopProcess::addOptionWidget(layout);

  // VOptionable::addCheckBox(layout, "chkDrop", "Drop", drop); // gilgil temp 2014.02.19
}

void SnoopKeyMgr::saveOptionDlg(QDialog* dialog)
{
  SnoopProcess::saveOptionDlg(dialog);

  // drop = dialog->findChild<QCheckBox*>("chkDrop")->checkState() == Qt::Checked; // gilgil temp 2014.02.19
}
#endif // QT_GUI_LIB
