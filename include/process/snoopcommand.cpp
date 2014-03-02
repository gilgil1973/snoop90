#include <SnoopCommand>

REGISTER_METACLASS(SnoopCommand, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopCommand
// ----------------------------------------------------------------------------
SnoopCommand::SnoopCommand(void* owner) : SnoopProcess(owner)
{
  openCommands.clear();
  closeCommands.clear();
}

SnoopCommand::~SnoopCommand()
{
  close();
}

bool SnoopCommand::doOpen()
{
  if (!SnoopProcess::doOpen()) return false;

  foreach (QString command, openCommands)
  {
    QProcess process;
    int res  = process.execute(command);
    LOG_DEBUG("execute(%s) return %d", qPrintable(command), res);
    if (res == -2)
    {
      SET_ERROR(VError, qformat("Can not execute(%s)", qPrintable(command)),VERR_RUN_PROCESS);
      return false;
    }
  }
  return true;
}

bool SnoopCommand::doClose()
{
  // gilgil temp 2014.03.02

  return SnoopProcess::doClose();
}

void SnoopCommand::load(VXml xml)
{
  SnoopProcess::load(xml);

  {
    openCommands.clear();
    VXml openXml = xml.gotoChild("openCommands");
    int count = openXml.childs().count();
    for (int i = 0; i < count; i++)
    {
      QString command = openXml.gotoChild("item" + QString::number(i)).getStr("command");
      openCommands.push_back(command);
    }
  }
}

void SnoopCommand::save(VXml xml)
{
  SnoopProcess::save(xml);

  {
    VXml openXml = xml.gotoChild("openCommands");
    openXml.clearChild();
    openCommands.push_back("ping 8.8.8.8"); // gilgil temp 2014.03.02
    int count = openCommands.count();
    for (int i = 0; i < count; i++)
    {
      QString command = openCommands.at(i);
      openXml.gotoChild("item" + QString::number(i)).setStr("command", command);
    }
  }
}

#ifdef QT_GUI_LIB
void SnoopCommand::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  // gilgil temp 2014.03.02
}

void SnoopCommand::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  // gilgil temp 2014.03.02
}
#endif // QT_GUI_LIB
