#include <SnoopUdpReceiver>

REGISTER_METACLASS(SnoopUdpReceiver, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopUdpReceiverFlowItem
// ----------------------------------------------------------------------------
void SnoopUdpReceiverFlowItem::clear()
{
  first  = true;
  lastId = 0;
}

// ----------------------------------------------------------------------------
// SnoopUdpReceiver
// ----------------------------------------------------------------------------
SnoopUdpReceiver::SnoopUdpReceiver(void* owner) : SnoopProcess(owner)
{
  flowMgr          = NULL;
  writer           = NULL;
  dscr             = "GGGG";
  headerSize       = 0;
  autoBlockOnSplit = true;
}

SnoopUdpReceiver::~SnoopUdpReceiver()
{
  close();
}

bool SnoopUdpReceiver::doOpen()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }
  if (writer == NULL)
  {
    SET_ERROR(SnoopError, "writer is null", VERR_OBJECT_IS_NULL);
    return false;
  }

  udpFlowOffset = flowMgr->requestMemory_UdpFlow(this, sizeof(SnoopUdpReceiverFlowItem));
  flowMgr->connect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);
  flowMgr->connect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)), Qt::DirectConnection);

  return SnoopProcess::doOpen();
}

bool SnoopUdpReceiver::doClose()
{
  if (flowMgr == NULL)
  {
    SET_ERROR(SnoopError, "flowMgr is null", VERR_OBJECT_IS_NULL);
    return false;
  }
  if (writer == NULL)
  {
    SET_ERROR(SnoopError, "writer is null", VERR_OBJECT_IS_NULL);
    return false;
  }

  flowMgr->disconnect(SIGNAL(__udpFlowCreated(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowCreate(SnoopUdpFlowKey*,SnoopFlowValue*)));
  flowMgr->disconnect(SIGNAL(__udpFlowDeleted(SnoopUdpFlowKey*,SnoopFlowValue*)), this, SLOT(__udpFlowDelete(SnoopUdpFlowKey*,SnoopFlowValue*)));

  return SnoopProcess::doClose();
}

void SnoopUdpReceiver::__udpFlowCreate(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(key)

  SnoopUdpReceiverFlowItem* item = (SnoopUdpReceiverFlowItem*)(value->totalMem + udpFlowOffset);
  item->clear(); // gilgil temp
}

void SnoopUdpReceiver::__udpFlowDelete(SnoopUdpFlowKey* key, SnoopFlowValue* value)
{
  Q_UNUSED(key)

  SnoopUdpReceiverFlowItem* item = (SnoopUdpReceiverFlowItem*)(value->totalMem + udpFlowOffset);
  item->clear(); // gilgil temp
}

void SnoopUdpReceiver::doSplit(SnoopUdpChunk& chunk, SnoopPacket* packet)
{
  QByteArray newUdpData;

  newUdpData.append(chunk.payload.header);
  newUdpData.append(chunk.payload.body);
  int newDataLen = newUdpData.length();

  // Packet Header
  packet->pktHdr->caplen = sizeof(ETH_HDR) + sizeof(IP_HDR) + sizeof(UDP_HDR) + newDataLen;

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

  LOG_DEBUG("newUdpDataLen=%d", newUdpData.length()); // gilgil temp 2014.07.30
}

void SnoopUdpReceiver::split(SnoopPacket* packet)
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
  SnoopUdpReceiverFlowItem* flowItem = (SnoopUdpReceiverFlowItem*)(packet->flowValue->totalMem + udpFlowOffset);
  // LOG_DEBUG("flowItem->lastId=%d", flowItem->lastId); // gilgil temp 2014.07.31

  QByteArray udpData;
  udpData.append((const char*)packet->data, packet->dataLen);
  // LOG_DEBUG("udpDataLen=%d", udpData.length()); // gilgil temp 2014.07.31

  QList<SnoopUdpChunk> chunks;
  while (true)
  {
    SnoopUdpChunk chunk;
    int decodeLen = chunk.decode(headerSize, udpData);
    if (decodeLen == -1)
    {
      LOG_WARN("chunk.decode return -1");
      return;
    }

    //
    // Check dscr
    //
    if (chunk.info.dscr != this->dscr)
    {
      LOG_WARN("chunk.dscr is %s", chunk.info.dscr.data());
      return;
    }
    chunks.append(chunk);

    if (udpData.length() == 0) break;
  }

  int chunkCount = chunks.count();
  if (chunkCount <= 1) return;

  if (flowItem->first)
  {
    SnoopUdpChunk& chunk = (SnoopUdpChunk)chunks.at(chunkCount - 1);

    doSplit(chunk, packet);
    writer->write(packet);
    emit splitted(packet);

    flowItem->lastId = chunk.info.id;
    flowItem->first = false;
  } else // if not first
  {
    for (int i = 0; i < chunkCount; i++)
    {
      SnoopUdpChunk& chunk = (SnoopUdpChunk)chunks.at(i);

      //
      // Check id
      //
      bool idOk = false;
      static const int MAX_DIFF = 100;
      qint16 diff = (qint16)(chunk.info.id - flowItem->lastId);
      if (diff > 0 && diff < MAX_DIFF)
      {
        idOk = true;
      }
      if (!idOk)
      {
        continue;
      }

      doSplit(chunk, packet);
      writer->write(packet);
      emit splitted(packet);
    }
    flowItem->lastId = chunks.at(chunkCount - 1).info.id;
  }

  if (autoBlockOnSplit) packet->drop = true;
}

