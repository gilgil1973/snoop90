#include <SnoopWriteAdapter>

REGISTER_METACLASS(SnoopWriteAdapter, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopWriteAdapter
// ----------------------------------------------------------------------------
SnoopWriteAdapter::SnoopWriteAdapter(void* owner) : SnoopAdapter(owner)
{
  adapterIndex = snoop::INVALID_ADAPTER_INDEX;
  autoRead = false;
}

SnoopWriteAdapter::~SnoopWriteAdapter()
{
  close();
}

void SnoopWriteAdapter::copy(SnoopPacket* packet)
{
  SnoopAdapter::write(packet->pktData, packet->pktHdr->caplen);
  emit copied(packet);
}

void SnoopWriteAdapter::move(SnoopPacket* packet)
{
  SnoopAdapter::write(packet->pktData, packet->pktHdr->caplen);
  packet->drop = true;
  emit moved(packet);
}
