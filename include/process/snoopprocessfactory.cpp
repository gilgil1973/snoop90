#include <SnoopProcessFactory>

#include <SnoopBlock>
#include <SnoopChecksum>
#include <SnoopCommand>
#include <SnoopDataChange>
#include <SnoopDataFind>
#include <SnoopDelay>
#include <SnoopDnsChange>
#include <SnoopFlowChange>
#include <SnoopFlowMgr>
#include <SnoopFlowMgrTest>
#include <SnoopDump>
#include <SnoopTcpBlock>
#include <SnoopUdpReceiver>
#include <SnoopUdpSender>
#include <SnoopWriteAdapter>
#include <SnoopWriteWinDivert>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopProcessFactory
// ----------------------------------------------------------------------------
void SnoopProcessFactory::explicitLink()
{
  SnoopBlock          block;
  SnoopChecksum       checksum;
  SnoopCommand        command;
  SnoopDataChange     dataChange;
  SnoopDataFind       dataFind;
  SnoopDelay          delay;
  SnoopDnsChange      dnsChange;
  SnoopDump           dump;
  SnoopFlowChange     flowChange;
  SnoopFlowMgr        flowMgr;
  SnoopFlowMgrTest    flowMgrTest;
  SnoopTcpBlock       tcpBlock;
  SnoopUdpReceiver    udpReceiver;
  SnoopUdpSender      udpSender;
  SnoopWriteAdapter   writeAdapter;
  SnoopWriteWinDivert writeWinDivert;
}

SnoopProcess* SnoopProcessFactory::createDefaultProcess()
{
  // ----- gilgil temp 2012.06.08 -----
  // return new SnoopProcessLog;
  // ----------------------------------
  return NULL;
}
