#include <SnoopDataChange>
#include <SnoopIp>
#include <SnoopTcp>
#include <SnoopUdp>

REGISTER_METACLASS(SnoopDataChange, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopDataChangeItem
// ----------------------------------------------------------------------------
SnoopDataChangeItem::SnoopDataChangeItem()
{
  enabled = true;
  from    = "";
  to      = "";
}

void SnoopDataChangeItem::load(VXml xml)
{
  enabled = xml.getBool("enabled", enabled);
  from    = qPrintable(xml.getStr("from", from));
  to      = qPrintable(xml.getStr("to",   to));
}

void SnoopDataChangeItem::save(VXml xml)
{
  xml.setBool("enabled", enabled);
  xml.setStr("from", qPrintable(from));
  xml.setStr("to",   qPrintable(to));
}

// ----------------------------------------------------------------------------
// SnoopDataChangeItems
// ----------------------------------------------------------------------------
void SnoopDataChangeItems::load(VXml xml)
{
  clear();
  {
    xml_foreach (childXml, xml.childs())
    {
      SnoopDataChangeItem item;
      item.load(childXml);
      push_back(item);
    }
  }
}

void SnoopDataChangeItems::save(VXml xml)
{
  xml.clearChild();
  for (SnoopDataChangeItems::iterator it = begin(); it != end(); it++)
  {
    SnoopDataChangeItem& item = *it;
    VXml childXml = xml.addChild("item");
    item.save(childXml);
  }
}

// ----------------------------------------------------------------------------
// SnoopDataChange
// ----------------------------------------------------------------------------
SnoopDataChange::SnoopDataChange(void* owner) : SnoopProcess(owner)
{
  tcpChange = true;
  udpChange = true;
  items.clear();
}

SnoopDataChange::~SnoopDataChange()
{
  close();
}

void SnoopDataChange::change(SnoopPacket* packet)
{
  bool _changed = false;
  if (SnoopIp::parseAll(packet))
  {
    if (SnoopIp::isTcp(packet->ipHdr, &packet->tcpHdr))
    {
      if (SnoopTcp::isData(packet->ipHdr, packet->tcpHdr, &packet->data, &packet->dataLen))
      {
        QByteArray ba(packet->data, packet->dataLen);
        if (_change(ba))
        {
          memcpy(packet->data, ba.data(), (size_t)packet->dataLen);
          packet->tcpChanged = true;
          _changed = true;
        }
      }
    } else
    if (SnoopIp::isUdp(packet->ipHdr, &packet->udpHdr))
    {
      if (SnoopUdp::isData(packet->ipHdr, packet->udpHdr, &packet->data, &packet->dataLen))
      {
        QByteArray ba(packet->data, packet->dataLen);
        if (_change(ba))
        {
          memcpy(packet->data, ba.data(), (size_t)packet->dataLen);
          packet->udpChanged = true;
          _changed = true;
        }
      }
    }
  }
  if (_changed) emit changed(packet);
}

bool SnoopDataChange::_change(QByteArray& ba)
{
  bool res = false;
  QByteArray _old = ba;
  for (int i = 0; i < items.count(); i++)
  {
    SnoopDataChangeItem& item = (SnoopDataChangeItem&)items.at(i);
    if (!item.enabled) continue;
    ba.replace(item.from, item.to);
    if (ba != _old)
    {
      LOG_DEBUG("changed %s > %s", qPrintable(item.from), qPrintable(item.to));
      _old = ba;
      res = true;
    }
  }
  return res;
}

void SnoopDataChange::load(VXml xml)
{
  SnoopProcess::load(xml);
  tcpChange = xml.getBool("tcpChange", tcpChange);
  udpChange = xml.getBool("udpChange", udpChange);
  items.load(xml.gotoChild("items"));
}

void SnoopDataChange::save(VXml xml)
{
  SnoopProcess::save(xml);
  xml.setBool("tcpChange", tcpChange);
  xml.setBool("udpChange", udpChange);
  items.save(xml.gotoChild("items"));
}

#ifdef QT_GUI_LIB
void SnoopDataChange::on_pbAdd_clicked()
{
  QPushButton* pbAdd = dynamic_cast<QPushButton*>(sender());
  QDialog* dialog = dynamic_cast<QDialog*>(pbAdd->parent());
  QTableWidget* tableWidget = dialog->findChild<QTableWidget*>("tableWidget");
  int rowCount = tableWidget->rowCount();
  tableWidget->setRowCount(tableWidget->rowCount() + 1);

  SnoopDataChangeItem item; // default value
  addItem(item, tableWidget, rowCount);
}

void SnoopDataChange::on_pbDel_clicked()
{
  QPushButton* pbAdd = dynamic_cast<QPushButton*>(sender());
  QDialog* dialog = dynamic_cast<QDialog*>(pbAdd->parent());
  QTableWidget* tableWidget = dialog->findChild<QTableWidget*>("tableWidget");
  QList<QTableWidgetItem*> items = tableWidget->selectedItems();
  if (items.count() == 0) return;
  QTableWidgetItem* item = items.at(0);
  if (item == NULL) return;
  int row = item->row();
  if (row == -1) return;
  if (row != -1) tableWidget->removeRow(row);
}

void SnoopDataChange::addItem(SnoopDataChangeItem& item, QTableWidget* tableWidget, int row)
{
  QTableWidgetItem* itemEnabled = new QTableWidgetItem;
  itemEnabled->setCheckState(item.enabled ? Qt::Checked : Qt::Unchecked);
  tableWidget->setItem(row, 0, itemEnabled);

  QTableWidgetItem* itemFrom = new QTableWidgetItem(QString(item.from));
  tableWidget->setItem(row, 1, itemFrom);

  QTableWidgetItem* itemTo = new QTableWidgetItem(QString(item.to));
  tableWidget->setItem(row, 2, itemTo);

  tableWidget->setRowHeight(row, 15);
}

void SnoopDataChange::loadFromItems(QTableWidget* tableWidget)
{
  tableWidget->verticalHeader()->setVisible(false);

  tableWidget->setColumnCount(3);
  tableWidget->setColumnWidth(0, 25);
  tableWidget->setColumnWidth(1, 100);
  tableWidget->setColumnWidth(2, 100);
  QStringList headers; headers << "" << "From" << "To";
  tableWidget->setHorizontalHeaderLabels(headers);

  int count = items.count();
  tableWidget->setRowCount(count);
  for (int row = 0; row < count; row++)
  {
    SnoopDataChangeItem& item = (SnoopDataChangeItem&)items.at(row);
    addItem(item, tableWidget, row);
  }
}

void SnoopDataChange::saveToItems(QTableWidget* tableWidget)
{
  items.clear();

  int count = tableWidget->rowCount();

  for (int row = 0; row < count; row++)
  {
    SnoopDataChangeItem item;
    item.enabled = tableWidget->item(row, 0)->checkState() == Qt::Checked;
    item.from    = qPrintable(tableWidget->item(row, 1)->text());
    item.to      = qPrintable(tableWidget->item(row, 2)->text());
    items.push_back(item);
  }
}

void SnoopDataChange::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  VOptionable::addCheckBox(layout, "chkTcpChange", "TCP Change", tcpChange);
  VOptionable::addCheckBox(layout, "chkUdpChange", "UDP Change", udpChange);

  QTableWidget* tableWidget = new QTableWidget(layout->parentWidget());
  tableWidget->setObjectName("tableWidget");
  loadFromItems(tableWidget);

  QPushButton* pbAdd = new QPushButton(layout->parentWidget());
  pbAdd->setText("Add");
  QObject::connect(pbAdd, SIGNAL(clicked()), this, SLOT(on_pbAdd_clicked()));

  QPushButton* pbDel = new QPushButton(layout->parentWidget());
  pbDel->setText("Delete");
  QObject::connect(pbDel, SIGNAL(clicked()), this, SLOT(on_pbDel_clicked()));

  QGridLayout* myLayout = new QGridLayout;
  myLayout->setHorizontalSpacing(8);
  myLayout->addWidget(tableWidget, 0, 0, 10, 8);
  myLayout->addWidget(pbAdd, 0, 8);
  myLayout->addWidget(pbDel, 1, 8);
  layout->addItem(myLayout);
}

bool SnoopDataChange::optionShowDlg(QDialog* dialog)
{
  return VOptionable::optionShowDlg(dialog);
}

void SnoopDataChange::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  tcpChange = dialog->findChild<QCheckBox*>("chkTcpChange")->checkState() == Qt::Checked;
  udpChange = dialog->findChild<QCheckBox*>("chkUdpChange")->checkState() == Qt::Checked;
  saveToItems(dialog->findChild<QTableWidget*>("tableWidget"));
}
#endif // QT_GUI_LIB
