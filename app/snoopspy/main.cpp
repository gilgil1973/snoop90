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
  VApp::initialize();
  LOG_INFO("snoopspy version 9.0 started %s / %s", VDREAM_VERSION, SNOOP_VERSION);
  explicitLink();
  int res = run(argc, argv);
  VApp::finalize();
  LOG_INFO("SnoopSpy version 9.0 terminated");
  return res;
}
