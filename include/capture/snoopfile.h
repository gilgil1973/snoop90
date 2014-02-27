// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_FILE_H__
#define __SNOOP_FILE_H__

#include <SnoopPcap>
#include <VTick>

// ----------------------------------------------------------------------------
// SnoopFile
// ----------------------------------------------------------------------------
class SnoopFile : public SnoopPcap
{
public:
  SnoopFile(void* owner = NULL);
  virtual ~SnoopFile();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  virtual int read(SnoopPacket* packet);

public:
  QString fileName;
  double      speed;

protected:
  long  startTS;
  VTick startTick;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_FILE_H__

