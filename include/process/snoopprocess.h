// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_PROCESS_H__
#define __SNOOP_PROCESS_H__

#include <Snoop>
#include <VObjectWidget>
#include <VGraph>

// ----------------------------------------------------------------------------
// SnoopProcess
// ----------------------------------------------------------------------------
/// Base class of all process classes
class SnoopProcess : public VObject, public VOptionable
{
  Q_OBJECT

public:
  SnoopProcess(void* owner = NULL);
  virtual ~SnoopProcess();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_PROCESS_H__
