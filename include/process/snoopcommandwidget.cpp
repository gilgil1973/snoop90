#ifdef QT_GUI_LIB

#include "snoopcommandwidget.h"
#include "ui_snoopcommandwidget.h"
#include <SnoopCommand>

SnoopCommandWidget::SnoopCommandWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SnoopCommandWidget)
{
  ui->setupUi(this);
  initializeTreeWidget(ui->tvOpen);
  initializeTreeWidget(ui->tvClose);
}

SnoopCommandWidget::~SnoopCommandWidget()
{
  delete ui;
}

void SnoopCommandWidget::initializeTreeWidget(QTreeWidget* treeWidget)
{
  treeWidget->setIndentation(0);
  QStringList headerLables; headerLables << "Enabled" << "Command" << "Sync";
  treeWidget->setHeaderLabels(headerLables);
  treeWidget->setColumnWidth(0, 50);
  treeWidget->setColumnWidth(2, 50);
  treeWidget->header()->setSectionResizeMode(0, QHeaderView::Interactive);
  treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(2, QHeaderView::Interactive);
}

void SnoopCommandWidget::addTreeWidgetItem(QTreeWidget* treeWidget)
{
  int count = treeWidget->topLevelItemCount();

  QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem(treeWidget);
  SnoopCommandItem newItem;
  *treeWidgetItem << newItem;
  treeWidget->insertTopLevelItem(count, treeWidgetItem);
}

void SnoopCommandWidget::delTreeWidgetItem(QTreeWidget* treeWidget)
{
  QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
  foreach (QTreeWidgetItem* item, items)
  {
    delete item;
  }
}

void SnoopCommandWidget::on_pbAddOpen_clicked()
{
  addTreeWidgetItem(ui->tvOpen);
}

void SnoopCommandWidget::on_pbDelOpen_clicked()
{
  delTreeWidgetItem(ui->tvOpen);
}

void SnoopCommandWidget::on_pbAddClose_clicked()
{
  addTreeWidgetItem(ui->tvClose);
}

void SnoopCommandWidget::on_pbDelClose_clicked()
{
  delTreeWidgetItem(ui->tvClose);
}

void operator << (SnoopCommandItem& item, QTreeWidgetItem& treeWidgetItem)
{
  item.enabled = treeWidgetItem.checkState(0) == Qt::Checked;
  item.command = treeWidgetItem.text(1);
  item.sync    = treeWidgetItem.checkState(2) == Qt::Checked;
}

void operator << (QTreeWidgetItem& treeWidgetItem, SnoopCommandItem& item)
{
  treeWidgetItem.setCheckState(0, item.enabled ? Qt::Checked : Qt::Unchecked);
  treeWidgetItem.setText(1, item.command);
  treeWidgetItem.setCheckState(2, item.sync ? Qt::Checked : Qt::Unchecked);
  treeWidgetItem.setFlags(treeWidgetItem.flags() | Qt::ItemFlag::ItemIsEditable);
}

void operator << (SnoopCommandItems& items, QTreeWidget& treeWidget)
{
  items.clear();
  int count = treeWidget.topLevelItemCount();
  for (int i = 0; i < count; i++)
  {
    QTreeWidgetItem* treeWidgetItem = treeWidget.topLevelItem(i);
    SnoopCommandItem newItem;
    newItem << *treeWidgetItem;
    items.push_back(newItem);
  }
}

void operator << (QTreeWidget& treeWidget, SnoopCommandItems& items)
{
  treeWidget.clear();
  QList<QTreeWidgetItem*> treeWidgetItems;
  for (int i = 0; i < items.count(); i++)
  {
    SnoopCommandItem& item = (SnoopCommandItem&)items.at(i);
    QTreeWidgetItem* newWidgetItem = new QTreeWidgetItem(&treeWidget);
    *newWidgetItem << item;
    treeWidgetItems.push_back(newWidgetItem);
  }
  treeWidget.insertTopLevelItems(0, treeWidgetItems);
}

#endif // QT_GUI_LIB
