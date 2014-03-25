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
    item.sender->write((u_char*)item.buf.data(), item.buf.size(), &item.divertAddr);
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
  timeout = 1000; // 1 sec
  thread  = NULL;
}

SnoopDelay::~SnoopDelay()
{
  close();
}

bool SnoopDelay::doOpen()
{
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
  item.sender     = packet->sender;
  packet->write(item.buf);
  item.divertAddr = packet->divertAddr;

  VLock lock(thread->itemMgr);
  thread->itemMgr.items.push_back(item);
}

void SnoopDelay::load(VXml xml)
{
  SnoopProcess::load(xml);

  timeout = xml.getInt("timeout", (int)timeout);
}

void SnoopDelay::save(VXml xml)
{
  SnoopProcess::save(xml);

  xml.setInt("timeout", (int)timeout);
}

#ifdef QT_GUI_LIB
void SnoopDelay::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  VOptionable::addLineEdit(layout, "leTimeout", "Timeout", QString::number(timeout));
}

void SnoopDelay::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  timeout = (VTimeout)dialog->findChild<QLineEdit*>("leTimeout")->text().toLong();
}
#endif // QT_GUI_LIB
