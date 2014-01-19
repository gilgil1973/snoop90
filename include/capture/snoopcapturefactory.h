// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_CAPTURE_FACTORY_H__
#define __SNOOP_CAPTURE_FACTORY_H__

#include <VFactory>
#include <SnoopCapture>

// ----------------------------------------------------------------------------
// SnoopCaptureFactory
// ----------------------------------------------------------------------------
class SnoopCaptureFactory : public VFactory<SnoopCapture>
{
public:
  static void          explicitLink();
  static SnoopCapture* createDefaultCapture();
};

#endif // __SNOOP_CAPTURE_FACTORY_H__
