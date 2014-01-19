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

// ----------------------------------------------------------------------------
// SnoopFilter
// ----------------------------------------------------------------------------
class SnoopFilter : public VObject, public VShowOption
{
public:
  SnoopFilter(void* owner = NULL);
  virtual ~SnoopFilter();

#ifdef QT_GUI_LIB
public: // for VShowOption
  virtual void addOptionWidget(QLayout* layout);
  virtual void saveOption(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_FILTER_H__
