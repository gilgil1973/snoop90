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
#include <SnoopKeyMgr>
#include <VGraph>

// ----------------------------------------------------------------------------
// SnoopKeyMgrTest
// ----------------------------------------------------------------------------
class SnoopKeyMgrTest : public SnoopProcess, public ISnoopKeyMgrAccessible_MacFlow
{
  Q_OBJECT

public:
  SnoopKeyMgrTest(void* owner = NULL);
  virtual ~SnoopKeyMgrTest();

public:
  SnoopKeyMgr* keyMgr;
  int          user;
  size_t       memSize;

protected:
  virtual bool doOpen();
  virtual bool doClose();

public: // ISnoopKeyMgr_Mac_Accessible
  virtual void onNew_MacFlow(SnoopMacFlowKey* key, int user, void* mem);
  virtual void onDel_MacFlow(SnoopMacFlowKey* key, int user, void* mem);

public slots:
  void test(SnoopPacket* packet);

public:

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void addOptionWidget(QLayout* layout);
  virtual void saveOptionDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_KEY_MGR_TEST_H__
