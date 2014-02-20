#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <VDebugNew>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  aboutDlg = NULL;
  initializeControl();
  loadControl();
  setControl();
}

MainWindow::~MainWindow()
{
  SAFE_DELETE(aboutDlg);
  saveControl();
  finalizeControl();
  delete ui;
}

void MainWindow::initializeControl()
{
  move(0, 0);
  resize(640, 480);

  ui->dockWidget->setWidget(ui->leftSplitter); // (ui->treeWidget);

  mainSplitter = new QSplitter(this);

  scene    = new Scene(this);
  scene->setTreeWidget(ui->treeWidget);

  view     = new View(this);
  view->setScene(scene);

  model = new DomModel(QDomDocument(), this);
  ui->treeView->setModel(model);
  selectObject = NULL;
  selectDoc.createRoot("object");

  mainSplitter->addWidget(ui->dockWidget);
  mainSplitter->addWidget(view);
  this->setCentralWidget(mainSplitter);
  QList<int> sizes; sizes << 100 << 200;
  mainSplitter->setSizes(sizes);

  sizes.clear(); sizes << 200 << 100;
  ui->leftSplitter->setSizes(sizes);

  connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(changed(QList<QRectF>))); // gilgil temp 2012.07.31
  connect(scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));

  m_changed = false;
  fileName  = "";
  QStringList filters;
  filters << "snoopspy files(*.ss)" << "any files(*)";
  fileDialog.setNameFilters(filters);
  fileDialog.setDefaultSuffix("ss");
  fileDialog.setViewMode(QFileDialog::Detail);
}

void MainWindow::finalizeControl()
{
  delete scene;
  delete view;
  delete model;
}

void MainWindow::loadControl()
{
  this->loadFromDefaultDoc("coord/main");
  scene->addClasses();
  ui->treeWidget->expandAll();
}

void MainWindow::saveControl()
{
  this->saveToDefaultDoc("coord/main");
}

void MainWindow::setControl()
{
  ui->actionSaveFile->setEnabled(m_changed && fileName != "");
  ui->actionSaveFileAs->setEnabled(m_changed);

  Scene::Mode mode = scene->mode();

  ui->actionPointer->setChecked(mode == Scene::MoveItem);
  ui->actionLinePointer->setChecked(mode == Scene::InsertLine);

  bool active = scene->graph->active();
  ui->actionStart->setEnabled(!active);
  ui->actionStop->setEnabled(active);

  bool selected = scene->selectedItems().count() > 0;

  ui->actionDelete->setEnabled(selected);
  ui->actionBringToFront->setEnabled(selected);
  ui->actionSendToBack->setEnabled(selected);

  bool showOptionEnabled = false;
  if (selected)
  {
    QGraphicsItem* item = scene->selectedItems().first();
    Node* node = dynamic_cast<Node*>(item);
    if (node != NULL)
    {
      VObject* object = node->object;
      VOptionable* showOption = dynamic_cast<VOptionable*>(object);
      if (showOption != NULL)
      {
        showOptionEnabled = true;
      }
    }
  }
  ui->actionShowOption->setEnabled(showOptionEnabled);
}

void MainWindow::changed(QList<QRectF> region)
{
  //LOG_DEBUG("changed %d", region.count()); // gilgil temp 2012.07.30
  m_changed = true;
  setControl();
  //view->updateScene(region);
}

void MainWindow::selectionChanged()
{
  setControl();

  if (scene->selectedItems().count() == 0)
  {
    QDomDocument document;
    DomModel *newModel = new DomModel(document, this);
    ui->treeView->setModel(newModel);
    delete model;
    model = newModel;
    return;
  }
  QGraphicsItem* item = scene->selectedItems().first();
  if (IS_CLASS(item, Node*))
  {
    Node* node = dynamic_cast<Node*>(item);
    LOG_DEBUG("%p scene=%p", node, node->scene()); // gilgil temp 2012.12.07

    selectObject = node->object;
    selectDoc.clear();
    selectDoc.createRoot("object");
    selectObject->save(selectDoc.root());
    QDomDocument document = *((QDomDocument*)&selectDoc);
    DomModel *newModel = new DomModel(document, this);
    ui->treeView->setModel(newModel);
    delete model;
    model = newModel;
    QObject::connect(newModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex,QModelIndex)));
    ui->treeView->expandAll();
  }
}

