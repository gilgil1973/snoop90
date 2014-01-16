#include <SnoopInterface>
#include <SnoopNetInfo>
#include <VDebugNew>

int main()
{
  SnoopInterfaces& interfaces = SnoopInterfaces::instance();

  for (int adapterIndex = 1; adapterIndex < interfaces.count(); adapterIndex++)
  {
    const SnoopInterface& intf = interfaces.at(adapterIndex);

    printf("Inteface #%d%s\n", adapterIndex, intf.name == interfaces.at(0).name ? " (Best)" : "");

    if (intf.description !=  "")
      printf("  description = %s\n", qPrintable(intf.description));
    if (intf.name != "")
      printf("  name        = %s\n", qPrintable(intf.name));

    SnoopNetInfo netInfo;
    netInfo.adapterIndex = adapterIndex;

    if (!netInfo.mac.isClean())
      printf("  mac         = %s\n", qPrintable(netInfo.mac.str()));
    if (netInfo.ip != 0)
      printf("  ip          = %s\n", qPrintable(netInfo.ip.str()));
    if (netInfo.subnet != 0)
      printf("  subnet      = %s\n", qPrintable(netInfo.subnet.str()));
    if (netInfo.gateway != 0 && netInfo.gateway != 0xFFFFFFFF)
      printf("  gateway     = %s\n", qPrintable(netInfo.gateway.str()));
    // ----- gilgil temp 2012.08.24 -----
    /*
    if (netInfo.getStartIP() != 1)
      printf("  startIP     = %s\n", qPrintable(netInfo.getStartIP().str()));
    if (netInfo.getEndIP() != 0xFFFFFFFF)
      printf("  endIP       = %s\n", qPrintable(netInfo.getEndIP().str()));
    */
    // ----------------------------------
    printf("\n");
  }
}
