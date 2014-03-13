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
  re      = false;
  log     = true;
  from    = "";
  to      = "";
}

void SnoopDataChangeItem::load(VXml xml)
{
  enabled = xml.getBool("enabled", enabled);
  re      = xml.getBool("re", re);
  log     = xml.getBool("log", log);
  from    = qPrintable(xml.getStr("from", from));
  to      = qPrintable(xml.getStr("to",   to));
}

void SnoopDataChangeItem::save(VXml xml)
{
  xml.setBool("enabled", enabled);
  xml.setBool("re", re);
  xml.setBool("log", log);
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
  flowMgr   = NULL;
  tcpChange = true;
  udpChange = true;
  items.clear();

  tcpFlowOffset = 0;
}

SnoopDataChange::~SnoopDataChange()
{
  close();
}

bool SnoopDataChange::doOpen()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }

  tcpFlowOffset = flowMgr->requestMemory_TcpFlow(this, sizeof(SnoopDataChangeFlowItem));
  flowMgr->connect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
  flowMgr->connect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);

  return SnoopProcess::doOpen();
}

bool SnoopDataChange::doClose()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }

  flowMgr->disconnect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)));
  flowMgr->disconnect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)));

  return SnoopProcess::doClose();
}

void SnoopDataChange::change(SnoopPacket* packet)
{
  bool _changed = false;
  if (packet->ipHdr == NULL) return;

  if (packet->proto == IPPROTO_TCP)
  {
    SnoopDataChangeFlowItem* flowItem = (SnoopDataChangeFlowItem*)(packet->flowValue->totalMem + tcpFlowOffset);
    // LOG_DEBUG("%p %d", flowItem, flowItem->seqDiff); // gilgil temp 2014.03.13

    if (flowItem->seqDiff != 0)
    {
      UINT32 oldSeq = ntohl(packet->tcpHdr->th_seq);
      UINT32 newSeq = oldSeq + flowItem->seqDiff;
      packet->tcpHdr->th_seq = htonl(newSeq);
      packet->ipChanged = true; // gilgil temp 2014.03.13
      packet->tcpChanged = true; // gilgil temp 2014.03.13
    }
    if (flowItem->ackDiff != 0)
    {
      UINT32 oldAck = ntohl(packet->tcpHdr->th_ack);
      UINT32 newAck = oldAck + flowItem->ackDiff;
      packet->tcpHdr->th_ack = htonl(newAck);
      packet->ipChanged = true; // gilgil temp 2014.03.13
      packet->tcpChanged = true; // gilgil temp 2014.03.13
    }

    if (packet->data != NULL)
    {
      BYTE* data = packet->data;
      int len  = packet->dataLen;
      QByteArray ba((const char*)data, (uint)len);
      if (_change(ba))
      {
        int newLen = ba.size();
        int diff   = newLen - len;
        INT16 diff16 = (INT16)diff;
        memcpy(data, ba.constData(), newLen);

        if (newLen != len)
        {
          packet->pktHdr->caplen += (UINT32)diff;
          packet->ipHdr->ip_len = htons(ntohs(packet->ipHdr->ip_len) + (UINT16)diff16);
          flowItem->seqDiff += diff;

          SnoopTcpFlowKey* flowKey = (SnoopTcpFlowKey*)packet->flowKey;
          SnoopTcpFlowKey rflowKey = flowKey->reverse();
          Snoop_TcpFlow_Map::iterator it = flowMgr->tcpFlow_Map.find(rflowKey);
          if (it != flowMgr->tcpFlow_Map.end())
          {
            SnoopFlowValue rvalue = it.value();
            SnoopDataChangeFlowItem* rflowItem = (SnoopDataChangeFlowItem*)(rvalue.totalMem + tcpFlowOffset);
            rflowItem->ackDiff -= diff;
          }
          packet->ipChanged = true; // gilgil temp 2014.03.13
        }
        packet->tcpChanged = true;
        _changed = true;
      }
    }
  } else
  if (packet->proto != IPPROTO_UDP)
  {
    BYTE* data = packet->data;
    int len  = packet->dataLen;
    QByteArray ba((const char*)data, (uint)len);
    if (_change(ba))
    {
      memcpy(data, ba.constData(), (size_t)ba.size());
      packet->udpChanged = true;
      _changed = true;
    }
  }
  if (_changed)
  {
    emit changed(packet);
  }
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
      if (item.log)
      {
        LOG_INFO("changed %s > %s", qPrintable(item.from), qPrintable(item.to));
      }
      _old = ba;
      res = true;
    }
  }
  return res;
}

void SnoopDataChange::__tcpFlowCreate(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  SnoopDataChangeFlowItem* flowItem = (SnoopDataChangeFlowItem*)(value->totalMem + tcpFlowOffset);
  flowItem->seqDiff = 0;
  flowItem->ackDiff = 0;
  // LOG_DEBUG("%p %d", flowItem, flowItem->seqDiff); // gilgil temp 2014.03.13
  SnoopTcpFlowKey rkey = key->reverse();
  Snoop_TcpFlow_Map::iterator it = flowMgr->tcpFlow_Map.find(rkey);
  if (it != flowMgr->tcpFlow_Map.end())
  {
    SnoopFlowValue& rvalue = it.value();
    SnoopDataChangeFlowItem* rflowItem = (SnoopDataChangeFlowItem*)(rvalue.totalMem + tcpFlowOffset);
    flowItem->ackDiff = -rflowItem->seqDiff;
  }
}

void SnoopDataChange::__tcpFlowDelete(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(key)
  Q_UNUSED(value)
  // LOG_DEBUG(""); // gilgil temp 2014.03.13
}

void SnoopDataChange::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString flowMgrName = xml.getStr("flowMgr", "");
  if (flowMgrName != "") flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(flowMgrName));
  tcpChange = xml.getBool("tcpChange", tcpChange);
  udpChange = xml.getBool("udpChange", udpChange);
  items.load(xml.gotoChild("items"));
}

void SnoopDataChange::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString flowMgrName = flowMgr == NULL ? "" : flowMgr->name;
  xml.setStr("flowMgr", flowMgrName);
  xml.setBool("tcpChange", tcpChange);
  xml.setBool("udpChange", udpChange);
  items.save(xml.gotoChild("items"));
}

#ifdef QT_GUI_LIB
#include "snoopdatachangewidget.h"
#include "ui_snoopdatachangewidget.h"
void SnoopDataChange::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  VOptionable::addComboBox(layout, "cbxFlowMgr", "FlowMgr", flowMgrList, -1, flowMgr == NULL ? "" : flowMgr->name);
  VOptionable::addCheckBox(layout, "chkTcpChange", "TCP Change", tcpChange);
  VOptionable::addCheckBox(layout, "chkUdpChange", "UDP Change", udpChange);
  SnoopDataChangeWidget* widget = new SnoopDataChangeWidget(layout->parentWidget());
  widget->setObjectName("snoopDataChangeWidget");
  *(widget->ui->treeWidget) << items;
  layout->addWidget(widget);
}

void SnoopDataChange::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxFlowMgr")->currentText()));
  tcpChange = dialog->findChild<QCheckBox*>("chkTcpChange")->checkState() == Qt::Checked;
  udpChange = dialog->findChild<QCheckBox*>("chkUdpChange")->checkState() == Qt::Checked;
  SnoopDataChangeWidget* widget = dialog->findChild<SnoopDataChangeWidget*>("snoopDataChangeWidget");
  LOG_ASSERT(widget != NULL);
  items << *(widget->ui->treeWidget);
}
#endif // QT_GUI_LIB
