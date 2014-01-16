#include "aboutdlg.h"
#include "ui_aboutdlg.h"
#include <VCommon>
#include <SnoopCommon>

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
  ui->setupUi(this);

  ui->lblSnoopSpy->setText("<a href=\"http://www.snoopspy.com/\">http://www.snoopspy.com</a>");
  ui->lblSnoopSpy->setTextFormat(Qt::RichText);
  ui->lblSnoopSpy->setTextInteractionFlags(Qt::TextBrowserInteraction);
  ui->lblSnoopSpy->setOpenExternalLinks(true);

  ui->lblWinPcap->setText("<a href=\"http://www.winpcap.org/\">http://www.winpcap.org</a>");
  ui->lblWinPcap->setTextFormat(Qt::RichText);
  ui->lblWinPcap->setTextInteractionFlags(Qt::TextBrowserInteraction);
  ui->lblWinPcap->setOpenExternalLinks(true);

  ui->lblWinDivert->setText("<a href=\"http://reqrypt.org/windivert.html\">http://reqrypt.org/windivert.html</a>");
  ui->lblWinDivert->setTextFormat(Qt::RichText);
  ui->lblWinDivert->setTextInteractionFlags(Qt::TextBrowserInteraction);
  ui->lblWinDivert->setOpenExternalLinks(true);
}

AboutDlg::~AboutDlg()
{
  delete ui;
}

void AboutDlg::showEvent(QShowEvent *)
{
  ui->lblVDreamVersion->setText(VDREAM_VERSION);
  ui->lblSnoopVersion->setText(SNOOP_VERSION);
}

void AboutDlg::on_btnOK_clicked()
{
  this->close();
}
