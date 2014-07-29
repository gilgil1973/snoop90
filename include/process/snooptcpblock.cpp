#include <SnoopTcpBlock>
#include <SnoopEth>
#include <SnoopIp>
#include <SnoopTcp>

REGISTER_METACLASS(SnoopTcpBlock, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopTcpBlock
// ----------------------------------------------------------------------------
SnoopTcpBlock::SnoopTcpBlock(void* owner) : SnoopProcess(owner)
{
  writer         = NULL;
  forwardRst     = true;
  backwardRst    = true;
  forwardFin     = false;
  forwardFinMsg  = "";
  backwardFin    = false;
  backwardFinMsg = "";
}

SnoopTcpBlock::~SnoopTcpBlock()
{
  close();
}

bool SnoopTcpBlock::doOpen()
{
  if (writer == NULL)
  {
    SET_ERROR(SnoopError, "writer is null", VERR_OBJECT_IS_NULL);
    return false;
  }
  if (forwardRst && forwardFin)
  {
    SET_ERROR(SnoopError, "both forwardRst and forwardFin can not be true", VERR_NOT_SUPPORTED);
    return false;
  }
  if (backwardRst && backwardFin)
  {
    SET_ERROR(SnoopError, "both backwardRst and backwardFin can not be true", VERR_NOT_SUPPORTED);
    return false;
  }
  return SnoopProcess::doOpen();
}

bool SnoopTcpBlock::doClose()
{
  return SnoopProcess::doClose();
}

int SnoopTcpBlock::sendForwardBlock(SnoopCapture* capture, SnoopPacket* packet, UINT8 flag, QByteArray msg)
{
  if (packet->proto != IPPROTO_TCP)
  {
    LOG_ERROR("packet->proto is %d", (int)packet->proto);
    return -1;
  }

  int bufSize = sizeof(ETH_HDR) + sizeof(IP_HDR) + sizeof(TCP_HDR) + msg.length();
  QByteArray ba; ba.resize(bufSize); BYTE* buf = (BYTE*)ba.data();

  ETH_HDR* ethHdr = (ETH_HDR*) &buf[0];
  IP_HDR*  ipHdr  = (IP_HDR*)  &buf[sizeof(ETH_HDR)];
  TCP_HDR* tcpHdr = (TCP_HDR*) &buf[sizeof(ETH_HDR) + sizeof(IP_HDR)];
  BYTE*    data   = (BYTE*)    &buf[sizeof(ETH_HDR) + sizeof(IP_HDR) + sizeof(TCP_HDR)];

  //
  // Ethernet Header
  //
  memcpy(ethHdr, packet->ethHdr, sizeof(ETH_HDR));

  //
  // IP Header
  //
  memcpy(ipHdr, packet->ipHdr, sizeof(IP_HDR));
  ipHdr->ip_tos = TCP_BLOCK_TOS_NO; // value of 44 means tag identifier of Snoop Component RST sending.
  ipHdr->ip_len = htons(sizeof(IP_HDR) + sizeof(TCP_HDR) + msg.length());
  ipHdr->ip_ttl = 255;

  //
  // TCP Header
  //
  int tcpDataLen;
  if (!SnoopTcp::isData(packet->ipHdr, packet->tcpHdr, NULL, &tcpDataLen)) tcpDataLen = 0;
  int flagAddLen = ((packet->tcpHdr->th_flags & (TH_SYN | TH_FIN))) ? 1 : 0;
  UINT32 newSeq = ntohl(packet->tcpHdr->th_seq) + tcpDataLen + flagAddLen;

  memcpy(tcpHdr, packet->tcpHdr, sizeof(TCP_HDR));
  tcpHdr->th_seq   = htonl(newSeq);
  tcpHdr->th_off   = sizeof(TCP_HDR) / sizeof(UINT32);
  tcpHdr->th_flags = flag | TH_ACK;
  tcpHdr->th_win   = 0;

  //
  // Data
  //
  memcpy(data, msg.data(), msg.length());

  //
  // Checksum
  //
  tcpHdr->th_sum = htons(SnoopTcp::checksum(ipHdr, tcpHdr));
  ipHdr->ip_sum  = htons(SnoopIp::checksum(ipHdr));


  //
  // Write
  //
  int res = capture->write(buf, bufSize, &packet->divertAddr);
  if (res == VERR_FAIL)
  {
    LOG_ERROR("capture->write return %d", res);
  }
  return res;
}

int SnoopTcpBlock::sendBackwardBlock(SnoopCapture* capture, SnoopPacket* packet, UINT8 flag, QByteArray msg)
{
  if (packet->proto != IPPROTO_TCP)
  {
    LOG_ERROR("packet->proto is %d", (int)packet->proto);
    return -1;
  }

  int bufSize = sizeof(ETH_HDR) + sizeof(IP_HDR) + sizeof(TCP_HDR) + msg.length();
  QByteArray ba; ba.resize(bufSize); BYTE* buf = (BYTE*)ba.data();

  ETH_HDR* ethHdr = (ETH_HDR*) &buf[0];
  IP_HDR*  ipHdr  = (IP_HDR*)  &buf[sizeof(ETH_HDR)];
  TCP_HDR* tcpHdr = (TCP_HDR*) &buf[sizeof(ETH_HDR) + sizeof(IP_HDR)];
  char*    data   = (char*)    &buf[sizeof(ETH_HDR) + sizeof(IP_HDR) + sizeof(TCP_HDR)];

  //
  // Ethernet Header
  //
  ethHdr->ether_dhost = packet->ethHdr->ether_shost;
  ethHdr->ether_shost = packet->ethHdr->ether_dhost;
  ethHdr->ether_type  = packet->ethHdr->ether_type;

  //
  // IP Header
  //
  memcpy(ipHdr, packet->ipHdr, sizeof(IP_HDR));
  ipHdr->ip_tos = TCP_BLOCK_TOS_NO; // value of 44 means tag identifier of Snoop Component RST sending.
  ipHdr->ip_len = htons(sizeof(IP_HDR) + sizeof(TCP_HDR) + msg.length());
  ipHdr->ip_ttl = 255;
  ipHdr->ip_src = packet->ipHdr->ip_dst;
  ipHdr->ip_dst = packet->ipHdr->ip_src;

  //
  // TCP Header
  //
  int tcpDataLen;
  if (!SnoopTcp::isData(packet->ipHdr, packet->tcpHdr, NULL, &tcpDataLen)) tcpDataLen = 0;
  int flagAddLen = ((packet->tcpHdr->th_flags & (TH_SYN | TH_FIN))) ? 1 : 0;
  UINT32 newSeq = ntohl(packet->tcpHdr->th_seq) + tcpDataLen + flagAddLen;

  memcpy(tcpHdr, packet->tcpHdr, sizeof(TCP_HDR));
  tcpHdr->th_sport = packet->tcpHdr->th_dport;
  tcpHdr->th_dport = packet->tcpHdr->th_sport;
  tcpHdr->th_seq   = packet->tcpHdr->th_ack;
  tcpHdr->th_ack   = htonl(newSeq);
  tcpHdr->th_off   = sizeof(TCP_HDR) / sizeof(UINT32);
  tcpHdr->th_flags = flag | TH_ACK;
  tcpHdr->th_win   = 0;

  //
  // Data
  //
  memcpy(data, msg.data(), msg.length());

  //
  // Checksum
  //
  tcpHdr->th_sum = htons(SnoopTcp::checksum(ipHdr, tcpHdr));
  ipHdr->ip_sum  = htons(SnoopIp::checksum(ipHdr));

  //
  // Write
  //
  int res = capture->write(buf, bufSize, &packet->divertAddr);
  if (res == VERR_FAIL)
  {
    LOG_ERROR("capture->write return %d", res);
  }
  return res;
}

void SnoopTcpBlock::tcpBlock(SnoopPacket* packet)
{
  if (packet->tcpHdr == NULL) return;
  if ((packet->tcpHdr->th_flags & (TH_RST | TH_FIN)) != 0) return;
  LOG_DEBUG("BLOCK!!!"); // gilgil temp 2013.11.30

  if (forwardRst)
    sendForwardBlock(writer, packet, TH_RST);
  if (backwardRst)
    sendBackwardBlock(writer, packet, TH_RST);

  if (forwardFin)
    sendForwardBlock(writer, packet, TH_FIN, forwardFinMsg);
  if (backwardFin)
    sendBackwardBlock(writer, packet, TH_FIN, backwardFinMsg);
}

void SnoopTcpBlock::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString writerName = xml.getStr("writer", "");
  if (writerName != "") writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(writerName));
  forwardRst     = xml.getBool("forwardRst",    forwardRst);
  backwardRst    = xml.getBool("backwardRst",   backwardRst);
  forwardFin     = xml.getBool("forwardFin",    forwardFin);
  forwardFinMsg  = xml.getArr("forwardFinMsg",  forwardFinMsg);
  backwardFin    = xml.getBool("backwardFin",   backwardFin);
  backwardFinMsg = xml.getArr("backwardFinMsg", backwardFinMsg);
}

