#include "scene.h"
#include "ui_signalslotform.h"

Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
  m_mode         = MoveItem;
  graph          = new VGraph;
  signalSlotForm = NULL;
  line           = NULL;

  // categoryNames.push_back("VMyObject"); // gilgil temp 2012.08.02
  categoryNames.push_back("SnoopCapture");
  categoryNames.push_back("SnoopFilter");
  categoryNames.push_back("SnoopProcess");
  categoryNames.push_back("VNet");

  removePrefixNames.push_back("VMy");
  removePrefixNames.push_back("Snoop");
  removePrefixNames.push_back("V");

  removeSignalNames.push_back("destroyed(QObject*)");
  removeSignalNames.push_back("destroyed()");
  removeSignalNames.push_back("objectNameChanged(QString)");
  removeSignalNames.push_back("opened()");
  removeSignalNames.push_back("closed()");

  removeSlotNames.push_back("deleteLater()");
  removeSlotNames.push_back("_q_reregisterTimers(void*)");
  removeSlotNames.push_back("open()");
  removeSlotNames.push_back("close()");
}

Scene::~Scene()
{
  while (items().count() > 0)
  {
    QGraphicsItem* item = items().first();
    if (item->type() == Arrow::Type)
    {
      Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
      arrow->startItem()->removeArrow(arrow);
      arrow->endItem()->removeArrow(arrow);
      delete item;
    };
    if (item->type() == Node::Type)
    {
      Node* node = (Node*)items().first();
      node->removeArrows();
      delete node;
    }
  }
  SAFE_DELETE(graph);
  SAFE_DELETE(signalSlotForm);
}

void Scene::clear()
{
  QGraphicsScene::clear();
  graph->clear();
}

void Scene::addClass(QString categoryName)
{
  if (treeWidget == NULL)
  {
    LOG_ERROR("treeWidget is null");
    return;
  }

  QTreeWidgetItem* rootItem = new QTreeWidgetItem(treeWidget);
  rootItem->setText(0, categoryName);
  VMetaClassList& list = VMetaClassMap::getList((char*)qPrintable(categoryName));
  for (VMetaClassList::iterator it = list.begin(); it != list.end(); it++)
  {
    VMetaClass* metaClass = *it;
    QTreeWidgetItem* item = new QTreeWidgetItem(rootItem);
    QString className = metaClass->className();
    item->setText(0, className);
    treeWidget->addTopLevelItem(item);
    addClass(metaClass, item);
  }
}

void Scene::addClass(VMetaClass* parentMetaClass, QTreeWidgetItem* parentItem)
{
  char* parentCategoryName = parentMetaClass->className();
  VMetaClassList& childList = VMetaClassMap::getList(parentCategoryName);
  for (VMetaClassList::iterator it = childList.begin(); it != childList.end(); it++)
  {
    VMetaClass* childMetaClass = *it;
    QTreeWidgetItem* childItem = new QTreeWidgetItem(parentItem);
    QString childClassName = childMetaClass->className();
    childItem->setText(0, childClassName);
    addClass(childMetaClass, childItem);
  }
}

void Scene::addClasses()
{
  foreach (QString categoryName, categoryNames)
  {
    addClass(categoryName);
  }
}

QString Scene::generateObjectClassName(QString className)
{
  if (className == "")
  {
    LOG_FATAL("className is blank");
    return "";
  }
  QString baseName = className;

  foreach(QString removePrefixName, removePrefixNames)
  {
    int length = removePrefixName.length();
    if (baseName.left(length) == removePrefixName)
      baseName = baseName.mid(length);
  }

  int index = 1;
  while (true)
  {
    QString res = baseName + QString::number(index);

    bool find = false;
    int _count = this->items().count();
    for (int i = 0; i < _count; i++)
    {
      QGraphicsItem* item = this->items().at(i);
      if (!IS_CLASS(item, Node*)) continue;
      Node* node = dynamic_cast<Node*>(item);
      if (res == node->object->name)
      {
        find = true;
        break;
      }
    }
    if (!find)
    {
      return res;
    }
    index++;
  }
  return ""; // unreachable code
}

Node* Scene::createNode(QString className, QString name, bool createObject)
{
  VObject* object = NULL;
  if (createObject)
  {
    object = (VObject*)VMetaClassMgr::createByClassName((char*)qPrintable(className));
    if (object == NULL) return NULL;
    if (name == "") name = generateObjectClassName(className);
    object->owner = this->graph;
    object->name  = name;
    this->graph->objectList.addObject(object);
  }

  Node* res = new Node(object);
  res->setPlainText(name);
  this->addItem(res);

  return res;
}

