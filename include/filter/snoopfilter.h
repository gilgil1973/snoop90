// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_FILTER_H__
#define __SNOOP_FILTER_H__

#include <Snoop>
#include <VObjectWidget>
#include <VGraph>

// ----------------------------------------------------------------------------
// SnoopFilter
// ----------------------------------------------------------------------------
class SnoopFilter : public VObject, public VOptionable
{
public:
  SnoopFilter(void* owner = NULL);
  virtual ~SnoopFilter();

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void addOptionWidget(QLayout* layout);
  virtual void saveOptionDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_FILTER_H__
