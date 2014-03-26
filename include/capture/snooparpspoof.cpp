#include <SnoopArpSpoof>
#include <VDebugNew>

REGISTER_METACLASS(SnoopArpSpoof, SnoopCapture)

// ----------------------------------------------------------------------------
// SnoopArpSpoof
// ----------------------------------------------------------------------------
SnoopArpSpoof::SnoopArpSpoof(void* owner) : SnoopAdapter(owner)
{

}

SnoopArpSpoof::~SnoopArpSpoof()
{
  close();
}

bool SnoopArpSpoof::doOpen()
{
  return SnoopAdapter::doOpen();
}

bool SnoopArpSpoof::doClose()
{

  return SnoopAdapter::doClose();
}

int SnoopArpSpoof::read(SnoopPacket* packet)
{
  int readLen = SnoopAdapter::read(packet);
  if (readLen <= 0) return readLen;
  return VERR_FAIL; // gilgil temp 2014.03.27
}

int SnoopArpSpoof::write(SnoopPacket* packet)
{
  return SnoopAdapter::write(packet); // gilgil temp 2014.03.27
}

int SnoopArpSpoof::write(u_char* buf, int size, WINDIVERT_ADDRESS* divertAddr)
{
  return SnoopAdapter::write(buf, size, divertAddr); // gilgil temp 2014.03.27
}

bool SnoopArpSpoof::relay(SnoopPacket* packet)
{
  Q_UNUSED(packet)
  return false; // gilgil temp 2014.03.27
}

void SnoopArpSpoof::process(SnoopPacket* packet)
{
  Q_UNUSED(packet)
  return; // gilgil temp 2014.03.27
}

void SnoopArpSpoof::load(VXml xml)
{
  SnoopAdapter::load(xml);

  // gilgil temp 2014.03.27
}

void SnoopArpSpoof::save(VXml xml)
{
  SnoopAdapter::save(xml);

  // gilgil temp 2014.03.27
}

#ifdef QT_GUI_LIB
void SnoopArpSpoof::optionAddWidget(QLayout* layout)
{
  SnoopAdapter::optionAddWidget(layout);

  // gilgil temp 2014.03.27
}

void SnoopArpSpoof::optionSaveDlg(QDialog* dialog)
{
  SnoopAdapter::optionSaveDlg(dialog);

  // gilgil temp 2014.03.27
}
#endif // QT_GUI_LIB
