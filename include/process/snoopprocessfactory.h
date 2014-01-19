// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_PROCESS_FACTORY_H__
#define __SNOOP_PROCESS_FACTORY_H__

#include <VFactory>
#include <SnoopProcess>

// ----------------------------------------------------------------------------
// SnoopProcessFactory
// ----------------------------------------------------------------------------
class SnoopProcessFactory : public VFactory<SnoopProcess>
{
public:
  static void          explicitLink();
  static SnoopProcess* createDefaultProcess();
};

#endif // __SNOOP_PROCESS_FACTORY_H__
