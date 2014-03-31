#include <QCoreApplication>
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

  runThread().open();

  return true;
}

bool Main::doClose()
{
  graph.close();
  runThread().close();
  return true;
}

void Main::terminate()
{
  if (runThread().active())
  {
    LOG_DEBUG("terminate application by force");
    _exit(0);
  }
}

void Main::run()
{
  printf("press any key to close");
  std::string s; std::getline(std::cin, s);
  QCoreApplication::exit(0); // awaken a.exec()
}

// ----------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

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
  return res;
}
