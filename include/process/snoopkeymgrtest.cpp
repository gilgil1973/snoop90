#include <SnoopKeyMgrTest>

REGISTER_METACLASS(SnoopKeyMgrTest, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopKeyMgrTest
// ----------------------------------------------------------------------------
SnoopKeyMgrTest::SnoopKeyMgrTest(void* owner) : SnoopProcess(owner)
{
  keyMgr = NULL;
}

SnoopKeyMgrTest::~SnoopKeyMgrTest()
{
  close();
}

bool SnoopKeyMgrTest::doOpen()
{
  if (keyMgr == NULL)
  {
    SET_ERROR(SnoopError, "keyMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }
  keyMgr->registerAccessible(this, keyMgr->mac_items, 0, 1024);

  return SnoopProcess::doOpen();
}

bool SnoopKeyMgrTest::doClose()
{
  return SnoopProcess::doClose();
}

void SnoopKeyMgrTest::onNew_Mac(SnoopMacKey* key, int user, void* mem)
{
  LOG_DEBUG("user=%d mem=%p", user, mem);
}

void SnoopKeyMgrTest::onDel_Mac(SnoopMacKey* key, int user, void* mem)
{
  LOG_DEBUG("user=%d mem=%p", user, mem);
}

void SnoopKeyMgrTest::test(SnoopPacket* packet)
{
  LOG_DEBUG("");
}

void SnoopKeyMgrTest::load(VXml xml)
{
  SnoopProcess::load(xml);

  VGraph* graph  = (VGraph*)owner;
  QString keyMgrName = xml.getStr("keyMgr", "");
  if (keyMgrName != "") keyMgr = (SnoopKeyMgr*)graph->objectList.findByName(keyMgrName);
}

void SnoopKeyMgrTest::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString keyMgrName = keyMgr == NULL ? "" : keyMgr->name;
  xml.setStr("keyMgr", keyMgrName);
}

#ifdef QT_GUI_LIB
void SnoopKeyMgrTest::addOptionWidget(QLayout* layout)
{
  SnoopProcess::addOptionWidget(layout);

  QList<VObject*> keyMgrList = ((VGraph*)owner)->objectList.findObjectsByClassName("SnoopKeyMgr");
  QStringList stringList;
  foreach (VObject* obj, keyMgrList) stringList.push_back(obj->name);
  QComboBox* cbxKeyMgr = VOptionable::addComboBox(layout, "cbxKeyMgr", "KeyMgr", stringList, -1);
  cbxKeyMgr->setCurrentText(keyMgr == NULL ? "" : keyMgr->name);
}

void SnoopKeyMgrTest::saveOptionDlg(QDialog* dialog)
{
  SnoopProcess::saveOptionDlg(dialog);

  keyMgr = (SnoopKeyMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxKeyMgr")->currentText()));
}
#endif // QT_GUI_LIB
