#include <SnoopChecksum>
#include <SnoopIp>  // for SnoopIp::checksum
#include <SnoopTcp> // for SnoopTcp::checksum
#include <SnoopUdp> // for SnoopUdp::checksum

REGISTER_METACLASS(SnoopChecksum, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopChecksum
// ----------------------------------------------------------------------------
SnoopChecksum::SnoopChecksum(void* owner) : SnoopProcess(owner)
{
  ipChecksum  = true;
  tcpChecksum = true;
  udpChecksum = true;
}

SnoopChecksum::~SnoopChecksum()
{
  close();
}

void SnoopChecksum::calculate(SnoopPacket* packet)
{
  if (packet->netType != ETHERTYPE_IP) return;
  LOG_ASSERT(packet->ipHdr!= NULL);
  switch (packet->proto)
  {
    case IPPROTO_TCP:
    {
      LOG_ASSERT(packet->tcpHdr != NULL);
      packet->tcpHdr->th_sum = htons(SnoopTcp::checksum(packet->ipHdr, packet->tcpHdr));
      break;
    }
    case IPPROTO_UDP:
    {
      LOG_ASSERT(packet->udpHdr != NULL);
      packet->udpHdr->uh_sum = htons(SnoopUdp::checksum(packet->ipHdr, packet->udpHdr));
      break;
    }
  }
  if (ipChecksum)
  {
    packet->ipHdr->ip_sum = htons(SnoopIp::checksum(packet->ipHdr));
  }
  emit calculated(packet);
}

void SnoopChecksum::load(VXml xml)
{
  SnoopProcess::load(xml);

  ipChecksum  = xml.getBool("ipChecksum",  ipChecksum);
  tcpChecksum = xml.getBool("tcpChecksum", tcpChecksum);
  udpChecksum = xml.getBool("udpChecksum", udpChecksum);
}

void SnoopChecksum::save(VXml xml)
{
  SnoopProcess::save(xml);

  xml.setBool("ipChecksum",  ipChecksum);
  xml.setBool("tcpChecksum", tcpChecksum);
  xml.setBool("udpChecksum", udpChecksum);
}

#ifdef QT_GUI_LIB
void SnoopChecksum::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  VOptionable::addCheckBox(layout, "chkIpChecksum", "IP Checksum",   ipChecksum);
  VOptionable::addCheckBox(layout, "chkTcpChecksum", "TCP Checksum", tcpChecksum);
  VOptionable::addCheckBox(layout, "chkUdpChecksum", "UDP Checksum", udpChecksum);
}

void SnoopChecksum::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  ipChecksum  = dialog->findChild<QCheckBox*>("chkIpChecksum")->checkState() == Qt::Checked;
  tcpChecksum = dialog->findChild<QCheckBox*>("chkTcpChecksum")->checkState() == Qt::Checked;
  udpChecksum = dialog->findChild<QCheckBox*>("chkUdpChecksum")->checkState() == Qt::Checked;
}
#endif // QT_GUI_LIB
