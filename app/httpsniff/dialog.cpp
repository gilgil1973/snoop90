#include "dialog.h"
#include "ui_dialog.h"

// ----------------------------------------------------------------------------
// Dialog
// ----------------------------------------------------------------------------
Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog)
{
  ui->setupUi(this);
  initializeControl();
  loadControl();
  setControl();
}

Dialog::~Dialog()
{
  saveControl();
  finalizeControl();
  delete ui;
}


void Dialog::initializeControl()
{
  move(0, 0); resize(640, 480);
}

void Dialog::finalizeControl()
{

}

void Dialog::loadControl()
{
  this->loadFromDefaultDoc("Dialog");
}

void Dialog::saveControl()
{
  this->saveToDefaultDoc("Dialog");
}

void Dialog::setControl()
{

}

void Dialog::load(VXml xml)
{
  {
    VXml coordXml = xml.findChild("coord");
    if (!coordXml.isNull())
    {
      QRect rect = geometry();
      rect.setLeft  ((coordXml.getInt("left",   0)));
      rect.setTop   ((coordXml.getInt("top",    0)));
      rect.setWidth ((coordXml.getInt("width",  640)));
      rect.setHeight((coordXml.getInt("height", 480)));
      setGeometry(rect);
    }
  }

  ui->tabWidget->setCurrentIndex(xml.getInt("tabCurrentIndex", 0));
}

void Dialog::save(VXml xml)
{
  {
    VXml coordXml = xml.gotoChild("coord");
    QRect rect = geometry();
    coordXml.setInt("left",   rect.left());
    coordXml.setInt("top",    rect.top());
    coordXml.setInt("width",  rect.width());
    coordXml.setInt("height", rect.height());
  }

  xml.setInt("tabCurrentIndex", ui->tabWidget->currentIndex());
}
