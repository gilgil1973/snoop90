#include <SnoopNetStat>

// ----------------------------------------------------------------------------
// SnoopNetStat
// ----------------------------------------------------------------------------
const QString SnoopNetStat::UNKNOWN_PROCESS_NAME = "";

SnoopNetStat::SnoopNetStat()
{
}

SnoopNetStat::~SnoopNetStat()
{
}

SnoopNetStat& SnoopNetStat::instance()
{
#ifdef WIN32
  static SnoopNetStatWin g_instance;
  return g_instance;
#endif // WIN32
#ifdef linux
  static SnoopNetStatLinux g_instance;
  return g_instance;
#endif // linux
}

#ifdef WIN32

#include <SnoopInterface>
#include <SnoopNetInfo>

// ----------------------------------------------------------------------------
// SnoopNetStatWin
// ----------------------------------------------------------------------------
SnoopNetStatWin::SnoopNetStatWin()
{
  dwTcpSize = 0;
  dwUdpSize = 0;
  pTcp      = NULL;
  pUdp      = NULL;
  if ( (hDLL = LoadLibraryA("Iphlpapi.dll")) == NULL )
  {
    LOG_FATAL("fail to LoadLibrary 'Iphlpapi.dll'\n");
    return;
  }
  GetExtendedTcpTable = (GetExtendedTcpTable_t)GetProcAddress(hDLL, "GetExtendedTcpTable");
  GetExtendedUdpTable = (GetExtendedUdpTable_t)GetProcAddress(hDLL, "GetExtendedUdpTable");
  if (GetExtendedTcpTable == NULL)
  {
    LOG_FATAL("GetExtendedTcpTable is null");
    return;
  }
  if (GetExtendedUdpTable == NULL)
  {
    LOG_FATAL("GetExtendedUdpTable is null");
    return;
  }

  SnoopInterfaces& intfs = SnoopInterfaces::instance();
  for (int i = 1; i < intfs.count(); i++)
  {
    SnoopNetInfo netInfo;
    netInfo.adapterIndex = i;
    Ip ip = netInfo.ip;
    if (ip != 0)
      myIpList.push_back(ip);
  }
  myIpList.push_back(Ip("127.0.0.1"));
  foreach (Ip ip, myIpList)
  {
    LOG_DEBUG("ip=%s", qPrintable(ip.str()));
  }
}

SnoopNetStatWin::~SnoopNetStatWin()
{
  if (pTcp)
  {
    free(pTcp);
    pTcp = NULL;
  }
  if (pUdp)
  {
    free(pUdp);
    pUdp = NULL;
  }
  if (hDLL)
  {
    FreeLibrary(hDLL);
    hDLL = (HMODULE)0;
  }
}

