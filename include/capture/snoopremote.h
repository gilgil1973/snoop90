// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 8.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_REMOTE_H__
#define __SNOOP_REMOTE_H__

#include <SnoopPcap>
#include <SnoopInterface>

#ifdef WIN32

// ----------------------------------------------------------------------------
// SnoopRemote
// ----------------------------------------------------------------------------
class SnoopRemote : public SnoopPcap
{
  Q_OBJECT

public:
  SnoopRemote(void* owner = NULL);
  virtual ~SnoopRemote();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  QString host;
  QString userName;
  QString password;
  int     adapterIndex;

protected:
  SnoopInterfaces interfaces;

signals:
  void interfacesReceived(SnoopRemote* remote, SnoopInterfaces* interfaces, int* adapterIndex);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VShowOption
  virtual void addOptionWidget(QLayout* layout);
  virtual void saveOption(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // WIN32

#endif // __SNOOP_REMOTE_H__
