#include <SnoopProcess>

// ----------------------------------------------------------------------------
// SnoopProcess
// ----------------------------------------------------------------------------
SnoopProcess::SnoopProcess(void* owner) : VObject(owner)
{
}

SnoopProcess::~SnoopProcess()
{
}

bool SnoopProcess::doOpen()
{
  return true;
}

bool SnoopProcess::doClose()
{
  return true;
}

void SnoopProcess::load(VXml xml)
{
  VObject::load(xml);
}

void SnoopProcess::save(VXml xml)
{
  VObject::save(xml);
}

#ifdef QT_GUI_LIB
void SnoopProcess::optionAddWidget(QLayout *layout)
{
  Q_UNUSED(layout)
}

void SnoopProcess::optionSaveDlg(QDialog *dialog)
{
  Q_UNUSED(dialog)
}
#endif // QT_GUI_LIB
