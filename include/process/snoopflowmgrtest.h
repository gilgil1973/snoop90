// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_KEY_MGR_TEST_H__
#define __SNOOP_KEY_MGR_TEST_H__

#include <SnoopProcess>
#include <SnoopFlowMgr>

// ----------------------------------------------------------------------------
// SnoopFlowMgrTest
// ----------------------------------------------------------------------------
class SnoopFlowMgrTest : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopFlowMgrTest(void* owner = NULL);
  virtual ~SnoopFlowMgrTest();

public:
  SnoopFlowMgr* flowMgr;

  bool          macFlowEnabled;
  size_t        macFlowMemSize;
  bool          ipFlowEnabled;
  size_t        ipFlowMemSize;
  bool          tcpFlowEnabled;
  size_t        tcpFlowMemSize;
  bool          udpFlowEnabled;
  size_t        udpFlowMemSize;

protected:
  virtual bool doOpen();
  virtual bool doClose();

protected:
  size_t macFlowOffset;
  size_t ipFlowOffset;
  size_t tcpFlowOffset;
  size_t udpFlowOffset;

public slots:
  //
  // MacFlow
  //
  void __macFlowCreate(SnoopMacFlowKey* key, SnoopFlowValue* value);
  void __macFlowDelete(SnoopMacFlowKey* key, SnoopFlowValue* value);
  void __macCaptured(SnoopPacket* packet);

  //
  // IpFlow
  //
  void __ipFlowCreate(SnoopIpFlowKey* key, SnoopFlowValue* value);
  void __ipFlowDelete(SnoopIpFlowKey* key, SnoopFlowValue* value);
  void __ipCaptured(SnoopPacket* packet);

  //
  // TcpFlow
  //
  void __tcpFlowCreate(SnoopTcpFlowKey* key, SnoopFlowValue* value);
  void __tcpFlowDelete(SnoopTcpFlowKey* key, SnoopFlowValue* value);
  void __tcpCaptured(SnoopPacket* packet);

  //
  // UdpFlow
  //
  void __udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value);
  void __udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value);
  void __udpCaptured(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_KEY_MGR_TEST_H__
