#ifndef __V_HTTP_SNIFF_CONFIG_H__
#define __V_HTTP_SNIFF_CONFIG_H__

#include <SnoopWinDivert>
#include <SnoopArpSpoof>
#include <VHttpProxy>
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
  HttpSniffConfig();
  virtual ~HttpSniffConfig();

public:
  //
  // Port
  //
  QList<int> httpPortList;
  QList<int> httpsPortList;

  //
  // Capture
  //
  CaptureType   captureType;
  SnoopArpSpoof arpSpoof;

  //
  // Proxy
  //
  QStringList proxyProcessNameList;
  int         proxyTcpInPort;
  int         proxyTcpOutPort;
  int         proxySslInPort;
  int         proxySslOutPort;
  VDataChange proxyInboundDataChange;
  VDataChange proxyOutboundDataChange;

  //
  // Write
  //
  bool              dumpEnabled;
  QString           dumpFilePath;
  bool              writeAdapterEnabled;
  SnoopAdapterIndex writeAdapterIndex;

public:
  void loadFromFile(QString fileName);
  void saveToFile(QString fileName);

  void loadFromGraph(VGraph graph);
  void setGraph(VGraph graph);
  void saveToGraph(VGraph graph);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

#endif // __V_HTTP_SNIFF_CONFIG_H__
