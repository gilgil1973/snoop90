#ifdef QT_GUI_LIB

#include "snoopprocessfilterwidget.h"
#include "ui_snoopprocessfilterwidget.h"
#include <SnoopProcessFilter>

SnoopProcessFilterWidget::SnoopProcessFilterWidget(QWidget *parent, Qt::WindowFlags f) :
  QWidget(parent, f),
  ui(new Ui::SnoopProcessFilterWidget)
{
  ui->setupUi(this);
}

SnoopProcessFilterWidget::~SnoopProcessFilterWidget()
{
  delete ui;
}

bool SnoopProcessFilterWidget::event(QEvent* event)
{
  if (event->type() == QEvent::User)
  {
    LOG_DEBUG(""); // gilgil temp 2014.03.11
    return true;
  }
  return QWidget::event(event);
}

#endif // QT_GUI_LIB
