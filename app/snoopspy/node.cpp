#include "node.h"
#include "scene.h"

#include <VDebugNew>

Node::Node(VObject* object)
{
  // LOG_DEBUG("%p scene()=%p", this, this->scene()); // gilgil temp 2012.07.27
  setFlag(QGraphicsItem::ItemIsMovable);
  setFlag(QGraphicsItem::ItemIsSelectable);
  this->object = object;
  // LOG_DEBUG("scene=%p", scene()); // gilgil temp 2012.07.27
}

Node::~Node()
{
  if (object != NULL)
  {
    Scene* scene = (Scene*)this->scene();
    // LOG_DEBUG("%p scene=%p", this, scene); // gilgil temp 2012.07.27
    scene->graph->objectList.delObject(object);
    object = NULL;
  }
}

void Node::addArrow(Arrow *arrow)
{
  arrows.append(arrow);
}

void Node::removeArrow(Arrow *arrow)
{
  int index = arrows.indexOf(arrow);

  if (index != -1)
    arrows.removeAt(index);
}

void Node::removeArrows()
{
  foreach (Arrow *arrow, arrows)
  {
    arrow->startItem()->removeArrow(arrow);
    arrow->endItem()->removeArrow(arrow);
    // scene()->removeItem(arrow); // gilgil temp 2012.07.27
    delete arrow;
  }
}

/*
QRectF Node::boundingRect() const
{
  LOG_DEBUG("boundingRect");
  ;
  return QRectF(0, 0, this->textWidth(), 50);
}
*/

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->drawRect(boundingRect());
  painter->fillRect(boundingRect(), Qt::gray);
  QGraphicsTextItem::paint(painter, option, widget);
}
