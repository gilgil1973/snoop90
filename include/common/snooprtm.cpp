#include <SnoopRtm>
#include <VXmlDoc>
#include <QRegExp>

// ----------------------------------------------------------------------------
// SnoopRtmEntry
// ----------------------------------------------------------------------------
SnoopRtmEntry::SnoopRtmEntry()
{
  clear();
}

SnoopRtmEntry::~SnoopRtmEntry()
{
}

void SnoopRtmEntry::clear()
{
  dst     = 0;
  mask    = 0;
  gateway = 0;
  intf    = "";
  metric  = 0;
}

void SnoopRtmEntry::load(VXml xml)
{
  dst     = xml.getStr("dst",     dst.str());
  mask    = xml.getStr("mask",    mask.str());
  gateway = xml.getStr("gateway", gateway.str());
  intf    = xml.getStr("intf",    intf);
  metric  = xml.getInt("metric",  metric);
}

void SnoopRtmEntry::save(VXml xml)
{
  xml.setStr("dst",     dst.str());
  xml.setStr("mask",    mask.str());
  xml.setStr("gateway", gateway.str());
  xml.setStr("intf",    intf);
  xml.setInt("metric",  metric);
}

// ----------------------------------------------------------------------------
// SnoopRtmEntryList
// ----------------------------------------------------------------------------
int SnoopRtmEntryList::find(Ip dst, Ip mask)
{
  int i = 0;
  foreach(const SnoopRtmEntry& entry, *this)
  {
    if (entry.dst == dst && entry.mask == mask) return i;
  }
  return -1;
}

void SnoopRtmEntryList::load(VXml xml)
{
  clear();
  xml_foreach (childXml, xml.childs())
  {
    SnoopRtmEntry entity;
    entity.load(childXml);
    push_back(entity);
  }
}

void SnoopRtmEntryList::save(VXml xml)
{
  xml.clearChild();
  foreach (SnoopRtmEntry entry, *this)
  {
    VXml childXml = xml.addChild("entry");
    entry.save(childXml);
  }
}

// ----------------------------------------------------------------------------
// SnoopRtmInstance
// ----------------------------------------------------------------------------
class SnoopRtmInstance : public SnoopRtm
{
public:
  SnoopRtmInstance()
  {
    if (QFile::exists(DEFAULT_RTM_FILE_NAME))
    {
      loadFromFile(DEFAULT_RTM_FILE_NAME, "");
    } else
    {
      loadFromSystem();
    }
  }
  virtual ~SnoopRtmInstance()
  {
  }
};

// ----------------------------------------------------------------------------
// SnoopRtm
// ----------------------------------------------------------------------------
const QString SnoopRtm::DEFAULT_RTM_FILE_NAME = "rtm.xml";

SnoopRtm::SnoopRtm()
{
}

SnoopRtm::~SnoopRtm()
{
}

