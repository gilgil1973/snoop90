// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_COMMAND_H__
#define __SNOOP_COMMAND_H__

#include <SnoopProcess>

// ----------------------------------------------------------------------------
// SnoopCommandItem
// ----------------------------------------------------------------------------
class SnoopCommandItem : public VXmlable
{
public:
  SnoopCommandItem();
  virtual ~SnoopCommandItem();

public:
  bool    enabled;
  QString command;
  bool    sync;

protected:
  QProcess* process;

public:
  bool execute();

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// SnoopCommandItems
// ----------------------------------------------------------------------------
class SnoopCommandItems : public QList<SnoopCommandItem>, public VXmlable
{
public:
  bool execute();

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// SnoopCommand
// ----------------------------------------------------------------------------
class SnoopCommand : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopCommand(void* owner = NULL);
  virtual ~SnoopCommand();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  SnoopCommandItems openCommands;
  SnoopCommandItems closeCommands;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_COMMAND_H__
