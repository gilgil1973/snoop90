#ifdef QT_GUI_LIB

#include "snoopprocessfilterwidget.h"
#include "ui_snoopprocessfilterwidget.h"
#include <SnoopProcessFilter>

// ----------------------------------------------------------------------------
// QShowPolicyMapEvent
// ----------------------------------------------------------------------------
QShowPolicyMapEvent::QShowPolicyMapEvent() : QEvent(QEvent::User)
{
}

// ----------------------------------------------------------------------------
// SnoopProcessFilterWidget
// ----------------------------------------------------------------------------
SnoopProcessFilterWidget::SnoopProcessFilterWidget(QWidget *parent, Qt::WindowFlags f) :
  QWidget(parent, f),
  ui(new Ui::SnoopProcessFilterWidget)
{
  ui->setupUi(this);
  ui->treeWidget->setIndentation(0);
  ui->treeWidget->setHeaderHidden(true);
}

SnoopProcessFilterWidget::~SnoopProcessFilterWidget()
{
  delete ui;
}

bool SnoopProcessFilterWidget::event(QEvent* event)
{
  QShowPolicyMapEvent* showPolicyMapEvent = dynamic_cast<QShowPolicyMapEvent*>(event);
  if (showPolicyMapEvent != NULL)
  {
    showPolicyMap();
    return true;
  }
  return QWidget::event(event);
}

void SnoopProcessFilterWidget::showPolicyMap()
{
  VLock lock(filter->policyMap);

  ui->treeWidget->clear();
  SnoopProcessPolicyMap& map = filter->policyMap;
  int i = 0;
  for (SnoopProcessPolicyMap::iterator it = map.begin(); it != map.end(); it++)
  {
    QString processName = it.key();
    bool    ack         = it.value();

    QStringList sl; sl.push_back(processName);
    QTreeWidgetItem* item = new QTreeWidgetItem(sl);
    item->setCheckState(0, ack ? Qt::Checked : Qt::Unchecked);
    ui->treeWidget->insertTopLevelItem(i, item);
    i++;
  }
}

void SnoopProcessFilterWidget::setPolicyMap()
{
  VLock lock(filter->policyMap);

  int _count = ui->treeWidget->topLevelItemCount();
  for (int i = 0; i < _count; i++)
  {
    QTreeWidgetItem* item = ui->treeWidget->topLevelItem(i);
    QString processName = item->text(0);
    bool    ack         = item->checkState(0) == Qt::Checked;
    filter->policyMap[processName] = ack;
  }
}

void SnoopProcessFilterWidget::on_pbClear_clicked()
{
  filter->policyMap.clear();
  showPolicyMap();
}

void SnoopProcessFilterWidget::on_treeWidget_clicked(const QModelIndex &index)
{
  Q_UNUSED(index)
  setPolicyMap();
}

#endif // QT_GUI_LIB
