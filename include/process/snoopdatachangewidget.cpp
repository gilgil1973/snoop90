#include "snoopdatachangewidget.h"
#include "ui_snoopdatachangewidget.h"
#include <SnoopDataChange>

SnoopDataChangeWidget::SnoopDataChangeWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SnoopDataChangeWidget)
{
  ui->setupUi(this);

  layout()->setContentsMargins(0, 0, 0, 0);
  QStringList headerLables; headerLables << "Enabled" << "RegExp" << "Log" << "From" << "To";
  ui->treeWidget->setHeaderLabels(headerLables);
  ui->treeWidget->setColumnWidth(ENABLED_IDX, 70);
  ui->treeWidget->setColumnWidth(RE_IDX,      70);
  ui->treeWidget->setColumnWidth(LOG_IDX,     70);
  ui->treeWidget->header()->setSectionResizeMode(ENABLED_IDX, QHeaderView::Fixed);
  ui->treeWidget->header()->setSectionResizeMode(RE_IDX,      QHeaderView::Fixed);
  ui->treeWidget->header()->setSectionResizeMode(LOG_IDX,     QHeaderView::Fixed);
  ui->treeWidget->header()->setSectionResizeMode(FROM_IDX,    QHeaderView::Stretch);
  ui->treeWidget->header()->setSectionResizeMode(TO_IDX,      QHeaderView::Stretch);
  ui->treeWidget->header()->setStretchLastSection(false);
  ui->treeWidget->setMinimumWidth(600);
}

SnoopDataChangeWidget::~SnoopDataChangeWidget()
{
  delete ui;
}

void SnoopDataChangeWidget::on_pbAdd_clicked()
{
  QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem(ui->treeWidget);
  SnoopDataChangeItem newItem;
  *treeWidgetItem << newItem;
  int count = ui->treeWidget->topLevelItemCount();
  ui->treeWidget->insertTopLevelItem(count, treeWidgetItem);
}

void SnoopDataChangeWidget::on_pbDel_clicked()
{
  QList<QTreeWidgetItem*> items = ui->treeWidget->selectedItems();
  foreach (QTreeWidgetItem* item, items)
  {
    delete item;
  }
}

void operator << (SnoopDataChangeItem& item, QTreeWidgetItem& treeWidgetItem)
{
  item.enabled = treeWidgetItem.checkState(SnoopDataChangeWidget::ENABLED_IDX) == Qt::Checked;
  item.re      = treeWidgetItem.checkState(SnoopDataChangeWidget::RE_IDX)      == Qt::Checked;
  item.log     = treeWidgetItem.checkState(SnoopDataChangeWidget::LOG_IDX)     == Qt::Checked;
  item.from    = qPrintable(treeWidgetItem.text(SnoopDataChangeWidget::FROM_IDX));
  item.to      = qPrintable(treeWidgetItem.text(SnoopDataChangeWidget::TO_IDX));
}

void operator << (QTreeWidgetItem& treeWidgetItem, SnoopDataChangeItem& item)
{
  treeWidgetItem.setCheckState(SnoopDataChangeWidget::ENABLED_IDX, item.enabled ? Qt::Checked : Qt::Unchecked);
  treeWidgetItem.setCheckState(SnoopDataChangeWidget::RE_IDX,      item.re      ? Qt::Checked : Qt::Unchecked);
  treeWidgetItem.setCheckState(SnoopDataChangeWidget::LOG_IDX,     item.log     ? Qt::Checked : Qt::Unchecked);
  treeWidgetItem.setText(SnoopDataChangeWidget::FROM_IDX, item.from);
  treeWidgetItem.setText(SnoopDataChangeWidget::TO_IDX,   item.to);
  treeWidgetItem.setFlags(treeWidgetItem.flags() | Qt::ItemFlag::ItemIsEditable);
}

void operator << (SnoopDataChangeItems& items, QTreeWidget& treeWidget)
{
  items.clear();
  int count = treeWidget.topLevelItemCount();
  for (int i = 0; i < count; i++)
  {
    QTreeWidgetItem* treeWidgetItem = treeWidget.topLevelItem(i);
    SnoopDataChangeItem newItem;
    newItem << *treeWidgetItem;
    items.push_back(newItem);
  }
}

void operator << (QTreeWidget& treeWidget, SnoopDataChangeItems& items)
{
  treeWidget.clear();
  QList<QTreeWidgetItem*> treeWidgetItems;
  for (int i = 0; i < items.count(); i++)
  {
    SnoopDataChangeItem& item = (SnoopDataChangeItem&)items.at(i);
    QTreeWidgetItem* newWidgetItem = new QTreeWidgetItem(&treeWidget);
    *newWidgetItem << item;
    treeWidgetItems.push_back(newWidgetItem);
  }
  treeWidget.insertTopLevelItems(0, treeWidgetItems);
}
