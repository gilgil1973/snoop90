#include <SnoopAutoDetectAdapter>
#include <SnoopInterface>

#ifdef SNOOP_AUTO_DETECT_USE_GETBESTINTERFACE

// ----------------------------------------------------------------------------
// SnoopAutoDetectAdapter
// ----------------------------------------------------------------------------
int SnoopAutoDetectAdapter::detect(QString& host)
{
  IP ip;
  DWORD  bestIfIndex;
  ip = htonl(VNet::resolve(host));
  DWORD res = GetBestInterface(ip, &bestIfIndex);
  if (res != NO_ERROR)
  {
    LOG_FATAL("GetBestInterface return %d", res);
    return snoop::INVALID_ADAPTER_INDEX;
  }

  foreach (const SnoopInterface& intf, SnoopInterfaces::instance())
  {
    if (intf.adapterInfo != NULL && intf.adapterInfo->Index == bestIfIndex)
      return intf.index;
  }

  LOG_FATAL("can not find appropriate adapter for %s", qPrintable(host));
  return snoop::INVALID_ADAPTER_INDEX;;
}

#endif // SNOOP_AUTO_DETECT_USE_GETBESTINTERFACE

#ifdef SNOOP_AUTO_DETECT_USE_RTM

#include <SnoopRtm>
#ifdef WIN32
#include <SnoopNetInfo>
#endif // WIN32

// ----------------------------------------------------------------------------
// SnoopAutoDetectAdapter
// ----------------------------------------------------------------------------
int SnoopAutoDetectAdapter::detect(QString host)
{
  Ip ip = host;
  SnoopRtmEntry* entry = SnoopRtm::instance().getBestEntry(ip);
  if (entry == NULL) return snoop::INVALID_ADAPTER_INDEX;

  QString intfName = entry->intf;
  SnoopInterfaces& intfs = SnoopInterfaces::instance();
  int _count = intfs.count();
#ifdef WIN32
  for (int i = 1; i < _count; i++)
  {
    SnoopNetInfo netInfo;
    netInfo.adapterIndex = i;
    if (netInfo.ip.str() == intfName)
      return i;
  }
#endif // WIN2
#ifdef linux
  for (int i = 1; i < _count; i++)
  {
    const SnoopInterface& intf = intfs.at(i);
    if (intf.name == intfName)
      return i;
  }
#endif // linux
  return snoop::INVALID_ADAPTER_INDEX;
}

#endif // SNOOP_AUTO_DETECT_USE_RTM

#ifdef SNOOP_AUTO_DETECT_USE_ADAPTER

// ----------------------------------------------------------------------------
// SnoopAutoDetectAdapterItem
// ----------------------------------------------------------------------------
SnoopAutoDetectAdapterItem::SnoopAutoDetectAdapterItem(void* owner) : VObject(owner)
{
  adapter.autoRead = true;
  VObject::connect(&adapter, SIGNAL(captured(SnoopPacket*)), this, SLOT(recv(SnoopPacket*)), Qt::DirectConnection);
}

SnoopAutoDetectAdapterItem::~SnoopAutoDetectAdapterItem()
{
  close();
}

bool SnoopAutoDetectAdapterItem::doOpen()
{
  bool res = adapter.open();
  if (!res)
  {
    error = adapter.error;
  }
  return res;
}

bool SnoopAutoDetectAdapterItem::doClose()
{
  adapter.close();
  return true;
}

void SnoopAutoDetectAdapterItem::recv(SnoopPacket* packet)
{
  Q_UNUSED(packet)

  SnoopAutoDetectAdapter* detect = (SnoopAutoDetectAdapter*)owner;
  if (detect == NULL)
  {
    LOG_ERROR("adapter is null");
    return;
  }
  if (detect->m_adapterIndex != snoop::INVALID_ADAPTER_INDEX) return;
  int adapterIndex = adapter.adapterIndex;
  detect->m_adapterIndex = adapterIndex;
  const SnoopInterface& intf = SnoopInterfaces::instance().at(adapterIndex);
  LOG_DEBUG("adapterIndex=%d %s(%s)", adapterIndex, qPrintable(intf.description), qPrintable(intf.name));
  detect->m_event.setEvent();
}

