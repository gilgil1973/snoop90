// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __PARAM_H__
#define __PARAM_H__

#include <QString>

// ----------------------------------------------------------------------------
// Param
// ----------------------------------------------------------------------------
class Param
{
public:
  QString fileName;

public:
  bool parse(int argc, char* argv[]);
  static void usage();
};

#endif // __PARAM_H__
