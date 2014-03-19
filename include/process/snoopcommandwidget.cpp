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
  treeWidget->setItemsExpandable(false);
  treeWidget->setMinimumWidth(400);
  treeWidget->header()->setStretchLastSection(false);

  QStringList headerLables;
  headerLables << "Enabled" << "Command" << "Sync";
  treeWidget->setHeaderLabels(headerLables);

  treeWidget->setColumnWidth(SnoopCommandItem::ENABLED_IDX, 70);
  // treeWidget->setColumnWidth(SnoopCommandItem::COMMAND_IDX, 0);
  treeWidget->setColumnWidth(SnoopCommandItem::SYNC_IDX, 70);

  treeWidget->header()->setSectionResizeMode(SnoopCommandItem::ENABLED_IDX, QHeaderView::Interactive);
  treeWidget->header()->setSectionResizeMode(SnoopCommandItem::COMMAND_IDX, QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(SnoopCommandItem::SYNC_IDX,    QHeaderView::Interactive);
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

#endif // QT_GUI_LIB
