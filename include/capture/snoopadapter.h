// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_ADAPTER_H__
#define __SNOOP_ADAPTER_H__

#include <SnoopPcap>
#include <SnoopInterface>

// ----------------------------------------------------------------------------
// SnoopAdapterIndex
// ----------------------------------------------------------------------------
class SnoopAdapterIndex
{
protected:
  int m_adapterIndex;

public:
  SnoopAdapterIndex();
  operator int();
  void operator = (const int i);

protected:
  void setAdapterIndex(int value);
};

// ----------------------------------------------------------------------------
// SnoopAdapter
// ----------------------------------------------------------------------------
class SnoopAdapter : public SnoopPcap
{
public:
  SnoopAdapter(void* owner = NULL);
  virtual ~SnoopAdapter();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  SnoopAdapterIndex adapterIndex;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_ADAPTER_H__
