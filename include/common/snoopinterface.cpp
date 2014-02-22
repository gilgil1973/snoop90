#include <SnoopInterface>
#include <SnoopAutoDetectAdapter>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopInterfacesInstance
// ----------------------------------------------------------------------------
class SnoopInterfacesInstance : public SnoopInterfaces
{
public:
  bool initialized;
};

// ----------------------------------------------------------------------------
// SnoopInterface
// ----------------------------------------------------------------------------
SnoopInterface::SnoopInterface()
{
  index         = snoop::INVALID_ADAPTER_INDEX;
  name          = "";
  description   = "";
  dev           = NULL;
#ifdef WIN32
  adapterInfo   = NULL;
#endif // WIN32
}

SnoopInterface::~SnoopInterface()
{
}

bool SnoopInterface::operator == (SnoopInterface& rhs)
{
  if (this->index != rhs.index) return false;
  if (name != rhs.name) return false;
  if (description != rhs.description) return false;
  return true;
}

bool SnoopInterface::operator != (SnoopInterface& rhs)
{
  return !(*this == rhs);
}

void SnoopInterface::load(VXml xml)
{
  index = xml.getInt("index", index);
  name = xml.getStr("name", name);
  description = xml.getStr("description", description);
}

void SnoopInterface::save(VXml xml)
{
  xml.setInt("index", index);
  xml.setStr("name", name);
  xml.setStr("description", description);
}

// ----------------------------------------------------------------------------
// SnoopInterfaces
// ----------------------------------------------------------------------------
SnoopInterfaces::SnoopInterfaces()
{
  allDevs         = NULL;
#ifdef WIN32
  allAdaptersInfo = NULL;
#endif // WIN32
}

SnoopInterfaces::~SnoopInterfaces()
{
  finalize();
}

void SnoopInterfaces::initialize()
{
  LOG_DEBUG("stt"); // gilgil temp 2012.08.11
  //
  // Initialize allDevs using pcap API.
  //
  if (allDevs != NULL) return;

  char errBuf[PCAP_ERRBUF_SIZE];

#ifdef WIN32
  int i = pcap_findalldevs_ex("rpcap://", NULL, &allDevs, errBuf);
#endif // WIN32
#ifdef linux
  int i = pcap_findalldevs(&allDevs, errBuf);
#endif // linux
  if (i != 0) // if error occured
  {
    LOG_ERROR("error in pcap_findalldevs_ex (%s)", errBuf);
    return;
  }

  //
  // Add null interface(for best adapter)
  //
  SnoopInterface nullInterface;
  push_back(nullInterface);

  //
  // Add interfaces
  //
  pcap_if_t* dev = allDevs;
  i = 1;
  while (dev != NULL)
  {
    SnoopInterface _interface;

    _interface.index = i;
    _interface.name = dev->name;
    _interface.description = dev->description;
    _interface.dev = dev;

    push_back(_interface);

    dev = dev->next;
    i++;
  }

#ifdef WIN32
  //
  // Initialize allAdapterInfos using IPHelper API.
  //
  ULONG outBufLen = 0;
  DWORD res = GetAdaptersInfo(allAdaptersInfo, &outBufLen);
  if (res == ERROR_BUFFER_OVERFLOW)
  {
    allAdaptersInfo = (PIP_ADAPTER_INFO)malloc(outBufLen);
    res = GetAdaptersInfo(allAdaptersInfo, &outBufLen);
  }
  if (res != ERROR_SUCCESS)
  {
    LOG_ERROR("GetAdaptersInfo return %d(0x%x)", res, res);
    return;
  }

  //
  // Set adapterInfo (for Windows)
  //
  int _count = this->count();
  for (int i = 1; i < _count; i++)
  {
    SnoopInterface& _interface = (SnoopInterface&)at(i);
    QString adapterName = _interface.name;

    PIP_ADAPTER_INFO p = allAdaptersInfo;
    while (p != NULL)
    {
      if (strstr(qPrintable(adapterName), p->AdapterName) != NULL)
        break;
      p = p->Next;
    }
    if (p == NULL)
    {
      LOG_DEBUG("can not find adapter(%d) %s", i, qPrintable(adapterName));
    }
    _interface.adapterInfo = p;
  }
#endif // WIN32

  //
  // Change null interface into best interface
  //
  {
    int bestAdapterIndex = getBestAdapterIndex();
    if (bestAdapterIndex != snoop::INVALID_ADAPTER_INDEX)
    {
      SnoopInterface& nullInterface = (SnoopInterface&)at(0);
      SnoopInterface& bestInterface = (SnoopInterface&)at(bestAdapterIndex);
      nullInterface = bestInterface;
    }
  }
  LOG_DEBUG("end"); // gilgil temp 2012.08.11
}

void SnoopInterfaces::finalize()
{
  clear();

  //
  // Finalize allDevs
  //
  if (allDevs != NULL)
  {
    pcap_freealldevs(allDevs);
    allDevs = NULL;
  }

#ifdef WIN32
  //
  // Finalize allAdaptersInfo
  //
  if (allAdaptersInfo != NULL)
  {
    ::free(allAdaptersInfo);
    allAdaptersInfo = NULL;
  }
#endif // WIN32
}

bool SnoopInterfaces::operator == (SnoopInterfaces& rhs)
{
  if (size() != rhs.size()) return false;

  QList<SnoopInterface>::iterator it1 = begin();
  SnoopInterfaces::iterator it2 = rhs.begin();
  while (it1 != end())
  {
    SnoopInterface& interface1 = *it1;
    SnoopInterface& interface2 = *it2;
    if (interface1 != interface2) return false;
    it1++;
    it2++;
  }
  return true;
}

bool SnoopInterfaces::operator != (SnoopInterfaces& rhs)
{
  return !(*this == rhs);
}

void SnoopInterfaces::load(VXml xml)
{
  xml_foreach(childXml, xml.childs())
  {
    SnoopInterface _interface;
    _interface.load(childXml);
    push_back(_interface);
  }
}

void SnoopInterfaces::save(VXml xml)
{
  xml.clearChild();
  for (SnoopInterfaces::iterator it = begin(); it != end(); it++)
  {
    SnoopInterface& _interface = *it;
    VXml childXml = xml.addChild("interface");
    _interface.save(childXml);
  }
}

int SnoopInterfaces::getBestAdapterIndex(QString host)
{
  if (host == "") host = "8.8.8.8"; // Well-knowon Google DNS Server IP

  SnoopAutoDetectAdapter& detect = SnoopAutoDetectAdapter::instance();
  return detect.detect(host);
}

SnoopInterfaces& SnoopInterfaces::instance()
{
  static SnoopInterfacesInstance g_instance;
  if (!g_instance.initialized)
  {
    g_instance.initialized = true;
    g_instance.initialize();
  }
  return g_instance;
}
