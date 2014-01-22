#include <SnoopFilter>

// ----------------------------------------------------------------------------
// SnoopFilter
// ----------------------------------------------------------------------------
SnoopFilter::SnoopFilter(void* owner) : VObject(owner)
{
}

SnoopFilter::~SnoopFilter()
{
}

#ifdef QT_GUI_LIB
void SnoopFilter::addOptionWidget(QLayout *layout)
{
  Q_UNUSED(layout)
}

void SnoopFilter::saveOptionDlg(QDialog *dialog)
{
  Q_UNUSED(dialog)
}
#endif // QT_GUI_LIB
