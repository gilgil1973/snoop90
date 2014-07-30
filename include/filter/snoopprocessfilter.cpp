#include <SnoopProcessFilter>
#include <SnoopTcp>
#include <SnoopUdp>
#include <SnoopNetStat>
#include <VDebugNew>
#include "snoopprocessfilterwidget.h"

REGISTER_METACLASS(SnoopProcessFilter, SnoopFilter)

// ----------------------------------------------------------------------------
// SnoopProcessPolicyMap
// ----------------------------------------------------------------------------
void SnoopProcessPolicyMap::clear()
{
  QMap<QString,bool>::clear();
  (*this)[""] = false;
}

void SnoopProcessPolicyMap::load(VXml xml)
{
  clear();
  {
    xml_foreach (childXml, xml.childs())
    {
      QString processName  = childXml.getStr("processName", SnoopNetStat::UNKNOWN_PROCESS_NAME);
      bool    ack          = childXml.getBool("ack", false);
      (*this)[processName] = ack;
    }
  }
}

void SnoopProcessPolicyMap::save(VXml xml)
{
  xml.clearChild();
  for (SnoopProcessPolicyMap::iterator it = begin(); it != end(); it++)
  {
    QString& processName = (QString&)it.key();
    bool&    ack         = (bool&)it.value();
    VXml childXml = xml.addChild("policy");
    childXml.setStr("processName", processName);
    childXml.setBool("ack", ack);
  }
}

// ----------------------------------------------------------------------------
// SnoopProcessFilter
// ----------------------------------------------------------------------------
SnoopProcessFilter::SnoopProcessFilter(void* owner) : SnoopFilter(owner)
{
  flowMgr         = NULL;
  policyMap.clear();
  tcpFlowOffset   = 0;
  udpFlowOffset   = 0;
#ifdef QT_GUI_LIB
  showStatus      = true;
  widget          = NULL;
#endif // QT_GUI_LIB
}

SnoopProcessFilter::~SnoopProcessFilter()
{
  close();
#ifdef QT_GUI_LIB
  SAFE_DELETE(widget);
#endif // QT_GUI_LIB
}

#ifdef QT_GUI_LIB
#include <QApplication>
#endif // QT_GUI_LIB
bool SnoopProcessFilter::doOpen()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }

  tcpFlowOffset = flowMgr->requestMemory_TcpFlow(this, sizeof(SnoopProcessFilterFlowItem));
  flowMgr->connect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
  flowMgr->connect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);

  udpFlowOffset = flowMgr->requestMemory_UdpFlow(this, sizeof(SnoopProcessFilterFlowItem)); // gilgil temp 2014.07.30 (unnecessary?)
  flowMgr->connect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
  flowMgr->connect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);

#ifdef QT_GUI_LIB
  if (showStatus)
  {
    QWidget* mainWindow = NULL;
    if (widget == NULL)
    {
      QWidgetList list = QApplication::allWidgets();
      for (int i = 0; i < list.count(); i++)
      {
        QWidget* widget = list.at(i);
        QString className = widget->metaObject()->className();
        if (className == "MainWindow")
        {
          mainWindow = widget;
          break;
        }
      }
      widget = new SnoopProcessFilterWidget(mainWindow, Qt::Window);
      widget->setWindowTitle(name + " Status");
      widget->filter = this;
    }
    widget->show();
    widget->showPolicyMap();
  }
#endif // QT_GUI_LIB

  return SnoopFilter::doOpen();
}

bool SnoopProcessFilter::doClose()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return true;
  }

  flowMgr->disconnect(SIGNAL(__tcpFlowCreated(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowCreate(SnoopTcpFlowKey*,SnoopFlowValue*)));
  flowMgr->disconnect(SIGNAL(__tcpFlowDeleted(SnoopTcpFlowKey*,SnoopFlowValue*)), this, SLOT(__tcpFlowDelete(SnoopTcpFlowKey*,SnoopFlowValue*)));

  udpFlowOffset = flowMgr->requestMemory_UdpFlow(this, sizeof(SnoopProcessFilterFlowItem));
  flowMgr->disconnect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)));
  flowMgr->disconnect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)));

  return SnoopFilter::doClose();
}

bool SnoopProcessFilter::getProcessInfo(/*in*/ SnoopTupleFlowKey& tuple, /*out*/ quint32& pid, /*out*/ QString& processName)
{
  pid = SnoopNetStat::instance().getPID(tuple);
  if (pid == SnoopNetStat::UNKNOWN_PROCESS_ID)
  {
    LOG_DEBUG("getPID return UNKNOWN_PROCESS_ID(%s %s:%u > %s:%u)",
      tuple.proto == IPPROTO_TCP ? "TCP" : "UDP",
      qPrintable(tuple.flow.srcIp.str()), tuple.flow.srcPort,
      qPrintable(tuple.flow.dstIp.str()), tuple.flow.dstPort
    );
    processName = SnoopNetStat::UNKNOWN_PROCESS_NAME;
  } else
  {
    processName = SnoopNetStat::instance().getProcessName(pid);
    if (processName == SnoopNetStat::UNKNOWN_PROCESS_NAME)
    {
      LOG_ERROR("can not find processName for pid(%d)(%s %s:%u > %s:%u)",
        pid,
        tuple.proto == IPPROTO_TCP ? "TCP" : "UDP",
        qPrintable(tuple.flow.srcIp.str()), tuple.flow.srcPort,
        qPrintable(tuple.flow.dstIp.str()), tuple.flow.dstPort
      );
      return false;
    }
  }
  LOG_DEBUG("%s %s:%d > %s:%d pid=%5u name=%s",
    tuple.proto == IPPROTO_TCP ? "TCP" : "UDP",
    qPrintable(tuple.flow.srcIp.str()), tuple.flow.srcPort,
    qPrintable(tuple.flow.dstIp.str()), tuple.flow.dstPort,
    pid, qPrintable(processName)
  );
  return true;
}

