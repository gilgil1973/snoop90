// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_FILTER_FACTORY_H__
#define __SNOOP_FILTER_FACTORY_H__

#include <VFactory>
#include <SnoopFilter>

// ----------------------------------------------------------------------------
// SnoopFilterFactory
// ----------------------------------------------------------------------------
class SnoopFilterFactory : public VFactory<SnoopFilter>
{
public:
  static void          explicitLink();
};

#endif // __SNOOP_FILTER_FACTORY_H__
