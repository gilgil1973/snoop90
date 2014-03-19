#include <SnoopDataFind>

REGISTER_METACLASS(SnoopDataFind, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopDataFind
// ----------------------------------------------------------------------------
SnoopDataFind::SnoopDataFind(void* owner) : SnoopProcess(owner)
{
  dataFind.clear();
}

SnoopDataFind::~SnoopDataFind()
{
  close();
}

bool SnoopDataFind::doOpen()
{
  if (!dataFind.prepare(error)) return false;

  return SnoopProcess::doOpen();
}

bool SnoopDataFind::doClose()
{
  return SnoopProcess::doClose();
}

void SnoopDataFind::find(SnoopPacket* packet)
{
  if (packet->ipHdr == NULL)
  {
    emit unfound(packet);
    return;
  }

  bool _found = false;
  if (packet->proto == IPPROTO_TCP)
  {
    if (tcpFind)
    {
      //
      // check tcp data find
      //
      QByteArray ba((const char*)packet->data, (uint)packet->dataLen);
      _found = dataFind.find(ba);
    }
  } else
  if (packet->proto == IPPROTO_UDP)
  {
    if (udpFind)
    {
      //
      // check udp data find
      //
      QByteArray ba((const char*)packet->data, (uint)packet->dataLen);
      _found = dataFind.find(ba);
    }
  }

  if (_found)
  {
    emit found(packet);
  } else
  {
    emit unfound(packet);
  }
}

void SnoopDataFind::load(VXml xml)
{
  SnoopProcess::load(xml);

  dataFind.load(xml.gotoChild("dataFind"));
}

void SnoopDataFind::save(VXml xml)
{
  SnoopProcess::save(xml);

  dataFind.save(xml.gotoChild("dataFind"));
}

#ifdef QT_GUI_LIB
void SnoopDataFind::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  VOptionable::addCheckBox(layout, "chkTcpFind", "TCP Find", tcpFind);
  VOptionable::addCheckBox(layout, "chkUdpFind", "UDP Find", udpFind);
  dataFind.optionAddWidget(layout);
}

void SnoopDataFind::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  tcpFind = dialog->findChild<QCheckBox*>("chkTcpFind")->checkState() == Qt::Checked;
  udpFind = dialog->findChild<QCheckBox*>("chkUdpFind")->checkState() == Qt::Checked;
  dataFind.optionSaveDlg(dialog);
}
#endif // QT_GUI_LIB
