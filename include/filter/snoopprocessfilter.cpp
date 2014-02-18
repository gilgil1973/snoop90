#include <SnoopProcessFilter>
#include <SnoopTcp>
#include <SnoopUdp>
#include <SnoopNetStat>
#include <VDebugNew>

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

#ifdef QT_GUI_LIB
// ----------------------------------------------------------------------------
// SnoopProcessFilterModel
// ----------------------------------------------------------------------------
void SnoopProcessFilterModel::initialize()
{
  QStringList strList;
  for (SnoopProcessPolicyMap::iterator it = policyMap->begin(); it != policyMap->end(); it++)
  {
    strList.push_back(it.key());
  }
  this->setStringList(strList);
}

void SnoopProcessFilterModel::addPolicy(QString processName, bool ack)
{
  QStringList strList = this->stringList();
  if (strList.contains(processName))
  {
    LOG_ERROR("%s alread exists", qPrintable(processName));
    return;
  }
  (*policyMap)[processName] = ack;
  this->setStringList(strList);
}

QVariant SnoopProcessFilterModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::CheckStateRole)
  {
    QString processName = index.data().toString();
    SnoopProcessPolicyMap::iterator it = policyMap->find(processName);
    if (it == policyMap->end()) return QVariant();
    return it.value();
  }
  return QStringListModel::data(index, role);
}

bool SnoopProcessFilterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (role == Qt::CheckStateRole)
  {
    QString processName = index.data().toString();
    SnoopProcessPolicyMap::iterator it = policyMap->find(processName);
    if (it == policyMap->end()) return false;
    (*policyMap)[processName] = !it.value();
    return true;
  }
  return QStringListModel::setData(index, value, role);
}

Qt::ItemFlags SnoopProcessFilterModel::flags(const QModelIndex &index) const
{
  if (!index.isValid()) return 0;
  switch (index.column())
  {
    case 0: return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
     default: return Qt::NoItemFlags;
  }
}
#endif // QT_GUI_LIB

REGISTER_METACLASS(SnoopProcessFilter, SnoopFilter)

// ----------------------------------------------------------------------------
// SnoopProcessFilter
// ----------------------------------------------------------------------------
SnoopProcessFilter::SnoopProcessFilter(void* owner) : SnoopFilter(owner)
{
  policyMap[""] = false;
#ifdef QT_GUI_LIB
  myModel = new SnoopProcessFilterModel;
  myModel->policyMap = &policyMap;
#endif // QT_GUI_LIB
}

SnoopProcessFilter::~SnoopProcessFilter()
{
  close();
#ifdef QT_GUI_LIB
  SAFE_DELETE(myModel);
#endif // QT_GUI_LIB
}

bool SnoopProcessFilter::doOpen()
{
  this->owner;
  VLock lock(tupleMap);
  tupleMap.clear();
  return true;
}

bool SnoopProcessFilter::doClose()
{
  VLock lock(tupleMap);
  tupleMap.clear();
  return true;
}

bool SnoopProcessFilter::getProcessInfo(/*in*/ SnoopTupleKey& tuple, /*out*/ quint32& pid, /*out*/ QString& processName)
{
  pid = SnoopNetStat::instance().getPID(tuple);
  if (pid == SnoopNetStat::UNKNOWN_PROCESS_ID)
  {
    LOG_ERROR("getPID return UNKNOWN_PROCESS_ID");
    processName = SnoopNetStat::UNKNOWN_PROCESS_NAME;
  } else
  {
    processName = SnoopNetStat::instance().getProcessName(pid);
    if (processName == SnoopNetStat::UNKNOWN_PROCESS_NAME)
    {
      LOG_ERROR("can not find processName for pid(%d) is", pid);
      return false;
    }
  }
  LOG_DEBUG("%s %s:%d > %s:%d pid=%5u name=%s",
    tuple.proto == IPPROTO_TCP ? "TCP" : "UDP",
    qPrintable(tuple.flow.src_ip.str()), tuple.flow.src_port,
    qPrintable(tuple.flow.dst_ip.str()), tuple.flow.dst_port,
    pid, qPrintable(processName)
  );
  return true; // gilgil temp 2012.06.11
}

