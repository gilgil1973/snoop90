#include <SnoopKeyMgr>
#include <VDebugNew>

REGISTER_METACLASS(SnoopKeyMgr, SnoopProcess)

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

void SnoopKeyMgr::processMac(SnoopPacket* packet, Mac* mac)
{
  SnoopMacKey key; key.mac = *mac;
  SnoopKeyMgr_Mac_Map::iterator it = mac_map.find(key);

  void* totalMem;
  if (it != mac_map.end())
  {
    totalMem = it.value();
  }
  if (it == mac_map.end())
  {
    totalMem = new char[total_mac_userSize];
    it = mac_map.insert(key, totalMem);
    foreach (SnoopKeyMgr_Mac_Accessible* accessible, mac_accessibles)
    {
      int user  = accessible->user;
      void* mem = (void*)((char*)totalMem + accessible->offset);
      accessible->onNew_Mac(&key, user, mem);
    }
  }
  LOG_ASSERT(totalMem != NULL);

  foreach(SnoopKeyMgr_Mac_Accessible* mac_accessible, mac_accessibles)
  {
    packet->user  = mac_accessible->user;
    packet->mem = ((char*)totalMem + mac_accessible->offset);
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
