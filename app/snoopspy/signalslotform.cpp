#include "signalslotform.h"
#include "ui_signalslotform.h"

SignalSlotForm::SignalSlotForm(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::SignalSlotForm)
{
  ui->setupUi(this);
  setControl();
}

SignalSlotForm::~SignalSlotForm()
{
  delete ui;
}

void SignalSlotForm::initializeControl()
{
}

void SignalSlotForm::finalizeControl()
{
}

void SignalSlotForm::loadControl()
{
}

void SignalSlotForm::saveControl()
{
}

void SignalSlotForm::setControl()
{
  bool ok = true;
  if (ui->lwSignalList->selectedItems().count() == 0) ok = false;
  if (ui->lwSlotList->selectedItems().count() == 0)   ok = false;

  ui->btnOK->setEnabled(ok);
}

void SignalSlotForm::on_btnOK_clicked()
{
  this->done(QDialog::Accepted);
}

void SignalSlotForm::on_btnCancel_clicked()
{
  this->done(QDialog::Rejected);
}

void SignalSlotForm::on_lwSignalList_currentRowChanged(int currentRow)
{
  Q_UNUSED(currentRow)
  setControl();
}

void SignalSlotForm::on_lwSlotList_currentRowChanged(int currentRow)
{
  Q_UNUSED(currentRow)
  setControl();
}

void SignalSlotForm::on_lwSignalList_clicked(const QModelIndex &index)
{
  Q_UNUSED(index)
  setControl();
}

void SignalSlotForm::on_lwSlotList_clicked(const QModelIndex &index)
{
  Q_UNUSED(index)
  setControl();
}
