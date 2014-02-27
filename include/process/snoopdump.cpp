#include <SnoopDump>

#include <VFile> // for FileError
#include <VDebugNew>

REGISTER_METACLASS(SnoopDump, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopDump
// ----------------------------------------------------------------------------
const char* SnoopDump::DEFAULT_DUMP_FILE_NAME = "pcap/%04d%02d%02d.%02d%02d.%02d.%03d.pcap";

SnoopDump::SnoopDump(void* owner) : SnoopProcess(owner)
{
  m_pcap        = NULL;
  m_pcap_dumper = NULL;
  filePath      = DEFAULT_DUMP_FILE_NAME;
  linkType      = DLT_EN10MB;
}

SnoopDump::~SnoopDump()
{
  close();
}

bool SnoopDump::doOpen()
{
  m_pcap = pcap_open_dead(linkType, snoop::DEFAULT_SNAPLEN);
  if (m_pcap == NULL)
  {
    SET_ERROR(SnoopError, "error in pcap_open_dead return NULL", VERR_IN_PCAP_OPEN_DEAD);
    return false;
  }

  if (filePath == "")
  {
    SET_ERROR(VFileError, "file name not specified", VERR_FILENAME_NOT_SPECIFIED);
    return false;
  }

  QString _path     = QFileInfo(filePath).path();
  QString _fileName = QFileInfo(filePath).fileName();
  VFile::createFolder(_path);
  if (_fileName == "") _fileName = DEFAULT_DUMP_FILE_NAME;

  QDateTime now = QDateTime::currentDateTime();
  QString newFileName = qformat(qPrintable(_fileName),
    now.date().year(), now.date().month(), now.date().day(),
    now.time().hour(), now.time().minute(), now.time().second(), now.time().msec());

  m_pcap_dumper = pcap_dump_open(m_pcap, qPrintable(_path + "/" + newFileName));
  if (m_pcap_dumper == NULL)
  {
    SET_ERROR(SnoopError, pcap_geterr(m_pcap), VERR_IN_PCAP_DUMP_OPEN);
    return false;
  }
  return true;
}

bool SnoopDump::doClose()
{
  if (m_pcap_dumper != NULL)
  {
    pcap_dump_close(m_pcap_dumper);
    m_pcap_dumper = NULL;
  }

  if (m_pcap != NULL)
  {
    pcap_close(m_pcap);
    m_pcap = NULL;
  }

  return true;
}

void SnoopDump::dump(SnoopPacket* packet)
{
  LOG_ASSERT(m_pcap_dumper != NULL);
  pcap_dump((u_char*)m_pcap_dumper, packet->pktHdr, (const u_char*)packet->pktData);
  emit dumped(packet);
}

void SnoopDump::load(VXml xml)
{
  SnoopProcess::load(xml);

  filePath = xml.getStr("filePath", filePath);
  linkType = xml.getInt("linkType", linkType);
}

void SnoopDump::save(VXml xml)
{
  SnoopProcess::save(xml);

  xml.setStr("filePath", filePath);
  xml.setInt("linkType", linkType);
}

#ifdef QT_GUI_LIB
void SnoopDump::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  VOptionable::addLineEdit(layout, "leFilePath", "File Path", filePath);
  VOptionable::addLineEdit(layout, "leLinkType", "Link Type", QString::number(linkType));
}

void SnoopDump::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  filePath = dialog->findChild<QLineEdit*>("leFilePath")->text();
  linkType = dialog->findChild<QLineEdit*>("leLinkType")->text().toInt();
}
#endif // QT_GUI_LIB
