#include <SnoopCommand>
#include <VDebugNew>

REGISTER_METACLASS(SnoopCommand, SnoopProcess)

// ----------------------------------------------------------------------------
// SnoopCommandItem
// ----------------------------------------------------------------------------
SnoopCommandItem::SnoopCommandItem()
{
  enabled = true;
  command = "";
  sync    = "";
  process = NULL;
}

SnoopCommandItem::~SnoopCommandItem()
{
  if (process != NULL)
  {
    delete process;
    process = NULL;
  }
}

void SnoopCommandItem::load(VXml xml)
{
  enabled = xml.getBool("enabled", enabled);
  command = xml.getStr("command",  command);
  sync    = xml.getBool("sync",    sync);
}

void SnoopCommandItem::save(VXml xml)
{
  xml.setBool("enabled", enabled);
  xml.setStr("command",  command);
  xml.setBool("sync",    sync);
}

bool SnoopCommandItem::execute()
{
  if (!enabled) return true;
  if (process == NULL) process = new QProcess;
  process->start(command);
  if (!process->waitForStarted())
  {
    LOG_ERROR("process->waitForStarted(%s) return false", qPrintable(command));
    return false;
  }

  if (sync)
  {
    if (!process->waitForFinished())
    {
      LOG_ERROR("process->waitForFinished(%s) return false", qPrintable(command));
      return false;
    }
  }
  return true;
}

// ----------------------------------------------------------------------------
// SnoopCommandItems
// ----------------------------------------------------------------------------
bool SnoopCommandItems::execute()
{
  for (int i = 0; i < this->count(); i++)
  {
    SnoopCommandItem& item = (SnoopCommandItem&)this->at(i);
    if (!item.execute()) return false;
  }
  return true;
}

void SnoopCommandItems::load(VXml xml)
{
  clear();
  xml_foreach (childXml, xml.childs())
  {
    SnoopCommandItem item;
    item.load(childXml);
    push_back(item);
  }
}

void SnoopCommandItems::save(VXml xml)
{
  xml.clearChild();
  for (int i = 0; i < this->count(); i++)
  {
    SnoopCommandItem& item = (SnoopCommandItem&)this->at(i);
    item.save(xml.gotoChild("command" + QString::number(i++)));
  }
}

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

  if (!openCommands.execute())
  {
    SET_ERROR(VError, "openCommands.execute() return false", VERR_RUN_PROCESS);
    return false;
  }

  return true;
}

bool SnoopCommand::doClose()
{
  if (!closeCommands.execute())
  {
    SET_ERROR(VError, "closeCommands.execute() return false", VERR_RUN_PROCESS);
    return false;
  }

  return SnoopProcess::doClose();
}

void SnoopCommand::load(VXml xml)
{
  SnoopProcess::load(xml);

  if (!xml.findChild("open").isNull())  openCommands.load(xml.gotoChild("open"));
  if (!xml.findChild("close").isNull()) closeCommands.load(xml.gotoChild("close"));
}

void SnoopCommand::save(VXml xml)
{
  SnoopProcess::save(xml);

  openCommands.save(xml.gotoChild("open"));
  closeCommands.save(xml.gotoChild("close"));
}

#ifdef QT_GUI_LIB
#include "snoopcommandwidget.h"
#include "ui_snoopcommandwidget.h"
void SnoopCommand::optionAddWidget(QLayout* layout)
{
  SnoopProcess::optionAddWidget(layout);

  SnoopCommandWidget* widget = new SnoopCommandWidget(layout->parentWidget());
  widget->setObjectName("snoopCommandWidget");
  *(widget->ui->tvOpen)  << openCommands;
  *(widget->ui->tvClose) << closeCommands;
  layout->addWidget(widget);
}

void SnoopCommand::optionSaveDlg(QDialog* dialog)
{
  SnoopProcess::optionSaveDlg(dialog);

  SnoopCommandWidget* widget = dialog->findChild<SnoopCommandWidget*>("snoopCommandWidget");
  LOG_ASSERT(widget != NULL);
  openCommands  << *(widget->ui->tvOpen);
  closeCommands << *(widget->ui->tvClose);
}
#endif // QT_GUI_LIB
