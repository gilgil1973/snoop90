#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "websniffconfig.h"

// ----------------------------------------------------------------------------
// Dialog
// ----------------------------------------------------------------------------
namespace Ui {
  class Dialog;
}

class Dialog : public QDialog, public VXmlable
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = 0);
  ~Dialog();

public:
  void initializeControl();
  void finalizeControl();
  void loadControl();
  void saveControl();
  void setControl();

public:
  HttpSniffConfig config;

protected:
  QFileDialog saveDialog;

public:
  void load(VXml xml);
  void save(VXml xml);

private slots:

  void on_pbOutboundDataChange_clicked();

  void on_pbInboundDataChange_clicked();

  void on_chkDump_clicked();

  void on_chkWriteAdapter_clicked();

  void on_pbRun_clicked();

  void on_pbSave_clicked();

  void on_chkSslStripEnabled_clicked();

  void on_pbWinDivertOption_clicked();

  void on_pbArpSpoofOption_clicked();

private:
  Ui::Dialog *ui;
};

#endif // DIALOG_H
