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
    flowMgr->checkConnect(SIGNAL(macFlowCreated(SnoopMacFlowKey*)), this, SLOT(macFlowCreate(SnoopMacFlowKey*)), true);
    flowMgr->checkConnect(SIGNAL(macFlowDeleted(SnoopMacFlowKey*)), this, SLOT(macFlowDelete(SnoopMacFlowKey*)), true);
  }

  //
  // IpFlow
  //
  if (ipFlowEnabled)
  {
    ipFlowOffset = flowMgr->requestMemory_IpFlow(this, ipFlowMemSize);
    flowMgr->checkConnect(SIGNAL(ipFlowCreated(SnoopIpFlowKey*)), this, SLOT(ipFlowCreate(SnoopIpFlowKey*)), true);
    flowMgr->checkConnect(SIGNAL(ipFlowDeleted(SnoopIpFlowKey*)), this, SLOT(ipFlowDelete(SnoopIpFlowKey*)), true);
  }

  //
  // TcpFlow
  //
  if (tcpFlowEnabled)
  {
    tcpFlowOffset = flowMgr->requestMemory_TcpFlow(this, tcpFlowMemSize);
    flowMgr->checkConnect(SIGNAL(tcpFlowCreated(SnoopTcpFlowKey*)), this, SLOT(tcpFlowCreate(SnoopTcpFlowKey*)), true);
    flowMgr->checkConnect(SIGNAL(tcpFlowDeleted(SnoopTcpFlowKey*)), this, SLOT(tcpFlowDelete(SnoopTcpFlowKey*)), true);
  }

  //
  // UdpFlow
  //
  if (udpFlowEnabled)
  {
    udpFlowOffset = flowMgr->requestMemory_UdpFlow(this, udpFlowMemSize);
    flowMgr->checkConnect(SIGNAL(udpFlowCreated(SnoopUdpFlowKey*)), this, SLOT(udpFlowCreate(SnoopUdpFlowKey*)), true);
    flowMgr->checkConnect(SIGNAL(udpFlowDeleted(SnoopUdpFlowKey*)), this, SLOT(udpFlowDelete(SnoopUdpFlowKey*)), true);
  }




  return SnoopProcess::doOpen();
}

bool SnoopFlowMgrTest::doClose()
{
  return SnoopProcess::doClose();
}

void SnoopFlowMgrTest::macFlowCreate(SnoopMacFlowKey* key)
{
  LOG_DEBUG("srcMac=%s dstMac=%s", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()));
}

void SnoopFlowMgrTest::macFlowDelete(SnoopMacFlowKey* key)
{
  LOG_DEBUG("srcMac=%s dstMac=%s", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()));
}

void SnoopFlowMgrTest::macFlowProcess(SnoopPacket* packet)
{
  SnoopMacFlowKey* key = static_cast<SnoopMacFlowKey*>(packet->flowKey);

  size_t packets = packet->flowValue->packets;
  size_t bytes   = packet->flowValue->bytes;
  BYTE* mem = packet->flowValue->totalMem + macFlowOffset;
  LOG_DEBUG("srcMac=%s dstMac=%s pkts=%u bytes=%u mem=%p", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()), packets, bytes, mem);
}

void SnoopFlowMgrTest::ipFlowCreate(SnoopIpFlowKey* key)
{
  LOG_DEBUG("srcIp=%s dstIp=%s", qPrintable(key->srcIp.str()), qPrintable(key->dstIp.str()));
}

void SnoopFlowMgrTest::ipFlowDelete(SnoopIpFlowKey* key)
{
  LOG_DEBUG("srcIp=%s dstIp=%s", qPrintable(key->srcIp.str()), qPrintable(key->dstIp.str()));
}

void SnoopFlowMgrTest::ipFlowProcess(SnoopPacket* packet)
{
  SnoopIpFlowKey* key = static_cast<SnoopIpFlowKey*>(packet->flowKey);

  size_t packets = packet->flowValue->packets;
  size_t bytes   = packet->flowValue->bytes;
  BYTE* mem = packet->flowValue->totalMem + macFlowOffset;
  LOG_DEBUG("srcIp=%s dstIp=%s pkts=%u bytes=%u mem=%p", qPrintable(key->srcIp.str()), qPrintable(key->dstIp.str()), packets, bytes, mem);
}

void SnoopFlowMgrTest::tcpFlowCreate(SnoopIpFlowKey* key)
{

}

void SnoopFlowMgrTest::tcpFlowDelete(SnoopIpFlowKey* key)
{

}

void SnoopFlowMgrTest::tcpFlowProcess(SnoopPacket* packet)
{

}

void SnoopFlowMgrTest::udpFlowCreate(SnoopIpFlowKey* key)
{

}

void SnoopFlowMgrTest::udpFlowDelete(SnoopIpFlowKey* key)
{

}

void SnoopFlowMgrTest::udpFlowProcess(SnoopPacket* packet)
{

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
  tcpFlowEnabled = dialog->findChild<QCheckBox*>("chkMacFlowEnabled")->checkState() == Qt::Checked;
  tcpFlowMemSize = (size_t)dialog->findChild<QLineEdit*>("leMacFlowMemSize")->text().toInt();
  udpFlowEnabled = dialog->findChild<QCheckBox*>("chkMacFlowEnabled")->checkState() == Qt::Checked;
  udpFlowMemSize = (size_t)dialog->findChild<QLineEdit*>("leMacFlowMemSize")->text().toInt();
}
#endif // QT_GUI_LIB
