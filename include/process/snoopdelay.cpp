#include <SnoopDelay>
#include <VDebugNew>

REGISTER_METACLASS(SnoopDelay, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopDelayItemMgr
// ----------------------------------------------------------------------------
SnoopDelayItemMgr::SnoopDelayItemMgr()
{
}

SnoopDelayItemMgr::~SnoopDelayItemMgr()
{
  flush(0);
}

int SnoopDelayItemMgr::flush(VTick now)
{
  int res = 0;
  VLock lock(*this);
  while (true)
  {
    QList<SnoopDelayItem>::iterator it = items.begin();
    if (it == items.end()) break;
    SnoopDelayItem& item = *it;
    if (now < item.tick) break;
    capture->write(&item.packet);
    items.removeAt(0);
    res++;
  }
  return res;
}

// ----------------------------------------------------------------------------
// SnoopDelayThread
// ----------------------------------------------------------------------------
SnoopDelayThread::SnoopDelayThread(SnoopDelay* delay) : VThread(delay)
{
  itemMgr.capture = delay->capture;
}

SnoopDelayThread::~SnoopDelayThread()
{
  itemMgr.flush();
  close();
}

void SnoopDelayThread::run()
{
  while (active())
  {
    msleep(1);
    volatile VTick now = tick();
    itemMgr.flush(now);
  }
}

// ----------------------------------------------------------------------------
// SnoopDelay
// ----------------------------------------------------------------------------
SnoopDelay::SnoopDelay(void* owner) : SnoopProcess(owner)
{
  capture = NULL;
  timeout = 1000; // 1 sec
  thread  = NULL;
}

SnoopDelay::~SnoopDelay()
{
  close();
}

bool SnoopDelay::doOpen()
{
  if (capture == NULL)
  {
    SET_ERROR(SnoopError, "capture is null", VERR_OBJECT_IS_NULL);
    return false;
  }
  thread = new SnoopDelayThread(this);
  thread->open();
  return SnoopProcess::doOpen();
}

bool SnoopDelay::doClose()
{
  SAFE_DELETE(thread);
  return SnoopProcess::doClose();
}

void SnoopDelay::delay(SnoopPacket* packet)
{
  packet->block = true;

  SnoopDelayItem item;
  item.tick = tick() + this->timeout;
  item.packet = *packet;

  VLock lock(thread->itemMgr);
  thread->itemMgr.items.push_back(item);
}

void SnoopDelay::load(VXml xml)
{
  SnoopProcess::load(xml);

  VGraph* graph  = (VGraph*)owner;
  QString captureName = xml.getStr("capture", "");
  if (captureName != "") capture = (SnoopCapture*)graph->objectList.findByName(captureName);
  timeout = xml.getInt("timeout", (int)timeout);
}

void SnoopDelay::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString captureName = capture == NULL ? "" : capture->name;
  xml.setStr("capture", captureName);
  xml.setInt("timeout", (int)timeout);
}

#ifdef QT_GUI_LIB
void SnoopDelay::addOptionWidget(QLayout* layout)
{
  SnoopProcess::addOptionWidget(layout);

  QList<VObject*> captureList = ((VGraph*)owner)->objectList.findChildren("SnoopCapture");
  QStringList stringList;
  foreach (VObject* obj, captureList)
  {
    stringList.push_back(obj->name);
  }
  QComboBox* cbxCapture = VOptionable::addComboBox(layout, "cbxCapture", "Capture", stringList, -1);
  cbxCapture->setCurrentText(capture == NULL ? "" : capture->name);
  VOptionable::addLineEdit(layout, "leTimeout", "Timeout", QString::number(timeout));
}

void SnoopDelay::saveOptionDlg(QDialog* dialog)
{
  SnoopProcess::saveOptionDlg(dialog);

  capture = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxCapture")->currentText()));
  timeout = (VTimeout)dialog->findChild<QLineEdit*>("leTimeout")->text().toLong();
}
#endif // QT_GUI_LIB
