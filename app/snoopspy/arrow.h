#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include <VGraph>

class Node;

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class Arrow : public QGraphicsLineItem
{
public:
  enum { Type = UserType + 4 };
  Arrow(Node *startNode, QString signal, Node *endNode, QString slot);
  virtual ~Arrow();

  int type() const { return Type; }
  QRectF boundingRect() const;
  QPainterPath shape() const;
  void setColor(const QColor &color) { myColor = color;    }
  Node *startItem() const            { return myStartNode; }
  Node *endItem() const              { return myEndNode;   }

  void updatePosition();

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
  Node*     myStartNode;
  Node*     myEndNode;
  QColor    myColor;
  QPolygonF arrowHead;

  VGraphConnect connect;
};

#endif
