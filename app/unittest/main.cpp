#include <gtest/gtest.h>
#include <VApp>
#include <VDebugNew>

int main(int argc, char* argv[])
{
  QCoreApplication a(argc, argv);
  VApp::initialize();
  ::testing::InitGoogleTest(&argc, argv);
  int res = RUN_ALL_TESTS();
  VApp::finalize();
  return res;
}
