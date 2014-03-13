// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_DATA_CHANGE_H__
#define __SNOOP_DATA_CHANGE_H__

#include <SnoopProcess>
#include <SnoopFlowMgr>
#include <QRegExp>

// ----------------------------------------------------------------------------
// SnoopDataChangeFlowItem
// ----------------------------------------------------------------------------
class SnoopDataChangeFlowItem
{
public:
  int seqDiff;
  int ackDiff;
};

// ----------------------------------------------------------------------------
// SnoopDataChangeItem
// ----------------------------------------------------------------------------
class SnoopDataChangeItem : public VXmlable
{
public:
  bool       enabled;
  bool       re;
  bool       log;
  QByteArray from;
  QByteArray to;

public:
  SnoopDataChangeItem();

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// SnoopDataChangeItems
// ----------------------------------------------------------------------------
class SnoopDataChangeItems : public QList<SnoopDataChangeItem>, public VXmlable
{
public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

// ----------------------------------------------------------------------------
// SnoopDataChange
// ----------------------------------------------------------------------------
class SnoopDataChange : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopDataChange(void* owner = NULL);
  virtual ~SnoopDataChange();

protected:
  virtual bool doOpen();
  virtual bool doClose();

signals:
  void changed(SnoopPacket* packet);

public slots:
  void change(SnoopPacket* packet);

protected:
  bool _change(QByteArray& ba);

public:
  SnoopFlowMgr*        flowMgr;
  bool                 tcpChange;
  bool                 udpChange;
  SnoopDataChangeItems items;

protected:
    size_t tcpFlowOffset;

protected slots:
  void __tcpFlowCreate(SnoopTcpFlowKey* key, SnoopFlowValue* value);
  void __tcpFlowDelete(SnoopTcpFlowKey* key, SnoopFlowValue* value);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_DATA_CHANGE_H__
