#include <QCoreApplication>
#include <VApp>
#include <VFile> // for VERR_INVALID_FILENAME
#include "explicit_link.h"
#include "param.h"
#include "main.h"

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
Main::Main()
{
  fileName = "";
  VObject::connect(&graph, SIGNAL(closed()), this, SLOT(terminate()));
}

Main::~Main()
{
  close();
}

bool Main::doOpen()
{
  if (!graph.loadFromFile(fileName, "graph"))
  {
    SET_ERROR(VError, qformat("can not open file(%s)", qPrintable(fileName)), VERR_INVALID_FILENAME);
    return false;
  }

  if (!graph.open())
  {
    error = graph.error;
    return false;
  }

  return true;
}

bool Main::doClose()
{
  graph.close();
  return true;
}

void Main::terminate()
{
  LOG_DEBUG("terminate application");
  QCoreApplication::exit(0); // awaken a.exec()
}

// ----------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  VApp::initialize(false, true);

  explicitLink();

  Param param;
  if (!param.parse(argc, argv))
  {
    Param::usage();
    return 0;
  }

  Main main;
  main.fileName = param.fileName;
  if (!main.open())
  {
    printf("%s\n", main.error.msg);
    return 0;
  }

  int res = a.exec();
  main.close();

  VApp::finalize(true);

  return res;
}
