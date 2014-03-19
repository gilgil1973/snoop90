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

#endif // QT_GUI_LIB

#endif // SNOOPCOMMANDWIDGET_H
