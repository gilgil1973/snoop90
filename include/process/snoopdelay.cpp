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
    writer->write((u_char*)item.buf.data(), item.buf.size(), &item.divertAddr);
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
  itemMgr.writer = delay->writer;
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
  writer  = NULL;
  timeout = 1000; // 1 sec
  thread  = NULL;
}

SnoopDelay::~SnoopDelay()
{
  close();
}

bool SnoopDelay::doOpen()
{
  if (writer == NULL)
  {
    SET_ERROR(SnoopError, "writer is null", VERR_OBJECT_IS_NULL);
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
  packet->drop = true;

  SnoopDelayItem item;
  item.tick = tick() + this->timeout;
  packet->write(item.buf);
  item.divertAddr = packet->divertAddr;

  VLock lock(thread->itemMgr);
  thread->itemMgr.items.push_back(item);
}

void SnoopDelay::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString writerName = xml.getStr("writer", "");
  if (writerName != "") writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(writerName));
  timeout = xml.getULong("timeout", timeout);
}

void SnoopDelay::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString writerName = writer == NULL ? "" : writer->name;
  xml.setStr("writer", writerName);
  xml.setULong("timeout", timeout);
}

#ifdef QT_GUI_LIB
void SnoopDelay::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList writerList = ((VGraph*)owner)->objectList.findNamesByCategoryName("SnoopCapture");
  VOptionable::addComboBox(layout, "cbxWriter", "Writer", writerList, -1, writer == NULL ? "" : writer->name);
  VOptionable::addLineEdit(layout, "leTimeout", "Timeout", QString::number(timeout));
}

void SnoopDelay::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxWriter")->currentText()));
  timeout = (VTimeout)dialog->findChild<QLineEdit*>("leTimeout")->text().toLong();
}
#endif // QT_GUI_LIB
