#include <VApp>
#include <SnoopType>
#include <VDebugNew>

int main(int argc, char* argv[])
{
  VApp::initialize();
  Mac mac1 = "00:11:22:33:44:55";
  Mac mac2 = "00:11:22:33:44:55";
  bool res = mac1 == mac2;
  printf("%d\n", res);
  VApp::finalize();
}