quint32 SnoopNetStatWin::getPID(SnoopTupleFlowKey &infoKey)
{
  VLock lock(*this);

  quint32 pid = UNKNOWN_PROCESS_ID;

  bool outbound = false;
  if (myIpList.contains(infoKey.flow.srcIp)) outbound = true;

  if (infoKey.proto == IPPROTO_TCP)
  {
    //
    // Get TCP Table
    //
    while (true)
    {
      DWORD dwRet = GetExtendedTcpTable(pTcp, &dwTcpSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
      if (dwRet == NO_ERROR) break;
      if (dwRet != ERROR_INSUFFICIENT_BUFFER)
      {
        LOG_ERROR("GetExtendedTcpTable return %u", dwRet);
        return UNKNOWN_PROCESS_ID;
      }
      if (pTcp != NULL) free(pTcp);
      pTcp  = (MIB_TCPTABLE_OWNER_PID*)malloc(dwTcpSize);
      if (pTcp == NULL)
      {
        LOG_ERROR("malloc(%d) return null", dwTcpSize);
        return UNKNOWN_PROCESS_ID;
      }
    }

    if (outbound)
    {
      for (DWORD i = 0; i < pTcp->dwNumEntries; i++)
      {
        // LOG_DEBUG("IPv4 TCP Port %-10d %d", ntohs( (u_short)pTcp->table[i].dwLocalPort), pTcp->table[i].dwOwningPid);
        PMIB_TCPROW_OWNER_PID table = &(pTcp->table[i]);
        DWORD LA = ntohl(table->dwLocalAddr);
        WORD  LP = ntohs(table->dwLocalPort);
        DWORD RA = ntohl(table->dwRemoteAddr);
        WORD  RP = ntohs(table->dwRemotePort);
        if (LA == infoKey.flow.srcIp && LP == infoKey.flow.srcPort && RA == infoKey.flow.dstIp && RP == infoKey.flow.dstPort)
        {
          pid = table->dwOwningPid;
          break;
        }
      }
    } else
    { // inbound
      for (DWORD i = 0; i < pTcp->dwNumEntries; i++)
      {
        // LOG_DEBUG("IPv4 TCP Port %-10d %d", ntohs( (u_short)pTcp->table[i].dwLocalPort), pTcp->table[i].dwOwningPid);
        PMIB_TCPROW_OWNER_PID table = &(pTcp->table[i]);
        DWORD LA = ntohl(table->dwLocalAddr);
        WORD  LP = ntohs(table->dwLocalPort);
        DWORD RA = ntohl(table->dwRemoteAddr);
        WORD  RP = ntohs(table->dwRemotePort);
        if (LA == infoKey.flow.dstIp && LP == infoKey.flow.dstPort && RA == infoKey.flow.srcIp && RP == infoKey.flow.srcPort)
        {
          pid = table->dwOwningPid;
          break;
        }
      }
     }
  }

  if (infoKey.proto == IPPROTO_UDP)
  {
    //
    // Get UDP Table
    //
    while (true)
    {
      DWORD dwRet = GetExtendedUdpTable(pUdp, &dwUdpSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0);
      if (dwRet == NO_ERROR) break;
      if (dwRet != ERROR_INSUFFICIENT_BUFFER)
      {
        LOG_ERROR("GetExtendedUdpTable return %u", dwRet);
        return UNKNOWN_PROCESS_ID;
      }
      if (pUdp != NULL) free(pUdp);
      pUdp  = (MIB_UDPTABLE_OWNER_PID*)malloc(dwUdpSize);
      if (pUdp == NULL)
      {
        LOG_ERROR("malloc(%d) return null", dwUdpSize);
        return UNKNOWN_PROCESS_ID;
      }
    }

    if (outbound)
    {
      for (DWORD i = 0; i < pUdp->dwNumEntries; i++)
      {
        // LOG_DEBUG("IPv4 UDP Port %-10d %d", ntohs( (u_short)pUdp->table[i].dwLocalPort), pUdp->table[i].dwOwningPid);
        PMIB_UDPROW_OWNER_PID table = &(pUdp->table[i]);
        // DWORD LA = ntohl(table->dwLocalAddr); // gilgil temp 2013.01.07
        WORD  LP = ntohs(table->dwLocalPort);
        if (LP == infoKey.flow.srcPort)
        {
          pid = table->dwOwningPid;
          break;
        }
      }

    } else
    { // inbound
      for (DWORD i = 0; i < pUdp->dwNumEntries; i++)
      {
        // LOG_DEBUG("IPv4 UDP Port %-10d %d", ntohs( (u_short)pUdp->table[i].dwLocalPort), pUdp->table[i].dwOwningPid);
        PMIB_UDPROW_OWNER_PID table = &(pUdp->table[i]);
        // DWORD LA = ntohl(table->dwLocalAddr); // gilgil temp 2013.01.07
        WORD  LP = ntohs(table->dwLocalPort);
        if (LP == infoKey.flow.dstPort)
        {
          pid = table->dwOwningPid;
          break;
        }
      }
    }
  }

  return pid;
}

QString SnoopNetStatWin::getProcessName(quint32 pid)
{
  VLock lock(*this);

  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
  if (hSnapshot == INVALID_HANDLE_VALUE)
  {
    LOG_FATAL("CreateToolhelp32Snapshot return INVALID_HANDLE_VALUE GetLastError=%u", GetLastError());
    return UNKNOWN_PROCESS_NAME;
  }

  QString res = UNKNOWN_PROCESS_NAME;
  PROCESSENTRY32 pe32;
  memset(&pe32, 0, sizeof(PROCESSENTRY32));
  pe32.dwSize = sizeof(PROCESSENTRY32);
  if(Process32First(hSnapshot,&pe32))
  {
    while (true)
    {
      // ----- gilgil temp 2012.09.04 -----
      /* {
        QString exeFileName = QString::fromStdWString(pe32.szExeFile);
        LOG_DEBUG("pid %d %s",pe32.th32ProcessID, qPrintable(exeFileName));
      }*/
      // ----------------------------------
      if (pe32.th32ProcessID == pid)
      {
        std::wstring ws = pe32.szExeFile;
        res = QString::fromStdWString(ws);
        break;
      }
      if(!Process32Next(hSnapshot,&pe32)) break;
    }
  }
  if (hSnapshot != INVALID_HANDLE_VALUE)
  {
    CloseHandle(hSnapshot);
    hSnapshot = INVALID_HANDLE_VALUE;
  }
  return res;
}

#endif // WIN32

#ifdef linux
// ----------------------------------------------------------------------------
// SnoopNetStatLinux
// ----------------------------------------------------------------------------
quint32 SnoopNetStatLinux::getPID(SnoopTupleFlowKey& infoKey)
{
  LOG_ERROR("not implemented"); // gilgil temp 2013.02.08
  return UNKNOWN_PROCESS_ID;
}

QString SnoopNetStatLinux::getProcessName(quint32 pid)
{
  LOG_ERROR("not implemented"); // gilgil temp 2013.02.08
  return UNKNOWN_PROCESS_NAME;
}

#endif // linux
