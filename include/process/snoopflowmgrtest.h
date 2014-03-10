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
  void macFlowCreate(SnoopMacFlowKey* key);
  void macFlowDelete(SnoopMacFlowKey* key);
  void macFlowProcess(SnoopPacket* packet);

  //
  // IpFlow
  //
  void ipFlowCreate(SnoopIpFlowKey* key);
  void ipFlowDelete(SnoopIpFlowKey* key);
  void ipFlowProcess(SnoopPacket* packet);

  //
  // TcpFlow
  //
  void tcpFlowCreate(SnoopTcpFlowKey* key);
  void tcpFlowDelete(SnoopTcpFlowKey* key);
  void tcpFlowProcess(SnoopPacket* packet);

  //
  // UdpFlow
  //
  void udpFlowCreate(SnoopUdpFlowKey* key);
  void udpFlowDelete(SnoopUdpFlowKey* key);
  void udpFlowProcess(SnoopPacket* packet);

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
