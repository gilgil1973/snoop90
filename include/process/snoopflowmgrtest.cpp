#include <SnoopFlowMgrTest>
#include <VDebugNew>

REGISTER_METACLASS(SnoopFlowMgrTest, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopFlowMgrTest
// ----------------------------------------------------------------------------
SnoopFlowMgrTest::SnoopFlowMgrTest(void* owner) : SnoopProcess(owner)
{
  flowMgr        = NULL;
  macFlowEnabled = true;
  macFlowMemSize = 0;
  ipFlowEnabled  = true;
  ipFlowMemSize  = 0;
  tcpFlowEnabled = true;
  tcpFlowMemSize = 0;
  udpFlowEnabled = true;
  udpFlowMemSize = 0;

  macFlowOffset  = 0;
  ipFlowOffset   = 0;
  tcpFlowOffset  = 0;
  udpFlowOffset  = 0;
}

SnoopFlowMgrTest::~SnoopFlowMgrTest()
{
  close();
}

bool SnoopFlowMgrTest::doOpen()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }

  //
  // MacFlow
  //
  if (macFlowEnabled)
  {
    macFlowOffset = flowMgr->requestMemory_MacFlow(this, macFlowMemSize);
    flowMgr->connect(SIGNAL(__macFlowCreated(SnoopMacFlowKey*,SnoopFlowValue*)), this, SLOT(__macFlowCreate(SnoopMacFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    flowMgr->connect(SIGNAL(__macFlowDeleted(SnoopMacFlowKey*,SnoopFlowValue*)), this, SLOT(__macFlowDelete(SnoopMacFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    flowMgr->connect(SIGNAL(__macCaptured(SnoopPacket*)), this, SLOT(__macCaptured(SnoopPacket*)), Qt::DirectConnection);
  }

  //
  // IpFlow
  //
  if (ipFlowEnabled)
  {
    ipFlowOffset = flowMgr->requestMemory_IpFlow(this, ipFlowMemSize);
    flowMgr->connect(SIGNAL(__ipFlowCreated(SnoopIpFlowKey*,SnoopFlowValue*)), this, SLOT(__ipFlowCreate(SnoopIpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    flowMgr->connect(SIGNAL(__ipFlowDeleted(SnoopIpFlowKey*,SnoopFlowValue*)), this, SLOT(__ipFlowDelete(SnoopIpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    flowMgr->connect(SIGNAL(__ipCaptured(SnoopPacket*)), this, SLOT(__ipCaptured(SnoopPacket*)), Qt::DirectConnection);
  }

  //
  // TcpFlow
  //
  if (tcpFlowEnabled)
  {
    tcpFlowOffset = flowMgr->requestMemory_TcpFlow(this, tcpFlowMemSize);
    flowMgr->connect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    flowMgr->connect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    flowMgr->connect(SIGNAL(__tcpCaptured(SnoopPacket*)), this, SLOT(__tcpCaptured(SnoopPacket*)), Qt::DirectConnection);
  }

  //
  // UdpFlow
  //
  if (udpFlowEnabled)
  {
    udpFlowOffset = flowMgr->requestMemory_UdpFlow(this, udpFlowMemSize);
    flowMgr->connect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    flowMgr->connect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    flowMgr->connect(SIGNAL(__udpCaptured(SnoopPacket*)), this, SLOT(__udpCaptured(SnoopPacket*)), Qt::DirectConnection);
  }

  return SnoopProcess::doOpen();
}

bool SnoopFlowMgrTest::doClose()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return true;
  }

  //
  // MacFlow
  //
  if (macFlowEnabled)
  {
    flowMgr->disconnect(SIGNAL(__macFlowCreated(SnoopMacFlowKey*,SnoopFlowValue*)), this, SLOT(__macFlowCreate(SnoopMacFlowKey*,SnoopFlowValue*)));
    flowMgr->disconnect(SIGNAL(__macFlowDeleted(SnoopMacFlowKey*,SnoopFlowValue*)), this, SLOT(__macFlowDelete(SnoopMacFlowKey*,SnoopFlowValue*)));
    flowMgr->disconnect(SIGNAL(__macCaptured(SnoopPacket*)), this, SLOT(__macCaptured(SnoopPacket*)));
  }

  //
  // IpFlow
  //
  if (ipFlowEnabled)
  {
    flowMgr->disconnect(SIGNAL(__ipFlowCreated(SnoopIpFlowKey*,SnoopFlowValue*)), this, SLOT(__ipFlowCreate(SnoopIpFlowKey*,SnoopFlowValue*)));
    flowMgr->disconnect(SIGNAL(__ipFlowDeleted(SnoopIpFlowKey*,SnoopFlowValue*)), this, SLOT(__ipFlowDelete(SnoopIpFlowKey*,SnoopFlowValue*)));
    flowMgr->disconnect(SIGNAL(__ipCaptured(SnoopPacket*)), this, SLOT(__ipCaptured(SnoopPacket*)));
  }

  //
  // TcpFlow
  //
  if (tcpFlowEnabled)
  {
    flowMgr->disconnect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)));
    flowMgr->disconnect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)));
    flowMgr->disconnect(SIGNAL(__tcpCaptured(SnoopPacket*)), this, SLOT(__tcpCaptured(SnoopPacket*)));
  }

  //
  // UdpFlow
  //
  if (udpFlowEnabled)
  {
    flowMgr->disconnect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)));
    flowMgr->disconnect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)));
    flowMgr->disconnect(SIGNAL(__udpCaptured(SnoopPacket*)), this, SLOT(__udpCaptured(SnoopPacket*)));
  }

  return SnoopProcess::doClose();
}

