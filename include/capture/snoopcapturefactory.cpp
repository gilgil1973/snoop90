#include <SnoopCaptureFactory>

#include <VXmlDoc>
#include <SnoopAdapter>
#include <SnoopFile>
#include <SnoopSourcePcap>
#include <SnoopRemote>
// #include <snoop/capture/SnoopArpSpoof.h> // gilgil temp 2012.06.07
#include <SnoopVirtualNat>
#include <SnoopWinDivert>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopCaptureFactory
// ----------------------------------------------------------------------------
void SnoopCaptureFactory::explicitLink()
{
  SnoopAdapter     adapter;
  SnoopFile        file;
  SnoopSourcePcap  pcap;
#ifdef WIN32
  SnoopRemote      remote;
#endif // WIN32
  // SnoopArpSpoof arpSpoof; // gilgil temp 2012.06.07
  SnoopVirtualNat  virtualNAT;
  SnoopWinDivert   winDivert;
}

SnoopCapture* SnoopCaptureFactory::createDefaultCapture()
{
  SnoopAdapter* adapter = new SnoopAdapter;
  return adapter;
}