Arrow* Scene::createArrow(Node* startNode, QString signal, Node* endNode, QString slot)
{
  Arrow *res = new Arrow(startNode, signal, endNode, slot);
  res->setColor(Qt::black);
  startNode->addArrow(res);
  endNode->addArrow(res);
  return res;
}

Arrow* Scene::createArrow(QString startNodeName, QString signal, QString endNodeName, QString slot)
{
  Node* startNode = findNodeByName(startNodeName);
  if (startNode == NULL) return NULL;
  Node* endNode   = findNodeByName(endNodeName);
  if (endNode == NULL) return NULL;
  return createArrow(startNode, signal, endNode, slot);
}

Node* Scene::findNodeByName(QString name)
{
  int _count = this->items().count();
  for (int i = 0; i < _count; i++)
  {
    QGraphicsItem* item = this->items().at(i);
    if (!IS_CLASS(item, Node*)) continue;
    Node* res = dynamic_cast<Node*>(item);
    if (res->object->name == name)
    {
      return res;
    }
  }
  LOG_ERROR("can not find for '%s'", qPrintable(name));
  return NULL;
}

bool Scene::newFile(QString& errStr)
{
  clear();
  errStr = "";
  return true;
}

bool Scene::loadFromFile(QString fileName, QString& errStr)
{
  clear();
  bool res = this->graph->loadFromFile(fileName, "graph");
  if (!res)
  {
    errStr = qformat("can not open file(%s)", qPrintable(fileName));
    LOG_ERROR("%s", qPrintable(errStr));
    return false;
  }

  //
  // coord
  //
  {
    VXmlDoc doc(fileName);
    doc.loadFromFile(fileName);
    VXml xml = doc.root().gotoChild("coord");
    int _count = xml.childs().count();
    for (int i = 0 ; i < _count; i++)
    {
      VXml     childXml = xml.childs().at(i);
      QString  name = childXml.getStr("name");
      if (name == "")
      {
        errStr = "name is null";
        LOG_ERROR("%s", qPrintable(errStr));
        return false;
      }

      VObject* object   = this->graph->objectList.findByName(name);
      if (object == NULL)
      {
        errStr = qformat("can not find object ('%s')", qPrintable(name));
        LOG_ERROR("%s", qPrintable(errStr));
        return false;
      }

      Node*    node = this->createNode(object->className(), object->name, false);
      node->object = object;
      QPointF  f;
      f.setX(childXml.getDouble("x", f.x()));
      f.setY(childXml.getDouble("y", f.y()));
      node->setPos(f);
    }
  }

  //
  // arrow
  //
  {
    int _count = this->graph->connectList.count();
    for (int i = 0; i < _count; i++)
    {
      VGraphConnect connect = (VGraphConnect&)this->graph->connectList.at(i);
      Arrow* arrow = createArrow(connect.sender, connect.signal, connect.receiver, connect.slot);
      if (arrow == NULL) return false;
      addItem(arrow);
      arrow->updatePosition();
    }
  }

  return true;
}

