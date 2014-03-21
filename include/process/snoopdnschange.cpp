#include <SnoopDnsChange>
#include <VDebugNew>

REGISTER_METACLASS(SnoopDnsChange, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopDnsChangeItem
// ----------------------------------------------------------------------------
SnoopDnsChangeItem::SnoopDnsChangeItem()
{
  enabled = true;
  log     = true;
  ip      = 0;
}

void SnoopDnsChangeItem::load(VXml xml)
{
  VRegExp::load(xml);

  enabled = xml.getBool("enabled", enabled);
  log     = xml.getBool("log", log);
  ip      = xml.getStr("ip", ip.str());
}

void SnoopDnsChangeItem::save(VXml xml)
{
  VRegExp::save(xml);

  xml.setBool("enabled", enabled);
  xml.setBool("log", log);
  xml.setStr("ip", ip.str());
}

#ifdef QT_GUI_LIB
void SnoopDnsChangeItem::initialize(QTreeWidget* treeWidget)
{
  VRegExp::initialize(treeWidget);

  QStringList headerLables;
  for (int i = 0; i < treeWidget->columnCount(); i++) headerLables << treeWidget->headerItem()->text(i);
  headerLables << "Enabled" << "Log" << "IP";
  treeWidget->setHeaderLabels(headerLables);

  treeWidget->setColumnWidth(ENABLED_IDX, 30);
  treeWidget->setColumnWidth(LOG_IDX,     30);
  //treeWidget->setColumnWidth(IP_IDX,      0);

  treeWidget->header()->setSectionResizeMode(ENABLED_IDX, QHeaderView::Interactive);
  treeWidget->header()->setSectionResizeMode(LOG_IDX,     QHeaderView::Interactive);
  treeWidget->header()->setSectionResizeMode(IP_IDX,      QHeaderView::Stretch);
}

void operator << (QTreeWidgetItem& treeWidgetItem, SnoopDnsChangeItem& item)
{
  treeWidgetItem << (VRegExp&)item;

  treeWidgetItem.setCheckState(SnoopDnsChangeItem::ENABLED_IDX, item.enabled ? Qt::Checked : Qt::Unchecked);
  treeWidgetItem.setCheckState(SnoopDnsChangeItem::LOG_IDX,     item.log     ? Qt::Checked : Qt::Unchecked);
  treeWidgetItem.setText(SnoopDnsChangeItem::IP_IDX, item.ip.str());
}

void operator << (SnoopDnsChangeItem& item, QTreeWidgetItem& treeWidgetItem)
{
  (VRegExp&)item << treeWidgetItem;

  item.enabled = treeWidgetItem.checkState(SnoopDnsChangeItem::ENABLED_IDX) == Qt::Checked;
  item.log     = treeWidgetItem.checkState(SnoopDnsChangeItem::LOG_IDX) == Qt::Checked;
  item.ip      = treeWidgetItem.text(SnoopDnsChangeItem::IP_IDX);
}
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopDnsChangeItems
// ----------------------------------------------------------------------------
bool SnoopDnsChangeItems::prepare(VError& error)
{
  for (int i = 0; i < count(); i++)
  {
    SnoopDnsChangeItem& item = (SnoopDnsChangeItem&)at(i);
    if (!item.prepare(error)) return false;
  }
  return true;
}

void SnoopDnsChangeItems::load(VXml xml)
{
  clear();
  {
    xml_foreach (childXml, xml.childs())
    {
      SnoopDnsChangeItem item;
      item.load(childXml);
      push_back(item);
    }
  }
}

void SnoopDnsChangeItems::save(VXml xml)
{
  xml.clearChild();
  for (SnoopDnsChangeItems::iterator it = begin(); it != end(); it++)
  {
    SnoopDnsChangeItem& item = *it;
    VXml childXml = xml.addChild("item");
    item.save(childXml);
  }
}

#ifdef QT_GUI_LIB
#include "other/vdatafindwidget.h"     // gilgil temp 2014.03.19
#include "../lib/ui_vdatafindwidget.h" // gilgil temp 2014.03.19
void SnoopDnsChangeItems::__on_pbAdd_clicked()
{
  QPushButton* pbAdd = dynamic_cast<QPushButton*>(this->sender());
  LOG_ASSERT(pbAdd != NULL);
  VDataFindWidget* widget = dynamic_cast<VDataFindWidget*>(pbAdd->parent());
  LOG_ASSERT(widget != NULL);
  QTreeWidget* treeWidget = widget->ui->treeWidget;

  QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem(treeWidget);
  SnoopDnsChangeItem newItem;
  *treeWidgetItem << newItem;
  int count = treeWidget->topLevelItemCount();
  treeWidget->insertTopLevelItem(count, treeWidgetItem);
}

void SnoopDnsChangeItems::__on_pbDel_clicked()
{
  QPushButton* pbDel = dynamic_cast<QPushButton*>(this->sender());
  LOG_ASSERT(pbDel != NULL);
  VDataFindWidget* widget = dynamic_cast<VDataFindWidget*>(pbDel->parent());
  LOG_ASSERT(widget != NULL);
  QTreeWidget* treeWidget = widget->ui->treeWidget;

  QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
  foreach (QTreeWidgetItem* item, items)
  {
    delete item;
  }
}

void SnoopDnsChangeItems::optionAddWidget(QLayout* layout)
{
  VDataFindWidget* widget = new VDataFindWidget(layout->parentWidget());
  SnoopDnsChangeItem::initialize(widget->ui->treeWidget);
  widget->setObjectName("snoopDnsChangeWidget");
  *(widget->ui->treeWidget) << *this;
  layout->addWidget(widget);
  VObject::connect(widget->ui->pbAdd, SIGNAL(clicked()), this, SLOT(__on_pbAdd_clicked()));
  VObject::connect(widget->ui->pbDel, SIGNAL(clicked()), this, SLOT(__on_pbDel_clicked()));
}

void SnoopDnsChangeItems::optionSaveDlg(QDialog* dialog)
{
  VDataFindWidget* widget = dialog->findChild<VDataFindWidget*>("snoopDnsChangeWidget");
  LOG_ASSERT(widget != NULL);
  *this << *(widget->ui->treeWidget);
}

void operator << (QTreeWidget& treeWidget, SnoopDnsChangeItems& changeItems)
{
  treeWidget.clear();
  QList<QTreeWidgetItem*> treeWidgetItems;
  for (int i = 0; i < changeItems.count(); i++)
  {
    SnoopDnsChangeItem& item = (SnoopDnsChangeItem&)changeItems.at(i);
    QTreeWidgetItem* newWidgetItem = new QTreeWidgetItem(&treeWidget);
    *newWidgetItem << item;
    treeWidgetItems.push_back(newWidgetItem);
  }
  treeWidget.insertTopLevelItems(0, treeWidgetItems);
}

void operator << (SnoopDnsChangeItems& changeItems, QTreeWidget& treeWidget)
{
  changeItems.clear();
  int count = treeWidget.topLevelItemCount();
  for (int i = 0; i < count; i++)
  {
    QTreeWidgetItem* treeWidgetItem = treeWidget.topLevelItem(i);
    SnoopDnsChangeItem newItem;
    newItem << *treeWidgetItem;
    changeItems.push_back(newItem);
  }
}
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopDnsChange
// ----------------------------------------------------------------------------
SnoopDnsChange::SnoopDnsChange(void* owner) : SnoopProcess(owner)
{
  capture = NULL;
  changeItems.clear();
}

SnoopDnsChange::~SnoopDnsChange()
{
  close();
}

bool SnoopDnsChange::doOpen()
{
  if (capture == NULL)
  {
    SET_ERROR(SnoopError, "capture is null", VERR_OBJECT_IS_NULL);
    return false;
  }

  if (!changeItems.prepare(error)) return false;

  return SnoopProcess::doOpen();
}

bool SnoopDnsChange::doClose()
{
  return SnoopProcess::doClose();
}

#include <SnoopUdp>
void SnoopDnsChange::check(SnoopPacket* packet)
{
  if (packet->udpHdr == NULL) return;

  if (packet->data == NULL)
  {
    LOG_WARN("packet->data is null");
    return;
  }

  SnoopDns dns;

  int offset = 0;
  if (!dns.decode(packet->data, packet->dataLen, &offset))
  {
    LOG_DEBUG("dns decode return false");
    return;
  }

  DNS_HDR* dnsHdr = &dns.dnsHdr;
  LOG_DEBUG("id=0x%02x flags=0x%02x num_q=%u num_answ_rr=%u num_auth_rr=%u num_addi_rr=%u",
    ntohs(dnsHdr->id),
    ntohs(dnsHdr->flags),
    ntohs(dnsHdr->num_q),
    ntohs(dnsHdr->num_answ_rr),
    ntohs(dnsHdr->num_auth_rr),
    ntohs(dnsHdr->num_addi_rr));
  LOG_DEBUG("%s", qPrintable(dns.questions.at(0).name));

   // gilgil temp 2014.03.19
}

void SnoopDnsChange::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString captureName = xml.getStr("capture", "");
  if (captureName != "") capture = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(captureName));
  changeItems.load(xml.gotoChild("changeItems"));
}

void SnoopDnsChange::save(VXml xml)
{
  SnoopProcess::save(xml);
  QString captureName = capture == NULL ? "" : capture->name;
  xml.setStr("capture", captureName);
  changeItems.save(xml.gotoChild("changeItems"));
}

#ifdef QT_GUI_LIB
void SnoopDnsChange::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList captureList = ((VGraph*)owner)->objectList.findNamesByCategoryName("SnoopCapture");
  VOptionable::addComboBox(layout, "cbxCapture", "Capture", captureList, -1, capture == NULL ? "" : capture->name);
  changeItems.optionAddWidget(layout);
}

void SnoopDnsChange::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  capture = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxCapture")->currentText()));
  changeItems.optionSaveDlg(dialog);
}
#endif // QT_GUI_LIB
