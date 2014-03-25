#include <SnoopFlowChange>
#include <SnoopIp>
#include <SnoopTcp>
#include <SnoopUdp>

REGISTER_METACLASS(SnoopFlowChange, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopFlowChangeItem
// ----------------------------------------------------------------------------
SnoopFlowChangeItem::SnoopFlowChangeItem()
{
  enabled = true;
  log     = true;
  protocol = Tcp;

  srcIp             = 0;
  srcIpMask         = 0;
  srcIpChangeType   = Copy;
  srcIpFixValue     = 0;

  srcPort           = 0;
  srcPortMask       = 0;
  srcPortChangeType = AutoInc;
  srcPortFixValue   = 0;

  dstIp             = 0;
  dstIpMask         = 0;
  dstIpChangeType   = Fix;
  dstIpFixValue     = 0;

  dstPort           = 0;
  dstPortMask       = 0;
  dstPortChangeType = Copy;
  dstPortFixValue   = 0;
}

bool SnoopFlowChangeItem::prepare(VError& error)
{
  if (srcIpChangeType == Fix && srcIpFixValue == 0)
  {
    SET_ERROR(VError, "srcIpFixValue is zero", VERR_VALUE_IS_ZERO);
    return false;
  }
  if (srcPortChangeType == Fix && srcPortFixValue == 0)
  {
    SET_ERROR(VError, "srcPortFixValue is zero", VERR_VALUE_IS_ZERO);
    return false;
  }
  if (dstIpChangeType == Fix && dstIpFixValue == 0)
  {
    SET_ERROR(VError, "dstIpFixValue is zero", VERR_VALUE_IS_ZERO);
    return false;
  }
  if (dstPortChangeType == Fix && dstPortFixValue == 0)
  {
    SET_ERROR(VError, "dstPortFixValue is zero", VERR_VALUE_IS_ZERO);
    return false;
  }
  return true;
}

bool SnoopFlowChangeItem::check(SnoopTransportFlowKey& flowKey)
{
  if ((flowKey.srcIp & this->srcIpMask)     != this->srcIp)   return false;
  if ((flowKey.srcPort & this->srcPortMask) != this->srcPort) return false;
  if ((flowKey.dstIp & this->dstIpMask)     != this->dstIp)   return false;
  if ((flowKey.dstPort & this->dstPortMask) != this->dstPort) return false;
  return true;
}

void SnoopFlowChangeItem::load(VXml xml)
{
  enabled           = xml.getBool("enabled", enabled);
  log               = xml.getBool("log", log);

  srcIp             = xml.getStr("srcIp", srcIp.str());
  srcIpMask         = xml.getStr("srcIpMask", srcIpMask.str());
  srcIpChangeType   = (ChangeType)xml.getInt("srcIpChangeType", (int)srcIpChangeType);
  srcIpFixValue     = xml.getStr("srcIpFixValue", srcIpFixValue.str());

  srcPort           = (UINT16)xml.getInt("srcPort", (int)srcPort);
  srcPortMask       = (UINT16)xml.getInt("srcPortMask", (int)srcPortMask);
  srcPortChangeType = (ChangeType)xml.getInt("srcPortChangeType", (int)srcPortChangeType);
  srcPortFixValue   = (UINT16)xml.getInt("srcPortFixValue", (int)srcPortFixValue);

  dstIp             = xml.getStr("dstIp", dstIp.str());
  dstIpMask         = xml.getStr("dstIpMask", dstIpMask.str());
  dstIpChangeType   = (ChangeType)xml.getInt("dstIpChangeType", (int)dstIpChangeType);
  dstIpFixValue     = xml.getStr("dstIpFixValue", dstIpFixValue.str());

  dstPort           = (UINT16)xml.getInt("dstPort", (int)dstPort);
  dstPortMask       = (UINT16)xml.getInt("dstPortMask", (int)dstPortMask);
  dstPortChangeType = (ChangeType)xml.getInt("dstPortChangeType", (int)dstPortChangeType);
  dstPortFixValue   = (UINT16)xml.getInt("dstPortFixValue", (int)dstPortFixValue);
}

void SnoopFlowChangeItem::save(VXml xml)
{
  xml.setBool("enabled", enabled);
  xml.setBool("log", log);

  xml.setStr("srcIp", srcIp.str());
  xml.setStr("srcIpMask", srcIpMask.str());
  xml.setInt("srcIpChangeType", (int)srcIpChangeType);
  xml.setStr("srcIpFixValue", srcIpFixValue.str());

  xml.setInt("srcPort", (int)srcPort);
  xml.setInt("srcPortMask", (int)srcPortMask);
  xml.setInt("srcPortChangeType", (int)srcPortChangeType);
  xml.setInt("srcPortFixValue", (int)srcPortFixValue);

  xml.setStr("dstIp", dstIp.str());
  xml.setStr("dstIpMask", dstIpMask.str());
  xml.setInt("dstIpChangeType", (int)dstIpChangeType);
  xml.setStr("dstIpFixValue", dstIpFixValue.str());

  xml.setInt("dstPort", (int)dstPort);
  xml.setInt("dstPortMask", (int)dstPortMask);
  xml.setInt("dstPortChangeType", (int)dstPortChangeType);
  xml.setInt("dstPortFixValue", (int)dstPortFixValue);
}

#ifdef QT_GUI_LIB
void SnoopFlowChangeItem::initialize(QTreeWidget* treeWidget)
{
  treeWidget->setMinimumWidth(1000);

  QStringList headerLables;
  headerLables << "Enabled" << "Log" << "Protocol"
    << "srcIp"   << "SrcIpMask"   << "SrcIpChangeType"   << "SrcIpFixValue"
    << "srcPort" << "SrcPortMask" << "SrcPortChangeType" << "SrcPortFixValue"
    << "dstIp"   << "dstIpMask"   << "dstIpChangeType"   << "dstIpFixValue"
    << "dstPort" << "dstPortMask" << "dstPortChangeType" << "dstPortFixValue";
  treeWidget->setHeaderLabels(headerLables);

  treeWidget->setColumnWidth(ENABLED_IDX,          30);
  treeWidget->setColumnWidth(LOG_IDX,              30);
  treeWidget->setColumnWidth(PROTOCOL_IDX,         50);

  treeWidget->setColumnWidth(SRC_IP_IDX,           100);
  treeWidget->setColumnWidth(SRC_IP_MASK_IDX,      100);
  treeWidget->setColumnWidth(SRC_IP_CHANGE_TYPE,   70);
  treeWidget->setColumnWidth(SRC_IP_FIX_VALUE,     100);

  treeWidget->setColumnWidth(SRC_PORT_IDX,         50);
  treeWidget->setColumnWidth(SRC_PORT_MASK_IDX,    50);
  treeWidget->setColumnWidth(SRC_PORT_CHANGE_TYPE, 70);
  treeWidget->setColumnWidth(SRC_PORT_FIX_VALUE,   50);

  treeWidget->setColumnWidth(DST_IP_IDX,           100);
  treeWidget->setColumnWidth(DST_IP_MASK_IDX,      100);
  treeWidget->setColumnWidth(DST_IP_CHANGE_TYPE,   70);
  treeWidget->setColumnWidth(DST_IP_FIX_VALUE,     100);

  treeWidget->setColumnWidth(DST_PORT_IDX,         50);
  treeWidget->setColumnWidth(DST_PORT_MASK_IDX,    50);
  treeWidget->setColumnWidth(DST_PORT_CHANGE_TYPE, 70);
  treeWidget->setColumnWidth(DST_PORT_FIX_VALUE,   50);

  treeWidget->header()->setSectionResizeMode(ENABLED_IDX,          QHeaderView::Interactive);
  treeWidget->header()->setSectionResizeMode(LOG_IDX,              QHeaderView::Interactive);
  treeWidget->header()->setSectionResizeMode(PROTOCOL_IDX,         QHeaderView::Interactive);

  treeWidget->header()->setSectionResizeMode(SRC_IP_IDX,           QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(SRC_IP_MASK_IDX,      QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(SRC_IP_CHANGE_TYPE,   QHeaderView::Interactive);
  treeWidget->header()->setSectionResizeMode(SRC_IP_FIX_VALUE,     QHeaderView::Stretch);

  treeWidget->header()->setSectionResizeMode(SRC_PORT_IDX,         QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(SRC_PORT_MASK_IDX,    QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(SRC_PORT_CHANGE_TYPE, QHeaderView::Interactive);
  treeWidget->header()->setSectionResizeMode(SRC_PORT_FIX_VALUE,   QHeaderView::Stretch);

  treeWidget->header()->setSectionResizeMode(DST_IP_IDX,           QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(DST_IP_MASK_IDX,      QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(DST_IP_CHANGE_TYPE,   QHeaderView::Interactive);
  treeWidget->header()->setSectionResizeMode(DST_IP_FIX_VALUE,     QHeaderView::Stretch);

  treeWidget->header()->setSectionResizeMode(DST_PORT_IDX,         QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(DST_PORT_MASK_IDX,    QHeaderView::Stretch);
  treeWidget->header()->setSectionResizeMode(DST_PORT_CHANGE_TYPE, QHeaderView::Interactive);
  treeWidget->header()->setSectionResizeMode(DST_PORT_FIX_VALUE,   QHeaderView::Stretch);
}

void operator << (QTreeWidgetItem& treeWidgetItem, SnoopFlowChangeItem& item)
{
  treeWidgetItem.setCheckState(SnoopFlowChangeItem::ENABLED_IDX, item.enabled ? Qt::Checked : Qt::Unchecked);
  treeWidgetItem.setCheckState(SnoopFlowChangeItem::LOG_IDX,     item.log ? Qt::Checked : Qt::Unchecked);

  QComboBox* protocolComboBox = new QComboBox(treeWidgetItem.treeWidget());
  QStringList sl; sl << "TCP" << "UDP";
  protocolComboBox->insertItems(0, sl);
  protocolComboBox->setCurrentIndex((int)item.protocol);
  treeWidgetItem.treeWidget()->setItemWidget(&treeWidgetItem, SnoopFlowChangeItem::PROTOCOL_IDX, protocolComboBox);

  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_IP_IDX, item.srcIp.str());
  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_IP_MASK_IDX, item.srcIpMask.str());
  QComboBox* srcIpComboBox = new QComboBox(treeWidgetItem.treeWidget());
  QStringList changeTypeList; changeTypeList << "Copy" << "AutoInc" << "Fix";
  srcIpComboBox->insertItems(0, changeTypeList);
  srcIpComboBox->setCurrentIndex((int)item.srcIpChangeType);
  treeWidgetItem.treeWidget()->setItemWidget(&treeWidgetItem, SnoopFlowChangeItem::SRC_IP_CHANGE_TYPE, srcIpComboBox);
  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_IP_FIX_VALUE, item.srcIpFixValue.str());

  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_PORT_IDX, QString::number(item.srcPort));
  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_PORT_MASK_IDX, QString::number(item.srcPortMask));
  QComboBox* srcPortComboBox = new QComboBox(treeWidgetItem.treeWidget());
  srcPortComboBox->insertItems(0, changeTypeList);
  srcPortComboBox->setCurrentIndex((int)item.srcPortChangeType);
  treeWidgetItem.treeWidget()->setItemWidget(&treeWidgetItem, SnoopFlowChangeItem::SRC_PORT_CHANGE_TYPE, srcPortComboBox);
  treeWidgetItem.setText(SnoopFlowChangeItem::SRC_PORT_FIX_VALUE, QString::number(item.srcPortFixValue));


  treeWidgetItem.setText(SnoopFlowChangeItem::DST_IP_IDX, item.dstIp.str());
  treeWidgetItem.setText(SnoopFlowChangeItem::DST_IP_MASK_IDX, item.dstIpMask.str());
  QComboBox* dstIpComboBox = new QComboBox(treeWidgetItem.treeWidget());
  dstIpComboBox->insertItems(0, changeTypeList);
  dstIpComboBox->setCurrentIndex((int)item.dstIpChangeType);
  treeWidgetItem.treeWidget()->setItemWidget(&treeWidgetItem, SnoopFlowChangeItem::DST_IP_CHANGE_TYPE, dstIpComboBox);
  treeWidgetItem.setText(SnoopFlowChangeItem::DST_IP_FIX_VALUE, item.dstIpFixValue.str());

  treeWidgetItem.setText(SnoopFlowChangeItem::DST_PORT_IDX, QString::number(item.dstPort));
  treeWidgetItem.setText(SnoopFlowChangeItem::DST_PORT_MASK_IDX, QString::number(item.dstPortMask));
  QComboBox* dstPortComboBox = new QComboBox(treeWidgetItem.treeWidget());
  dstPortComboBox->insertItems(0, changeTypeList);
  dstPortComboBox->setCurrentIndex((int)item.dstPortChangeType);
  treeWidgetItem.treeWidget()->setItemWidget(&treeWidgetItem, SnoopFlowChangeItem::DST_PORT_CHANGE_TYPE, dstPortComboBox);
  treeWidgetItem.setText(SnoopFlowChangeItem::DST_PORT_FIX_VALUE, QString::number(item.dstPortFixValue));

  treeWidgetItem.setFlags(treeWidgetItem.flags() | Qt::ItemIsEditable);
}

void operator << (SnoopFlowChangeItem& item, QTreeWidgetItem& treeWidgetItem)
{
  item.enabled  = treeWidgetItem.checkState(SnoopFlowChangeItem::ENABLED_IDX) == Qt::Checked;
  item.log      = treeWidgetItem.checkState(SnoopFlowChangeItem::LOG_IDX)     == Qt::Checked;
  QComboBox* protocolComboBox = dynamic_cast<QComboBox*>(treeWidgetItem.treeWidget()->itemWidget(&treeWidgetItem, SnoopFlowChangeItem::PROTOCOL_IDX));
  item.protocol = (SnoopFlowChangeItem::Protocol)(protocolComboBox->currentIndex());

  item.srcIp = treeWidgetItem.text(SnoopFlowChangeItem::SRC_IP_IDX);
  item.srcIpMask = treeWidgetItem.text(SnoopFlowChangeItem::SRC_IP_MASK_IDX);
  QComboBox* srcIpComboBox = dynamic_cast<QComboBox*>(treeWidgetItem.treeWidget()->itemWidget(&treeWidgetItem, SnoopFlowChangeItem::SRC_IP_CHANGE_TYPE));
  item.srcIpChangeType = (SnoopFlowChangeItem::ChangeType)(srcIpComboBox->currentIndex());
  item.srcIpFixValue = treeWidgetItem.text(SnoopFlowChangeItem::SRC_IP_FIX_VALUE);

  item.srcPort = treeWidgetItem.text(SnoopFlowChangeItem::SRC_PORT_IDX).toUShort();
  item.srcPortMask = treeWidgetItem.text(SnoopFlowChangeItem::SRC_PORT_MASK_IDX).toUShort();
  QComboBox* srcPortComboBox = dynamic_cast<QComboBox*>(treeWidgetItem.treeWidget()->itemWidget(&treeWidgetItem, SnoopFlowChangeItem::SRC_PORT_CHANGE_TYPE));
  item.srcPortChangeType = (SnoopFlowChangeItem::ChangeType)(srcPortComboBox->currentIndex());
  item.srcPortFixValue = treeWidgetItem.text(SnoopFlowChangeItem::SRC_PORT_FIX_VALUE).toUShort();

  item.dstIp = treeWidgetItem.text(SnoopFlowChangeItem::DST_IP_IDX);
  item.dstIpMask = treeWidgetItem.text(SnoopFlowChangeItem::DST_IP_MASK_IDX);
  QComboBox* dstIpComboBox = dynamic_cast<QComboBox*>(treeWidgetItem.treeWidget()->itemWidget(&treeWidgetItem, SnoopFlowChangeItem::DST_IP_CHANGE_TYPE));
  item.dstIpChangeType = (SnoopFlowChangeItem::ChangeType)(dstIpComboBox->currentIndex());
  item.dstIpFixValue = treeWidgetItem.text(SnoopFlowChangeItem::DST_IP_FIX_VALUE);

  item.dstPort = treeWidgetItem.text(SnoopFlowChangeItem::DST_PORT_IDX).toUShort();
  item.dstPortMask = treeWidgetItem.text(SnoopFlowChangeItem::DST_PORT_MASK_IDX).toUShort();
  QComboBox* dstPortComboBox = dynamic_cast<QComboBox*>(treeWidgetItem.treeWidget()->itemWidget(&treeWidgetItem, SnoopFlowChangeItem::DST_PORT_CHANGE_TYPE));
  item.dstPortChangeType = (SnoopFlowChangeItem::ChangeType)(dstPortComboBox->currentIndex());
  item.dstPortFixValue = treeWidgetItem.text(SnoopFlowChangeItem::DST_PORT_FIX_VALUE).toUShort();

  // gilgil temp 2014.03.25

  VError error; item.prepare(error);
}
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopFlowChangeItems
// ----------------------------------------------------------------------------
SnoopFlowChangeItems::SnoopFlowChangeItems()
{
  VError error; prepare(error);
}

SnoopFlowChangeItems::~SnoopFlowChangeItems()
{

}

bool SnoopFlowChangeItems::prepare(VError& error)
{
  lastAutoIncSrcIp   = 0;
  lastAutoIncSrcPort = 1024;
  lastAutoIncDstIp   = 0;
  lastAutoIncDstPort = 1024;

  for (int i = 0; i < count(); i++)
  {
    SnoopFlowChangeItem& item = (SnoopFlowChangeItem&)at(i);
    if (!item.prepare(error)) return false;
  }
  return true;
}

SnoopFlowChangeItem* SnoopFlowChangeItems::find(SnoopTransportFlowKey& flowKey)
{
  for (int i = 0; i < count(); i++)
  {
    SnoopFlowChangeItem& item = (SnoopFlowChangeItem&)at(i);
    if (item.check(flowKey)) return &item;
  }
  return NULL;
}

SnoopTransportFlowKey SnoopFlowChangeItems::change(SnoopFlowChangeItem& item, SnoopTransportFlowKey& flowKey)
{
  SnoopTransportFlowKey res;

  switch (item.srcIpChangeType)
  {
    case SnoopFlowChangeItem::Copy:
      res.srcIp = flowKey.srcIp;
      break;
    case SnoopFlowChangeItem::AutoInc:
      res.srcIp = lastAutoIncSrcIp;
      lastAutoIncSrcIp = lastAutoIncSrcIp + 1;
      break;
    case SnoopFlowChangeItem::Fix:
      res.srcIp = item.srcIpFixValue;
      break;
  }

  switch (item.srcPortChangeType)
  {
    case SnoopFlowChangeItem::Copy:
      res.srcPort = flowKey.srcPort;
      break;
    case SnoopFlowChangeItem::AutoInc:
      res.srcPort = lastAutoIncSrcPort;
      lastAutoIncSrcPort++;
      break;
    case SnoopFlowChangeItem::Fix:
      res.srcPort = item.srcPortFixValue;
      break;
  }

  switch (item.dstIpChangeType)
  {
    case SnoopFlowChangeItem::Copy:
      res.dstIp = flowKey.dstIp;
      break;
    case SnoopFlowChangeItem::AutoInc:
      res.dstIp = lastAutoIncDstIp;
      lastAutoIncDstIp = lastAutoIncDstIp + 1;
      break;
    case SnoopFlowChangeItem::Fix:
      res.dstIp = item.dstIpFixValue;
      break;
  }

  switch (item.dstPortChangeType)
  {
    case SnoopFlowChangeItem::Copy:
      res.dstPort = flowKey.dstPort;
      break;
    case SnoopFlowChangeItem::AutoInc:
      res.dstPort = lastAutoIncDstPort;
      lastAutoIncDstPort++;
      break;
    case SnoopFlowChangeItem::Fix:
      res.dstPort = item.dstPortFixValue;
      break;
  }

  return res;
}

void SnoopFlowChangeItems::load(VXml xml)
{
  clear();
  {
    xml_foreach (childXml, xml.childs())
    {
      SnoopFlowChangeItem item;
      item.load(childXml);
      push_back(item);
    }
  }
}

void SnoopFlowChangeItems::save(VXml xml)
{
  xml.clearChild();
  for (SnoopFlowChangeItems::iterator it = begin(); it != end(); it++)
  {
    SnoopFlowChangeItem& item = *it;
    VXml childXml = xml.addChild("item");
    item.save(childXml);
  }
}

#ifdef QT_GUI_LIB
#include "../lib/ui_vlistwidget.h"
void SnoopFlowChangeItems::optionAddWidget(QLayout* layout)
{
  VListWidget* widget = new VListWidget(layout->parentWidget(), this);
  SnoopFlowChangeItem::initialize(widget->ui->treeWidget);
  widget->setObjectName("snoopFlowChangeWidget");
  widget->itemsIntoTreeWidget();
  layout->addWidget(widget);
}

void SnoopFlowChangeItems::optionSaveDlg(QDialog* dialog)
{
  VListWidget* widget = dialog->findChild<VListWidget*>("snoopFlowChangeWidget");
  LOG_ASSERT(widget != NULL);
  widget->treeWidgetIntoItems();
}
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// SnoopFlowChange
// ----------------------------------------------------------------------------
SnoopFlowChange::SnoopFlowChange(void* owner) : SnoopProcess(owner)
{
  flowMgr       = NULL;
  tcpChange     = true;
  udpChange     = true;
  changeItems.clear();
  tcpFlowOffset = 0;
  udpFlowOffset = 0;
}

SnoopFlowChange::~SnoopFlowChange()
{
  close();
}

bool SnoopFlowChange::doOpen()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }

  if (!changeItems.prepare(error)) return false;

  if (tcpChange)
  {
    tcpFlowOffset = flowMgr->requestMemory_TcpFlow("SnoopFlowChange", sizeof(SnoopFlowChangeFlowItem));
    flowMgr->connect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    flowMgr->connect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
  }

  if (udpChange)
  {
    udpFlowOffset = flowMgr->requestMemory_UdpFlow("SnoopFlowChange", sizeof(SnoopFlowChangeFlowItem));
    flowMgr->connect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
    flowMgr->connect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
  }

  return SnoopProcess::doOpen();
}

bool SnoopFlowChange::doClose()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return true;
  }

  if (tcpChange)
  {
    flowMgr->disconnect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)));
    flowMgr->disconnect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)));
  }

  if (udpChange)
  {
    flowMgr->disconnect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)));
    flowMgr->disconnect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)));
  }

  return SnoopProcess::doClose();
}

