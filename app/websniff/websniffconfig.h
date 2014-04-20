#ifndef __V_HTTP_SNIFF_CONFIG_H__
#define __V_HTTP_SNIFF_CONFIG_H__

#include <SnoopWinDivert>
#include <SnoopArpSpoof>
#include <SnoopProcessFilter>
#include <SnoopFlowChange>
#include <SnoopWriteWinDivert>
#include <VWebProxy>
#include <SnoopDnsChange>
#include <SnoopDump>
#include <SnoopWriteAdapter>
#include <VGraph>

// ----------------------------------------------------------------------------
// HttpSniffConfig
// ----------------------------------------------------------------------------
class HttpSniffConfig : public VXmlable
{
public:
  typedef enum {
    WinDivert, // 0
    ArpSpoof   // 1
  } CaptureType;

public:
  static const QString HTTP_STRIP_VIRTUAL_IP; // "80.43.80.43"
  static const QString HTTP_STRIP_DOMAIN_PREFIX; // "ss."
  static const int     HTTP_STRIP_PORT  = 8043;

public:
  HttpSniffConfig();
  virtual ~HttpSniffConfig();

public:
  VError error;

public:
  //
  // Port
  //
  QList<int> httpPortList;
  QList<int> httpsPortList;

  //
  // Capture
  //
  CaptureType    captureType;
  SnoopWinDivert winDivert;
  SnoopArpSpoof  arpSpoof;

  //
  // Proxy
  //
  QStringList proxyProcessNameList;
  int         proxyHttpInPort;
  int         proxyHttpOutPort;
  int         proxyHttpsInPort;
  int         proxyHttpsOutPort;
  int         proxyStripInPort;
  int         proxyStripOutPort;
  VDataChange proxyInboundDataChange;
  VDataChange proxyOutboundDataChange;

  //
  // SslStrip
  //
  bool    sslStripEnabled;
  QString sslStripVirtualIp;
  QString sslStripDomainPrefix;

  //
  // Write
  //
  bool              dumpEnabled;
  QString           dumpFilePath;
  bool              writeAdapterEnabled;
  SnoopAdapterIndex writeAdapterIndex;

protected:
  void addSslStripOutboundDataChange(VDataChange& dataChange);
  void addSslStripInboundDataChange(VDataChange& dataChange);

public:
  bool saveToFile(QString fileName);
  bool saveToGraph(VGraph& graph);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

#endif // __V_HTTP_SNIFF_CONFIG_H__