void MainWindow::dataChanged(QModelIndex,QModelIndex)
{
  LOG_DEBUG("dataChanged");
  selectObject->load(selectDoc.root());
  selectionChanged();
}

void MainWindow::load(VXml xml)
{
  {
    QRect rect = geometry();
    rect.setLeft  ((xml.getInt("left",   0)));
    rect.setTop   ((xml.getInt("top",    0)));
    rect.setWidth ((xml.getInt("width",  640)));
    rect.setHeight((xml.getInt("height", 480)));
    setGeometry(rect);
  }

  {
    VXml sizesXML = xml.gotoChild("sizes");
    QList<int> sizes;

    sizes.clear();
    QStringList mainSplitterValues = sizesXML.getStr("main_splitter").split(",");
    foreach (QString value, mainSplitterValues)
    {
      sizes.push_back(value.toInt());
    }
    mainSplitter->setSizes(sizes);

    sizes.clear();
    QStringList leftSplitterValues = sizesXML.getStr("left_splitter").split(",");
    foreach (QString value, leftSplitterValues)
    {
      sizes.push_back(value.toInt());
    }
    ui->leftSplitter->setSizes(sizes);

    sizes.clear();
    QStringList treeViewValues = sizesXML.getStr("treeView").split(",");
    int column = 0;
    foreach (QString value, treeViewValues)
    {
      if (column <= 2 && value != "") ui->treeView->setColumnWidth(column, value.toInt());
      column++;
    }
  }

  scene->setSceneRect(view->geometry());

  scene->load(xml.gotoChild("scene"));
}

void MainWindow::save(VXml xml)
{
  QRect rect = geometry();
  xml.setInt("left",   rect.left());
  xml.setInt("top",    rect.top());
  xml.setInt("width",  rect.width());
  xml.setInt("height", rect.height());

  VXml childXml = xml.gotoChild("sizes");

  QString mainSplitterValues;
  for (int i = 0; i < mainSplitter->count(); i++)
  {
    mainSplitterValues += QString::number(mainSplitter->sizes().at(i));
    if (i < mainSplitter->count() - 1)
      mainSplitterValues += ",";
  }
  childXml.setStr("main_splitter", mainSplitterValues);

  QString leftSplitterValues;
  for (int i = 0; i < ui->leftSplitter->count(); i++)
  {
    leftSplitterValues += QString::number(ui->leftSplitter->sizes().at(i));
    if (i < mainSplitter->count() - 1)
      leftSplitterValues += ",";
  }
  childXml.setStr("left_splitter", leftSplitterValues);

  QString treeViewValues;
  for (int i =0; i < 2 - 1; i++) // 2 == column count of treeView
  {
    treeViewValues += QString::number(ui->treeView->columnWidth(i));
    if (i < 2)
      treeViewValues += ",";
  }
  childXml.setStr("treeView", treeViewValues);

  scene->save(xml.gotoChild("scene"));
}

void MainWindow::on_actionNewFile_triggered()
{
  scene->clear();
  setControl();
}

void MainWindow::on_actionOpenFile_triggered()
{
  fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  if (fileDialog.exec() == QDialog::Accepted)
  {
    fileName = fileDialog.selectedFiles().first();
    QString errStr;
    if (scene->loadFromFile(fileName, errStr))
    {
      m_changed = false;
    } else
    {
      QMessageBox::information(this, "error", errStr);
    }
    setControl();
  }
}

void MainWindow::on_actionSaveFile_triggered()
{
  QString errStr;
  if (scene->saveToFile(fileName, errStr))
  {
    m_changed = false;
  } else
  {
    QMessageBox::information(this, "error", errStr);
  }
  setControl();
}

void MainWindow::on_actionSaveFileAs_triggered()
{
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog.setFileMode(QFileDialog::AnyFile);
  
  if (fileDialog.exec() == QDialog::Accepted)
  {
    fileName = fileDialog.selectedFiles().first();
    on_actionSaveFile_triggered();
  }
}

void MainWindow::on_actionStart_triggered()
{
  bool res = scene->graph->open();
  if (!res)
  {
    QString msg = QString::fromLocal8Bit(scene->graph->error.msg);
    QMessageBox::information(this, "error", msg);
  }
  setControl();
}

