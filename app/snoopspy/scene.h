#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QTreeWidget>
#include <QMessageBox>

#include <VMetaClass>
#include <VLog>

#include "node.h"
#include "arrow.h"
#include "signalslotform.h"

class Scene : public QGraphicsScene, public VXmlable
{
  Q_OBJECT

public:
  Scene(QObject *parent = 0);
  virtual ~Scene();

public:
  void clear();

public:
  enum Mode { InsertItem, MoveItem, InsertLine };
  Mode mode() { return m_mode; }
  void setMode(Mode mode) { m_mode = mode; }

protected:
  Mode m_mode;

protected:
  QTreeWidget* treeWidget;

public:
  QStringList categoryNames;
  QStringList removePrefixNames;
  QStringList removeSignalNames;
  QStringList removeSlotNames;

public: // for treeWidget
  void setTreeWidget(QTreeWidget* treeWidget) { this->treeWidget = treeWidget; }
  void addClass(QString categoryName);
  void addClass(VMetaClass* parentMetaClass, QTreeWidgetItem* parentItem);
  void addClasses();

public:
  VGraph*         graph;
  SignalSlotForm* signalSlotForm;

public:
  QString generateObjectClassName(QString className);
  Node*  createNode(QString className, QString name, bool createObject);
  Arrow* createArrow(Node* startNode, QString signal, Node* endNode, QString slot);
  Arrow* createArrow(QString startNodeName, QString signal, QString endNodeName, QString slot);
  Node*  findNodeByName(QString name);

public: // for file manipulation
  bool newFile(QString& errStr); // gilgil temp 2012.07.27
  bool loadFromFile(QString fileName, QString& errStr); // gilgil temp 2012.07.27
  bool saveToFile(QString fileName, QString& errStr); // gilgil temp 2012.07.27

protected: // for drag and drop
  QGraphicsLineItem *line;

protected:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);
};

#endif // SCENE_H
