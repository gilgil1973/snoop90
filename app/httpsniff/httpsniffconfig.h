#ifndef __V_HTTP_SNIFF_CONFIG_H__
#define __V_HTTP_SNIFF_CONFIG_H__

#include <SnoopWinDivert>
#include <SnoopArpSpoof>
#include <SnoopProcessFilter>
#include <SnoopFlowChange>
#include <SnoopWriteWinDivert>
#include <VHttpProxy>
#include <SnoopDump>
#include <SnoopWriteAdapter>

#include <VGraph>

// ----------------------------------------------------------------------------
// HttpSniffPortsConfig
// ----------------------------------------------------------------------------
class HttpSniffPortsConfig : public QList<int>, public VXmlable
{
public:
  bool enabled;

public:
  HttpSniffPortsConfig();

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// HttpSniffCaptureConfig
// ----------------------------------------------------------------------------
class HttpSniffCaptureConfig : public VXmlable
{
public:
  typedef enum {
    WinDivert, // 0
    ArpSpoof   // 1
  } CaptureType;

public:
  CaptureType    type;
  SnoopWinDivert winDivert;
  SnoopArpSpoof  arpSpoof;

public:
  HttpSniffCaptureConfig();

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// HttpSniffProxyConfig
// ----------------------------------------------------------------------------
class HttpSniffProxyConfig : public VXmlable
{
public:
  int         inPort;
  int         outPort;
  VDataChange proxyInboundDataChange;
  VDataChange proxyOutboundDataChange;

public:
  HttpSniffProxyConfig();

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// HttpWriteConfig
// ----------------------------------------------------------------------------
class HttpWriteConfig : public VXmlable
{
public:
  bool              dumpEnabled;
  QString           dumpFilePath;
  bool              writeAdapterEnabled;
  SnoopAdapterIndex writeAdapterIndex;

public:
  HttpWriteConfig();

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// HttpSniffConfig
// ----------------------------------------------------------------------------
class HttpSniffConfig : public VXmlable
{
public:
  static const QString STRIP_PROXY_VIRTUAL_IP; // "80.43.80.43";
  static const int     STRIP_PROXY_PORT = 8043;

public:
  HttpSniffConfig();
  virtual ~HttpSniffConfig();

public:
  VError error;

public:
  //
  // Port
  //
  HttpSniffPortsConfig tcpPorts;
  HttpSniffPortsConfig sslPorts;
  HttpSniffPortsConfig stripPorts;

  //
  // Capture
  //
  HttpSniffCaptureConfig capture;

  //
  // Proxy
  //
  QStringList          proxyProcessNameList;
  HttpSniffProxyConfig tcpProxy;
  HttpSniffProxyConfig sslProxy;
  HttpSniffProxyConfig stripProxy;

  //
  // Write
  //
  HttpWriteConfig write;

public:
  bool saveToFile(QString fileName);
  bool saveToGraph(VGraph& graph);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

#endif // __V_HTTP_SNIFF_CONFIG_H__