void SnoopFlowChange::processInOut(SnoopPacket* packet)
{
  if (packet->ipHdr == NULL)
  {
    emit unchanged(packet);
    return;
  }

  bool _changed = false;
  if (packet->tcpHdr != NULL)
  {
    if (tcpChange)
    {
      SnoopFlowChangeFlowItem* flowItem = (SnoopFlowChangeFlowItem*)(packet->flowValue->totalMem + tcpFlowOffset);
      if (flowItem->changed)
      {
        Ip     oldSrcIp   = ntohl(packet->ipHdr->ip_src);
        UINT16 oldSrcPort = ntohs(packet->tcpHdr->th_sport);
        Ip     oldDstIp   = ntohl(packet->ipHdr->ip_dst);
        UINT16 oldDstPort = ntohs(packet->tcpHdr->th_dport);

        Ip     newSrcIp   = flowItem->to.srcIp;
        UINT16 newSrcPort = flowItem->to.srcPort;
        Ip     newDstIp   = flowItem->to.dstIp;
        UINT16 newDstPort = flowItem->to.dstPort;

        packet->ipHdr->ip_src    = htonl(newSrcIp);
        packet->tcpHdr->th_sport = htons(newSrcPort);
        packet->ipHdr->ip_dst    = htonl(newDstIp);
        packet->tcpHdr->th_dport = htons(newDstPort);

        UINT16 oldIpChecksum  = ntohs(packet->ipHdr->ip_sum);
        UINT16 oldTcpChecksum = ntohs(packet->tcpHdr->th_sum);

        UINT16 newIpChecksum;
        newIpChecksum = SnoopIp::recalculateChecksum(oldIpChecksum, oldSrcIp, newSrcIp);
        newIpChecksum = SnoopIp::recalculateChecksum(newIpChecksum, oldDstIp, newDstIp);
        // UINT16 correctIpChecksum = SnoopIp::checksum(packet->ipHdr); // gilgil temp 2014.03.25
        // LOG_DEBUG("ip checksum=0x%x 0x%x 0x%x", oldIpChecksum, newIpChecksum, correctIpChecksum); // gilgil temp 2014.03.25
        packet->ipHdr->ip_sum = htons(newIpChecksum);

        UINT16 newTcpChecksum;
        newTcpChecksum = SnoopIp::recalculateChecksum(oldTcpChecksum, oldSrcIp,   newSrcIp);
        newTcpChecksum = SnoopIp::recalculateChecksum(newTcpChecksum, oldDstIp,   newDstIp);
        newTcpChecksum = SnoopIp::recalculateChecksum(newTcpChecksum, oldSrcPort, newSrcPort);
        newTcpChecksum = SnoopIp::recalculateChecksum(newTcpChecksum, oldDstPort, newDstPort);
        // UINT16 correctTcpChecksum = SnoopTcp::checksum(packet->ipHdr, packet->tcpHdr);  // gilgil temp 2014.03.25
        // LOG_DEBUG("tcp checksum=0x%x 0x%x 0x%x", oldTcpChecksum, newTcpChecksum, correctTcpChecksum); // gilgil temp 2014.03.25
        packet->tcpHdr->th_sum = htons(newTcpChecksum);
      }
    }
  }


  if (_changed)
  {
    emit changed(packet);
  } else
  {
    emit unchanged(packet);
  }
}

