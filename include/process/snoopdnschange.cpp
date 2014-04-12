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
#include "ui_vlistwidget.h"
void SnoopDnsChangeItems::optionAddWidget(QLayout* layout)
{
  VListWidget* widget = new VListWidget(layout->parentWidget(), this);
  SnoopDnsChangeItem::initialize(widget->ui->treeWidget);
  widget->setObjectName("snoopDnsChangeWidget");
  widget->itemsIntoTreeWidget();
  layout->addWidget(widget);
}

void SnoopDnsChangeItems::optionSaveDlg(QDialog* dialog)
{
  VListWidget* widget = dialog->findChild<VListWidget*>("snoopDnsChangeWidget");
  LOG_ASSERT(widget != NULL);
  widget->treeWidgetIntoItems();
}
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopDnsChange
// ----------------------------------------------------------------------------
SnoopDnsChange::SnoopDnsChange(void* owner) : SnoopProcess(owner)
{
  writer = NULL;
  changeItems.clear();
}

SnoopDnsChange::~SnoopDnsChange()
{
  close();
}

bool SnoopDnsChange::doOpen()
{
  if (writer == NULL)
  {
    SET_ERROR(SnoopError, "writer is null", VERR_OBJECT_IS_NULL);
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
  if (packet->ipHdr         == NULL) return;
  if (packet->ipHdr->ip_tos == 0x44) return;
  if (packet->udpHdr        == NULL) return;
  if (ntohs(packet->udpHdr->uh_dport) != 53) return;
  if (packet->data          == NULL)
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

  if ((ntohs(dns.dnsHdr.flags) & 0x8000) != 0x0000) return; // check "message is a query"
  if (ntohs(dns.dnsHdr.num_q) != 1) return;                 // check query number is one
  if (ntohs(dns.dnsHdr.num_answ_rr) != 0) return;           // check answer number is zero
  if (ntohs(dns.dnsHdr.num_auth_rr) != 0) return;           // check auth number is zero
  if (ntohs(dns.dnsHdr.num_addi_rr) != 0) return;           // check addi number is zero

  if (dns.questions.count() != 1) return;
  SnoopDnsQuestion question = dns.questions.at(0);
  QString questionName = question.name;

  LOG_DEBUG("id=0x%02x flags=0x%02x num_q=%u num_answ_rr=%u num_auth_rr=%u num_addi_rr=%u name=%s",
    ntohs(dnsHdr->id),
    ntohs(dnsHdr->flags),
    ntohs(dnsHdr->num_q),
    ntohs(dnsHdr->num_answ_rr),
    ntohs(dnsHdr->num_auth_rr),
    ntohs(dnsHdr->num_addi_rr),
    qPrintable(questionName)); // gilgil temp 2014.03.22

  for (int i = 0; i < changeItems.count(); i++)
  {
    SnoopDnsChangeItem& changeItem = (SnoopDnsChangeItem&)changeItems.at(i);
    if (!changeItem.enabled) continue;
    int index = changeItem.rx.indexIn(questionName);
    if (index != 0) continue;

    SnoopDns response;
    response.dnsHdr.id = dns.dnsHdr.id;
    response.dnsHdr.flags = htons(0x8180);
    response.dnsHdr.num_q = htons(0x0001);
    response.dnsHdr.num_answ_rr = htons(0x0001);
    response.dnsHdr.num_auth_rr = htons(0x0000);
    response.dnsHdr.num_addi_rr = htons(0x0000);

    SnoopDnsQuestion responseQuestion;
    {
      responseQuestion.name   = question.name;
      responseQuestion.type   = question.type;
      responseQuestion._class = question._class;
      response.questions.push_back(responseQuestion);
    }

    SnoopDnsResourceRecord responseAnswer;
    {
      responseAnswer.name       = question.name;
      responseAnswer.type       = 0x0001; // Type: A(Host address)
      responseAnswer._class     = 0x0001; // Class: IN(0x0001)
      responseAnswer.ttl        = 3600;   // (0x00000E10) 3600 seconds
      responseAnswer.dataLength = 0x0004; // IP address length
      Ip ip                     = htonl(changeItem.ip);
      responseAnswer.data.append((const char*)&ip, sizeof(UINT32));
      response.answers.push_back(responseAnswer);
    }

    QByteArray responseMsg = response.encode();

    int      bufSize = sizeof(ETH_HDR) + sizeof(IP_HDR) + sizeof(UDP_HDR) + responseMsg.size();
    BYTE*    buf     = new BYTE[bufSize];
    ETH_HDR* ethHdr  = (ETH_HDR*)buf;
    IP_HDR*  ipHdr   = (IP_HDR*) (buf + sizeof(ETH_HDR));
    UDP_HDR* udpHdr  = (UDP_HDR*)(buf + sizeof(ETH_HDR) + sizeof(IP_HDR));
    BYTE*    udpData = (BYTE*)   (buf + sizeof(ETH_HDR) + sizeof(IP_HDR) + sizeof(UDP_HDR));

    ethHdr->ether_dhost = packet->ethHdr->ether_shost;
    ethHdr->ether_shost = packet->ethHdr->ether_dhost;
    ethHdr->ether_type  = packet->ethHdr->ether_type;

    memcpy(ipHdr, packet->ipHdr, sizeof(IP_HDR));
    ipHdr->ip_tos = 0x44;
    ipHdr->ip_len = htons(sizeof(IP_HDR) + sizeof(UDP_HDR) + responseMsg.size());
    ipHdr->ip_src = packet->ipHdr->ip_dst;
    ipHdr->ip_dst = packet->ipHdr->ip_src;

    udpHdr->uh_sport = packet->udpHdr->uh_dport;
    udpHdr->uh_dport = packet->udpHdr->uh_sport;
    udpHdr->uh_ulen  = htons(sizeof(UDP_HDR) + (UINT16)responseMsg.size());

    memcpy(udpData, responseMsg.data(), responseMsg.size());

    WINDIVERT_ADDRESS responseDivertAddr = packet->divertAddr;
    responseDivertAddr.Direction         = 1 - responseDivertAddr.Direction;

    udpHdr->uh_sum = htons(SnoopUdp::checksum(ipHdr, udpHdr));
    ipHdr->ip_sum  = htons(SnoopIp::checksum(ipHdr));

    writer->write(buf, bufSize, &responseDivertAddr);
    packet->drop = true;

    if (changeItem.log)
    {
      QString ipStr = changeItem.ip.str();
      LOG_INFO("%s changed %s", qPrintable(questionName), qPrintable(ipStr)); // gilgil temp 2014.03.22
    }

    delete[] buf;

    break;
  }
}

void SnoopDnsChange::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString writerName = xml.getStr("writer", "");
  if (writerName != "") writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(writerName));
  changeItems.load(xml.gotoChild("changeItems"));
}

void SnoopDnsChange::save(VXml xml)
{
  SnoopProcess::save(xml);
  QString writerName = writer == NULL ? "" : writer->name;
  xml.setStr("writer", writerName);
  changeItems.save(xml.gotoChild("changeItems"));
}

#ifdef QT_GUI_LIB
void SnoopDnsChange::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList writerList = ((VGraph*)owner)->objectList.findNamesByCategoryName("SnoopCapture");
  VOptionable::addComboBox(layout, "cbxWriter", "Writer", writerList, -1, writer == NULL ? "" : writer->name);
  changeItems.optionAddWidget(layout);
}

void SnoopDnsChange::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxWriter")->currentText()));
  changeItems.optionSaveDlg(dialog);
}
#endif // QT_GUI_LIB
