#ifndef SNOOPCOMMANDWIDGET_H
#define SNOOPCOMMANDWIDGET_H

#ifdef QT_GUI_LIB

#include <QWidget>
#include <QTreeWidget>
#include <SnoopCommand>

namespace Ui {
  class SnoopCommandWidget;
}

class SnoopCommandWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SnoopCommandWidget(QWidget *parent = 0);
  ~SnoopCommandWidget();

public:
  SnoopCommandItems openCommands;
  SnoopCommandItems closeCommands;

protected:
  void initializeTreeWidget(QTreeWidget* treeWidget);
  void addTreeWidgetItem(QTreeWidget* treeWidget);
  void delTreeWidgetItem(QTreeWidget* treeWidget);

public:
  static const int ENABLED_IDX = 0;
  static const int COMMAND_IDX = 1;
  static const int SYNC_IDX    = 2;

public:
  Ui::SnoopCommandWidget *ui;
private slots:
  void on_pbAddOpen_clicked();
  void on_pbDelOpen_clicked();
  void on_pbAddClose_clicked();
  void on_pbDelClose_clicked();
};

void operator << (SnoopCommandItem& item, QTreeWidgetItem& treeWidgetItem);
void operator << (QTreeWidgetItem& treeWidgetItem, SnoopCommandItem& item);
void operator << (SnoopCommandItems& items, QTreeWidget& treeWidget);
void operator << (QTreeWidget& treeWidget, SnoopCommandItems& items);

#endif // QT_GUI_LIB

#endif // SNOOPCOMMANDWIDGET_H
