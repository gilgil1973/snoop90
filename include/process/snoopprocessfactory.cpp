#include <SnoopProcessFactory>
// #include <snoop/process/SnoopProcessList.h>  // gilgil temp 2012.08.26
// #include <snoop/process/SnoopProcessAdapter.h>
// #include <snoop/process/SnoopProcessDump.h>
// #include <snoop/process/SnoopProcessLog.h>   // gilgil temp 2012.08.26
#include <SnoopBlock>
#include <SnoopDump>
#include <SnoopTcpBlock>
#include <SnoopWriteAdapter>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopProcessFactory
// ----------------------------------------------------------------------------
void SnoopProcessFactory::explicitLink()
{
  // SnoopProcessList    processList;
  // SnoopProcessAdapter processAdapter;
  // SnoopProcessDump    processDump;
  // SnoopProcessLog     processLog;
  SnoopBlock        block;
  SnoopDump         dump;
  SnoopTcpBlock     tcpBlock;
  SnoopWriteAdapter writeAdapter;
}

SnoopProcess* SnoopProcessFactory::createDefaultProcess()
{
  // ----- gilgil temp 2012.06.08 -----
  // return new SnoopProcessLog;
  // ----------------------------------
  return NULL;
}
