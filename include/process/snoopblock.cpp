#include <SnoopBlock>
#include <SnoopEth>
#include <SnoopIp>
#include <SnoopTcp>

REGISTER_METACLASS(SnoopBlock, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopBlock
// ----------------------------------------------------------------------------
SnoopBlock::SnoopBlock(void* owner) : SnoopProcess(owner)
{
  drop = true;
}

SnoopBlock::~SnoopBlock()
{
  close();
}

void SnoopBlock::block(SnoopPacket* packet)
{
  packet->block = this->drop;
}

void SnoopBlock::load(VXml xml)
{
  SnoopProcess::load(xml);

  drop = xml.getBool("drop", drop);
}

void SnoopBlock::save(VXml xml)
{
  SnoopProcess::save(xml);

  xml.setBool("drop", drop);
}

#ifdef QT_GUI_LIB
void SnoopBlock::addOptionWidget(QLayout* layout)
{
  SnoopProcess::addOptionWidget(layout);

  VOptionable::addCheckBox(layout, "chkDrop", "Drop", drop);
}

void SnoopBlock::saveOptionDlg(QDialog* dialog)
{
  SnoopProcess::saveOptionDlg(dialog);

  drop = dialog->findChild<QCheckBox*>("chkDrop")->checkState() == Qt::Checked;
}
#endif // QT_GUI_LIB
