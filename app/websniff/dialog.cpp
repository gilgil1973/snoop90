#include "dialog.h"
#include "ui_dialog.h"

// ----------------------------------------------------------------------------
// Dialog
// ----------------------------------------------------------------------------
Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog),
  saveDialog(this)
{
  ui->setupUi(this);
  initializeControl();
  loadControl();
  setControl();
  ui->pbRun->setFocus();
}

Dialog::~Dialog()
{
  saveControl();
  finalizeControl();
  delete ui;
}

void Dialog::initializeControl()
{
  move(0, 0);

  SnoopInterfaces& intfs = SnoopInterfaces::instance();
  int _count = intfs.count();
  for (int i = 0; i < _count; i++)
  {
    SnoopInterface& intf = (SnoopInterface&)intfs.at(i);
    QString value = intf.description;
    if (value == "") value = intf.name;
    ui->cbxAdapterIndex->addItem(value);
  }

  QStringList filters; filters << "snoopspy files(*.ss)" << "any files(*)";
  saveDialog.setNameFilters(filters);
  saveDialog.setDefaultSuffix("ss");
  saveDialog.setViewMode(QFileDialog::Detail);
}

void Dialog::finalizeControl()
{

}

void Dialog::loadControl()
{
  this->loadFromDefaultDoc("Dialog");

  //
  // Port
  //
  ui->pteHttpPortList->clear();
  foreach (int port, config.httpPortList)
  {
    ui->pteHttpPortList->insertPlainText(QString::number(port) + "\n");
  }

  ui->pteHttpsPortList->clear();
  foreach (int port, config.httpsPortList)
  {
    ui->pteHttpsPortList->insertPlainText(QString::number(port) + "\n");
  }

  //
  // Capture
  //
  ui->rbWinDivert->setChecked(config.captureType == HttpSniffConfig::WinDivert);
  ui->rbArpSpoof->setChecked(config.captureType == HttpSniffConfig::ArpSpoof);

  //
  // Proxy
  //
  ui->pteProxyProcessNameList->clear();
  foreach (QString processName, config.proxyProcessNameList)
  {
    ui->pteProxyProcessNameList->insertPlainText(processName + "\n");
  }
  ui->leHttpInPort->setText(QString::number(config.proxyHttpInPort));
  ui->leHttpOutPort->setText(QString::number(config.proxyHttpOutPort));
  ui->leHttpsInPort->setText(QString::number(config.proxyHttpsInPort));
  ui->leHttpsOutPort->setText(QString::number(config.proxyHttpsOutPort));
  ui->leStripInPort->setText(QString::number(config.proxyStripInPort));
  ui->leStripOutPort->setText(QString::number(config.proxyStripOutPort));

  //
  // SslStrip
  //
  ui->chkSslStripEnabled->setChecked(config.sslStripEnabled);
  ui->leSslStripVirtualIp->setText(config.sslStripVirtualIp);
  ui->leSslStripDomainPrefix->setText(config.sslStripDomainPrefix);

  //
  // Write
  //
  ui->chkDump->setChecked(config.dumpEnabled);
  ui->leDumpFilePath->setText(config.dumpFilePath);
  ui->chkWriteAdapter->setChecked(config.writeAdapterEnabled);
  ui->cbxAdapterIndex->setCurrentIndex(config.writeAdapterIndex);
}