void SnoopFlowChange::processOutIn(SnoopPacket* packet)
{

}

void SnoopFlowChange::__tcpFlowCreate(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  SnoopFlowChangeFlowItem* flowItem = (SnoopFlowChangeFlowItem*)(value->totalMem + tcpFlowOffset);

  flowItem->changed = false;
  flowItem->from    = *key;
  flowItem->to      = *key;

  SnoopFlowChangeItem* item = changeItems.find(*key);
  if (item != NULL)
  {
    SnoopTransportFlowKey changeFlow = changeItems.change(*item, *key);
    if (*key == changeFlow)
    {
      LOG_DEBUG("key and changeFlow is same(%s:%d > %s:%d)", qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort); // gilgil temp 2014.03.25
      return;
    }
    flowItem->changed = true;
    flowItem->to      = changeFlow;
    if (item->log)
    {
      LOG_INFO("change %s:%d > %s:%d into %s:%d > %s:%d",
        qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort,
        qPrintable(flowItem->to.srcIp.str()), flowItem->to.srcPort, qPrintable(flowItem->to.dstIp.str()), flowItem->to.dstPort);
    }
    return;
  }

  SnoopTcpFlowKey rkey = key->reverse();
  Snoop_TcpFlow_Map::iterator it = flowMgr->tcpFlow_Map.find(rkey);
  if (it != flowMgr->tcpFlow_Map.end())
  {
    SnoopFlowValue& rvalue = it.value();
    SnoopFlowChangeFlowItem* rflowItem = (SnoopFlowChangeFlowItem*)(rvalue.totalMem + tcpFlowOffset);

    flowItem->changed  = true;
    flowItem->to       = rflowItem->from;
    if (item->log)
    {
      LOG_INFO("change (reverse) %s:%d > %s:%d into %s:%d > %s:%d",
        qPrintable(key->srcIp.str()), key->srcPort, qPrintable(key->dstIp.str()), key->dstPort,
        qPrintable(flowItem->to.srcIp.str()), flowItem->to.srcPort, qPrintable(flowItem->to.dstIp.str()), flowItem->to.dstPort);
    }
    return;
  }


}

