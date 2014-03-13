#ifndef SNOOPDATACHANGEWIDGET_H
#define SNOOPDATACHANGEWIDGET_H

#include <QWidget>
#include <QTreeWidget>

namespace Ui {
  class SnoopDataChangeWidget;
}

class SnoopDataChangeWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SnoopDataChangeWidget(QWidget *parent = 0);
  ~SnoopDataChangeWidget();

public:
  static const int ENABLED_IDX = 0;
  static const int RE_IDX      = 1;
  static const int LOG_IDX     = 2;
  static const int FROM_IDX    = 3;
  static const int TO_IDX      = 4;

public:
  Ui::SnoopDataChangeWidget *ui;
private slots:
  void on_pbAdd_clicked();
  void on_pbDel_clicked();
};

class SnoopDataChangeItem;
class SnoopDataChangeItems;
void operator << (SnoopDataChangeItem& item, QTreeWidgetItem& treeWidgetItem);
void operator << (QTreeWidgetItem& treeWidgetItem, SnoopDataChangeItem& item);
void operator << (SnoopDataChangeItems& items, QTreeWidget& treeWidget);
void operator << (QTreeWidget& treeWidget, SnoopDataChangeItems& items);

#endif // SNOOPDATACHANGEWIDGET_H