void MainWindow::on_actionStop_triggered()
{
  scene->graph->close();
  setControl();
}

void MainWindow::on_actionOption_triggered()
{
  LOG_DEBUG("MainWindow::on_actionOption_triggered"); // gilgil temp 2012.09.04
}

void MainWindow::on_actionPointer_triggered()
{
  scene->setMode(Scene::MoveItem);
  setControl();
}

void MainWindow::on_actionLinePointer_triggered()
{
  scene->setMode(Scene::InsertLine);
  setControl();
}

void MainWindow::on_actionDelete_triggered()
{
  foreach (QGraphicsItem *item, scene->selectedItems())
  {
    if (item->type() == Arrow::Type)
    {
      // scene->removeItem(item);
      Arrow *arrow = (Arrow*)item;
      arrow->startItem()->removeArrow(arrow);
      arrow->endItem()->removeArrow(arrow);
      delete arrow;
    }
  }

  foreach (QGraphicsItem *item, scene->selectedItems())
  {
    LOG_DEBUG("count=%d", scene->items().count()); // gilgil tem 2012.06.27
    // scene->removeItem(item);
    if (item->type() == Node::Type)
    {
      Node* node = (Node*)item;
      node->removeArrows();
      LOG_DEBUG("count=%d", scene->items().count()); // gilgil tem 2012.06.27
      delete node;
      LOG_DEBUG("count=%d", scene->items().count()); // gilgil tem 2012.06.27
    }
  }
}

void MainWindow::on_actionBringToFront_triggered()
{
  if (scene->selectedItems().isEmpty()) return;

  QGraphicsItem *selectedItem = scene->selectedItems().first();
  QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

  qreal zValue = 0;
  foreach (QGraphicsItem *item, overlapItems)
  {
    if (item->zValue() >= zValue && item->type() == Node::Type)
      zValue = item->zValue() + 0.1;
  }
  selectedItem->setZValue(zValue);
}

void MainWindow::on_actionSendToBack_triggered()
{
  if (scene->selectedItems().isEmpty()) return;

  QGraphicsItem *selectedItem = scene->selectedItems().first();
  QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

  qreal zValue = 0;
  foreach (QGraphicsItem *item, overlapItems)
  {
    if (item->zValue() <= zValue && item->type() == Node::Type)
      zValue = item->zValue() - 0.1;
  }
  selectedItem->setZValue(zValue);
}

void MainWindow::on_actionExit_triggered()
{
  close();
}

void MainWindow::on_actionAbout_triggered()
{
  if (aboutDlg == NULL)
  {
    aboutDlg = new AboutDlg;
  }
  aboutDlg->exec();
}

void MainWindow::on_treeWidget_clicked(const QModelIndex &index)
{
  Q_UNUSED(index)
  if (ui->treeWidget->selectedItems().isEmpty()) return;
  scene->setMode(Scene::InsertItem);
  setControl();
}


void MainWindow::on_actionShowOption_triggered()
{
  if (scene->selectedItems().isEmpty()) return;

  QGraphicsItem* item = scene->selectedItems().first();
  if (item == NULL) return;

  Node* node = dynamic_cast<Node*>(item);
  if (node == NULL) return;

  VObject* object = node->object;
  VOptionable* optionable = dynamic_cast<VOptionable*>(object);
  if (optionable == NULL) return;

  QDialog* dialog = optionable->createOptionDlg();
  dialog->setWindowTitle(object->name + " Option");
  optionable->addOptionWidget(dialog->layout());
  bool res = optionable->showOptionDlg(dialog);
  if (res)
  {
    optionable->saveOptionDlg(dialog);
    selectionChanged();
  }
}

void MainWindow::on_actionCapture_Filter_triggered()
{
  QDesktopServices::openUrl(QUrl("http://wiki.wireshark.org/CaptureFilters"));
}

void MainWindow::on_actionDisplay_Filter_triggered()
{
  QDesktopServices::openUrl(QUrl("http://wiki.wireshark.org/DisplayFilters"));
}

void MainWindow::on_actionWinDivert_Filter_triggered()
{
  QDesktopServices::openUrl(QUrl("http://reqrypt.org/windivert-doc.html#filter_language"));
}
