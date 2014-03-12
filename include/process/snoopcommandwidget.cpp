#ifdef QT_GUI_LIB

#include "snoopcommandwidget.h"
#include "ui_snoopcommandwidget.h"
#include <SnoopCommand>

SnoopCommandWidget::SnoopCommandWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SnoopCommandWidget)
{
  ui->setupUi(this);
  layout()->setContentsMargins(0, 0, 0, 0);
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
  treeWidget->setColumnWidth(ENABLED_IDX, 70);
  treeWidget->setColumnWidth(SYNC_IDX, 70);
  treeWidget->header()->setSectionResizeMode(ENABLED_IDX, QHeaderView::Fixed);
  treeWidget->header()->setSectionResizeMode(COMMAND_IDX, QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(SYNC_IDX,    QHeaderView::Fixed);
  treeWidget->header()->setStretchLastSection(false);
  treeWidget->setMinimumWidth(400);
}

void SnoopCommandWidget::addTreeWidgetItem(QTreeWidget* treeWidget)
{
  QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem(treeWidget);
  SnoopCommandItem newItem;
  *treeWidgetItem << newItem;
  int count = treeWidget->topLevelItemCount();
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
  item.enabled = treeWidgetItem.checkState(SnoopCommandWidget::ENABLED_IDX) == Qt::Checked;
  item.command = treeWidgetItem.text(SnoopCommandWidget::COMMAND_IDX);
  item.sync    = treeWidgetItem.checkState(SnoopCommandWidget::SYNC_IDX) == Qt::Checked;
}

void operator << (QTreeWidgetItem& treeWidgetItem, SnoopCommandItem& item)
{
  treeWidgetItem.setCheckState(SnoopCommandWidget::ENABLED_IDX, item.enabled ? Qt::Checked : Qt::Unchecked);
  treeWidgetItem.setText(SnoopCommandWidget::COMMAND_IDX, item.command);
  treeWidgetItem.setCheckState(SnoopCommandWidget::SYNC_IDX, item.sync ? Qt::Checked : Qt::Unchecked);
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
