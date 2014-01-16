#include <math.h>

#define WIN32_LEAN_AND_MEAN
#include <QtGui>

#include "arrow.h"
#include "node.h"
#include "scene.h"

const qreal Pi = 3.14;

//! [0]
Arrow::Arrow(Node *startNode, QString signal, Node *endNode, QString slot) : QGraphicsLineItem(NULL)
{
  myStartNode = startNode;
  myEndNode   = endNode;

  setFlag(QGraphicsItem::ItemIsSelectable, true);
  myColor = Qt::black;
  setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  setZValue(-1000.0);

  connect.sender   = startNode->object->name;
  connect.signal   = signal;
  connect.receiver = endNode->object->name;
  connect.slot     = slot;
}

Arrow::~Arrow()
{
  Scene* scene = (Scene*)this->scene();
  scene->graph->connectList.delConnect(connect);
}
//! [0]

//! [1]
QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath Arrow::shape() const
{
  QPainterPath path = QGraphicsLineItem::shape();
  path.addPolygon(arrowHead);
  return path;
}
//! [2]

//! [3]
void Arrow::updatePosition()
{
  QLineF line(mapFromItem(myStartNode, 0, 0), mapFromItem(myEndNode, 0, 0));
  setLine(line);
}
//! [3]

//! [4]
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
  if (myStartNode->collidesWithItem(myEndNode))
      return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    qreal arrowSize = 10;
    painter->setPen(myPen);
    painter->setBrush(myColor);
//! [4] //! [5]

    // ----- gilgil temp 2012.06.29 -----
    /*
    QLineF centerLine(myStartNode->pos(), myEndNode->pos());
    QPolygonF endPolygon = myEndNode->polygon();
    QPointF p1 = endPolygon.first() + myEndNode->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < endPolygon.count(); ++i) {
    p2 = endPolygon.at(i) + myEndNode->pos();
    polyLine = QLineF(p1, p2);
    QLineF::IntersectType intersectType =
        polyLine.intersect(centerLine, &intersectPoint);
    if (intersectType == QLineF::BoundedIntersection)
        break;
        p1 = p2;
    }
    */
    // ----------------------------------
    QPointF start, end;
    start.setX(myStartNode->pos().x() + myStartNode->boundingRect().width() / 2);
    start.setY(myStartNode->pos().y() + myStartNode->boundingRect().height() / 2);
    end.setX  (myEndNode->pos().x()   + myEndNode->boundingRect().width() / 2);
    end.setY  (myEndNode->pos().y()   + myEndNode->boundingRect().height() / 2);
    QLineF centerLine(start, end);

    QPointF intersectPoint;
    QLineF::IntersectType intersectType;
    QLineF polyLine;

    while (true)
    {
      polyLine = QLineF(myEndNode->sceneBoundingRect().topLeft(), myEndNode->sceneBoundingRect().topRight());
      intersectType = polyLine.intersect(centerLine, &intersectPoint);
      if (intersectType == QLineF::BoundedIntersection) break;

      polyLine = QLineF(myEndNode->sceneBoundingRect().topLeft(), myEndNode->sceneBoundingRect().bottomLeft());
      intersectType = polyLine.intersect(centerLine, &intersectPoint);
      if (intersectType == QLineF::BoundedIntersection) break;

      polyLine = QLineF(myEndNode->sceneBoundingRect().topRight(), myEndNode->sceneBoundingRect().bottomRight());
      intersectType = polyLine.intersect(centerLine, &intersectPoint);
      if (intersectType == QLineF::BoundedIntersection) break;

      polyLine = QLineF(myEndNode->sceneBoundingRect().bottomLeft(), myEndNode->sceneBoundingRect().bottomRight());
      intersectType = polyLine.intersect(centerLine, &intersectPoint);
      if (intersectType == QLineF::BoundedIntersection) break;

      break;
    }

    setLine(QLineF(intersectPoint, start));
//! [5] //! [6]

    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                        cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                        cos(angle + Pi - Pi / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;
//! [6] //! [7]
        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
        if (isSelected()) {
            painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
//! [7]
