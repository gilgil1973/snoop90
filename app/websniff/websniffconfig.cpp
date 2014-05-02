#include "websniffconfig.h"

// ----------------------------------------------------------------------------
// HttpSniffConfig
// ----------------------------------------------------------------------------
const QString HttpSniffConfig::HTTP_STRIP_VIRTUAL_IP    = "80.43.80.43";
const QString HttpSniffConfig::HTTP_STRIP_DOMAIN_PREFIX = "ss.";

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

  proxyHttpInPort   = VWebProxy::HTTP_PROXY_PORT;     // 8080
  proxyHttpOutPort  = VWebProxy::HTTP_PROXY_PORT + 1; // 8081
  proxyHttpsInPort  = VWebProxy::SSL_PROXY_PORT;      // 4433
  proxyHttpsOutPort = VWebProxy::SSL_PROXY_PORT + 1;  // 4434
  proxyStripInPort  = HTTP_STRIP_PORT;                // 8043
  proxyStripOutPort = HTTP_STRIP_PORT + 1;            // 8044

  VXmlDoc doc; VXml xml = doc.createRoot("temp");
  VWebProxy proxy; proxy.outboundDataChange.save(xml); proxyOutboundDataChange.load(xml);

  VDataChangeItem changeItem;
  //
  // Accept-Encoding
  //
  changeItem.pattern = "\r\nAccept-Encoding: ";
  changeItem.syntax  = QRegExp::FixedString;
  changeItem.cs      = Qt::CaseSensitive;
  changeItem.enabled = true;
  changeItem.log     = false;
  changeItem.replace = "\r\nAccept-Encoding-SS: ";
  proxyOutboundDataChange.push_back(changeItem);

  //
  // If-Modified-Since
  //
  changeItem.pattern = "\r\nIf-Modified-Since:[^\r\n]*";
  changeItem.syntax  = QRegExp::RegExp;
  changeItem.cs      = Qt::CaseSensitive;
  changeItem.enabled = true;
  changeItem.log     = false;
  changeItem.replace = "\r\nIf-Modified-Since-SS: ";
  proxyOutboundDataChange.push_back(changeItem);

  //
  // If-None-Match
  //
  changeItem.pattern = "\r\nIf-None-Match:[^\r\n]*";
  changeItem.syntax  = QRegExp::RegExp;
  changeItem.cs      = Qt::CaseSensitive;
  changeItem.enabled = true;
  changeItem.log     = false;
  changeItem.replace = "\r\nIf-None-Match-SS: ";
  proxyOutboundDataChange.push_back(changeItem);

  //
  // Last-Modified
  //
  changeItem.pattern = "\r\nLast-Modified:[^\r\n]*";
  changeItem.syntax  = QRegExp::RegExp;
  changeItem.cs      = Qt::CaseSensitive;
  changeItem.enabled = true;
  changeItem.log     = false;
  changeItem.replace = "\r\nLast-Modified-SS:";
  proxyInboundDataChange.push_back(changeItem);

  //
  // SslStrip
  //
  sslStripEnabled      = false;
  sslStripVirtualIp    = HTTP_STRIP_VIRTUAL_IP;
  sslStripDomainPrefix = HTTP_STRIP_DOMAIN_PREFIX;

  //
  // Write
  //
  dumpEnabled         = true;
  dumpFilePath        = "pcap/websniff_%04d%02d%02d.%02d%02d.%02d.%03d.pcap";
  writeAdapterEnabled = false;
  writeAdapterIndex = snoop::INVALID_ADAPTER_INDEX;
}

HttpSniffConfig::~HttpSniffConfig()
{

}

void HttpSniffConfig::addSslStripOutboundDataChange(VDataChange& dataChange)
{
  {
    VDataChangeItem item;
    item.pattern = "\r\nHost: " + this->sslStripDomainPrefix;
    item.syntax  = QRegExp::FixedString;
    item.enabled = true;
    item.log     = false;
    item.replace = "\r\nHost: ";
    dataChange.push_back(item);
  }

  {
    VDataChangeItem item;
    item.pattern = "http://" + this->sslStripDomainPrefix;
    item.syntax  = QRegExp::FixedString;
    item.enabled = true;
    item.log     = false;
    item.replace = "https://";
    dataChange.push_back(item);
  }
}

