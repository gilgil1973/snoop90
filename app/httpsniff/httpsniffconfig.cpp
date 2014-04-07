#include "httpsniffconfig.h"

// ----------------------------------------------------------------------------
// HttpSniffConfig
// ----------------------------------------------------------------------------
HttpSniffConfig::HttpSniffConfig()
{
  //
  // Port
  //
  httpPortList.push_back(80);
  httpPortList.push_back(8080);
  httpPortList.push_back(8888);

  httpsPortList.push_back(443);
  httpsPortList.push_back(4433); // gilgil temp 2014.04.08

  //
  // Capture
  //
  captureType = WinDivert;

  //
  // Proxy
  //
  proxyProcessNameList.push_back("httpproxy.exe");
  proxyProcessNameList.push_back("netserver.exe");
  proxyProcessNameList.push_back("snoopspy.exe");
  proxyProcessNameList.push_back("sscon.exe");

  proxyTcpInPort  = VHttpProxy::HTTP_PROXY_PORT;     // 8080
  proxyTcpOutPort = VHttpProxy::HTTP_PROXY_PORT + 1; // 8081
  proxySslInPort  = VHttpProxy::SSL_PROXY_PORT;      // 4433
  proxySslOutPort = VHttpProxy::SSL_PROXY_PORT + 1;  // 4434

  VDataChangeItem changeItem;
  changeItem.pattern = "Accept-Encoding: gzip,";
  changeItem.syntax  = QRegExp::FixedString;
  changeItem.cs      = Qt::CaseSensitive;
  changeItem.minimal = false;
  changeItem.enabled = true;
  changeItem.log     = false;
  changeItem.replace = "Accept-Encoding:      ";
  proxyOutboundDataChange.push_back(changeItem);

  //
  // Write
  //
  dumpEnabled         = true;
  dumpFilePath        = "pcap/httpsniff_%04d%02d%02d.%02d%02d.%02d.%03d.pcap";
  writeAdapterEnabled = false;
  writeAdapterIndex = snoop::INVALID_ADAPTER_INDEX;
}

HttpSniffConfig::~HttpSniffConfig()
{

}

void HttpSniffConfig::setGraph(VGraph graph)
{

}

void HttpSniffConfig::load(VXml xml)
{
  //
  // Port
  //
  if (!xml.findChild("httpPortList").isNull())
  {
    VXml childXml = xml.gotoChild("httpPortList");
    httpPortList.clear();
    xml_foreach (portXml, childXml.childs())
    {
      int port = portXml.getInt("port");
      httpPortList.push_back(port);
    }
  }

  if (!xml.findChild("httpsPortList").isNull())
  {
    VXml childXml = xml.gotoChild("httpsPortList");
    httpsPortList.clear();
    xml_foreach (portXml, childXml.childs())
    {
      int port = portXml.getInt("port");
      httpsPortList.push_back(port);
    }
  }

  //
  // Proxy
  //
  // gilgil temp 2014.04.08

  //
  // Write
  //
  // gilgil temp 2014.04.08
}

void HttpSniffConfig::save(VXml xml)
{
  //
  // Port
  //
  {
    VXml childXml = xml.gotoChild("httpPortList");
    childXml.clearChild();
    foreach (int port, httpPortList)
    {
      childXml.addChild("port").setInt("port", port);
    }
  }

  {
    VXml childXml = xml.gotoChild("httpsPortList");
    childXml.clearChild();
    foreach (int port, httpsPortList)
    {
      childXml.addChild("port").setInt("port", port);
    }
  }

  //
  // Proxy
  //
  // gilgil temp 2014.04.08

  //
  // Write
  //
  // gilgil temp 2014.04.08
}