void SnoopUdpReceiver::load(VXml xml)
{
  SnoopProcess::load(xml);

  QString flowMgrName = xml.getStr("flowMgr", "");
  if (flowMgrName != "") flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(flowMgrName));
  QString writerName = xml.getStr("writer", "");
  if (writerName != "") writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(writerName));
  dscr             = xml.getArr("dscr",       dscr);
  headerSize       = xml.getInt("headerSize", headerSize);
  autoBlockOnSplit = xml.getBool("autoBlockOnSplit", autoBlockOnSplit);
}

void SnoopUdpReceiver::save(VXml xml)
{
  SnoopProcess::save(xml);

  QString flowMgrName = flowMgr == NULL ? "" : flowMgr->name;
  xml.setStr("flowMgr", flowMgrName);
  QString writerName = writer == NULL ? "" : writer->name;
  xml.setStr("writer", writerName);
  xml.setArr("dscr",       dscr);
  xml.setInt("headerSize", headerSize);
  xml.setBool("autoBlockOnSplit", autoBlockOnSplit);
}

#ifdef QT_GUI_LIB
void SnoopUdpReceiver::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  QStringList flowMgrList = ((VGraph*)owner)->objectList.findNamesByClassName("SnoopFlowMgr");
  VOptionable::addComboBox(layout, "cbxFlowMgr", "FlowMgr", flowMgrList, -1, flowMgr == NULL ? "" : flowMgr->name);

  QStringList writerList = ((VGraph*)owner)->objectList.findNamesByCategoryName("SnoopCapture");
  VOptionable::addComboBox(layout, "cbxWriter", "Writer", writerList, -1, writer == NULL ? "" : writer->name);

  VOptionable::addLineEdit(layout, "leDscr", "Discriminator", dscr);
  VOptionable::addLineEdit(layout, "leHeaderSize",    "Header Size", QString::number(headerSize));
  VOptionable::addCheckBox(layout, "chkAutoBlockOnSplit", "Auto Block On Split", autoBlockOnSplit);
}

void SnoopUdpReceiver::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  flowMgr = (SnoopFlowMgr*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxFlowMgr")->currentText()));
  writer = (SnoopCapture*)(((VGraph*)owner)->objectList.findByName(dialog->findChild<QComboBox*>("cbxWriter")->currentText()));

  dscr             = qPrintable(dialog->findChild<QLineEdit*>("leDscr")->text());
  headerSize       = dialog->findChild<QLineEdit*>("leHeaderSize")->text().toInt();
  autoBlockOnSplit = dialog->findChild<QCheckBox*>("chkAutoBlockOnSplit")->isChecked();
}

#endif // QT_GUI_LIB
