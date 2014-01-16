#ifndef __VIEW_H__
#define __VIEW_H__

#include <QGraphicsView>

#include <VLog>

class View : public QGraphicsView
{
  Q_OBJECT

public:
  View(QWidget *parent = NULL);
  virtual ~View();

protected:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void dropEvent(QDropEvent *event);
};

#endif // __VIEW_H__
