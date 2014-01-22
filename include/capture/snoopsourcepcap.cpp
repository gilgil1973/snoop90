#include <SnoopSourcePcap>

REGISTER_METACLASS(SnoopSourcePcap, SnoopCapture)

// ----------------------------------------------------------------------------
// SnoopSourcePcap
// ----------------------------------------------------------------------------
SnoopSourcePcap::SnoopSourcePcap(void* owner) : SnoopPcap(owner)
{

}

SnoopSourcePcap::~SnoopSourcePcap()
{
  close();
}

bool SnoopSourcePcap::doOpen()
{
  if (!pcapOpen((char*)qPrintable(source), NULL, NULL))
  {
    return false;
  }

  return SnoopPcap::doOpen();
}

bool SnoopSourcePcap::doClose()
{
  return SnoopPcap::doClose();
}

void SnoopSourcePcap::load(VXml xml)
{
  SnoopPcap::load(xml);
  source = xml.getStr("source", source);
}

void SnoopSourcePcap::save(VXml xml)
{
  SnoopPcap::save(xml);
  xml.setStr("source", source);
}

#ifdef QT_GUI_LIB
void SnoopSourcePcap::addOptionWidget(QLayout* layout)
{
  SnoopPcap::addOptionWidget(layout);
  VOptionable::addLineEdit(layout, "leSource", "Source", source);
}

void SnoopSourcePcap::saveOptionDlg(QDialog* dialog)
{
  SnoopPcap::saveOptionDlg(dialog);
  source = dialog->findChild<QLineEdit*>("leSource")->text();
}
#endif // QT_GUI_LIB