void Dialog::saveControl()
{
  //
  // Port
  //
  QStringList sl = ui->pteHttpPortList->toPlainText().split("\n");
  config.httpPortList.clear();
  foreach (QString port, sl)
  {
    if (port == "") continue;
    config.httpPortList.push_back(port.toInt());
  }

  sl = ui->pteHttpsPortList->toPlainText().split("\n");
  config.httpsPortList.clear();
  foreach (QString port, sl)
  {
    if (port == "") continue;
    config.httpsPortList.push_back(port.toInt());
  }

  //
  // Capture
  //
  if (ui->rbWinDivert->isChecked()) config.captureType = HttpSniffConfig::WinDivert;
  else if (ui->rbArpSpoof->isChecked()) config.captureType = HttpSniffConfig::ArpSpoof;

  //
  // Proxy
  //
  sl = ui->pteProxyProcessNameList->toPlainText().split("\n");
  config.proxyProcessNameList.clear();
  foreach (QString processName, sl)
  {
    if (processName == "") continue;
    config.proxyProcessNameList.push_back(processName);
  }
  config.proxyHttpInPort = ui->leHttpInPort->text().toInt();
  config.proxyHttpOutPort = ui->leHttpOutPort->text().toInt();
  config.proxyHttpsInPort = ui->leHttpsInPort->text().toInt();
  config.proxyHttpsOutPort = ui->leHttpsOutPort->text().toInt();
  config.proxyStripInPort = ui->leStripInPort->text().toInt();
  config.proxyStripOutPort = ui->leStripOutPort->text().toInt();

  //
  // SslStrip
  //
  config.sslStripEnabled = ui->chkSslStripEnabled->isChecked();
  config.sslStripVirtualIp = ui->leSslStripVirtualIp->text();
  config.sslStripDomainPrefix = ui->leSslStripDomainPrefix->text();

  //
  // Write
  //
  config.dumpEnabled = ui->chkDump->checkState() == Qt::Checked;
  config.dumpFilePath = ui->leDumpFilePath->text();
  config.writeAdapterEnabled = ui->chkWriteAdapter->isChecked();
  config.writeAdapterIndex = ui->cbxAdapterIndex->currentIndex();

  this->saveToDefaultDoc("Dialog");
}

void Dialog::setControl()
{
  bool sslStrip = ui->chkSslStripEnabled->isChecked();
  ui->leStripInPort->setEnabled(sslStrip);
  ui->leStripOutPort->setEnabled(sslStrip);
  ui->leSslStripVirtualIp->setEnabled(sslStrip);
  ui->leSslStripDomainPrefix->setEnabled(sslStrip);

  ui->leDumpFilePath->setEnabled(ui->chkDump->isChecked());
  ui->cbxAdapterIndex->setEnabled(ui->chkWriteAdapter->isChecked());
}

void Dialog::load(VXml xml)
{
  {
    VXml coordXml = xml.findChild("coord");
    if (!coordXml.isNull())
    {
      QRect rect = geometry();
      rect.setLeft  ((coordXml.getInt("left",   0)));
      rect.setTop   ((coordXml.getInt("top",    0)));
      rect.setWidth ((coordXml.getInt("width",  640)));
      rect.setHeight((coordXml.getInt("height", 480)));
      setGeometry(rect);
    }
  }

  config.load(xml.gotoChild("config"));
}

void Dialog::save(VXml xml)
{
  {
    VXml coordXml = xml.gotoChild("coord");
    QRect rect = geometry();
    coordXml.setInt("left",   rect.left());
    coordXml.setInt("top",    rect.top());
    coordXml.setInt("width",  rect.width());
    coordXml.setInt("height", rect.height());
  }

  config.save(xml.gotoChild("config"));
}

void Dialog::on_pbOutboundDataChange_clicked()
{
  config.proxyOutboundDataChange.optionDoAll(this);
}

void Dialog::on_pbInboundDataChange_clicked()
{
  config.proxyInboundDataChange.optionDoAll(this);
}

void Dialog::on_chkDump_clicked()
{
  setControl();
}

void Dialog::on_chkWriteAdapter_clicked()
{
  setControl();
}

void Dialog::on_pbRun_clicked()
{
  QString fileName = "ss/httpsniff_temp.ss";
  saveControl();
  if (!config.saveToFile(fileName))
  {
    QMessageBox::warning(this, "Error", config.error.msg);
    return;
  }
  QString command = "sscon.exe " + fileName;
  UINT res = WinExec(qPrintable(command), SW_SHOW);
  if (res < 31)
  {
    LOG_ERROR("WinExec return %u (%s) retu", res, qPrintable(command));
  }
}

void Dialog::on_pbSave_clicked()
{
  saveDialog.setAcceptMode(QFileDialog::AcceptSave);
  saveDialog.setFileMode(QFileDialog::AnyFile);

  if (saveDialog.exec() == QDialog::Accepted)
  {
    QString fileName = saveDialog.selectedFiles().first();
    saveControl();
    if (!config.saveToFile(fileName))
    {
      QMessageBox::warning(this, "Error", config.error.msg);
      return;
    }
  }
}

void Dialog::on_chkSslStripEnabled_clicked()
{
  setControl();
}

void Dialog::on_pbWinDivertOption_clicked()
{
  config.winDivert.optionDoAll(this);
}

void Dialog::on_pbArpSpoofOption_clicked()
{
  config.arpSpoof.optionDoAll(this);
}
