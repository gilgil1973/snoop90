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
  policyMap[""]   = false;
  tupleFlowOffset = 0;
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

  tupleFlowOffset = flowMgr->requestMemory_MacFlow(this, sizeof(SnoopProcessFilterItem));
  flowMgr->checkConnect(SIGNAL(tupleFlowCreated(SnoopTupleFlowKey*,SnoopFlowValue*)), this, SLOT(tupleCreate(SnoopTupleFlowKey*,SnoopFlowValue*)), true);
  flowMgr->checkConnect(SIGNAL(tupleFlowDeleted(SnoopTupleFlowKey*,SnoopFlowValue*)), this, SLOT(tupleDelete(SnoopTupleFlowKey*,SnoopFlowValue*)), true);

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
  }
#endif // QT_GUI_LIB
  return true;
}

bool SnoopProcessFilter::doClose()
{
  return true;
}

bool SnoopProcessFilter::getProcessInfo(/*in*/ SnoopTupleFlowKey& tuple, /*out*/ quint32& pid, /*out*/ QString& processName)
{
  pid = SnoopNetStat::instance().getPID(tuple);
  if (pid == SnoopNetStat::UNKNOWN_PROCESS_ID)
  {
    LOG_ERROR("getPID return UNKNOWN_PROCESS_ID(%s %s:%u > %s:%u)",
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

// ----- gilgil temp 2014.03.10 -----
/*
bool SnoopProcessFilter::getACK(SnoopTupleFlowKey& tuple, bool& ack)
{
  VLock lock(tupleMap);
  SnoopProcessTupleMap::iterator it = tupleMap.find(tuple);
  if (it != tupleMap.end())
  {
    SnoopProcessTupleValue& value = it.value();
    ack = value.ack;
    return true;
  }

  SnoopTupleFlowKey reverseTuple;
  reverseTuple.proto         = tuple.proto;
  reverseTuple.flow.srcIp   = tuple.flow.dstIp;
  reverseTuple.flow.srcPort = tuple.flow.dstPort;
  reverseTuple.flow.dstIp   = tuple.flow.srcIp;
  reverseTuple.flow.dstPort = tuple.flow.srcPort;

  it = tupleMap.find(reverseTuple);
  if (it != tupleMap.end())
  {
    SnoopProcessTupleValue& value = it.value();
    ack = value.ack;
    return true;
  }


  ack = pit.value();

  SnoopProcessTupleValue value;
  value.pid = pid;
  value.ack = ack;
  tupleMap[tuple] = value;

  return true;
}
*/
// ----------------------------------

void SnoopProcessFilter::tupleCreate(SnoopTupleFlowKey* key, SnoopFlowValue* value)
{
  SnoopProcessFilterItem* item = (SnoopProcessFilterItem*)(value->totalMem + tupleFlowOffset);
  item->pid = 0;
  item->ack = false;

  QString processName;
  if (!getProcessInfo(*key, item->pid, processName))
  {
    LOG_ERROR("getProcessInfo %u (%s:%d > %s:%d) return false",
      key->proto,
      qPrintable(key->flow.srcIp.str()), key->flow.srcPort,
      qPrintable(key->flow.dstIp.str()), key->flow.dstPort);
    return;
  }

  SnoopProcessPolicyMap::iterator pit = policyMap.find(processName);
  if (pit == policyMap.end())
  {
    pit = policyMap.find(SnoopNetStat::UNKNOWN_PROCESS_NAME);
    if (pit == policyMap.end())
    {
      LOG_ERROR("can not find policy for (%s)", qPrintable(processName));
      return;
    }
    LOG_DEBUG("onNewProcess %s", qPrintable(processName)); // gilgil temp 2012.08.28
#ifdef QT_GUI_LIB
    // myModel->addPolicy(processName, false); // gilgil temp 2014.03.10
    QCoreApplication::postEvent(widget, new QEvent(QEvent::User)); // gilgil temp 2014.03.10
#endif // QT_GUI_LIB
  }
}

void SnoopProcessFilter::tupleDelete(SnoopTupleFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(key)
  Q_UNUSED(value)
}

void SnoopProcessFilter::tupleCheck(SnoopPacket* packet)
{
  SnoopProcessFilterItem* item  = (SnoopProcessFilterItem*)(packet->flowValue->totalMem + tupleFlowOffset);

  if (item->ack)
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
}

void SnoopProcessFilter::save(VXml xml)
{
  SnoopFilter::save(xml);

  QString flowMgrName = flowMgr == NULL ? "" : flowMgr->name;
  xml.setStr("flowMgr", flowMgrName);
  policyMap.save(xml.gotoChild("policies"));
}

#ifdef QT_GUI_LIB
// ----- gilgil temp 2014.03.10 -----
/*
bool SnoopProcessFilter::event(QEvent *)
{
  myModel->initialize();
  return true;
}
*/
// ----------------------------------

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
