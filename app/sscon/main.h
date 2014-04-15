// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __MAIN_H__
#define __MAIN_H__

#include <VObject>
#include <VGraph>
#include <VThread>
#include <iostream>

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
class Main : public VObject
{
  Q_OBJECT

public:
  Main();
  virtual ~Main();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  QString fileName;
  VGraph  graph;

public slots:
  void terminate();
};

#endif // __MAIN_H__
