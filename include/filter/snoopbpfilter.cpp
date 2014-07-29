#include <SnoopBpFilter>
#include <VDebugNew>

REGISTER_METACLASS(SnoopBpFilter, SnoopFilter)

// ----------------------------------------------------------------------------
// SnoopBpFilter
// ----------------------------------------------------------------------------
SnoopBpFilter::SnoopBpFilter(void* owner) : SnoopFilter(owner)
{
  m_pcap   = NULL;
  m_code   = NULL;
  filter   = "";
  linkType = DLT_EN10MB;
}

SnoopBpFilter::~SnoopBpFilter()
{
  close();
}

bool SnoopBpFilter::doOpen()
{
  int res;

  m_pcap = pcap_open_dead(linkType, snoop::DEFAULT_SNAPLEN); // gilgil temp 2010.09.24
  if (m_pcap == NULL)
  {
    SET_ERROR(SnoopError, "error in pcap_open_dead return NULL", VERR_IN_PCAP_OPEN_DEAD);
    return false;
  }

  m_code = (bpf_program*)malloc(sizeof(bpf_program));
  res = pcap_compile(m_pcap, m_code, qPrintable(filter), 1, 0xFFFFFFFF);
  if (res < 0)
  {
    SET_ERROR(SnoopError, qformat("error in pcap_compile(%s)", pcap_geterr(m_pcap)), VERR_IN_PCAP_COMPILE);
    return false;
  }

  return SnoopFilter::doOpen();
}

bool SnoopBpFilter::doClose()
{
  if (m_pcap != NULL)
  {
    pcap_close(m_pcap);
    m_pcap = NULL;
  }

  if (m_code != NULL)
  {
    pcap_freecode(m_code);
    free(m_code);
    m_code = NULL;
  }

  return SnoopFilter::doClose();
}

bool SnoopBpFilter::_check(BYTE* pktData, UINT pktLen)
{
  if (m_state != VState::Opened)
  {
    SET_ERROR(VError, qformat("not opened state(%s %s)", qPrintable(name), qPrintable(className())), VERR_NOT_OPENED_STATE);
    return false;
  }
  LOG_ASSERT(m_pcap != NULL && m_code != NULL);
  int res = bpf_filter(m_code->bf_insns, (const u_char*)pktData, pktLen, pktLen);
  return res > 0;
}

void SnoopBpFilter::check(SnoopPacket* packet)
{
  bool res = _check(packet->pktData, packet->pktHdr->caplen);
  if (res)
  {
    emit ack(packet);
  } else
  {
    emit nak(packet);
  }
}

void SnoopBpFilter::load(VXml xml)
{
  SnoopFilter::load(xml);

  filter = xml.getStr("filter", filter);
  linkType = xml.getInt("linkType", linkType);
}

void SnoopBpFilter::save(VXml xml)
{
  SnoopFilter::save(xml);

  xml.setStr("filter", filter);
  xml.setInt("linkType", linkType);
}

#ifdef QT_GUI_LIB
void SnoopBpFilter::optionAddWidget(QLayout *layout)
{
  SnoopFilter::optionAddWidget(layout);

  VOptionable::addLineEdit(layout, "leFilter",   "Filter",    filter);
  VOptionable::addLineEdit(layout, "leLinkType", "Link Type", QString::number(linkType));
}

void SnoopBpFilter::optionSaveDlg(QDialog *dialog)
{
  SnoopFilter::optionSaveDlg(dialog);

  filter   = dialog->findChild<QLineEdit*>("leFilter")->text();
  linkType = dialog->findChild<QLineEdit*>("leLinkType")->text().toInt();
}
#endif // QT_GUI_LIB
