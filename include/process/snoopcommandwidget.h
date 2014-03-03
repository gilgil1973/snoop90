#ifndef SNOOPCOMMANDWIDGET_H
#define SNOOPCOMMANDWIDGET_H

#ifdef QT_GUI_LIB

#include <QWidget>
#include <QTreeWidget>

namespace Ui {
  class SnoopCommandWidget;
}

class SnoopCommandWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SnoopCommandWidget(QWidget *parent = 0);
  ~SnoopCommandWidget();

protected:
  void initializeTreeWidget(QTreeWidget* treeWidget);
  void addTreeWidgetItem(QTreeWidget* treeWidget);
  void delTreeWidgetItem(QTreeWidget* treeWidget);

public:

public:
  Ui::SnoopCommandWidget *ui;
private slots:
  void on_pbAddOpen_clicked();
  void on_pbDelOpen_clicked();
  void on_pbAddClose_clicked();
  void on_pbDelClose_clicked();
};

class SnoopCommandItem;
class SnoopCommandItems;
void operator << (SnoopCommandItem& item, QTreeWidgetItem& treeWidgetItem);
void operator << (QTreeWidgetItem& treeWidgetItem, SnoopCommandItem& item);
void operator << (SnoopCommandItems& items, QTreeWidget& treeWidget);
void operator << (QTreeWidget& treeWidget, SnoopCommandItems& items);

#endif // QT_GUI_LIB

#endif // SNOOPCOMMANDWIDGET_H


