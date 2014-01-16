#ifndef SIGNALSLOTFORM_H
#define SIGNALSLOTFORM_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
  class SignalSlotForm;
}

class SignalSlotForm : public QDialog
{
  Q_OBJECT
  
public:
  explicit SignalSlotForm(QWidget *parent = 0);
  ~SignalSlotForm();

public:
  void initializeControl();
  void finalizeControl();
  void loadControl();
  void saveControl();
  void setControl();
  
private slots:
  void on_btnOK_clicked();

  void on_btnCancel_clicked();

  void on_lwSignalList_currentRowChanged(int currentRow);

  void on_lwSlotList_currentRowChanged(int currentRow);

  void on_lwSignalList_clicked(const QModelIndex &index);

  void on_lwSlotList_clicked(const QModelIndex &index);

public:
  Ui::SignalSlotForm *ui;
};

#endif // SIGNALSLOTFORM_H
