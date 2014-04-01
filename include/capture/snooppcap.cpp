#include <SnoopPcap>
//#include <VDebugNew>

#ifndef PCAP_OPENFLAG_PROMISCUOUS
#define PCAP_OPENFLAG_PROMISCUOUS 1
#endif // PCAP_OPENFLAG_PROMISCUOUS

// ----------------------------------------------------------------------------
// SnoopPcap
// ----------------------------------------------------------------------------
SnoopPcap::SnoopPcap(void* owner) : SnoopCapture(owner)
{
  filter       = "";
  snapLen      = snoop::DEFAULT_SNAPLEN;
  flags        = PCAP_OPENFLAG_PROMISCUOUS;
  readTimeout  = snoop::DEFAULT_READTIMEOUT;
  m_pcap       = NULL;
  m_dataLink   = DLT_NULL;
  m_source     = "";
}

SnoopPcap::~SnoopPcap()
{
  close();
}

bool SnoopPcap::doOpen()
{
  return SnoopCapture::doOpen();
}

bool SnoopPcap::doClose()
{
  // ----- by gilgil 2009.09.01 -----
  //
  // Strange to say, when pcap_next_ex is called after pcap_close is called, it occurs memory problem.
  // So waits until thread is terminated.
  //
  if (runThread().active())
  {
    runThread().close(false);
    runThread().wait();
  }
  // --------------------------------
  if (m_pcap != NULL)
  {
    pcap_close(m_pcap);
  }
  m_dataLink = DLT_NULL;
  m_source   = "";

  bool res = SnoopCapture::doClose();

  if (m_pcap != NULL)
  {
    m_pcap = NULL;
  }
  return res;
}

int SnoopPcap::read(SnoopPacket* packet)
{
  if (m_state != VState::Opened)
  {
    SET_DEBUG_ERROR(VError, qformat("not opened state(%s)", qPrintable(className())), VERR_NOT_OPENED_STATE);
    return VERR_FAIL;
  }
  LOG_ASSERT(m_pcap != NULL);
  
  packet->clear();
  int res;
  int i = pcap_next_ex(m_pcap, (pcap_pkthdr**)&packet->pktHdr, (const u_char**)&packet->pktData);
  switch (i) {
    case -2: // if EOF was reached reading from an offline capture
      SET_DEBUG_ERROR(SnoopError, qformat("pcap_next_ex return -2(%s)", pcap_geterr(m_pcap)), VERR_IN_PCAP_NEXT_EX);
      res = VERR_FAIL;
      break;
    case -1: // if an error occurred
      SET_DEBUG_ERROR(SnoopError, qformat("pcap_next_ex return -1(%s)", pcap_geterr(m_pcap)), VERR_IN_PCAP_NEXT_EX);
      res = VERR_FAIL;
      break;
    case 0 : // if the timeout occurs
      res = 0;
      break;
    default: // packet captured
      res = packet->pktHdr->caplen;
      packet->linkType = dataLink();
      if (autoParse) parse(packet);
      break;
  }
  return res;
}

int SnoopPcap::write(SnoopPacket* packet)
{
  return write(packet->pktData, packet->pktHdr->caplen);
}

int SnoopPcap::write(u_char* buf, int size, WINDIVERT_ADDRESS* divertAddr)
{
  Q_UNUSED(divertAddr)
  int res = pcap_sendpacket(m_pcap, (const u_char*)buf, size);
  if (res == 0) return size;
  LOG_ERROR("pcap_sendpacket return %d", res); // may be -1? // gilgil temp 2013.11.29
  return VERR_FAIL;
}

bool SnoopPcap::relay(SnoopPacket* packet)
{
  Q_UNUSED(packet)
  SET_ERROR(SnoopError, "relay not supported", VERR_NOT_SUPPORTED);
  return false;
}

// ----- gilgil temp 2009.08.30 -----
/*
int SnoopPcap::write(u_char* buf, int size)
{
  pcap_send_queue *squeue;
  int res;
  struct pcap_pkthdr pktHdr;

  squeue = pcap_sendqueue_alloc(size + 1000);

  pktHdr.ts.tv_sec = 0;
  pktHdr.ts.tv_usec = 0;
  pktHdr.caplen = size;
  res = pcap_sendqueue_queue(squeue, &pktHdr, (const u_char *)buf);
  if (res == -1) goto _end;

  res = pcap_sendqueue_transmit(pcap, squeue, TRUE);
_end:
  pcap_sendqueue_destroy(squeue);
  return res;
}
*/
// ----------------------------------