void SnoopTcpBlock::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString writerName = writer == NULL ? "" : writer->name;
  xml.setStr("writer",         writerName);
  xml.setBool("forwardRst",    forwardRst);
  xml.setBool("backwardRst",   backwardRst);
  xml.setBool("forwardFin",    forwardFin);
  xml.setArr("forwardFinMsg",  forwardFinMsg);
  xml.setBool("backwardFin",   backwardFin);
  xml.setArr("backwardFinMsg", backwardFinMsg);
}

#ifdef QT_GUI_LIB
void SnoopTcpBlock::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList writerList = ((VGraph*)owner)->objectList.findNamesByCategoryName("SnoopCapture");
  VOptionable::addComboBox(layout, "cbxWriter",        "Writer",          writerList, -1, writer == NULL ? "" : writer->name);
  VOptionable::addCheckBox(layout, "chkForwardRst",    "Forward Rst",     forwardRst);
  VOptionable::addCheckBox(layout, "chkBackwardRst",   "Backward Rst",    backwardRst);
  VOptionable::addCheckBox(layout, "chkForwardFin",    "Forward Fin",     forwardFin);
  VOptionable::addLineEdit(layout, "leForwardFinMsg",  "Forward Fin Msg", forwardFinMsg);
  VOptionable::addCheckBox(layout, "chkBackwardFin",   "Backward Fin",    backwardFin);
  VOptionable::addLineEdit(layout, "leBackwardFinMsg", "Forward Fin Msg", backwardFinMsg);
}

void SnoopTcpBlock::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  writer         = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxWriter")->currentText()));
  forwardRst     = dialog->findChild<QCheckBox*>("chkForwardRst")->checkState() == Qt::Checked;
  backwardRst    = dialog->findChild<QCheckBox*>("chkBackwardRst")->checkState() == Qt::Checked;
  forwardFin     = dialog->findChild<QCheckBox*>("chkForwardFin")->checkState() == Qt::Checked;
  forwardFinMsg  = qPrintable(dialog->findChild<QLineEdit*>("leForwardFinMsg")->text());
  backwardFin    = dialog->findChild<QCheckBox*>("chkBackwardFin")->checkState() == Qt::Checked;
  backwardFinMsg = qPrintable(dialog->findChild<QLineEdit*>("leBackwardFinMsg")->text());
}
#endif // QT_GUI_LIB
