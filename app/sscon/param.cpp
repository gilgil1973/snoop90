#include "param.h"

// ----------------------------------------------------------------------------
// Param
// ----------------------------------------------------------------------------
bool Param::parse(int argc, char* argv[])
{
  if (argc <= 1) return false;
  fileName = argv[1];
  return true;
}

void Param::usage()
{
  printf("snoopspy console version 3\n");
  printf("Copyright (c) Gilbert Lee All rights reserved\n");
  printf("\n");
  printf("sscon <snoopspy file name>\n");
  printf("\n");
  printf("example\n");
  printf("\n");
  printf("  sscon test.ss\n");
}