void SnoopFlowMgrTest::__macFlowCreate(SnoopMacFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(value)
  LOG_DEBUG("%s > %s", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()));
}

void SnoopFlowMgrTest::__macFlowDelete(SnoopMacFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(value)
  LOG_DEBUG("%s > %s", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()));
}

void SnoopFlowMgrTest::__macCaptured(SnoopPacket* packet)
{
  SnoopMacFlowKey* key = static_cast<SnoopMacFlowKey*>(packet->flowKey);

  size_t packets = packet->flowValue->packets;
  size_t bytes   = packet->flowValue->bytes;
  BYTE* mem = packet->flowValue->totalMem + macFlowOffset;
  LOG_DEBUG("%s > %s pkts=%u bytes=%u mem=%p", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()), packets, bytes, mem);
}

void SnoopFlowMgrTest::__ipFlowCreate(SnoopIpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(value)
  LOG_DEBUG("%s > %s", qPrintable(key->srcIp.str()), qPrintable(key->dstIp.str()));
}

void SnoopFlowMgrTest::__ipFlowDelete(SnoopIpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(value)
  LOG_DEBUG("%s > %s", qPrintable(key->srcIp.str()), qPrintable(key->dstIp.str()));
}

void SnoopFlowMgrTest::__ipCaptured(SnoopPacket* packet)
{
  SnoopIpFlowKey* key = static_cast<SnoopIpFlowKey*>(packet->flowKey);

  size_t packets = packet->flowValue->packets;
  size_t bytes   = packet->flowValue->bytes;
  BYTE* mem = packet->flowValue->totalMem + ipFlowOffset;
  LOG_DEBUG("%s > %s pkts=%u bytes=%u mem=%p", qPrintable(key->srcIp.str()), qPrintable(key->dstIp.str()), packets, bytes, mem);
}

void SnoopFlowMgrTest::__tcpFlowCreate(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(value)
  LOG_DEBUG("%s:%d > %s:%d", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort);
}

void SnoopFlowMgrTest::__tcpFlowDelete(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(value)
  LOG_DEBUG("%s:%d > %s:%d", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort);
}

void SnoopFlowMgrTest::__tcpCaptured(SnoopPacket* packet)
{
  SnoopTcpFlowKey* key = static_cast<SnoopTcpFlowKey*>(packet->flowKey);

  size_t packets = packet->flowValue->packets;
  size_t bytes   = packet->flowValue->bytes;
  BYTE* mem = packet->flowValue->totalMem + tcpFlowOffset;
  LOG_DEBUG("%s:%d > %s:%d pkts=%u bytes=%u mem=%p", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort, packets, bytes, mem);
}

void SnoopFlowMgrTest::__udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(value)
  LOG_DEBUG("%s:%d > %s:%d", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort);
}

void SnoopFlowMgrTest::__udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(value)
  LOG_DEBUG("%s:%d > %s:%d", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort);
}

void SnoopFlowMgrTest::__udpCaptured(SnoopPacket* packet)
{
  SnoopUdpFlowKey* key = static_cast<SnoopUdpFlowKey*>(packet->flowKey);

  size_t packets = packet->flowValue->packets;
  size_t bytes   = packet->flowValue->bytes;
  BYTE* mem = packet->flowValue->totalMem + udpFlowOffset;
  LOG_DEBUG("%s:%d > %s:%d pkts=%u bytes=%u mem=%p", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort, packets, bytes, mem);
}

