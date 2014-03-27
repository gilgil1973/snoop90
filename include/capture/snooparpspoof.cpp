#include <SnoopArpSpoof>
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
#include "../lib/ui_vlistwidget.h"
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

void SnoopArpSpoofInfectThread::run()
{
  VTimeout interval = (VDuration)arpSpoof->infectInterval;

  while (active())
  {
    bool res = event.wait(interval);
    if (res) break;
    {
      VLock lock(arpSpoof->sessionList);
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
  //
  // Inherited open
  //
  QString tempFilter = filter;
  filter = "ip or arp";
  bool res = SnoopAdapter::doOpen();
  filter = tempFilter;
  if (!res) return false;

  //
  // Set realVirtualMac using virtualMac
  //
  if (virtualMac.isClean())
  {
    netInfo.adapterIndex = this->adapterIndex;
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
  return SnoopAdapter::doClose();
}

int SnoopArpSpoof::read(SnoopPacket* packet)
{
  int readLen = SnoopAdapter::read(packet);
  if (readLen <= 0) return readLen;
  return VERR_FAIL; // gilgil temp 2014.03.27
}

int SnoopArpSpoof::write(SnoopPacket* packet)
{
  return SnoopAdapter::write(packet); // gilgil temp 2014.03.27
}

int SnoopArpSpoof::write(u_char* buf, int size, WINDIVERT_ADDRESS* divertAddr)
{
  return SnoopAdapter::write(buf, size, divertAddr); // gilgil temp 2014.03.27
}

bool SnoopArpSpoof::relay(SnoopPacket* packet)
{
  Q_UNUSED(packet)
  return false; // gilgil temp 2014.03.27
}

bool SnoopArpSpoof::retrieveUnknownMacHostList()
{
  bool res;
  VError findHostError;

  //
  // Check if sessionCount is zero.
  //
  if (sessionList.size() == 0)
  {
    SET_ERROR(SnoopError, "session count is zero", ERR_SESSION_COUNT_IS_ZERO);
    goto _error;
  }

  //
  // Check if source ip is same with target ip
  //
  BOOST_FOREACH(SnoopARPSpoofSession* session, sessionList)
  {
    if ((session->senderIP == session->targetIP))
    {
      SET_ERROR(SnoopError, format("source ip is same as target ip(%s)", ip_to_str(session->senderIP).c_str()).c_str(), ERR_THE_SAME_SOURCE_AND_TARGET_IP);
      goto _error;
    }
  }

  //
  // Find all ip and mac
  //
  if (!findHost.open()) goto _error;
  findHost.hostList.clear();
  findHost.hostList.add(netInfo.ip, netInfo.mac);
  BOOST_FOREACH(SnoopARPSpoofSession* session, sessionList)
  {
    findHost.hostList.add(session->senderIP);
    findHost.hostList.add(session->targetIP);
  }
  res = findHost.scan();
  findHostError = findHost.error;
  findHost.close();
  if (!res)
  {
    error = findHostError;
    goto _error;
  }

  //
  // Set mac of sessionList from findHost.
  //
  BOOST_FOREACH(SnoopARPSpoofSession* session, sessionList)
  {
    SnoopHost* host;

    // Set senderMac according to senderIP
    host = findHost.hostList.findByIP(session->senderIP);
    if (host == NULL)
    {
      SET_ERROR(SnoopError, format("can not find host(%s)", ip_to_str(session->senderIP)).c_str(), ERR_CAN_NOT_FIND_HOST);
      goto _error;
    }
    session->senderMac = host->mac;

    // Set targetMac according to targetIP
    host = findHost.hostList.findByIP(session->targetIP);
    if (host == NULL)
    {
      SET_ERROR(SnoopError,
        format("snoop error : can not find host(%s)", ip_to_str(session->targetIP)).c_str(), ERR_CAN_NOT_FIND_HOST);
      goto _error;
    }
    session->targetMac = host->mac;

    // Check if targetMac is same as realVirtualMac
    if (realVirtualMac == session->targetMac)
    {
      SET_ERROR(SnoopError,
        format("real virtual mac(%s) is same as target mac(IP=%s)",
          realVirtualMac.str().c_str(),
          ip_to_str(session->targetIP).c_str()
        ).c_str(),
        ERR_THE_SAME_REAL_AND_TARGET_MAC);
      goto _error;
    }
  }

  return true;
_error:
  return false;
}


void SnoopArpSpoof::process(SnoopPacket* packet)
{
  Q_UNUSED(packet)
  return; // gilgil temp 2014.03.27
}

void SnoopArpSpoof::load(VXml xml)
{
  SnoopAdapter::load(xml);

  // gilgil temp 2014.03.27
}

void SnoopArpSpoof::save(VXml xml)
{
  SnoopAdapter::save(xml);

  // gilgil temp 2014.03.27
}

#ifdef QT_GUI_LIB
void SnoopArpSpoof::optionAddWidget(QLayout* layout)
{
  SnoopAdapter::optionAddWidget(layout);

  // gilgil temp 2014.03.27
}

void SnoopArpSpoof::optionSaveDlg(QDialog* dialog)
{
  SnoopAdapter::optionSaveDlg(dialog);

  // gilgil temp 2014.03.27
}
#endif // QT_GUI_LIB
