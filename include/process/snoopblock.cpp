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
  relay = false;
}

SnoopBlock::~SnoopBlock()
{
  close();
}

void SnoopBlock::block(SnoopPacket* packet)
{
  packet->block = !this->relay;
}

void SnoopBlock::load(VXml xml)
{
  SnoopProcess::load(xml);
  relay = xml.getBool("relay", relay);
}

void SnoopBlock::save(VXml xml)
{
  SnoopProcess::save(xml);
  xml.setBool("relay", relay);
}

#ifdef QT_GUI_LIB
void SnoopBlock::addOptionWidget(QLayout* layout)
{
  SnoopProcess::addOptionWidget(layout);
  VShowOption::addCheckBox(layout, "chkRelay", "Relay", relay);
}

void SnoopBlock::saveOption(QDialog* dialog)
{
  SnoopProcess::saveOption(dialog);
  relay = dialog->findChild<QCheckBox*>("chkRelay")->checkState() == Qt::Checked;
}
#endif // QT_GUI_LIB