void HttpSniffConfig::addSslStripInboundDataChange(VDataChange& dataChange)
{
  {
    VDataChangeItem item;
    item.pattern = "https://";
    item.syntax  = QRegExp::FixedString;
    item.enabled = true;
    item.log     = false;
    item.replace = qPrintable(QString("http://") + sslStripDomainPrefix);
    dataChange.push_back(item);
  }

  {
    VDataChangeItem item;
    item.pattern = "\r\nLast-Modified:[^\r\n]*";
    item.syntax  = QRegExp::RegExp;
    item.enabled = true;
    item.log     = false;
    item.replace = "\r\nLast-Modified-SS:";
    dataChange.push_back(item);
  }

  {
    VDataChangeItem item;
    item.pattern = "; secure";
    item.syntax  = QRegExp::FixedString;
    item.enabled = true;
    item.log     = false;
    item.replace = "";
    dataChange.push_back(item);
  }
}

bool HttpSniffConfig::saveToFile(QString fileName)
{
  QString srcFileName = "ss/websniff._ss";
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

  //
  // wdOutbound
  //
  SnoopWinDivert* wdOutbound = dynamic_cast<SnoopWinDivert*>(graph.objectList.findByName("wdOutbound"));
  {
    if (wdOutbound == NULL)
    {
      SET_ERROR(SnoopError, "can not find wdOutbound", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    winDivert.save(xml); wdOutbound->load(xml);
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

    if (sslStripEnabled)
    {
      filter += " or udp.DstPort==53";
    }

    wdOutbound->filter = qformat("(ifIdx!=1) and outbound and (%s)", qPrintable(filter));
    LOG_INFO("wdOutbound->filter = \"%s\"", qPrintable(wdOutbound->filter));
  }

  //
  // wdInbound
  //
  {
    SnoopWinDivert* wdInbound = dynamic_cast<SnoopWinDivert*>(graph.objectList.findByName("wdInbound"));
    if (wdInbound == NULL)
    {
      SET_ERROR(SnoopError, "can not find wdInbound", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    wdInbound->enabled = true;

    QString filter = qformat("tcp.SrcPort==%d or tcp.SrcPort==%d", proxyHttpInPort, proxyHttpsInPort);
    if (sslStripEnabled)
    {
      filter += qformat(" or tcp.SrcPort==%d", proxyStripInPort);
    }
    filter = qformat("(ifIdx==1) and (%s)", qPrintable(filter));

    wdInbound->filter = filter;
    LOG_INFO("wdInbound->filter = \"%s\"", qPrintable(wdInbound->filter));
  }

  //
  // asOutbound
  //
  SnoopArpSpoof* asOutbound = dynamic_cast<SnoopArpSpoof*>(graph.objectList.findByName("asOutbound"));
  {
    if (asOutbound == NULL)
    {
      SET_ERROR(SnoopError, "can not find asOutbound", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    arpSpoof.save(xml); asOutbound->load(xml);

    asOutbound->enabled = captureType == ArpSpoof;

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

    if (sslStripEnabled)
    {
      filter += " or udp dst port 53";
    }

    asOutbound->filter = filter;
    LOG_INFO("ap->filter = \"%s\"", qPrintable(asOutbound->filter));
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

    pfOutbound->flowMgr = dynamic_cast<SnoopFlowMgr*>(graph.objectList.findByName("fmOutboundProc"));
    if (pfOutbound->flowMgr == NULL)
    {
      SET_ERROR(SnoopError, "can not find fmOutboundProc", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }
    pfInbound->flowMgr = dynamic_cast<SnoopFlowMgr*>(graph.objectList.findByName("fmInboundProc"));
    if (pfInbound->flowMgr == NULL)
    {
      SET_ERROR(SnoopError, "can not find fmInboundProc", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    pfOutbound->showStatus = false;
    pfInbound->showStatus = false;

    pfOutbound->policyMap.clear();
    pfInbound->policyMap.clear();
    foreach (QString processName, proxyProcessNameList)
    {
      pfOutbound->policyMap.insert(processName, true);
      pfInbound->policyMap.insert(processName, true);
    }
  }

  //
  // fcMain
  //
  {
    SnoopFlowChange* fcMain = dynamic_cast<SnoopFlowChange*>(graph.objectList.findByName("fcMain"));
    if (fcMain == NULL)
    {
      SET_ERROR(SnoopError, "can not find fcMain", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    fcMain->fromFlowMgr = dynamic_cast<SnoopFlowMgr*>(graph.objectList.findByName("fmOutbound"));
    if (fcMain->fromFlowMgr == NULL)
    {
      SET_ERROR(SnoopError, "can not find fmOutbound", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }
    fcMain->toFlowMgr = dynamic_cast<SnoopFlowMgr*>(graph.objectList.findByName("fmInbound"));
    if (fcMain->toFlowMgr == NULL)
    {
      SET_ERROR(SnoopError, "can not find fmInbound", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    fcMain->tcpChange = true;
    fcMain->udpChange = false;

    fcMain->changeItems.clear();
    if (sslStripEnabled)
    {
      foreach (int port, httpPortList)
      {
        SnoopFlowChangeItem item;
        item.log               = false;
        item.protocol          = SnoopFlowChangeItem::Tcp;
        item.srcIpChangeType   = SnoopFlowChangeItem::IpFix;
        item.srcIpFixValue     = Ip("127.0.0.1");
        item.srcPortChangeType = SnoopFlowChangeItem::PortAutoInc;
        item.dstIp             = sslStripVirtualIp;
        item.dstIpMask         = Ip("255.255.255.255");
        item.dstIpChangeType   = SnoopFlowChangeItem::IpFix;
        item.dstIpFixValue     = Ip("127.0.0.1");
        item.dstPort           = port;
        item.dstPortChangeType = SnoopFlowChangeItem::PortFix;
        item.dstPortFixValue   = this->proxyStripInPort;
        fcMain->changeItems.push_back(item);
      }
    }

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
      item.dstPortFixValue   = this->proxyHttpInPort;
      fcMain->changeItems.push_back(item);
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
      item.dstPortFixValue   = this->proxyHttpsInPort;
      fcMain->changeItems.push_back(item);
    }
  }

  //
  // wwdOutbound
  //
  {
    SnoopWriteWinDivert* wwdOutbound = dynamic_cast<SnoopWriteWinDivert*>(graph.objectList.findByName("wwdOutbound"));
    if (wwdOutbound == NULL)
    {
      SET_ERROR(SnoopError, "can not find wwdOutbound", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    wwdOutbound->enabled              = true;
    wwdOutbound->autoRead             = false;
    wwdOutbound->changeDivertAddr     = true;
    wwdOutbound->divertAddr.IfIdx     = 1;
    wwdOutbound->divertAddr.SubIfIdx  = 0;
    wwdOutbound->divertAddr.Direction = WINDIVERT_DIRECTION_OUTBOUND;
  }

  //
  // wpHttpIn
  //
  {
    VWebProxy* wpHttpIn = dynamic_cast<VWebProxy*>(graph.objectList.findByName("wpHttpIn"));
    if (wpHttpIn == NULL)
    {
      SET_ERROR(SnoopError, "can not find wpHttpIn", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    wpHttpIn->httpEnabled         = true;
    wpHttpIn->tcpServer.port      = this->proxyHttpInPort;
    wpHttpIn->tcpServer.localHost = "127.0.0.1";
    wpHttpIn->httpsEnabled        = false;
    wpHttpIn->outPolicy.method    = VWebProxyOutPolicy::Http;
    wpHttpIn->outPolicy.host      = "127.0.0.1";
    wpHttpIn->outPolicy.port      = this->proxyHttpOutPort;

    wpHttpIn->inboundDataChange.clear();
    this->proxyOutboundDataChange.save(xml); wpHttpIn->outboundDataChange.load(xml);
    wpHttpIn->disableLoopbackConnection = false;
  }

  //
  // wpHttpOut
  //
  {
    VWebProxy* wpHttpOut = dynamic_cast<VWebProxy*>(graph.objectList.findByName("wpHttpOut"));
    if (wpHttpOut == NULL)
    {
      SET_ERROR(SnoopError, "can not find wpHttpOut", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    wpHttpOut->httpEnabled         = true;
    wpHttpOut->tcpServer.port      = this->proxyHttpOutPort;
    wpHttpOut->tcpServer.localHost = "127.0.0.1";
    wpHttpOut->httpsEnabled        = false;
    wpHttpOut->outPolicy.method    = VWebProxyOutPolicy::Http;
    wpHttpOut->outPolicy.host      = "";
    wpHttpOut->outPolicy.port      = 0;

    this->proxyInboundDataChange.save(xml); wpHttpOut->inboundDataChange.load(xml);
    if (sslStripEnabled)
    {
      addSslStripInboundDataChange(wpHttpOut->inboundDataChange);
    }
    wpHttpOut->outboundDataChange.clear();
    wpHttpOut->disableLoopbackConnection = true;
  }

  //
  // wpHttpsIn
  //
  {
    VWebProxy* wpHttpsIn = dynamic_cast<VWebProxy*>(graph.objectList.findByName("wpHttpsIn"));
    if (wpHttpsIn == NULL)
    {
      SET_ERROR(SnoopError, "can not find wpHttpsIn", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    wpHttpsIn->httpEnabled         = false;
    wpHttpsIn->httpsEnabled        = true;
    wpHttpsIn->sslServer.port      = this->proxyHttpsInPort;
    wpHttpsIn->sslServer.localHost = "127.0.0.1";
    wpHttpsIn->sslServer.processConnectMessage = false;
    wpHttpsIn->outPolicy.method    = VWebProxyOutPolicy::Http;
    wpHttpsIn->outPolicy.host      = "127.0.0.1";
    wpHttpsIn->outPolicy.port      = this->proxyHttpsOutPort;

    wpHttpsIn->inboundDataChange.clear();
    this->proxyOutboundDataChange.save(xml); wpHttpsIn->outboundDataChange.load(xml);
    wpHttpsIn->disableLoopbackConnection = false;
  }

  //
  // wpHttpsOut
  //
  {
    VWebProxy* wpHttpsOut = dynamic_cast<VWebProxy*>(graph.objectList.findByName("wpHttpsOut"));
    if (wpHttpsOut == NULL)
    {
      SET_ERROR(SnoopError, "can not find wpHttpsOut", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    wpHttpsOut->httpEnabled         = true;
    wpHttpsOut->tcpServer.port      = this->proxyHttpsOutPort;
    wpHttpsOut->tcpServer.localHost = "127.0.0.1";
    wpHttpsOut->httpsEnabled        = false;
    wpHttpsOut->outPolicy.method    = VWebProxyOutPolicy::Https;
    wpHttpsOut->outPolicy.host      = "";
    wpHttpsOut->outPolicy.port      = 0;

    this->proxyInboundDataChange.save(xml); wpHttpsOut->inboundDataChange.load(xml);
    wpHttpsOut->outboundDataChange.clear();
    wpHttpsOut->disableLoopbackConnection = true;
  }

  //
  // wpStripIn
  //
  {
    VWebProxy* wpStripIn = dynamic_cast<VWebProxy*>(graph.objectList.findByName("wpStripIn"));
    if (wpStripIn == NULL)
    {
      SET_ERROR(SnoopError, "can not find wpStripIn", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    wpStripIn->enabled             = sslStripEnabled;
    wpStripIn->httpEnabled         = true;
    wpStripIn->tcpServer.port      = this->proxyStripInPort;
    wpStripIn->tcpServer.localHost = "127.0.0.1";
    wpStripIn->httpsEnabled        = false;
    wpStripIn->outPolicy.method    = VWebProxyOutPolicy::Http;
    wpStripIn->outPolicy.host      = "127.0.0.1";
    wpStripIn->outPolicy.port      = this->proxyStripOutPort;

    wpStripIn->inboundDataChange.clear();
    this->proxyOutboundDataChange.save(xml); wpStripIn->outboundDataChange.load(xml);
    {
      addSslStripOutboundDataChange(wpStripIn->outboundDataChange);
    }
    wpStripIn->disableLoopbackConnection = false;
  }

  //
  // wpStripOut
  //
  {
    VWebProxy* wpStripOut = dynamic_cast<VWebProxy*>(graph.objectList.findByName("wpStripOut"));
    if (wpStripOut == NULL)
    {
      SET_ERROR(SnoopError, "can not find wpStripOut", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    wpStripOut->enabled =           sslStripEnabled;
    wpStripOut->httpEnabled         = true;
    wpStripOut->tcpServer.port      = this->proxyStripOutPort;
    wpStripOut->tcpServer.localHost = "127.0.0.1";
    wpStripOut->httpsEnabled        = false;
    wpStripOut->outPolicy.method    = VWebProxyOutPolicy::Https;
    wpStripOut->outPolicy.host      = "";
    wpStripOut->outPolicy.port      = 0;

    this->proxyInboundDataChange.save(xml); wpStripOut->inboundDataChange.load(xml);
    {
      addSslStripInboundDataChange(wpStripOut->inboundDataChange);
    }
    wpStripOut->outboundDataChange.clear();
    wpStripOut->disableLoopbackConnection = true;
  }

  //
  // dnscStrip
  //
  {
    SnoopDnsChange* dnscStrip = dynamic_cast<SnoopDnsChange*>(graph.objectList.findByName("dnscStrip"));
    if (dnscStrip == NULL)
    {
      SET_ERROR(SnoopError, "can not find dnscStrip", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    if (captureType == WinDivert)     dnscStrip->writer = wdOutbound;
    else if (captureType == ArpSpoof) dnscStrip->writer = asOutbound;

    dnscStrip->changeItems.clear();
    SnoopDnsChangeItem item;
    item.pattern = sslStripDomainPrefix + "*";
    item.syntax  = QRegExp::Wildcard;
    item.enabled = true;
    item.log     = true;
    item.ip      = sslStripVirtualIp;
    dnscStrip->changeItems.push_back(item);

    if (!sslStripEnabled)
    {
      VObject::disconnect(wdOutbound, SIGNAL(captured(SnoopPacket*)), dnscStrip, SLOT(check(SnoopPacket*)));
      VObject::disconnect(asOutbound, SIGNAL(captured(SnoopPacket*)), dnscStrip, SLOT(check(SnoopPacket*)));
    }
  }

  //
  // wdProxy
  //
  SnoopWinDivert* wdProxy = dynamic_cast<SnoopWinDivert*>(graph.objectList.findByName("wdProxy"));
  {
    if (wdProxy == NULL)
    {
      SET_ERROR(SnoopError, "can not find wdProxy", VERR_CAN_NOT_FIND_OBJECT);
      return false;
    }

    QString filter = qformat("tcp.SrcPort==%d or tcp.DstPort==%d or tcp.SrcPort==%d or tcp.DstPort==%d",
      this->proxyHttpOutPort, this->proxyHttpOutPort, this->proxyHttpsOutPort, this->proxyHttpsOutPort);

    if (sslStripEnabled)
    {
      filter += qformat(" or tcp.SrcPort==%d or tcp.DstPort==%d",
       this->proxyStripOutPort, this->proxyStripOutPort);
    }
    filter = qformat("(ifIdx==1) and outbound and (%s)", qPrintable(filter));

    wdProxy->filter = filter;
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

    if (!dumpEnabled)
    {
      VObject::disconnect(wdProxy, SIGNAL(captured(SnoopPacket*)), dProxy, SLOT(dump(SnoopPacket*)));
    }
    dProxy->filePath = this->dumpFilePath;
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
  proxyHttpInPort   = xml.getInt("proxyHttpInPort",   proxyHttpInPort);
  proxyHttpOutPort  = xml.getInt("proxyHttpOutPort",  proxyHttpOutPort);
  proxyHttpsInPort  = xml.getInt("proxyHttpsInPort",  proxyHttpsInPort);
  proxyHttpsOutPort = xml.getInt("proxyHttpsOutPort", proxyHttpsOutPort);
  proxyStripInPort  = xml.getInt("proxyStripInPort",  proxyStripInPort);
  proxyStripOutPort = xml.getInt("proxyStripOutPort", proxyStripOutPort);
  if (!xml.findChild("proxyInboundDataChange").isNull()) proxyInboundDataChange.load(xml.gotoChild("proxyInboundDataChange"));
  if (!xml.findChild("proxyOutboundDataChange").isNull()) proxyOutboundDataChange.load(xml.gotoChild("proxyOutboundDataChange"));

  //
  // SslStrip
  //
  sslStripEnabled      = xml.getBool("sslStripEnabled",     sslStripEnabled);
  sslStripVirtualIp    = xml.getStr("sslStripVirtualIp",    sslStripVirtualIp);
  sslStripDomainPrefix = xml.getStr("sslStripDomainPrefix", sslStripDomainPrefix);

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
  xml.setInt("proxyHttpInPort",   proxyHttpInPort);
  xml.setInt("proxyHttpOutPort",  proxyHttpOutPort);
  xml.setInt("proxyHttpsInPort",  proxyHttpsInPort);
  xml.setInt("proxyHttpsOutPort", proxyHttpsOutPort);
  xml.setInt("proxyStripInPort",  proxyStripInPort);
  xml.setInt("proxyStripOutPort", proxyStripOutPort);
  proxyInboundDataChange.save(xml.gotoChild("proxyInboundDataChange"));
  proxyOutboundDataChange.save(xml.gotoChild("proxyOutboundDataChange"));

  //
  // SslStrip
  //
  xml.setBool("sslStripEnabled",     sslStripEnabled);
  xml.setStr("sslStripVirtualIp",    sslStripVirtualIp);
  xml.setStr("sslStripDomainPrefix", sslStripDomainPrefix);

  //
  // Write
  //
  xml.setBool("dumpEnabled", dumpEnabled);
  xml.setStr("dumpFilePath", dumpFilePath);
  xml.setBool("writeAdapterEnabled", writeAdapterEnabled);
  xml.setInt("writeAdapterIndex", writeAdapterIndex);
}
