#include "view.h"

View::View(QWidget *parent) : QGraphicsView(parent)
{
  setRenderHint(QPainter::Antialiasing);
  setAcceptDrops(true);
}

View::~View()
{
}

void View::mousePressEvent(QMouseEvent *event)
{
  // LOG_DEBUG("View::mousePressEvent"); // gilgil temp 2014.03.11
  QGraphicsView::mousePressEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
  // LOG_DEBUG("View::mouseMoveEvent"); // gilgil temp 2012.07.08
  QGraphicsView::mouseMoveEvent(event);
}

void View::dropEvent(QDropEvent *event)
{
  // LOG_DEBUG("View::dropEvent"); // gilgil temp 2014.03.11
  QGraphicsView::dropEvent(event);
}