void SnoopFlowMgrTest::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString flowMgrName = xml.getStr("flowMgr", "");
  if (flowMgrName != "") flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(flowMgrName));
  macFlowEnabled = xml.getBool("macFlowEnabled",     macFlowEnabled);
  macFlowMemSize = (int)xml.getInt("macFlowMemSize", (int)macFlowMemSize);
  ipFlowEnabled = xml.getBool("ipFlowEnabled",       ipFlowEnabled);
  ipFlowMemSize = (int)xml.getInt("ipFlowMemSize",   (int)ipFlowMemSize);
  tcpFlowEnabled = xml.getBool("tcpFlowEnabled",     tcpFlowEnabled);
  tcpFlowMemSize = (int)xml.getInt("tcpFlowMemSize", (int)tcpFlowMemSize);
  udpFlowEnabled = xml.getBool("udpFlowEnabled",     udpFlowEnabled);
  udpFlowMemSize = (int)xml.getInt("udpFlowMemSize", (int)udpFlowMemSize);
}

void SnoopFlowMgrTest::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString flowMgrName = flowMgr == NULL ? "" : flowMgr->name;
  xml.setStr("flowMgr", flowMgrName);
  xml.setBool("macFlowEnabled",     macFlowEnabled);
  xml.setInt("macFlowMemSize", (int)macFlowMemSize);
  xml.setBool("ipFlowEnabled",       ipFlowEnabled);
  xml.setInt("ipFlowMemSize",   (int)ipFlowMemSize);
  xml.setBool("tcpFlowEnabled",     tcpFlowEnabled);
  xml.setInt("tcpFlowMemSize", (int)tcpFlowMemSize);
  xml.setBool("udpFlowEnabled",     udpFlowEnabled);
  xml.setInt("udpFlowMemSize", (int)udpFlowMemSize);
}

#ifdef QT_GUI_LIB
void SnoopFlowMgrTest::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  VOptionable::addComboBox(layout, "cbxFlowMgr", "FlowMgr", flowMgrList, -1, flowMgr == NULL ? "" : flowMgr->name);
  VOptionable::addCheckBox(layout, "chkMacFlowEnabled", "Mac Flow Enabled", macFlowEnabled);
  VOptionable::addLineEdit(layout, "leMacFlowMemSize", "Mac Flow Mem Size", QString::number(macFlowMemSize));
  VOptionable::addCheckBox(layout, "chkIpFlowEnabled", "IP Flow Enabled", ipFlowEnabled);
  VOptionable::addLineEdit(layout, "leIpFlowMemSize", "IP Flow Mem Size", QString::number(ipFlowMemSize));
  VOptionable::addCheckBox(layout, "chkTcpFlowEnabled", "TCP Flow Enabled", tcpFlowEnabled);
  VOptionable::addLineEdit(layout, "leTcpFlowMemSize", "TCP Flow Mem Size", QString::number(tcpFlowMemSize));
  VOptionable::addCheckBox(layout, "chkUdpFlowEnabled", "UDP Flow Enabled", udpFlowEnabled);
  VOptionable::addLineEdit(layout, "leUdpFlowMemSize", "UDP Flow Mem Size", QString::number(udpFlowMemSize));
}

void SnoopFlowMgrTest::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxFlowMgr")->currentText()));
  macFlowEnabled = dialog->findChild<QCheckBox*>("chkMacFlowEnabled")->checkState() == Qt::Checked;
  macFlowMemSize = (size_t)dialog->findChild<QLineEdit*>("leMacFlowMemSize")->text().toInt();
  ipFlowEnabled  = dialog->findChild<QCheckBox*>("chkIpFlowEnabled")->checkState() == Qt::Checked;
  ipFlowMemSize  = (size_t)dialog->findChild<QLineEdit*>("leIpFlowMemSize")->text().toInt();
  tcpFlowEnabled = dialog->findChild<QCheckBox*>("chkTcpFlowEnabled")->checkState() == Qt::Checked;
  tcpFlowMemSize = (size_t)dialog->findChild<QLineEdit*>("leTcpFlowMemSize")->text().toInt();
  udpFlowEnabled = dialog->findChild<QCheckBox*>("chkUdpFlowEnabled")->checkState() == Qt::Checked;
  udpFlowMemSize = (size_t)dialog->findChild<QLineEdit*>("leUdpFlowMemSize")->text().toInt();
}
#endif // QT_GUI_LIB
