#ifndef SNOOPPROCESSFILTERWIDGET_H
#define SNOOPPROCESSFILTERWIDGET_H

#ifdef QT_GUI_LIB

#include <QWidget>

namespace Ui {
  class SnoopProcessFilterWidget;
}

class SnoopProcessFilter;
class SnoopProcessFilterWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SnoopProcessFilterWidget(QWidget *parent, Qt::WindowFlags f);
  ~SnoopProcessFilterWidget();

public:
  SnoopProcessFilter* filter; // reference

protected:
  virtual bool event(QEvent* event);

private:
  Ui::SnoopProcessFilterWidget *ui;
};

#endif // QT_GUI_LIB

#endif // SNOOPPROCESSFILTERWIDGET_H
