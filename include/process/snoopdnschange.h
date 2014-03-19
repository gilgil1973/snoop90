// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_DNS_CHANGE_H__
#define __SNOOP_DNS_CHANGE_H__

#include <SnoopProcess>
#include <SnoopCapture>
#include <VDataFind>

// ----------------------------------------------------------------------------
// SnoopDnsChangeItem
// ----------------------------------------------------------------------------
class SnoopDnsChangeItem : public VRegExp
{
public:
  SnoopDnsChangeItem();

public:
  bool enabled;
  bool log;
  Ip   ip;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public:
  static const int ENABLED_IDX = 4;
  static const int LOG_IDX     = 5;
  static const int IP_IDX      = 6;

  static void initialize(QTreeWidget* treeWidget);
#endif // QT_GUI_LIB
};

#ifdef QT_GUI_LIB
void operator << (QTreeWidgetItem& treeWidgetItem, SnoopDnsChangeItem& item);
void operator << (SnoopDnsChangeItem& item, QTreeWidgetItem& treeWidgetItem);
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopDnsChangeItems
// ----------------------------------------------------------------------------
class SnoopDnsChangeItems : public QObject, public QList<SnoopDnsChangeItem>, public VXmlable, public VOptionable
{
  Q_OBJECT

public:
  bool prepare(VError& error);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
protected slots:
  void __on_pbAdd_clicked();
  void __on_pbDel_clicked();

public:
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#ifdef QT_GUI_LIB
void operator << (QTreeWidget& treeWidget, SnoopDnsChangeItems& changeItems);
void operator << (SnoopDnsChangeItems& changeItems, QTreeWidget& treeWidget);
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopDnsChange
// ----------------------------------------------------------------------------
class SnoopDnsChange : public SnoopProcess
{
  Q_OBJECT

public:
  SnoopDnsChange(void* owner = NULL);
  virtual ~SnoopDnsChange();

public:
  SnoopCapture*       capture;
  SnoopDnsChangeItems changeItems;

protected:
  virtual bool doOpen();
  virtual bool doClose();

public slots:
  void check(SnoopPacket* packet);

signals:
  void changed(SnoopPacket* packet);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_DNS_CHANGE_H__
