#include <SnoopCapture>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopCapture
// ----------------------------------------------------------------------------
SnoopCapture::SnoopCapture(void* owner) : VObject(owner)
{
  autoRead  = true;
  packet.clear();
}

SnoopCapture::~SnoopCapture()
{
}

bool SnoopCapture::doOpen()
{
  if (autoRead)
  {
    // ----- by gilgil 2009.08.31 -----
    //
    // There can be a case that even if thread starts,
    // state remains not VState::Opened(still VState::Opening) state.
    // So set m_state into stOpened before open thread.
    //
    this->m_state = VState::Opened;
    runThread().open();
    // --------------------------------
  }
  return true;
}

bool SnoopCapture::doClose()
{
  runThread().close();
  return true;
}

int SnoopCapture::read(SnoopPacket* packet)
{
  Q_UNUSED(packet)
  SET_ERROR(SnoopError, "read not supported", VERR_NOT_READABLE);
  return -1;
}

int SnoopCapture::write(SnoopPacket* packet)
{
  Q_UNUSED(packet)
  SET_ERROR(SnoopError, "write not supported", VERR_NOT_WRITABLE);
  return -1;
}

int SnoopCapture::write(u_char* buf, int size, WINDIVERT_ADDRESS* divertAddr)
{
  Q_UNUSED(buf)
  Q_UNUSED(size)
  Q_UNUSED(divertAddr)
  SET_ERROR(SnoopError, "write not supported", VERR_NOT_WRITABLE);
  return -1;
}

int SnoopCapture::relay(SnoopPacket* packet)
{
  Q_UNUSED(packet)
  SET_ERROR(SnoopError, "relay not supported", VERR_NOT_SUPPORTED);
  return VERR_FAIL;
}

void SnoopCapture::run()
{
  int res;

  emit opened();
  while (runThread().active())
  {
    res = read(&packet);
    if (res == 0) continue;
    if (res < 0) break;
    emit captured(&packet);
    if (captureType() == SnoopCaptureType::InPath)
    {
      if (!packet.block)
        relay(&packet);
    }
  }
  emit closed();
}

void SnoopCapture::load(VXml xml)
{
  VObject::load(xml);
  autoRead = xml.getBool("autoRead", autoRead);
}

void SnoopCapture::save(VXml xml)
{
  VObject::save(xml);
  xml.setBool("autoRead", autoRead);
}

#ifdef QT_GUI_LIB
void SnoopCapture::addOptionWidget(QLayout* layout)
{
  VOptionable::addCheckBox(layout, "chkAutoRead", "Auto Read", autoRead);
}

void SnoopCapture::saveOptionDlg(QDialog* dialog)
{
  autoRead = dialog->findChild<QCheckBox*>("chkAutoRead")->checkState() == Qt::Checked;
}
#endif // QT_GUI_LIB
