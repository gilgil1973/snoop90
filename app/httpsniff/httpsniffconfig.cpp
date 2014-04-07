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

bool HttpSniffConfig::saveToFile(QString fileName)
{
  QString srcFileName = "ss/httpsniff.ss";
  if (QFile::exists(fileName))
  {
    if (!QFile::remove(fileName))
    {
      SET_ERROR(VError, qformat("can not remove file (%s)", qPrintable(fileName)), VERR_UNKNOWN);
      return false;
    }
  }
  if (!QFile::copy(srcFileName, fileName))
  {
    SET_ERROR(VError, qformat("can not copy file (%s > %s)", qPrintable(srcFileName), qPrintable(fileName)), VERR_UNKNOWN);
    return false;
  }

  VGraph graph;
  if (!graph.loadFromFile(fileName, "graph"))
  {
    error = graph.error;
    return false;
  }

  if (!this->saveToGraph(graph)) return false;

  if (!graph.saveToFile(fileName, "graph"))
  {
    error = graph.error;
    return false;
  }

  return true;
}

bool HttpSniffConfig::saveToGraph(VGraph& graph)
{
  VXmlDoc doc; VXml xml = doc.createRoot("temp");

  {
    //
    // wdOutbound
    //
    SnoopWinDivert* wdOutbound = dynamic_cast<SnoopWinDivert*>(graph.objectList.findByName("wdOutbound"));
    if (wdOutbound == NULL)
    {
      SET_ERROR(SnoopError, "can not find wdOutbound", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    wdOutbound->enabled = captureType == WinDivert;

    QString filter;

    int count = httpPortList.count();
    for (int i = 0; i < count; i++)
    {
      if (filter != "") filter += " or ";
      QString oneFilter = qformat("tcp.DstPort==%d", httpPortList.at(i));
      filter += oneFilter;
    }

    count = httpsPortList.count();
    for (int i = 0; i < count; i++)
    {
      if (filter != "") filter += " or ";
      QString oneFilter = qformat("tcp.DstPort==%d", httpsPortList.at(i));
      filter += oneFilter;
    }

    wdOutbound->filter = qformat("(ifIdx!=1) and (%s)", qPrintable(filter));
    LOG_INFO("wdOutbound->filter = \"%s\"", qPrintable(wdOutbound->filter));
  }

  //
  // as
  //
  {
    SnoopArpSpoof* as = dynamic_cast<SnoopArpSpoof*>(graph.objectList.findByName("as"));
    if (as == NULL)
    {
      SET_ERROR(SnoopError, "can not find as", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    as->enabled = captureType == ArpSpoof;

    QString filter;

    int count = httpPortList.count();
    for (int i = 0; i < count; i++)
    {
      if (filter != "") filter += " or ";
      QString oneFilter = qformat("tcp dst port %d", httpPortList.at(i));
      filter += oneFilter;
    }

    count = httpsPortList.count();
    for (int i = 0; i < count; i++)
    {
      if (filter != "") filter += " or ";
      QString oneFilter = qformat("tcp dst port %d", httpsPortList.at(i));
      filter += oneFilter;
    }

    as->filter = filter;
    LOG_INFO("ap->filter = \"%s\"", qPrintable(as->filter));
  }

  //
  // pfOutbound and pfInbound
  //
  {
    SnoopProcessFilter* pfOutbound = dynamic_cast<SnoopProcessFilter*>(graph.objectList.findByName("pfOutbound"));
    if (pfOutbound == NULL)
    {
      SET_ERROR(SnoopError, "can not find pfOutbound", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }
    SnoopProcessFilter* pfInbound = dynamic_cast<SnoopProcessFilter*>(graph.objectList.findByName("pfInbound"));
    if (pfInbound == NULL)
    {
      SET_ERROR(SnoopError, "can not find pfInbound", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    pfOutbound->showStatus = false;
    pfInbound->showStatus  = false;

    pfOutbound->policyMap.clear();
    pfInbound->policyMap.clear();
    foreach (QString processName, proxyProcessNameList)
    {
      pfOutbound->policyMap.insert(processName, true);
      pfInbound->policyMap.insert(processName, true);
    }
  }

  //
  // fc
  //
  {
    SnoopFlowChange* fc = dynamic_cast<SnoopFlowChange*>(graph.objectList.findByName("fc"));
    if (fc == NULL)
    {
      SET_ERROR(SnoopError, "can not find fc", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    fc->changeItems.clear();

    foreach (int port, httpPortList)
    {
      SnoopFlowChangeItem item;
      item.log               = false;
      item.protocol          = SnoopFlowChangeItem::Tcp;
      item.srcIpChangeType   = SnoopFlowChangeItem::IpFix;
      item.srcIpFixValue     = Ip("127.0.0.1");
      item.srcPortChangeType = SnoopFlowChangeItem::PortAutoInc;
      item.dstIpChangeType   = SnoopFlowChangeItem::IpFix;
      item.dstIpFixValue     = Ip("127.0.0.1");
      item.dstPort           = port;
      item.dstPortChangeType = SnoopFlowChangeItem::PortFix;
      item.dstPortFixValue   = this->proxyTcpInPort;
      fc->changeItems.push_back(item);
    }

    foreach (int port, httpsPortList)
    {
      SnoopFlowChangeItem item;
      item.log               = false;
      item.protocol          = SnoopFlowChangeItem::Tcp;
      item.srcIpChangeType   = SnoopFlowChangeItem::IpFix;
      item.srcIpFixValue     = Ip("127.0.0.1");
      item.srcPortChangeType = SnoopFlowChangeItem::PortAutoInc;
      item.dstIpChangeType   = SnoopFlowChangeItem::IpFix;
      item.dstIpFixValue     = Ip("127.0.0.1");
      item.dstPort           = port;
      item.dstPortChangeType = SnoopFlowChangeItem::PortFix;
      item.dstPortFixValue   = this->proxySslInPort;
      fc->changeItems.push_back(item);
    }
  }

  //
  // hpTcpIn
  //
  {
    VHttpProxy* hpTcpIn = dynamic_cast<VHttpProxy*>(graph.objectList.findByName("hpTcpIn"));
    if (hpTcpIn == NULL)
    {
      SET_ERROR(SnoopError, "can not find hpTcpIn", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    hpTcpIn->tcpEnabled          = true;
    hpTcpIn->tcpServer.port      = this->proxyTcpInPort;
    hpTcpIn->tcpServer.localHost = "127.0.0.1";
    hpTcpIn->sslEnabled          = false;
    hpTcpIn->outPolicy.method    = VHttpProxyOutPolicy::Tcp;
    hpTcpIn->outPolicy.host      = "127.0.0.1";
    hpTcpIn->outPolicy.port      = this->proxyTcpOutPort;

    hpTcpIn->inboundDataChange.clear();
    this->proxyOutboundDataChange.save(xml); hpTcpIn->outboundDataChange.load(xml);
    hpTcpIn->disableLoopbackConnection = false;
  }

  //
  // hpTcpOut
  //
  {
    VHttpProxy* hpTcpOut = dynamic_cast<VHttpProxy*>(graph.objectList.findByName("hpTcpOut"));
    if (hpTcpOut == NULL)
    {
      SET_ERROR(SnoopError, "can not find hpTcpOut", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    hpTcpOut->tcpEnabled          = true;
    hpTcpOut->tcpServer.port      = this->proxyTcpOutPort;
    hpTcpOut->tcpServer.localHost = "127.0.0.1";
    hpTcpOut->sslEnabled          = false;
    hpTcpOut->outPolicy.method    = VHttpProxyOutPolicy::Tcp;
    hpTcpOut->outPolicy.host      = "";
    hpTcpOut->outPolicy.port      = 0;

    this->proxyInboundDataChange.save(xml); hpTcpOut->inboundDataChange.load(xml);
    hpTcpOut->outboundDataChange.clear();
    hpTcpOut->disableLoopbackConnection = true;
  }

  //
  // hpSslIn
  //
  {
    VHttpProxy* hpSslIn = dynamic_cast<VHttpProxy*>(graph.objectList.findByName("hpSslIn"));
    if (hpSslIn == NULL)
    {
      SET_ERROR(SnoopError, "can not find hpSslIn", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    hpSslIn->tcpEnabled          = false;
    hpSslIn->sslEnabled          = true;
    hpSslIn->sslServer.port      = this->proxySslInPort;
    hpSslIn->sslServer.localHost = "127.0.0.1";
    hpSslIn->sslServer.processConnectMessage = false;
    hpSslIn->outPolicy.method    = VHttpProxyOutPolicy::Tcp;
    hpSslIn->outPolicy.host      = "127.0.0.1";
    hpSslIn->outPolicy.port      = this->proxySslOutPort;

    hpSslIn->inboundDataChange.clear();
    this->proxyOutboundDataChange.save(xml); hpSslIn->outboundDataChange.load(xml);
    hpSslIn->disableLoopbackConnection = false;
  }

  //
  // hpSslOut
  //
  {
    VHttpProxy* hpSslOut = dynamic_cast<VHttpProxy*>(graph.objectList.findByName("hpSslOut"));
    if (hpSslOut == NULL)
    {
      SET_ERROR(SnoopError, "can not find hpSslOut", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    hpSslOut->tcpEnabled          = true;
    hpSslOut->tcpServer.port      = this->proxySslOutPort;
    hpSslOut->tcpServer.localHost = "127.0.0.1";
    hpSslOut->sslEnabled          = false;
    hpSslOut->outPolicy.method    = VHttpProxyOutPolicy::Ssl;
    hpSslOut->outPolicy.host      = "";
    hpSslOut->outPolicy.port      = 0;

    this->proxyInboundDataChange.save(xml); hpSslOut->inboundDataChange.load(xml);
    hpSslOut->outboundDataChange.clear();
    hpSslOut->disableLoopbackConnection = true;
  }

  //
  // wdProxy
  //
  {
    SnoopWinDivert* wdProxy = dynamic_cast<SnoopWinDivert*>(graph.objectList.findByName("wdProxy"));
    if (wdProxy == NULL)
    {
      SET_ERROR(SnoopError, "can not find wdProxy", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }
    wdProxy->filter = qformat("ifIdx==1 and outbound and (tcp.SrcPort==%d or tcp.DstPort==%d or tcp.SrcPort==%d or tcp.DstPort==%d)",
      this->proxyTcpOutPort, this->proxyTcpOutPort, this->proxySslOutPort, this->proxySslOutPort);
    LOG_INFO("wdProxy->filter = \"%s\"", qPrintable(wdProxy->filter));

    wdProxy->flags = WINDIVERT_FLAG_SNIFF;
  }

  //
  // dProxy
  //
  {
    SnoopDump* dProxy = dynamic_cast<SnoopDump*>(graph.objectList.findByName("dProxy"));
    if (dProxy == NULL)
    {
      SET_ERROR(SnoopError, "can not find dProxy", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    dProxy->filePath = this->dumpFilePath;
    // if (!this->dumpEnabled) // gilgil temp 2014.04.08
  }

  //
  // waProxy
  //
  {
    SnoopWriteAdapter* waProxy = dynamic_cast<SnoopWriteAdapter*>(graph.objectList.findByName("waProxy"));
    if (waProxy == NULL)
    {
      SET_ERROR(SnoopError, "can not find waProxy", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    waProxy->enabled = this->writeAdapterEnabled;
    waProxy->adapterIndex = this->writeAdapterIndex;
  }

  return true;
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
  // Capture
  //
  captureType = (CaptureType)xml.getInt("captureType", (int)captureType);
  arpSpoof.load(xml.gotoChild("arpSpoof"));

  //
  // Proxy
  //
  QString s = xml.getStr("proxyProcessNameList");
  if (s != "") proxyProcessNameList = s.split(",");
  proxyTcpInPort = xml.getInt("proxyTcpInPort", proxyTcpInPort);
  proxyTcpOutPort = xml.getInt("proxyTcpOutPort", proxyTcpOutPort);
  proxySslInPort = xml.getInt("proxySslInPort", proxySslInPort);
  proxySslOutPort = xml.getInt("proxySslOutPort", proxySslOutPort);
  if (!xml.findChild("proxyInboundDataChange").isNull()) proxyInboundDataChange.load(xml.gotoChild("proxyInboundDataChange"));
  if (!xml.findChild("proxyOutboundDataChange").isNull()) proxyOutboundDataChange.load(xml.gotoChild("proxyOutboundDataChange"));

  //
  // Write
  //
  dumpEnabled = xml.getBool("dumpEnabled", dumpEnabled);
  dumpFilePath = xml.getStr("dumpFilePath", dumpFilePath);
  writeAdapterEnabled = xml.getBool("writeAdapterEnabled", writeAdapterEnabled);
  writeAdapterIndex = xml.getInt("writeAdapterIndex", writeAdapterIndex);
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
  // Capture
  //
  xml.setInt("captureType", (int)captureType);
  arpSpoof.save(xml.gotoChild("arpSpoof"));

  //
  // Proxy
  //
  xml.setStr("proxyProcessNameList", proxyProcessNameList.join(","));
  xml.setInt("proxyTcpInPort", proxyTcpInPort);
  xml.setInt("proxyTcpOutPort", proxyTcpOutPort);
  xml.setInt("proxySslInPort", proxySslInPort);
  xml.setInt("proxySslOutPort", proxySslOutPort);
  proxyInboundDataChange.save(xml.gotoChild("proxyInboundDataChange"));
  proxyOutboundDataChange.save(xml.gotoChild("proxyOutboundDataChange"));

  //
  // Write
  //
  xml.setBool("dumpEnabled", dumpEnabled);
  xml.setStr("dumpFilePath", dumpFilePath);
  xml.setBool("writeAdapterEnabled", writeAdapterEnabled);
  xml.setInt("writeAdapterIndex", writeAdapterIndex);
}