void SnoopFlowChange::__tcpFlowDelete(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(key)
  Q_UNUSED(value)
}

void SnoopFlowChange::__udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{

}

void SnoopFlowChange::__udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{

}

void SnoopFlowChange::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString flowMgrName = xml.getStr("flowMgr", "");
  if (flowMgrName != "") flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(flowMgrName));
  tcpChange = xml.getBool("tcpChange", tcpChange);
  udpChange = xml.getBool("udpChange", udpChange);
  changeItems.load(xml.gotoChild("changeItems"));
}

void SnoopFlowChange::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString flowMgrName = flowMgr == NULL ? "" : flowMgr->name;
  xml.setStr("flowMgr", flowMgrName);
  xml.setBool("tcpChange", tcpChange);
  xml.setBool("udpChange", udpChange);
  changeItems.save(xml.gotoChild("changeItems"));
}

#ifdef QT_GUI_LIB
void SnoopFlowChange::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  VOptionable::addComboBox(layout, "cbxFlowMgr", "FlowMgr", flowMgrList, -1, flowMgr == NULL ? "" : flowMgr->name);
  VOptionable::addCheckBox(layout, "chkTcpChange", "TCP Change", tcpChange);
  VOptionable::addCheckBox(layout, "chkUdpChange", "UDP Change", udpChange);
  changeItems.optionAddWidget(layout);
}

void SnoopFlowChange::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxFlowMgr")->currentText()));
  tcpChange = dialog->findChild<QCheckBox*>("chkTcpChange")->checkState() == Qt::Checked;
  udpChange = dialog->findChild<QCheckBox*>("chkUdpChange")->checkState() == Qt::Checked;
  changeItems.optionSaveDlg(dialog);
}
#endif // QT_GUI_LIB
