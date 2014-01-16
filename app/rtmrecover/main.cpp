#include <VApp>
#include <SnoopRtm>
#include <iostream>
#include <VDebugNew>

void run()
{
  VApp& app = VApp::instance();

  QString fileName;
  if (app.argc() > 1) fileName = app.argv()[1];

  SnoopRtm& rtm = SnoopRtm::instance();
  if (fileName == "")
  {
    if (!rtm.loadFromSystem())
    {
      std::cout << "rtm.loadFromSystem return false " << rtm.error.msg << std::endl;
      return;
    }
    if (!rtm.saveToFile(SnoopRtm::DEFAULT_RTM_FILE_NAME, ""))
    {
      std::cout << "rtm.saveToFile return false " << rtm.error.msg << std::endl;
      return;
    }
  } else
  {
    if (!rtm.loadFromFile(fileName, ""))
    {
      std::cout << format("can not open file(%s)", qPrintable(fileName)) << std::endl;
    }
    if (!rtm.recoverSystem())
    {
      std::cout << "can not recover rtm" << std::endl;
    }
  }
}

int main(int argc, char* argv[])
{
  VApp::initialize(true, false, "stdout");
  VApp::instance().setArguments(argc, argv);
  run();
  VApp::finalize(false);
}
