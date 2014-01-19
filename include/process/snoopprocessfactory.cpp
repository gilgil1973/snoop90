#include <SnoopProcessFactory>

#include <SnoopBlock>
#include <SnoopDelay>
#include <SnoopDump>
#include <SnoopTcpBlock>
#include <SnoopWriteAdapter>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopProcessFactory
// ----------------------------------------------------------------------------
void SnoopProcessFactory::explicitLink()
{
  SnoopBlock        block;
  SnoopDelay        delay;
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
