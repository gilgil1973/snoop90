#include <SnoopUdpSender>

REGISTER_METACLASS(SnoopUdpSender, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopUdpSenderFlowItem
// ----------------------------------------------------------------------------
SnoopUdpSenderFlowItem::SnoopUdpSenderFlowItem()
{
  clear();
}

SnoopUdpSenderFlowItem::~SnoopUdpSenderFlowItem()
{
  clear();
}

void SnoopUdpSenderFlowItem::clear()
{
  count = 0;
  bufferList.clear();
}

// ----------------------------------------------------------------------------
// SnoopUdpSender
// ----------------------------------------------------------------------------
SnoopUdpSender::SnoopUdpSender(void* owner) : SnoopProcess(owner)
{
  flowMgr        = NULL;
  writer         = NULL;
  discriminator  = "GGGG";
  maxBufferCount = 10;
}

SnoopUdpSender::~SnoopUdpSender()
{
  close();
}

bool SnoopUdpSender::doOpen()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }

  udpFlowOffset = flowMgr->requestMemory_UdpFlow(this, sizeof(SnoopUdpSenderFlowItem*));
  flowMgr->connect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
  flowMgr->connect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);

  return SnoopProcess::doOpen();
}

bool SnoopUdpSender::doClose()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return true;
  }

  flowMgr->disconnect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)));
  flowMgr->disconnect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)));

  return SnoopProcess::doClose();
}

void SnoopUdpSender::__udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  SnoopUdpSenderFlowItem* item = new SnoopUdpSenderFlowItem;
  SnoopUdpSenderFlowItem** p = (SnoopUdpSenderFlowItem**)(value->totalMem + udpFlowOffset);
  *p = item;
}

void SnoopUdpSender::__udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  SnoopUdpSenderFlowItem** p = (SnoopUdpSenderFlowItem**)(value->totalMem + udpFlowOffset);
  SnoopUdpSenderFlowItem* item = *p;
  delete item;
}

void SnoopUdpSender::merge(SnoopPacket* packet)
{
  if (packet->ipHdr == NULL) return;
  if (packet->udpHdr == NULL) return;
  if (packet->dataLen < 2) return;

  if (packet->flowValue == NULL)
  {
    LOG_ERROR("packet->flowValue is null");
    return;
  }
  SnoopUdpSenderFlowItem** p = (SnoopUdpSenderFlowItem**)(packet->flowValue->totalMem + udpFlowOffset);
  SnoopUdpSenderFlowItem* flowItem = *p;
  if (flowItem->count++ % 2 == 0)
  {
    for (int i = 20; i < packet->dataLen; i++)
    {
      packet->data[i] = 'G';
    }
    packet->udpHdr->uh_sum = htons(SnoopUdp::checksum(packet->ipHdr, packet->udpHdr));
    LOG_DEBUG("merge"); // gilgil temp
  }
}

void SnoopUdpSender::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString flowMgrName = xml.getStr("flowMgr", "");
  if (flowMgrName != "") flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(flowMgrName));
  QString writerName = xml.getStr("writer", "");
  if (writerName != "") writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(writerName));
  discriminator  = xml.getStr("discriminator", discriminator);
  maxBufferCount = xml.getInt("maxBufferCount", maxBufferCount);
}

void SnoopUdpSender::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString flowMgrName = flowMgr == NULL ? "" : flowMgr->name;
  xml.setStr("flowMgr", flowMgrName);
  QString writerName = writer == NULL ? "" : writer->name;
  xml.setStr("writer", writerName);
  xml.setStr("discriminator", discriminator);
  xml.setInt("maxBufferCount", maxBufferCount);
}

#ifdef QT_GUI_LIB
void SnoopUdpSender::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  VOptionable::addComboBox(layout, "cbxFlowMgr", "FlowMgr", flowMgrList, -1, flowMgr == NULL ? "" : flowMgr->name);

  QStringList writerList = ((VGraph*)owner)->objectList.findNamesByCategoryName("SnoopCapture");
  VOptionable::addComboBox(layout, "cbxWriter", "Writer", writerList, -1, writer == NULL ? "" : writer->name);

  VOptionable::addLineEdit(layout, "leDiscriminator", "Discriminator", discriminator);
  VOptionable::addLineEdit(layout, "leMaxBufferCount", "Max Buffer Count", QString::number(maxBufferCount));
}

void SnoopUdpSender::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxFlowMgr")->currentText()));
  writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxWriter")->currentText()));

  discriminator = dialog->findChild<QLineEdit*>("leDiscriminator")->text();
  maxBufferCount = dialog->findChild<QLineEdit*>("leDiscriminator")->text().toInt();
}
#endif // QT_GUI_LIB
