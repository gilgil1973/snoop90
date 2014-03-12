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
  udpFlowOffset = 0;
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

  return true;
}

bool SnoopDataChange::doClose()
{

}

void SnoopDataChange::change(SnoopPacket* packet)
{
  bool _changed = false;
  if (packet->ipHdr == NULL) return;
  if (packet->tcpHdr != NULL)
  {
    if (packet->data != NULL)
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
  if (packet->udpHdr != NULL)
  {
    QByteArray ba(packet->data, packet->dataLen);
    if (_change(ba))
    {
      memcpy(packet->data, ba.data(), (size_t)packet->dataLen);
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
#include "snoopdatachangewidget.h"
#include "ui_snoopdatachangewidget.h"
void SnoopDataChange::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

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

  tcpChange = dialog->findChild<QCheckBox*>("chkTcpChange")->checkState() == Qt::Checked;
  udpChange = dialog->findChild<QCheckBox*>("chkUdpChange")->checkState() == Qt::Checked;
  SnoopDataChangeWidget* widget = dialog->findChild<SnoopDataChangeWidget*>("snoopDataChangeWidget");
  LOG_ASSERT(widget != NULL);
  items << *(widget->ui->treeWidget);
}
#endif // QT_GUI_LIB
