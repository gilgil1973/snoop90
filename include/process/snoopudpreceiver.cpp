#include <SnoopUdpReceiver>

REGISTER_METACLASS(SnoopUdpReceiver, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopUdpReceiver
// ----------------------------------------------------------------------------
SnoopUdpReceiver::SnoopUdpReceiver(void* owner) : SnoopProcess(owner)
{

}

SnoopUdpReceiver::~SnoopUdpReceiver()
{

}

bool SnoopUdpReceiver::doOpen()
{
  return false; // gilgil temp 2015.07.30
}

bool SnoopUdpReceiver::doClose()
{
  return false; // gilgil temp 2015.07.30
}

void SnoopUdpReceiver::split(SnoopPacket* packet)
{

}

void SnoopUdpReceiver::load(VXml xml)
{

}

void SnoopUdpReceiver::save(VXml xml)
{

}

#ifdef QT_GUI_LIB
void SnoopUdpReceiver::optionAddWidget(QLayout* layout)
{

}
void SnoopUdpReceiver::optionSaveDlg(QDialog* dialog)
{

}
#endif // QT_GUI_LIB
