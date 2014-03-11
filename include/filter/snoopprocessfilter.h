// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_PROCESS_FILTER_H__
#define __SNOOP_PROCESS_FILTER_H__

#include <QMap>
#include <SnoopFilter>
#include <SnoopTypeKey>
#include <SnoopFlowMgr>

// ----------------------------------------------------------------------------
// SnoopProcessFilterItem
// ----------------------------------------------------------------------------
class SnoopProcessFilterItem
{
public:
  quint32 pid;
  bool    ack;
};

// ----------------------------------------------------------------------------
// SnoopProcessPolicyMap
// ----------------------------------------------------------------------------
class SnoopProcessPolicyMap : public QMap<QString /*processName*/, bool /*ack*/>, public VLockable, public VXmlable
{
public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// SnoopProcessFilter
// ----------------------------------------------------------------------------
#ifdef QT_GUI_LIB
class SnoopProcessFilterWidget;
#endif // QT_GUI_LIB

class SnoopProcessFilter : public SnoopFilter
{
  Q_OBJECT

public:
  SnoopProcessFilter(void* owner = NULL);
  virtual ~SnoopProcessFilter();

protected:
  virtual bool doOpen();
  virtual bool doClose();

protected:
  bool getProcessInfo(/*in*/ SnoopTupleFlowKey& tuple, /*out*/ quint32& pid, /*out*/ QString& processName);
  bool getACK(/*in*/ SnoopTupleFlowKey& tuple, /*out*/ bool& ack);

public:
  SnoopFlowMgr*         flowMgr;
  SnoopProcessPolicyMap policyMap;

protected:
    size_t tupleFlowOffset;
#ifdef QT_GUI_LIB
public:
  bool showStatus;

protected:
  SnoopProcessFilterWidget* widget;
#endif // QT_GUI_LIB

protected slots:
  void tupleCreate(SnoopTupleFlowKey* key, SnoopFlowValue* value);
  void tupleDelete(SnoopTupleFlowKey* key, SnoopFlowValue* value);

public slots:
  void tupleCheck(SnoopPacket* packet);

signals:
  void ack(SnoopPacket* packet);
  void nak(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_PROCESS_FILTER_H__
