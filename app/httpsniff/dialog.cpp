#include "dialog.h"
#include "ui_dialog.h"

// ----------------------------------------------------------------------------
// Dialog
// ----------------------------------------------------------------------------
Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog)
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
  move(0, 0); resize(640, 480);

  SnoopInterfaces& intfs = SnoopInterfaces::instance();
  int _count = intfs.count();
  for (int i = 0; i < _count; i++)
  {
    SnoopInterface& intf = (SnoopInterface&)intfs.at(i);
    QString value = intf.description;
    if (value == "") value = intf.name;
    ui->cbxAdapterIndex->addItem(value);
  }
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
    ui->pteHttpPortList->insertPlainText(QString::number(port) + "\r\n");
  }

  ui->pteHttpsPortList->clear();
  foreach (int port, config.httpsPortList)
  {
    ui->pteHttpsPortList->insertPlainText(QString::number(port) + "\r\n");
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
    ui->pteProxyProcessNameList->insertPlainText(processName + "\r\n");
  }
  ui->leTcpInPort->setText(QString::number(config.proxyTcpInPort));
  ui->leTcpOutPort->setText(QString::number(config.proxyTcpOutPort));
  ui->leSslInPort->setText(QString::number(config.proxySslInPort));
  ui->leSslOutPort->setText(QString::number(config.proxySslOutPort));

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
  this->saveToDefaultDoc("Dialog");
}

void Dialog::setControl()
{
  ui->leDumpFilePath->setEnabled(ui->chkDump->checkState() == Qt::Checked);
  ui->cbxAdapterIndex->setEnabled(ui->chkWriteAdapter->checkState() == Qt::Checked);
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