bool SnoopPcap::pcapOpen(char* source, pcap_rmtauth* auth, pcap_if_t* dev)
{
#ifdef linux
  Q_UNUSED(auth)
#endif // linux

  LOG_DEBUG("source=%s", source);
  char errBuf[PCAP_ERRBUF_SIZE];
#ifdef WIN32
  m_pcap = pcap_open(source, snapLen, flags, readTimeout, auth, errBuf);
#endif // WIN32
#ifdef linux
  m_pcap = pcap_open_live(source, snapLen, flags, readTimeout, errBuf);
#endif // linux
  if (m_pcap == NULL)
  {
    SET_ERROR(SnoopError, qformat("error in pcap_open(%s)", errBuf), VERR_IN_PCAP_OPEN);
    return false;
  }
  m_dataLink = pcap_datalink(m_pcap);
  if (m_dataLink != DLT_EN10MB)
  {
    LOG_WARN("pcap_datalink return =%d(0x%x) source=%s", m_dataLink, m_dataLink, source);
  }
  m_source = (QString)source;

  bool filtering = true;
  switch (m_dataLink)
  {
    case DLT_NFLOG:
    case DLT_USB_LINUX_MMAPPED:
      filtering = false; break;
  }
  if (filtering && filter != "") if (!pcapProcessFilter(dev)) return false;

  return true;
}

bool SnoopPcap::pcapProcessFilter(pcap_if_t* dev)
{
  u_int uNetMask;
  bpf_program code;

  if (dev != NULL && dev->addresses != NULL && dev->addresses->netmask != NULL)
    uNetMask = ((struct sockaddr_in*)(dev->addresses->netmask))->sin_addr.s_addr;
  else
    uNetMask = 0xFFFFFFFF;
  if (pcap_compile(m_pcap, &code, qPrintable(filter), 1, uNetMask) < 0)
  {
    SET_ERROR(SnoopError, qformat("error in pcap_compile(%s)", pcap_geterr(m_pcap)), VERR_IN_PCAP_COMPILE);
    return false;
  }
  if (pcap_setfilter(m_pcap, &code) < 0)
  {
    SET_ERROR(SnoopError, qformat("error in pcap_setfilter(%s)", pcap_geterr(m_pcap)), VERR_IN_PCAP_SETFILTER);
    return false;
  }
  return true;
}

void SnoopPcap::load(VXml xml)
{
  SnoopCapture::load(xml);

  filter      = xml.getStr("filter", filter);
  snapLen     = xml.getInt("snapLen", snapLen);
  flags       = xml.getInt("flags", flags);
  readTimeout = xml.getInt("readTimeout", readTimeout);
}

void SnoopPcap::save(VXml xml)
{
  SnoopCapture::save(xml);

  xml.setStr("filter", filter);
  xml.setInt("snapLen", snapLen);
  xml.setInt("flags", flags);
  xml.setInt("readTimeout", readTimeout);
}

#ifdef QT_GUI_LIB
void SnoopPcap::optionAddWidget(QLayout* layout)
{
  SnoopCapture::optionAddWidget(layout);

  VOptionable::addLineEdit(layout, "leFilter",      "Filter",       filter);
  VOptionable::addLineEdit(layout, "leSnapLen",     "Snap Len",     QString::number(snapLen));
  VOptionable::addLineEdit(layout, "leFlags",       "Flags",        QString::number(flags));
  VOptionable::addLineEdit(layout, "leReadTimeout", "Read Timeout", QString::number(readTimeout));
}

void SnoopPcap::optionSaveDlg(QDialog* dialog)
{
  SnoopCapture::optionSaveDlg(dialog);

  filter      = dialog->findChild<QLineEdit*>("leFilter")->text();
  snapLen     = dialog->findChild<QLineEdit*>("leSnapLen")->text().toInt();
  flags       = dialog->findChild<QLineEdit*>("leFlags")->text().toInt();
  readTimeout = dialog->findChild<QLineEdit*>("leReadTimeout")->text().toInt();
}
#endif // QT_GUI_LIB
