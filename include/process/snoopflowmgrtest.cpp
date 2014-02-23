#include <SnoopFlowMgrTest>
#include <VDebugNew>

REGISTER_METACLASS(SnoopFlowMgrTest, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopFlowMgrTest
// ----------------------------------------------------------------------------
SnoopFlowMgrTest::SnoopFlowMgrTest(void* owner) : SnoopProcess(owner)
{
  flowMgr       = NULL;
  memSize       = 0;
  macFlowOffset = 0;
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
  macFlowOffset = flowMgr->requestMemory_MacFlow(this, memSize);

  return SnoopProcess::doOpen();
}

bool SnoopFlowMgrTest::doClose()
{
  return SnoopProcess::doClose();
}

void SnoopFlowMgrTest::onNew_MacFlow(SnoopMacFlowKey& key)
{
  LOG_DEBUG("srcMac=%s dstMac=%s", qPrintable(key.srcMac.str()), qPrintable(key.dstMac.str()));
}

void SnoopFlowMgrTest::onDel_MacFlow(SnoopMacFlowKey& key)
{
  LOG_DEBUG("srcMac=%s dstMac=%s", qPrintable(key.srcMac.str()), qPrintable(key.dstMac.str()));
}

void SnoopFlowMgrTest::process_MacFlow(SnoopPacket* packet)
{
  BYTE* mem = packet->flowValue->totalMem + macFlowOffset;
  LOG_DEBUG("srcMac=%s dstMac=%s mem=%p", qPrintable(packet->ethHdr->ether_shost.str()), qPrintable(packet->ethHdr->ether_dhost.str()), mem);
}

void SnoopFlowMgrTest::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString flowMgrName = xml.getStr("flowMgr", "");
  if (flowMgrName != "") flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(flowMgrName));
  memSize = (int)xml.getInt("memSize", (int)memSize);
}

void SnoopFlowMgrTest::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString flowMgrName = flowMgr == NULL ? "" : flowMgr->name;
  xml.setStr("flowMgr", flowMgrName);
  xml.setInt("memSize", (int)memSize);
}

#ifdef QT_GUI_LIB
void SnoopFlowMgrTest::addOptionWidget(QLayout* layout)
{
  SnoopProcess::addOptionWidget(layout);

  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  VOptionable::addComboBox(layout, "cbxFlowMgr", "FlowMgr", flowMgrList, -1, flowMgr == NULL ? "" : flowMgr->name);
  VOptionable::addLineEdit(layout, "leMemSize", "Mem Size", QString::number(memSize));
}

void SnoopFlowMgrTest::saveOptionDlg(QDialog* dialog)
{
  SnoopProcess::saveOptionDlg(dialog);

  flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxFlowMgr")->currentText()));
  memSize = (size_t)dialog->findChild<QLineEdit*>("leMemSize")->text().toInt();
}
#endif // QT_GUI_LIB