#ifdef WIN32
bool SnoopRtm::loadFromSystem()
{
  items.clear();

  bool persistentMode = false;
  char buf[vd::DEFAULT_BUF_SIZE];
  char* command = "route -4 print";

  FILE* fp = popen(command, "r");
  if (fp == NULL)
  {
    LOG_ERROR("popen(%s) return NULL", command);
    return false;
  }

  while (true)
  {
    char* p = fgets(buf, vd::DEFAULT_BUF_SIZE, fp);
    if (p == NULL) break;
    QString s = buf;

    //
    // Check Persistent mode
    //
    if (s.startsWith("Persistent Routes:") || s.startsWith("���� ����:"))
    {
      persistentMode = true;
      continue;
    }

    SnoopRtmEntry entry;
    if (!persistentMode) // activeMode
    {
      static QRegExp activeRex(
        /* (1) Network Destination */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
        /* (2) Netmask             */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
        /* (3) Gateway             */ "([^ ]+)[ ]*"\
        /* (4) Interface           */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
        /* (5) Metric              */ "([0-9]+)");

      if (activeRex.indexIn(s) == -1) continue;

      // ----- gilgil temp 2012.06.09 -----
      /*
      LOG_DEBUG("dst=%s mask=%s gateway=%s intf=%s metric=%s",
        qPrintable(activeRex.cap(1)), // Network Destination
        qPrintable(activeRex.cap(2)), // Netmask
        qPrintable(activeRex.cap(3)), // Gateway
        qPrintable(activeRex.cap(4)), // Interface
        qPrintable(activeRex.cap(5))  // Metric
      );
      */
      // ----------------------------------
      entry.dst     = activeRex.cap(1);
      entry.mask    = activeRex.cap(2);
      entry.gateway = activeRex.cap(3);
      entry.intf    = activeRex.cap(4);
      entry.metric  = activeRex.cap(5).toInt();
    } else
    {
      static QRegExp persistentRex(
        /* Network Destination */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
        /* Netmask             */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
        /* Gateway             */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
        /* Metric              */ "([^ ]+)");

      if (persistentRex.indexIn(s) == -1) continue;

      // ----- gilgil temp 2012.06.09 -----
      /*
      LOG_DEBUG("dst=%s mask=%s gateway=%s metric=%s",
        qPrintable(persistentRex.cap(1)), // Network Destination
        qPrintable(persistentRex.cap(2)), // Netmask
        qPrintable(persistentRex.cap(3)), // Gateway
        qPrintable(persistentRex.cap(4))  // Metric
      );
      */
      // ----------------------------------
      entry.dst     = persistentRex.cap(1);
      entry.mask    = persistentRex.cap(2);
      entry.gateway = persistentRex.cap(3);
      entry.intf    = "";
      entry.metric  = persistentRex.cap(5).toInt();
    }

    if (entry.gateway == 0xFFFFFFFF) entry.gateway = 0;
    if (entry.metric  == 0)          entry.metric  = 9999; // big enough value
    items.push_back(entry);
  }
  pclose(fp);

  LOG_DEBUG("count=%d", items.count()); // gilgil temp 2012.06.09

  return true;
}
#endif // WIN32
#ifdef linux
bool SnoopRtm::loadFromSystem()
{
  items.clear();

  char buf[vd::DEFAULT_BUF_SIZE];
  char* command = "route -n";

  FILE* fp = popen(command, "r");
  if (fp == NULL)
  {
    LOG_ERROR("popen(%s) return NULL", command);
    return false;
  }

  while (true)
  {
    char* p = fgets(buf, vd::DEFAULT_BUF_SIZE, fp);
    if (p == NULL) break;
    QString s = buf;

    SnoopRtmEntry entry;
    static QRegExp regex(
      /* (1) Destination */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
      /* (2) Gateway     */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
      /* (3) Genmask     */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
      /* (X) Flags       */ "[A-Za-z]+[ ]+"\
      /* (4) Metric      */ "([0-9]+)[ ]+"\
      /* (X) Ref         */ "[0-9]+[ ]+"\
      /* (X) Use         */ "[0-9]+[ ]+"\
      /* (5) Iface       */ "([A-Za-z0-9]+)");

    if (regex.indexIn(s) == -1) continue;

    // ----- gilgil temp 2012.06.09 -----
    LOG_DEBUG("dst=%s mask=%s gateway=%s intf=%s metric=%s",
      qPrintable(regex.cap(1)), // Destination
      qPrintable(regex.cap(3)), // Genmask
      qPrintable(regex.cap(2)), // Gateway
      qPrintable(regex.cap(5)), // Iface
      qPrintable(regex.cap(4))  // Metric
    );
    // ----------------------------------
    entry.dst      = regex.cap(1);
    entry.mask     = regex.cap(3);
    entry.gateway  = regex.cap(2);
    entry.intf     = regex.cap(5);
    entry.metric   = regex.cap(4).toInt();

    if (entry.gateway == 0xFFFFFFFF) entry.gateway = 0;
    // if (entry.metric  == 0)          entry.metric  = 9999; // big enough value
    items.push_back(entry);
  }
  pclose(fp);

  LOG_DEBUG("count=%d", items.count()); // gilgil temp 2012.06.09

  return true;
}
#endif // linux


bool SnoopRtm::recoverSystem()
{
  SnoopRtm systemRTM;
  systemRTM.loadFromSystem();

  bool res = true;
  for (SnoopRtmEntryList::iterator it = this->items.begin(); it != this->items.end(); it++)
  {
    SnoopRtmEntry& entry = *it;

    int index = systemRTM.items.find(entry.dst, entry.mask);
    QString routeCommand =  (index == -1) ? "add" : "add";

    QString command = "route " + routeCommand + " " + entry.dst.str() + " mask " + entry.mask.str();
    if (entry.gateway ==  0)
      command += " " + entry.intf;
    else
      command += " " + entry.gateway.str();
    LOG_DEBUG("%s", qPrintable(command)); // gilgil temp 2012.06.10
    res = VProcess::run(qPrintable(command));
    if (!res)
    {
      SET_ERROR(VError, qformat("can not run %s", qPrintable(command)), VERR_RUN_PROCESS);
      return false;
    }
  }
  return true;
}

SnoopRtmEntry* SnoopRtm::getBestEntry(Ip ip)
{
  SnoopRtmEntry* res = NULL;

  int _count = items.count();
  for (int i = 0; i < _count; i++)
  {
    SnoopRtmEntry& entry = (SnoopRtmEntry&)items.at(i);

    if ((entry.dst & entry.mask) != (ip & entry.mask)) continue;
    if (res == NULL)
    {
      res = &entry;
      continue;
    }
    if (entry.mask > res->mask)
    {
      res = &entry;
      continue;
    } else
    if (entry.mask == res->mask)
    {
      if (entry.metric < res->metric)
      {
        res = &entry;
        continue;
      }
    }
  }

  return res;
}

void SnoopRtm::load(VXml xml)
{
  items.load(xml);
}

void SnoopRtm::save(VXml xml)
{
  items.save(xml);
}

SnoopRtm& SnoopRtm::instance()
{
  static SnoopRtmInstance g_instance;
  return g_instance;
}
