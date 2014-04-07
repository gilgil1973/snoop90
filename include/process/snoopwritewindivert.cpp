#include <SnoopWriteWinDivert>

REGISTER_METACLASS(SnoopWriteWinDivert, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopWriteWinDivert
// ----------------------------------------------------------------------------
SnoopWriteWinDivert::SnoopWriteWinDivert(void* owner) : SnoopWinDivert(owner)
{
  autoRead             = false;
  autoParse            = false;
  flags                = WINDIVERT_FLAG_SNIFF;

  changeDivertAddr     = false;
  divertAddr.IfIdx     = 0;
  divertAddr.SubIfIdx  = 0;
  divertAddr.Direction = WINDIVERT_DIRECTION_OUTBOUND;
}

SnoopWriteWinDivert::~SnoopWriteWinDivert()
{
  close();
}

void SnoopWriteWinDivert::copy(SnoopPacket* packet)
{
  if (changeDivertAddr)
  {
    packet->divertAddr = divertAddr;
  }
  SnoopWinDivert::write(packet);
  emit copied(packet);
}

void SnoopWriteWinDivert::move(SnoopPacket* packet)
{
  if (changeDivertAddr)
  {
    packet->divertAddr = divertAddr;
  }
  SnoopWinDivert::write(packet);
  packet->drop = true;
  emit moved(packet);
}

void SnoopWriteWinDivert::load(VXml xml)
{
  SnoopWinDivert::load(xml);

  changeDivertAddr     = xml.getBool("changeDivertAddr", changeDivertAddr);
  divertAddr.IfIdx     = xml.getInt("IfIdx",             divertAddr.IfIdx);
  divertAddr.SubIfIdx  = xml.getInt("SubIfIdx",          divertAddr.SubIfIdx);
  divertAddr.Direction = (UINT8)xml.getInt("Direction",  (int)divertAddr.Direction);
}

void SnoopWriteWinDivert::save(VXml xml)
{
  SnoopWinDivert::save(xml);

  xml.setBool("changeDivertAddr", changeDivertAddr);
  xml.setInt("IfIdx",             divertAddr.IfIdx);
  xml.setInt("SubIfIdx",          divertAddr.SubIfIdx);
  xml.setInt("Direction",         (int)divertAddr.Direction);
}

#ifdef QT_GUI_LIB
void SnoopWriteWinDivert::optionAddWidget(QLayout* layout)
{
  SnoopWinDivert::optionAddWidget(layout);

  VOptionable::addCheckBox(layout, "chkChangeDivertAddr",     "Change DivertAddr",     changeDivertAddr);
  VOptionable::addLineEdit(layout, "leIfIdx",                 "IfIdx",                 QString::number(divertAddr.IfIdx));
  VOptionable::addLineEdit(layout, "leSubIfIdx",              "SubIfIdx",              QString::number(divertAddr.SubIfIdx));
  VOptionable::addLineEdit(layout, "leDirection",             "Direction",             QString::number(divertAddr.Direction));
}

void SnoopWriteWinDivert::optionSaveDlg(QDialog* dialog)
{
  SnoopWinDivert::optionSaveDlg(dialog);

  changeDivertAddr     = dialog->findChild<QCheckBox*>("chkChangeDivertAddr")->checkState() == Qt::Checked;
  divertAddr.IfIdx     = dialog->findChild<QLineEdit*>("leIfIdx")->text().toUInt();
  divertAddr.SubIfIdx  = dialog->findChild<QLineEdit*>("leSubIfIdx")->text().toUInt();
  divertAddr.Direction = dialog->findChild<QLineEdit*>("leDirection")->text().toUInt();
}
#endif // QT_GUI_LIB
