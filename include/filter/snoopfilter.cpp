#include <SnoopFilter>

// ----------------------------------------------------------------------------
// SnoopFilter
// ----------------------------------------------------------------------------
SnoopFilter::SnoopFilter(void* owner) : VObject(owner)
{
}

SnoopFilter::~SnoopFilter()
{
  close();
}

bool SnoopFilter::doOpen()
{
  return true;
}

bool SnoopFilter::doClose()
{
  return true;
}

#ifdef QT_GUI_LIB
void SnoopFilter::optionAddWidget(QLayout *layout)
{
  Q_UNUSED(layout)
}

void SnoopFilter::optionSaveDlg(QDialog *dialog)
{
  Q_UNUSED(dialog)
}
#endif // QT_GUI_LIB
