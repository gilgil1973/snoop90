#include <SnoopUdpSender>

REGISTER_METACLASS(SnoopUdpSender, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopUdpSender
// ----------------------------------------------------------------------------
SnoopUdpSender::SnoopUdpSender(void* owner) : SnoopProcess(owner)
{
}

SnoopUdpSender::~SnoopUdpSender()
{

}

bool SnoopUdpSender::doOpen()
{
  return false; // gilgil temp 2015.07.30
}

bool SnoopUdpSender::doClose()
{
  return false; // gilgil temp 2015.07.30
}

void SnoopUdpSender::merge(SnoopPacket* packet)
{

}

void SnoopUdpSender::load(VXml xml)
{

}

void SnoopUdpSender::save(VXml xml)
{

}

#ifdef QT_GUI_LIB
void SnoopUdpSender::optionAddWidget(QLayout* layout)
{

}
void SnoopUdpSender::optionSaveDlg(QDialog* dialog)
{

}
#endif // QT_GUI_LIB
