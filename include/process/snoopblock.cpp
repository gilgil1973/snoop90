#include <SnoopBlock>

REGISTER_METACLASS(SnoopBlock, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopBlock
// ----------------------------------------------------------------------------
SnoopBlock::SnoopBlock(void* owner) : SnoopProcess(owner)
{
  dropRate = 100;
}

SnoopBlock::~SnoopBlock()
{
  close();
}

void SnoopBlock::block(SnoopPacket* packet)
{
  int r = rand() % 100; // 0 <= r <= 99
  if (r < dropRate)
  {
    packet->drop = true;
    emit blocked(packet);
  } else
  {
    emit unblocked(packet);
  }
}

void SnoopBlock::load(VXml xml)
{
  SnoopProcess::load(xml);

  dropRate = xml.getInt("dropRate", dropRate);
}

void SnoopBlock::save(VXml xml)
{
  SnoopProcess::save(xml);

  xml.setInt("dropRate", dropRate);
}

#ifdef QT_GUI_LIB
void SnoopBlock::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  VOptionable::addLineEdit(layout, "leDropRate", "Drop Rate(%)", QString::number(dropRate));
}

void SnoopBlock::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  dropRate = dialog->findChild<QLineEdit*>("leDropRate")->text().toInt();
}
#endif // QT_GUI_LIB
