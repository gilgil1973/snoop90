#include <SnoopCaptureFactory>

#include <VXmlDoc>
#include <SnoopAdapter>
#include <SnoopArpSpoof>
#include <SnoopFile>
#include <SnoopSourcePcap>
#include <SnoopRemote>
#include <SnoopVirtualNat>
#include <SnoopWinDivert>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopCaptureFactory
// ----------------------------------------------------------------------------
void SnoopCaptureFactory::explicitLink()
{
  SnoopAdapter     adapter;
  SnoopArpSpoof    arpSpoof;
  SnoopFile        file;
  SnoopSourcePcap  pcap;
#ifdef WIN32
  SnoopRemote      remote;
#endif // WIN32
  SnoopVirtualNat  virtualNAT;
  SnoopWinDivert   winDivert;
}

SnoopCapture* SnoopCaptureFactory::createDefaultCapture()
{
  SnoopAdapter* adapter = new SnoopAdapter;
  return adapter;
}
