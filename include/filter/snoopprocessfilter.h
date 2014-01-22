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

// ----------------------------------------------------------------------------
// SnoopProcessTupleValue
// ----------------------------------------------------------------------------
class SnoopProcessTupleValue
{
public:
  quint32 pid;
  bool    ack;
};

// ----------------------------------------------------------------------------
// SnoopProcessTupleValue
// ----------------------------------------------------------------------------
class SnoopProcessTupleMap : public QMap<Snoop5TupleKey, SnoopProcessTupleValue>, public VLockable
{
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

#ifdef QT_GUI_LIB
#include <QStringListModel>
// ----------------------------------------------------------------------------
// SnoopProcessFilterModel
// ----------------------------------------------------------------------------
class SnoopProcessFilterModel : public QStringListModel
{
public:
  SnoopProcessPolicyMap* policyMap;

public:
  void initialize();
  void addPolicy(QString processName, bool ack);

public:
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
};
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopProcessFilter
// ----------------------------------------------------------------------------
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
  SnoopProcessTupleMap tupleMap;
  bool getProcessInfo(/*in*/ Snoop5TupleKey& tuple, /*out*/ quint32& pid, /*out*/ QString& processName);
  bool getACK(/*in*/ Snoop5TupleKey& tuple, /*out*/ bool& ack);

public:
  SnoopProcessPolicyMap policyMap;

public slots:
  void check(SnoopPacket* packet);

signals:
  void ack(SnoopPacket* packet);
  void nak(SnoopPacket* packet);

signals:
  void onNewProcess(QString& processName);
  void onNewPID(Snoop5TupleKey& tuple, QString& processName);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
private: // VOptionable
  SnoopProcessFilterModel* myModel;
public: // for VOptionable
  virtual bool event(QEvent *);
  virtual void addOptionWidget(QLayout* layout);
  virtual bool showOptionDlg(QDialog* dialog);
  virtual void saveOptionDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_PROCESS_FILTER_H__
