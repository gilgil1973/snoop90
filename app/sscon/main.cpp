#include <iostream>
#include <QCoreApplication>
#include <VGraph>
#include "explicit_link.h"

using namespace std;

class Param
{
public:
  QString fileName;

  bool parse(int argc, char* argv[])
  {
    if (argc <= 1) return false;
    fileName = argv[1];
    return true;
  }

  static void usage()
  {
    printf("snoopspy console version 9.0\n");
    printf("Copyright (c) Gilbert Lee All rights reserved\n");
    printf("\n");
    printf("sscon <snoopspy file name>\n");
    printf("\n");
    printf("example\n");
    printf("\n");
    printf("  sscon test.ss\n");
  }
};

int run(int argc, char *argv[])
{
  Param param;
  if (!param.parse(argc, argv))
  {
    Param::usage();
    return 0;
  }

  explicitLink();

  VGraph graph;
  if (!graph.loadFromFile(param.fileName, "graph"))
  {
    LOG_ERROR("can not open file %s\n", qPrintable(param.fileName));
    return 0;
  }
  if (!graph.open())
  {
    LOG_ERROR("can not open graph %s\n", graph.error.msg);
    return 0;
  }

  printf("press any key to close\n");
  string s; getline(cin, s);

  graph.close();

  return 0;
}

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  int res = run(argc, argv);
  return res;
}