void SnoopProcessFilter::__tcpFlowCreate(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  SnoopProcessFilterFlowItem* item = (SnoopProcessFilterFlowItem*)(value->totalMem + tcpFlowOffset);
  SnoopTupleFlowKey tuple;
  tuple.proto = IPPROTO_TCP;
  tuple.flow  = *key;
  _checkProcess(&tuple, item);
}

void SnoopProcessFilter::__tcpFlowDelete(SnoopTcpFlowKey* key, SnoopFlowValue* value)
{
  // LOG_DEBUG(""); // gilgil temp 2014.03.11
  Q_UNUSED(key)
  Q_UNUSED(value)
}

void SnoopProcessFilter::__udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  SnoopProcessFilterFlowItem* item = (SnoopProcessFilterFlowItem*)(value->totalMem + udpFlowOffset);
  SnoopTupleFlowKey tuple;
  tuple.proto = IPPROTO_UDP;
  tuple.flow  = *key;
  _checkProcess(&tuple, item);
}

void SnoopProcessFilter::__udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  // LOG_DEBUG(""); // gilgil temp 2014.03.11
  Q_UNUSED(key)
  Q_UNUSED(value)
}

void SnoopProcessFilter::_checkProcess(SnoopTupleFlowKey* tuple, SnoopProcessFilterFlowItem* flowItem)
{
  flowItem->pid = 0;
  flowItem->ack = false;

  QString processName;
  if (!getProcessInfo(*tuple, flowItem->pid, processName))
  {
    LOG_DEBUG("getProcessInfo %u (%s:%d > %s:%d) return false",
      tuple->proto,
      qPrintable(tuple->flow.srcIp.str()), tuple->flow.srcPort,
      qPrintable(tuple->flow.dstIp.str()), tuple->flow.dstPort);
    return;
  }

  VLock lock(policyMap);
  SnoopProcessPolicyMap::iterator pit = policyMap.find(processName);
  if (pit == policyMap.end())
  {
    LOG_DEBUG("onNewProcess %s", qPrintable(processName));
    pit = policyMap.insert(processName, false);
#ifdef QT_GUI_LIB
    if (widget != NULL) QCoreApplication::postEvent(widget, new QShowPolicyMapEvent);
#endif // QT_GUI_LIB
  }

  bool ack = pit.value();
  flowItem->ack= ack;
}

void SnoopProcessFilter::check(SnoopPacket* packet)
{
  SnoopProcessFilterFlowItem* flowItem;

  if (packet->proto == IPPROTO_TCP)
  {
    if (packet->flowValue == NULL)
    {
      LOG_ERROR("packet->flowValue is null");
      return;
    }
    flowItem  = (SnoopProcessFilterFlowItem*)(packet->flowValue->totalMem + tcpFlowOffset);
  } else
  if (packet->proto == IPPROTO_UDP)
  {
    if (packet->flowValue == NULL)
    {
      LOG_ERROR("packet->flowValue is null");
      return;
    }
    flowItem  = (SnoopProcessFilterFlowItem*)(packet->flowValue->totalMem + udpFlowOffset);
  } else
  {
    emit nak(packet);
    return;
  }

  if (flowItem->ack)
    emit ack(packet);
  else
    emit nak(packet);
}

void SnoopProcessFilter::load(VXml xml)
{
  SnoopFilter::load(xml);

  QString flowMgrName = xml.getStr("flowMgr", "");
  if (flowMgrName != "") flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(flowMgrName));
  policyMap.load(xml.gotoChild("policies"));
#ifdef QT_GUI_LIB
  showStatus = xml.getBool("showStatus", showStatus);
#endif //  QT_GUI_LIB
}

void SnoopProcessFilter::save(VXml xml)
{
  SnoopFilter::save(xml);

  QString flowMgrName = flowMgr == NULL ? "" : flowMgr->name;
  xml.setStr("flowMgr", flowMgrName);
  policyMap.save(xml.gotoChild("policies"));
#ifdef QT_GUI_LIB
  xml.setBool("showStatus", showStatus);
#endif //  QT_GUI_LIB
}

#ifdef QT_GUI_LIB
void SnoopProcessFilter::optionAddWidget(QLayout* layout)
{
  SnoopFilter::optionAddWidget(layout);

  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  VOptionable::addComboBox(layout, "cbxFlowMgr", "FlowMgr", flowMgrList, -1, flowMgr == NULL ? "" : flowMgr->name);
  VOptionable::addCheckBox(layout, "chkShowStatus", "Show Status", showStatus);
}

void SnoopProcessFilter::optionSaveDlg(QDialog* dialog)
{
  SnoopFilter::optionSaveDlg(dialog);

  flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxFlowMgr")->currentText()));
  showStatus = dialog->findChild<QCheckBox*>("chkShowStatus")->checkState() == Qt::Checked;
}
#endif // QT_GUI_LIB
