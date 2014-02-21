#include <SnoopFlowMgrTest>

REGISTER_METACLASS(SnoopFlowMgrTest, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopFlowMgrTest
// ----------------------------------------------------------------------------
SnoopFlowMgrTest::SnoopFlowMgrTest(void* owner) : SnoopProcess(owner)
{
  flowMgr  = NULL;
  user     = 0;
  memSize  = 0;
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
  flowMgr->registerAccessible_MacFlow(this, user, memSize);

  return SnoopProcess::doOpen();
}

bool SnoopFlowMgrTest::doClose()
{
  return SnoopProcess::doClose();
}

void SnoopFlowMgrTest::onNew_MacFlow(SnoopMacFlowKey& key, int user, void* mem)
{
  LOG_DEBUG("srcMac=%s dstMac=%s user=%d mem=%p", qPrintable(key.srcMac.str()), qPrintable(key.dstMac.str()), user, mem);
}

void SnoopFlowMgrTest::onDel_MacFlow(SnoopMacFlowKey& key, int user, void* mem)
{
  LOG_DEBUG("srcMac=%s dstMac=%s user=%d mem=%p", qPrintable(key.srcMac.str()), qPrintable(key.dstMac.str()), user, mem);
}

void SnoopFlowMgrTest::test(SnoopPacket* packet)
{
  LOG_DEBUG("srcMac=%s dstMac=%s user=%d mem=%p", qPrintable(packet->ethHdr->ether_shost.str()), qPrintable(packet->ethHdr->ether_dhost.str()), packet->user, packet->mem);
}

void SnoopFlowMgrTest::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString flowMgrName = xml.getStr("flowMgr", "");
  if (flowMgrName != "") flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(flowMgrName));
  user = xml.getInt("user", user);
  memSize = (int)xml.getInt("memSize", (int)memSize);
}

void SnoopFlowMgrTest::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString flowMgrName = flowMgr == NULL ? "" : flowMgr->name;
  xml.setStr("flowMgr", flowMgrName);
  xml.setInt("user", user);
  xml.setInt("memSize", (int)memSize);
}

#ifdef QT_GUI_LIB
void SnoopFlowMgrTest::addOptionWidget(QLayout* layout)
{
  SnoopProcess::addOptionWidget(layout);

  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  QComboBox* cbxFlowMgr = VOptionable::addComboBox(layout, "cbxFlowMgr", "FlowMgr", flowMgrList, -1);
  cbxFlowMgr->setCurrentText(flowMgr == NULL ? "" : flowMgr->name);
  VOptionable::addLineEdit(layout, "leUser", "User", QString::number(user));
  VOptionable::addLineEdit(layout, "leMemSize", "Mem Size", QString::number(memSize));
}

void SnoopFlowMgrTest::saveOptionDlg(QDialog* dialog)
{
  SnoopProcess::saveOptionDlg(dialog);

  flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxFlowMgr")->currentText()));
  user    = dialog->findChild<QLineEdit*>("leUser")->text().toInt();
  memSize = (size_t)dialog->findChild<QLineEdit*>("leMemSize")->text().toInt();
}
#endif // QT_GUI_LIB
