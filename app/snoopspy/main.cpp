#include <QApplication>
#include <VApp>
#include <SnoopCommon>
#include "mainwindow.h"
#include "explicit_link.h"

int run(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}

int main(int argc, char *argv[])
{
  VApp::initialize(false, true);
  LOG_INFO("snoopspy version 3 started %s / %s", VDREAM_VERSION, SNOOP_VERSION);
  VApp::instance().setArguments(argc, argv);
  explicitLink();
  int res = run(argc, argv);
  VApp::finalize();
  LOG_INFO("SnoopSpy version 3 terminated");
  return res;
}