bool SnoopProcessFilter::getACK(/*in*/ SnoopTupleKey& tuple, /*out*/ bool& ack)
{
  VLock lock(tupleMap);
  SnoopProcessTupleMap::iterator it = tupleMap.find(tuple);
  if (it != tupleMap.end())
  {
    SnoopProcessTupleValue& value = it.value();
    ack = value.ack;
    return true;
  }

  SnoopTupleKey reverseTuple;
  reverseTuple.proto         = tuple.proto;
  reverseTuple.flow.src_ip   = tuple.flow.dst_ip;
  reverseTuple.flow.src_port = tuple.flow.dst_port;
  reverseTuple.flow.dst_ip   = tuple.flow.src_ip;
  reverseTuple.flow.dst_port = tuple.flow.src_port;

  it = tupleMap.find(reverseTuple);
  if (it != tupleMap.end())
  {
    SnoopProcessTupleValue& value = it.value();
    ack = value.ack;
    return true;
  }

  quint32 pid;
  QString processName;
  if (!getProcessInfo(tuple, pid, processName))
  {
    LOG_ERROR("getProcessInfo %u (%s:%d > %s:%d) return false",
      tuple.proto,
      qPrintable(tuple.flow.src_ip.str()), tuple.flow.src_port,
      qPrintable(tuple.flow.dst_ip.str()), tuple.flow.dst_port);
    return false;
  }

  SnoopProcessPolicyMap::iterator pit = policyMap.find(processName);
  if (pit == policyMap.end())
  {
    pit = policyMap.find(SnoopNetStat::UNKNOWN_PROCESS_NAME);
    if (pit == policyMap.end())
    {
      LOG_ERROR("can not find policy for (%s)", qPrintable(processName));
      return false;
    }
    LOG_DEBUG("onNewProcess %s", qPrintable(processName)); // gilgil temp 2012.08.28
#ifdef QT_GUI_LIB
    myModel->addPolicy(processName, false);
#endif // QT_GUI_LIB
    QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    emit onNewProcess(processName);
  }
  ack = pit.value();

  SnoopProcessTupleValue value;
  value.pid = pid;
  value.ack = ack;
  tupleMap[tuple] = value;

  return true;
}

void SnoopProcessFilter::check(SnoopPacket* packet)
{
  SnoopTupleKey tuple;
  if (!SnoopIp::parseAll(packet))
  {
    emit nak(packet);
    return;
  }
  // ----- gilgil temp 2012.09.11 -----
  /*
  if (packet->ethHdr->ether_dhost.isBroadcast() || packet->ethHdr->ether_dhost.isMulticast())
  {
    emit nak(packet);
    return;
  }
  */
  // ----------------------------------
  if (SnoopTcp::parseAll(packet))
  {
    tuple.proto         = IPPROTO_TCP;
    tuple.flow.src_ip   = ntohl(packet->ipHdr->ip_src);
    tuple.flow.src_port = ntohs(packet->tcpHdr->th_sport);
    tuple.flow.dst_ip   = ntohl(packet->ipHdr->ip_dst);
    tuple.flow.dst_port = ntohs(packet->tcpHdr->th_dport);
  } else
  if (SnoopUdp::parseAll(packet))
  {
    tuple.proto         = IPPROTO_UDP;
    tuple.flow.src_ip   = ntohl(packet->ipHdr->ip_src);
    tuple.flow.src_port = ntohs(packet->udpHdr->uh_sport);
    tuple.flow.dst_ip   = ntohl(packet->ipHdr->ip_dst);
    tuple.flow.dst_port = ntohs(packet->udpHdr->uh_dport);
    // ----- gilgil temp 2012.09.11 -----
    /*
    if (tuple.flow.dst_ip.isBroadcast() || tuple.flow.dst_ip.isMulticast())
    {
      emit nak(packet);
      return;
    }
    */
    // ----------------------------------
  } else
  {
    emit nak(packet);
    return;
  }
  bool _ack = false;
  bool res  = getACK(tuple, _ack);
  if (!res)
  {
    LOG_ERROR("check return false");
    return;
  }
  if (_ack)
    emit ack(packet);
  else
    emit nak(packet);
}

void SnoopProcessFilter::load(VXml xml)
{
  SnoopFilter::load(xml);

  policyMap.load(xml.gotoChild("policies"));
}

void SnoopProcessFilter::save(VXml xml)
{
  SnoopFilter::save(xml);

  policyMap.save(xml.gotoChild("policies"));
}

#ifdef QT_GUI_LIB
bool SnoopProcessFilter::event(QEvent *)
{
  myModel->initialize();
  return true;
}

#include <QListView>
void SnoopProcessFilter::addOptionWidget(QLayout *layout)
{
  SnoopFilter::addOptionWidget(layout);
}

bool SnoopProcessFilter::showOptionDlg(QDialog* dialog)
{
  QListView* listView = new QListView(dialog);
  listView->setModel(myModel);
  myModel->initialize();
  dialog->layout()->addWidget(listView);
  dialog->show();
  return true;
}

void SnoopProcessFilter::saveOptionDlg(QDialog *dialog)
{
  SnoopFilter::saveOptionDlg(dialog);
}
#endif // QT_GUI_LIB
