#ifndef SNOOPPROCESSFILTERWIDGET_H
#define SNOOPPROCESSFILTERWIDGET_H

#ifdef QT_GUI_LIB

#include <QWidget>
#include <QEvent>

// ----------------------------------------------------------------------------
// SnoopProcessFilterWidget
// ----------------------------------------------------------------------------
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

public:
  void showPolicyMap();
  void setPolicyMap();

private slots:
  void on_pbClear_clicked();
  void on_treeWidget_clicked(const QModelIndex &index);

private:
  Ui::SnoopProcessFilterWidget *ui;

};

// ----------------------------------------------------------------------------
// QShowPolicyMapEvent
// ----------------------------------------------------------------------------
class QShowPolicyMapEvent : public QEvent
{
public:
  QShowPolicyMapEvent();
};

#endif // QT_GUI_LIB

#endif // SNOOPPROCESSFILTERWIDGET_H
