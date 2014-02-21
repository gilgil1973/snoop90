#include <SnoopCapture>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopCapture
// ----------------------------------------------------------------------------
SnoopCapture::SnoopCapture(void* owner) : VObject(owner)
{
  autoRead    = true;
  parsePacket = true;
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

#include <SnoopIp>  // for SnoopIp::checksum
#include <SnoopTcp> // for SnoopTcp::checksum
#include <SnoopUdp> // for SnoopUdp::checksum
void SnoopCapture::postRead(SnoopPacket* packet)
{
  if (captureType() != SnoopCaptureType::InPath) return;
  if (packet->drop) return;
  {
    if (!packet->drop)
    {
      //
      // checksum
      //
      if (packet->tcpChanged)
      {
        LOG_ASSERT(packet->ipHdr != NULL);
        LOG_ASSERT(packet->tcpHdr != NULL);
        packet->tcpHdr->th_sum = htons(SnoopTcp::checksum(packet->ipHdr, packet->tcpHdr));
      }
      if (packet->udpChanged)
      {
        LOG_ASSERT(packet->ipHdr != NULL);
        LOG_ASSERT(packet->udpHdr != NULL);
        packet->udpHdr->uh_sum = htons(SnoopUdp::checksum(packet->ipHdr, packet->udpHdr));
      }
      if (packet->ipChanged)
      {
        LOG_ASSERT(packet->ipHdr != NULL);
        packet->ipHdr->ip_sum  = htons(SnoopIp::checksum(packet->ipHdr));
      }

      //
      // relay
      //
      relay(packet);
    }
  }
}

void SnoopCapture::parse(SnoopPacket* packet)
{
  if (SnoopEth::parseAll(packet)) return;
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
  SnoopCaptureType _captureType = captureType();

  emit opened();
  while (runThread().active())
  {
    int res = read(&packet);
    if (res == 0) continue;
    if (res < 0) break;
    emit captured(&packet);
    postRead(&packet);
  }
  emit closed();
}

void SnoopCapture::load(VXml xml)
{
  VObject::load(xml);

  autoRead    = xml.getBool("autoRead", autoRead);
  parsePacket = xml.getBool("autoRead", autoRead);
}

void SnoopCapture::save(VXml xml)
{
  VObject::save(xml);

  xml.setBool("autoRead", autoRead);
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
