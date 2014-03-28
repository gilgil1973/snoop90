#include <SnoopHostList>

// ----------------------------------------------------------------------------
// SnoopHost
// ----------------------------------------------------------------------------
SnoopHost::SnoopHost(Ip ip, Mac mac, QString name)
{
  this->ip   = ip;
  this->mac  = mac;
  this->name = name;
}

SnoopHost::~SnoopHost()
{

}

void SnoopHost::load(VXml xml)
{
  ip   = xml.getStr("ip",   ip.str());
  mac  = xml.getStr("mac",  mac.str());
  name = xml.getStr("name", name);
}

void SnoopHost::save(VXml xml)
{
  xml.setStr("ip",   ip.str());
  xml.setStr("mac",  mac.str());
  xml.setStr("name", name);
}

// ----------------------------------------------------------------------------
// SnoopHostList
// ----------------------------------------------------------------------------
SnoopHost* SnoopHostList::findByIp(Ip ip)
{
  for (SnoopHostList::iterator it = begin(); it != end(); it++)
  {
    SnoopHost& host = *it;
    if (host.ip == ip) return &host;
  }
  return NULL;
}

void SnoopHostList::load(VXml xml)
{
  clear();
  {
    xml_foreach (childXml, xml.childs())
    {
      SnoopHost host;
      host.load(childXml);
      push_back(host);
    }
  }
}

void SnoopHostList::save(VXml xml)
{
  xml.clearChild();
  for (SnoopHostList::iterator it = begin(); it != end(); it++)
  {
    SnoopHost& host = *it;
    VXml childXml = xml.addChild("host");
    host.save(childXml);
  }
}