// ----------------------------------------------------------------------------
// GeneratePacketThread
// ----------------------------------------------------------------------------
class GeneratePacketThread : public VThread
{
protected:
  QString host;

public:
  GeneratePacketThread(QString host)
  {
    this->host = host;
  }
protected:
  virtual void run()
  {
    VUDPClient udpClient;
    udpClient.host = host;
    udpClient.port = 80;
    if (!udpClient.open())
    {
      LOG_ERROR("can not open udpClient(%s:%d)", qPrintable(udpClient.host), 80);
      return;
    }
    while (active())
    {
      int writeLen = udpClient.write("test");
      if (writeLen == VERR_FAIL) break;
      msleep(50);
    }
    udpClient.close();
  };
};

// ----------------------------------------------------------------------------
// SnoopAutoDetectAdapter
// ----------------------------------------------------------------------------
SnoopAutoDetectAdapter::SnoopAutoDetectAdapter() : m_event(false, false)
{
  m_adapterIndex = snoop::INVALID_ADAPTER_INDEX;
}

SnoopAutoDetectAdapter::~SnoopAutoDetectAdapter()
{
}

int SnoopAutoDetectAdapter::detect(QString& host)
{
  LOG_DEBUG("stt"); // gilgil temp 2012.08.11
  if (m_adapterIndex != snoop::INVALID_ADAPTER_INDEX)
  {
    return m_adapterIndex;
  }

  //
  // Create and open all adapters
  //
  int adapterCount = SnoopInterfaces::instance().count();
  QList<SnoopAutoDetectAdapterItem*> items;
  for (int i = 1; i < adapterCount; i++)
  {
    SnoopAutoDetectAdapterItem* item = new SnoopAutoDetectAdapterItem(this);
    item->adapter.adapterIndex = i;
    item->adapter.filter = qformat("(host %s) and (arp or udp or tcp)", qPrintable(host)); // arp(finding gateway), udp(resolve domainname or "test") 
    items.push_back(item);
    if (!item->open())
    {
      LOG_ERROR("can not open adapter(%d)", i);
    }
  }

  //
  // Generate packet
  //
  GeneratePacketThread* thread = new GeneratePacketThread(host);
  thread->open();

  //
  // Wait
  //
  m_event.wait(1000);

  //
  // Close
  //
  foreach (SnoopAutoDetectAdapterItem* item, items)
  {
    int adapterIndex = item->adapter.adapterIndex;
    delete item;
    LOG_DEBUG("adapterIndex(%d) deleted", adapterIndex);
  }
  items.clear();

  thread->close();
  delete thread;
  
  LOG_DEBUG("end"); // gilgil temp 2012.08.11
  return m_adapterIndex;
}

void SnoopAutoDetectAdapter::recv(SnoopPacket* packet)
{
  Q_UNUSED(packet)
  SnoopAdapter* adapter = (SnoopAdapter*)sender();
  if (adapter == NULL)
  {
    LOG_ERROR("adapter is null");
    return;
  }
  SnoopAutoDetectAdapter* detect = (SnoopAutoDetectAdapter*)adapter->owner;
  if (detect->m_adapterIndex != snoop::INVALID_ADAPTER_INDEX) return;
  int adapterIndex = adapter->adapterIndex;
  detect->m_adapterIndex = adapterIndex;
  const SnoopInterface& intf = SnoopInterfaces::instance().at(adapterIndex);
  LOG_INFO("adapterIndex=%d(%s)", detect->m_adapterIndex, qPrintable(intf.description));
  detect->m_event.setEvent();
}

#endif // SNOOP_AUTO_DETECT_USE_ADAPTER

#ifdef GTEST
#include <gtest/gtest.h>

TEST( SnoopAudoDetectAdapter, test )
{
  SnoopAutoDetectAdapter& ada = SnoopAutoDetectAdapter::instance();
  int adapterIndex = ada.detect("8.8.8.8");
  EXPECT_TRUE( adapterIndex != snoop::INVALID_ADAPTER_INDEX);
}
#endif // GTEST
