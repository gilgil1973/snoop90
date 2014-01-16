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

void SnoopWriteAdapter::output(SnoopPacket* packet)
{
  SnoopAdapter::write(packet->pktData, packet->pktHdr->caplen);
  emit captured(packet);
}
