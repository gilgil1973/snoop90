#include <SnoopFlowMgrTest>

REGISTER_METACLASS(SnoopFlowMgrTest, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopFlowMgrTest
// ----------------------------------------------------------------------------
SnoopFlowMgrTest::SnoopFlowMgrTest(void* owner) : SnoopProcess(owner)
{
  keyMgr  = NULL;
  user    = 0;
  memSize = 0;
}

SnoopFlowMgrTest::~SnoopFlowMgrTest()
{
  close();
}

bool SnoopFlowMgrTest::doOpen()
{
  if (keyMgr == NULL)
  {
    SET_ERROR(SnoopError, "keyMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }
  keyMgr->registerAccessible(this, keyMgr->macFlow_items, user, memSize);

  return SnoopProcess::doOpen();
}

bool SnoopFlowMgrTest::doClose()
{
  return SnoopProcess::doClose();
}

void SnoopFlowMgrTest::onNew_MacFlow(SnoopMacFlowKey* key, int user, void* mem)
{
  LOG_DEBUG("srcMac=%s dstMac=%s user=%d mem=%p", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()), user, mem);
}

void SnoopFlowMgrTest::onDel_MacFlow(SnoopMacFlowKey* key, int user, void* mem)
{
  LOG_DEBUG("srcMac=%s dstMac=%s user=%d mem=%p", qPrintable(key->srcMac.str()), qPrintable(key->dstMac.str()), user, mem);
}

void SnoopFlowMgrTest::test(SnoopPacket* packet)
{
  LOG_DEBUG("srcMac=%s dstMac=%s user=%d mem=%p", qPrintable(packet->ethHdr->ether_shost.str()), qPrintable(packet->ethHdr->ether_dhost.str()), packet->user, packet->mem);
}

void SnoopFlowMgrTest::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString keyMgrName = xml.getStr("keyMgr", "");
  if (keyMgrName != "") keyMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(keyMgrName));
  user = xml.getInt("user", user);
  memSize = (int)xml.getInt("memSize", (int)memSize);
}

void SnoopFlowMgrTest::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString keyMgrName = keyMgr == NULL ? "" : keyMgr->name;
  xml.setStr("keyMgr", keyMgrName);
  xml.setInt("user", user);
  xml.setInt("memSize", (int)memSize);
}

#ifdef QT_GUI_LIB
void SnoopFlowMgrTest::addOptionWidget(QLayout* layout)
{
  SnoopProcess::addOptionWidget(layout);

  QStringList keyMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  QComboBox* cbxKeyMgr = VOptionable::addComboBox(layout, "cbxKeyMgr", "KeyMgr", keyMgrList, -1);
  cbxKeyMgr->setCurrentText(keyMgr == NULL ? "" : keyMgr->name);
  VOptionable::addLineEdit(layout, "leUser", "User", QString::number(user));
  VOptionable::addLineEdit(layout, "leMemSize", "Mem Size", QString::number(memSize));
}

void SnoopFlowMgrTest::saveOptionDlg(QDialog* dialog)
{
  SnoopProcess::saveOptionDlg(dialog);

  keyMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxKeyMgr")->currentText()));
  user    = dialog->findChild<QLineEdit*>("leUser")->text().toInt();
  memSize = (size_t)dialog->findChild<QLineEdit*>("leMemSize")->text().toInt();
}
#endif // QT_GUI_LIB
