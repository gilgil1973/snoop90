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
  lastId = 0;
  chunks.clear();
}

// ----------------------------------------------------------------------------
// SnoopUdpSender
// ----------------------------------------------------------------------------
SnoopUdpSender::SnoopUdpSender(void* owner) : SnoopProcess(owner)
{
  flowMgr       = NULL;
  writer        = NULL;
  dscr          = "GGGG";
  headerSize    = 0;
  addChunkCount = 1;
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
    return false;
  }

  flowMgr->disconnect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)));
  flowMgr->disconnect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)));

  return SnoopProcess::doClose();
}

void SnoopUdpSender::__udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(key)

  SnoopUdpSenderFlowItem* item = new SnoopUdpSenderFlowItem;
  SnoopUdpSenderFlowItem** p = (SnoopUdpSenderFlowItem**)(value->totalMem + udpFlowOffset);
  *p = item;
}

void SnoopUdpSender::__udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(key)

  SnoopUdpSenderFlowItem** p = (SnoopUdpSenderFlowItem**)(value->totalMem + udpFlowOffset);
  SnoopUdpSenderFlowItem* item = *p;
  delete item;
}

void SnoopUdpSender::merge(SnoopPacket* packet)
{
  if (packet->ipHdr == NULL) return;
  if (packet->udpHdr == NULL) return;
  int dataLen = packet->dataLen; // for abbr
  if (dataLen < headerSize) return;

  if (packet->flowValue == NULL)
  {
    LOG_ERROR("packet->flowValue is null");
    return;
  }
  SnoopUdpSenderFlowItem** p = (SnoopUdpSenderFlowItem**)(packet->flowValue->totalMem + udpFlowOffset);
  SnoopUdpSenderFlowItem* flowItem = *p;

  SnoopUdpChunk newChunk;

  newChunk.info.dscr = this->dscr;
  newChunk.info.id   = flowItem->lastId++;
  newChunk.info.len  = dataLen - headerSize;
  newChunk.payload.header.append((const char*)packet->data, headerSize);
  newChunk.payload.body.append((const char*)packet->data + headerSize, dataLen - headerSize);

  int count = flowItem->chunks.count(); // for abbr
  if (count > 0)
  {
    QByteArray newUdpData;

    for (int i = 0; i < count; i++)
    {
      SnoopUdpChunk& chunk = (SnoopUdpChunk)flowItem->chunks.at(i);
      int udpDataSize = newUdpData.size() +
        chunk.payload.header.size()    + chunk.payload.body.size() +
        newChunk.payload.header.size() + chunk.payload.body.size();
      if (udpDataSize > 1472) // 1472 = MTU_SIZE(1500) - IP_HEADER_SIZE(20) - UDP_HEADER_SIZE(8)
        continue;
      chunk.encode(newUdpData);
    }

    newChunk.encode(newUdpData);
    int newDataLen = newUdpData.length();

    // Packet Header
    packet->pktHdr->caplen = packet->pktHdr->caplen + (newDataLen - dataLen);

    // IP header
    packet->ipHdr->ip_len = htons((u_int16_t)(sizeof(IP_HDR) + sizeof(UDP_HDR) + newDataLen));

    // UDP Header
    packet->udpHdr->uh_ulen = htons((u_int16_t)(sizeof(UDP_HDR) + newDataLen));

    // UDP Data
    BYTE* p = (BYTE*)newUdpData.data();
    memcpy(packet->data, p, newUdpData.length());

    // Checksum
    packet->udpHdr->uh_sum = htons(SnoopUdp::checksum(packet->ipHdr, packet->udpHdr));
    packet->ipHdr->ip_sum  = htons(SnoopIp::checksum(packet->ipHdr));

    // LOG_DEBUG("newUdpDataLen=%d", newUdpData.length()); // gilgil temp 2014.07.30
    emit merged(packet);
  }

  flowItem->chunks.append(newChunk);
  while (flowItem->chunks.count() > addChunkCount)
  {
    flowItem->chunks.removeAt(0);
  }
}

void SnoopUdpSender::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString flowMgrName = xml.getStr("flowMgr", "");
  if (flowMgrName != "") flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(flowMgrName));
  QString writerName = xml.getStr("writer", "");
  if (writerName != "") writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(writerName));
  dscr          = xml.getArr("dscr",          dscr);
  headerSize    = xml.getInt("headerSize",    headerSize);
  addChunkCount = xml.getInt("addChunkCount", addChunkCount);
}

void SnoopUdpSender::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString flowMgrName = flowMgr == NULL ? "" : flowMgr->name;
  xml.setStr("flowMgr", flowMgrName);
  QString writerName = writer == NULL ? "" : writer->name;
  xml.setStr("writer", writerName);
  xml.setArr("dscr",          dscr);
  xml.setInt("headerSize",    headerSize);
  xml.setInt("addChunkCount", addChunkCount);
}

#ifdef QT_GUI_LIB
void SnoopUdpSender::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  VOptionable::addComboBox(layout, "cbxFlowMgr", "FlowMgr", flowMgrList, -1, flowMgr == NULL ? "" : flowMgr->name);

  QStringList writerList = ((VGraph*)owner)->objectList.findNamesByCategoryName("SnoopCapture");
  VOptionable::addComboBox(layout, "cbxWriter", "Writer", writerList, -1, writer == NULL ? "" : writer->name);

  VOptionable::addLineEdit(layout, "leDscr", "Discriminator", dscr);
  VOptionable::addLineEdit(layout, "leHeaderSize",    "Header Size", QString::number(headerSize));
  VOptionable::addLineEdit(layout, "leAddChunkCount", "Add Chunk Count", QString::number(addChunkCount));
}

void SnoopUdpSender::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxFlowMgr")->currentText()));
  writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxWriter")->currentText()));

  dscr          = qPrintable(dialog->findChild<QLineEdit*>("leDscr")->text());
  headerSize    = dialog->findChild<QLineEdit*>("leHeaderSize")->text().toInt();
  addChunkCount = dialog->findChild<QLineEdit*>("leAddChunkCount")->text().toInt();
}
#endif // QT_GUI_LIB