bool Scene::saveToFile(QString fileName, QString& errStr)
{
  bool res = this->graph->saveToFile(fileName, "graph");
  if (!res)
  {
    errStr = this->graph->error.msg;
    return false;
  }

  //
  // coord
  //
  VXmlDoc doc(fileName);
  doc.loadFromFile(fileName);
  VXml xml = doc.root().gotoChild("coord");
  xml.clearChild();
  int _count = this->graph->objectList.count();
  for (int i = 0; i < _count; i++)
  {
    VObject* object = this->graph->objectList.at(i);
    Node*    node   = findNodeByName(object->name);
    if (node == NULL) continue;

    VXml     childXml = xml.addChild("object");
    childXml.setStr("name", object->name);
    childXml.setDouble("x", node->pos().x());
    childXml.setDouble("y", node->pos().y());
  }
  doc.saveToFile(fileName);

  return true;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  // LOG_DEBUG("mousePressEvent"); // gilgil temp 2012.06.30
  switch (m_mode)
  {
    case InsertItem:
      {
        QList<QTreeWidgetItem*> items = treeWidget->selectedItems();
        foreach(QTreeWidgetItem* item, items)
        {
          QString className = item->text(0);
          Node* node = createNode(className, "", true);
          if (node == NULL)
          {
            QString title = "error";
            QString text = "can not create object for \"" + className + "\"";
            QMessageBox::information(NULL, title, text);
          } else
          {
            node->setPos(event->scenePos());
          }
          setMode(MoveItem);
        }
      }
      break;
    case MoveItem:
      break;
    case InsertLine:
      line = new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));
      line->setPen(QPen(Qt::black, 2));
      addItem(line);
      break;
  }

  QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  // LOG_DEBUG("mouseMoveEvent"); // gilgil temp 2012.06.30
  switch (m_mode)
  {
    case MoveItem:
      break;
    case InsertLine:
      if (line != NULL)
      {
        QLineF newLine(line->line().p1(), event->scenePos());
        line->setLine(newLine);
        return;
      }
      break;
  }

  QGraphicsScene::mouseMoveEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  // LOG_DEBUG("mouseReleaseEvent"); // gilgil temp 2012.06.30
  switch (m_mode)
  {
    case MoveItem:
      break;
    case InsertLine:
      if (line != NULL)
      {
          QList<QGraphicsItem *> startItems = items(line->line().p1());
          if (startItems.count() && startItems.first() == line)
              startItems.removeFirst();
          QList<QGraphicsItem *> endItems = items(line->line().p2());
          if (endItems.count() && endItems.first() == line)
              endItems.removeFirst();

          if (startItems.count() > 0 && endItems.count() > 0 &&
              startItems.first() != endItems.first())
          {
              Node *startNode         = (Node*)startItems.first();
              Node *endNode           = (Node*)endItems.first();

              QStringList _signalList = VGraph::signalList(startNode->object);
              foreach (QString name, removeSignalNames)
              {
                _signalList.removeAll(name);
              }
              {
                int i = 0;
                while (i < _signalList.count())
                {
                  QString name = _signalList.at(i);
                  if (name.startsWith("__"))
                  {
                    _signalList.removeAt(i);
                    continue;
                  }
                  i++;
                }
              }

              QStringList _slotList   = VGraph::slotList(endNode->object);
              foreach (QString name, removeSlotNames)
              {
                _slotList.removeAll(name);
              }
              {
                int i = 0;
                while (i < _slotList.count())
                {
                  QString name = _slotList.at(i);
                  if (name.startsWith("__"))
                  {
                    _slotList.removeAt(i);
                    continue;
                  }
                  i++;
                }
              }

              if (signalSlotForm == NULL) signalSlotForm = new SignalSlotForm((QWidget*)this->parent());
              signalSlotForm->ui->lwSignalList->clear();
              signalSlotForm->ui->lwSignalList->addItems(_signalList);
              signalSlotForm->ui->lwSlotList->clear();
              signalSlotForm->ui->lwSlotList->addItems(_slotList);
              signalSlotForm->exec();
              if (signalSlotForm->result() == QDialog::Accepted)
              {
                QString signal = signalSlotForm->ui->lwSignalList->selectedItems().first()->text();
                QString slot   = signalSlotForm->ui->lwSlotList->selectedItems().first()->text();

                VGraphConnect connect;
                connect.sender   = startNode->object->name;
                connect.signal   = signal;
                connect.receiver = endNode->object->name;
                connect.slot     = slot;
                this->graph->connectList.addConnect(connect);

                Arrow *arrow = createArrow(startNode, signal, endNode, slot);
                if (arrow != NULL)
                {
                  addItem(arrow);
                  arrow->updatePosition();
                }
              }
          }
          removeItem(line);
          delete line;
          line = NULL;
      }
      break;
  }

  QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::load(VXml xml)
{
  categoryNames     = xml.getStr("categoryNames",     categoryNames.join("/")).split("/");
  removePrefixNames = xml.getStr("removePrefixNames", removePrefixNames.join("/")).split("/");
  removeSignalNames = xml.getStr("removeSignalNames", removeSignalNames.join("/")).split("/");
  removeSlotNames   = xml.getStr("removeSlotNames",   removeSlotNames.join("/")).split("/");
}

void Scene::save(VXml xml)
{
  xml.setStr("categoryNames",     categoryNames.join("/"));
  xml.setStr("removePrefixNames", removePrefixNames.join("/"));
  xml.setStr("removeSignalNames", removeSignalNames.join("/"));
  xml.setStr("removeSlotNames",   removeSlotNames.join("/"));
}
