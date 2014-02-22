#include <SnoopFlowMgrAccessible>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopFlowMgrMap_MacFlow
// ----------------------------------------------------------------------------
SnoopFlowMgrMap_MacFlow::SnoopFlowMgrMap_MacFlow()
{
  clear();
}

SnoopFlowMgrMap_MacFlow::~SnoopFlowMgrMap_MacFlow()
{
  clear();
}

void SnoopFlowMgrMap_MacFlow::clear()
{
  for (SnoopFlowMgrMap_MacFlow::iterator it = begin(); it != end(); it++)
  {
    BYTE* totalMem = it.value().totalMem;
    delete[] totalMem;
  }
  QMap<SnoopMacFlowKey, SnoopFlowMgrMapValue>::clear();
}

SnoopFlowMgrMap_MacFlow::iterator SnoopFlowMgrMap_MacFlow::erase(SnoopMacFlowKey& key)
{
  SnoopFlowMgrMap_MacFlow::iterator it = find(key);
  LOG_ASSERT(it != end());
  BYTE* totalMem = it.value().totalMem;
  delete[] totalMem;
  return QMap<SnoopMacFlowKey, SnoopFlowMgrMapValue>::erase(it);
}

// ----------------------------------------------------------------------------
// SnoopFlowMgrMap_TcpFlow
// ----------------------------------------------------------------------------
SnoopFlowMgrMap_TcpFlow::SnoopFlowMgrMap_TcpFlow()
{
  clear();
}

SnoopFlowMgrMap_TcpFlow::~SnoopFlowMgrMap_TcpFlow()
{
  clear();
}

void SnoopFlowMgrMap_TcpFlow::clear()
{
  for (SnoopFlowMgrMap_TcpFlow::iterator it = begin(); it != end(); it++)
  {
    BYTE* totalMem = it.value().totalMem;
    delete[] totalMem;
  }
  QMap<SnoopTcpFlowKey, SnoopFlowMgrMapValue>::clear();
}

SnoopFlowMgrMap_TcpFlow::iterator SnoopFlowMgrMap_TcpFlow::erase(SnoopTcpFlowKey& key)
{
  SnoopFlowMgrMap_TcpFlow::iterator it = find(key);
  LOG_ASSERT(it != end());
  BYTE* totalMem = it.value().totalMem;
  delete[] totalMem;
  return QMap<SnoopTcpFlowKey, SnoopFlowMgrMapValue>::erase(it);
}

// ----------------------------------------------------------------------------
// SnoopFlowMgrMap_UdpFlow
// ----------------------------------------------------------------------------
SnoopFlowMgrMap_UdpFlow::SnoopFlowMgrMap_UdpFlow()
{
  clear();
}

SnoopFlowMgrMap_UdpFlow::~SnoopFlowMgrMap_UdpFlow()
{
  clear();
}

void SnoopFlowMgrMap_UdpFlow::clear()
{
  for (SnoopFlowMgrMap_UdpFlow::iterator it = begin(); it != end(); it++)
  {
    BYTE* totalMem = it.value().totalMem;
    delete[] totalMem;
  }
  QMap<SnoopUdpFlowKey, SnoopFlowMgrMapValue>::clear();
}

SnoopFlowMgrMap_TcpFlow::iterator SnoopFlowMgrMap_UdpFlow::erase(SnoopUdpFlowKey& key)
{
  SnoopFlowMgrMap_UdpFlow::iterator it = find(key);
  LOG_ASSERT(it != end());
  BYTE* totalMem = it.value().totalMem;
  delete[] totalMem;
  return QMap<SnoopUdpFlowKey, SnoopFlowMgrMapValue>::erase(it);
}
