#include <SnoopFile>

#include <VFile> // for VERR_FILENAME_NOT_SPECIFIED
#include <VDebugNew>

REGISTER_METACLASS(SnoopFile, SnoopCapture)

// ----------------------------------------------------------------------------
// SnoopFile
// ----------------------------------------------------------------------------
SnoopFile::SnoopFile(void* owner) : SnoopPcap(owner)
{
  fileName  = "";
  speed     = 0;
  startTS   = 0;
  startTick = 0;
}

SnoopFile::~SnoopFile()
{
  close();
}

bool SnoopFile::doOpen()
{
  if (!enabled)
  {
    LOG_DEBUG("enabled is false");
    return true;
  }
  
  if (fileName == "")
  {
    SET_ERROR(VFileError, "file name not specified", VERR_FILENAME_NOT_SPECIFIED);
    return false;
  }
  if (!QFile::exists(fileName))
  {
    SET_ERROR(VFileError, qformat("file(%s) not exist", qPrintable(fileName)), VERR_FILE_NOT_EXIST);
    return false;
  }
  QString source = "file://" + fileName;
  if (!pcapOpen((char*)(qPrintable(source)), NULL, NULL))
  {
    return false;
  }

  if (speed != 0)
  {
    startTS = 0;
    startTick = tick();
  }

  return SnoopPcap::doOpen();
}

bool SnoopFile::doClose()
{
  if (!enabled)
  {
    LOG_DEBUG("enabled is false");
    return true;
  }
  
  return SnoopPcap::doClose();
}

int SnoopFile::read(SnoopPacket* packet)
{
  int res = SnoopPcap::read(packet);
  if (res < 0) return res;

  if (speed != 0)
  {
    long nowTS = packet->pktHdr->ts.tv_sec * 1000 + packet->pktHdr->ts.tv_usec / 1000;
    if (startTS == 0) startTS = nowTS;
    long elapsedTS = nowTS - startTS;
    while (true)
    {
      VTick nowTick     = tick();
      VTick elapsedTick = nowTick - startTick;
      if (elapsedTick * speed >= elapsedTS) break;
      msleep(1);
    }
  }
  return res;
}

void SnoopFile::load(VXml xml)
{
  SnoopPcap::load(xml);

  fileName = xml.getStr("fileName", fileName);
  speed    = xml.getDouble("speed", speed);
}

void SnoopFile::save(VXml xml)
{
  SnoopPcap::save(xml);

  xml.setStr("fileName", fileName);
  xml.setDouble("speed", speed);
}

#ifdef QT_GUI_LIB
void SnoopFile::optionAddWidget(QLayout *layout)
{
  SnoopPcap::optionAddWidget(layout);

  VOptionable::addLineEdit(layout, "leFileName", "File Name", fileName);
  VOptionable::addLineEdit(layout, "leSpeed",    "Speed",     QString::number(speed));
}

void SnoopFile::optionSaveDlg(QDialog *dialog)
{
  SnoopPcap::optionSaveDlg(dialog);

  fileName = dialog->findChild<QLineEdit*>("leFileName")->text();
  speed    = dialog->findChild<QLineEdit*>("leSpeed")->text().toDouble();
}
#endif // QT_GUI_LIB
