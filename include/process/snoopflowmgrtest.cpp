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

  VObjectConnection connection("onNew_MacFlow(SnoopMacFlowKey*)", this, "onNew_MacFlow(SnoopMacFlowKey*)");
  if (flowMgr->connections.indexOf(connection) == -1)
  {
    LOG_WARN("onNew_MacFlow must be connected");
  }

  return SnoopProcess::doOpen();
}

bool SnoopFlowMgrTest::doClose()
{
  return SnoopProcess::doClose();
}

void SnoopFlowMgrTest::onNew_MacFlow(SnoopMacFlowKey* key)
{
  LOG_DEBUG("srcMac=%s dstMac=%s", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()));
}

void SnoopFlowMgrTest::onDel_MacFlow(SnoopMacFlowKey* key)
{
  LOG_DEBUG("srcMac=%s dstMac=%s", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()));
}

void SnoopFlowMgrTest::process_MacFlow(SnoopPacket* packet)
{
  SnoopMacFlowKey* key = static_cast<SnoopMacFlowKey*>(packet->flowKey);

  size_t packets = packet->flowValue->packets;
  size_t bytes   = packet->flowValue->bytes;
  BYTE* mem = packet->flowValue->totalMem + macFlowOffset;
  LOG_DEBUG("srcMac=%s dstMac=%s pkts=%u bytes=%u mem=%p", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()), packets, bytes, mem);
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
void SnoopFlowMgrTest::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  VOptionable::addComboBox(layout, "cbxFlowMgr", "FlowMgr", flowMgrList, -1, flowMgr == NULL ? "" : flowMgr->name);
  VOptionable::addLineEdit(layout, "leMemSize", "Mem Size", QString::number(memSize));
}

void SnoopFlowMgrTest::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxFlowMgr")->currentText()));
  memSize = (size_t)dialog->findChild<QLineEdit*>("leMemSize")->text().toInt();
}
#endif // QT_GUI_LIB
