#include <QApplication>
#include <VApp>
#include <SnoopCommon>
#include "dialog.h"
#include "explicit_link.h"

int run(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Dialog w;
  w.show();
  return a.exec();
}

int main(int argc, char *argv[])
{
  VApp::initialize();
  LOG_INFO("httpsniff version 3 started %s / %s", VDREAM_VERSION, SNOOP_VERSION);
  VApp::instance().setArguments(argc, argv);
  explicitLink();
  int res = run(argc, argv);
  VApp::finalize();
  LOG_INFO("httpsniff version 3 terminated");
  return res;
}
