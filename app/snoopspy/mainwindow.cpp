#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <VApp>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// Param
// ----------------------------------------------------------------------------
Param::Param()
{
  fileName = "";
  autoOpen = false;
}

bool Param::parse(int argc, char* argv[])
{
  //
  // snoopspy [< filename > [autoopen]]
  //
  if (argc >= 2)
  {
    fileName = argv[1];
  }
  if (argc >= 3)
  {
    QString s = QString(argv[2]).toLower();
    autoOpen = s == "autoopen";
  }

  return true;
}

// ----------------------------------------------------------------------------
// MainWindow
// ----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  fileDialog(this)
{
  ui->setupUi(this);
  aboutDlg = NULL;
  initializeControl();
}

MainWindow::~MainWindow()
{
  SAFE_DELETE(aboutDlg);
  saveControl();
  finalizeControl();
  delete ui;
}

void MainWindow::showEvent(QShowEvent* showEvent)
{
  loadControl();
  setControl();
  QMainWindow::showEvent(showEvent);
}

void MainWindow::initializeControl()
{
  move(0, 0); resize(640, 640);

  ui->dockWidget->setWidget(ui->leftSplitter); // (ui->treeWidget);

  mainSplitter = new QSplitter(this);

  scene    = new Scene(this);
  scene->setTreeWidget(ui->treeWidget);

  VObject::connect(scene->graph, SIGNAL(closed()), this, SLOT(on_actionStop_triggered()));

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

  sizes.clear(); sizes << 400 << 100;
  ui->leftSplitter->setSizes(sizes);

  connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(changed(QList<QRectF>))); // gilgil temp 2012.07.31
  connect(scene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));

  m_changed = false;
  fileName  = "";
  QStringList filters; filters << "snoopspy files(*.ss)" << "any files(*)";
  fileDialog.setNameFilters(filters);
  fileDialog.setDefaultSuffix("ss");
  fileDialog.setViewMode(QFileDialog::Detail);
}

void MainWindow::finalizeControl()
{
  SAFE_DELETE(scene);
  SAFE_DELETE(view);
  SAFE_DELETE(model);
}

void MainWindow::loadControl()
{
  this->loadFromDefaultDoc("MainWindow");
  scene->addClasses();
  ui->treeWidget->expandAll();

  VApp& app = VApp::instance();
  Param param;
  param.parse(app.argc(), app.argv());
  if (param.fileName != "")
  {
    this->fileName = param.fileName;
    QString errStr;
    if (scene->loadFromFile(this->fileName, errStr))
    {
      m_changed = false;

      if (param.autoOpen)
        ui->actionStart->trigger();
    } else
    {
      this->fileName = "";
      QMessageBox::information(this, "error", errStr);
    }
    setControl();
  }
}

void MainWindow::saveControl()
{
  this->saveToDefaultDoc("MainWindow");
}

void MainWindow::setControl()
{
  QString title = "SnoopSpy";
  if (fileName != "")
  {
    QFileInfo fi(fileName);
    title = fi.completeBaseName();
  }
  setWindowTitle(title);

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

void MainWindow::runProcess(QString processName)
{
  QString url = "file:///" + VApp::_filePath() + processName;
  if (!QDesktopServices::openUrl(QUrl(url)))
  {
    LOG_ERROR("can not open url(%s)", qPrintable(url));
  }
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
    // LOG_DEBUG("%p scene=%p", node, node->scene()); // gilgil temp 2012.12.07

    selectObject = node->object;
    selectDoc.clear();
    selectDoc.createRoot("object");
    selectObject->save(selectDoc.root());
    QDomDocument document = *((QDomDocument*)&selectDoc);
    DomModel *newModel = new DomModel(document, this);
    ui->treeView->setModel(newModel);
    delete model;
    model = newModel;
    VObject::connect(newModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(dataChanged(QModelIndex,QModelIndex)));
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
    VXml coordXml = xml.findChild("coord");
    if (!coordXml.isNull())
    {
      QRect rect = geometry();
      rect.setLeft  ((coordXml.getInt("left",   0)));
      rect.setTop   ((coordXml.getInt("top",    0)));
      rect.setWidth ((coordXml.getInt("width",  640)));
      rect.setHeight((coordXml.getInt("height", 480)));
      setGeometry(rect);
    }
  }

  {
    VXml sizesXml = xml.findChild("sizes");
    QList<int> sizes;
    QStringList strList;
    if (!sizesXml.isNull())
    {
      strList = sizesXml.getStr("mainSplitter").split(",");
      sizes.clear(); foreach (QString s, strList) sizes << s.toInt();
      mainSplitter->setSizes(sizes);

      strList = sizesXml.getStr("leftSplitter").split(",");
      sizes.clear(); foreach (QString s, strList) sizes << s.toInt();
      ui->leftSplitter->setSizes(sizes);

      strList = sizesXml.getStr("treeView").split(",");
      sizes.clear(); foreach (QString s, strList) sizes << s.toInt();
      for (int i = 0; i < strList.count(); i++)  ui->treeView->setColumnWidth(i, sizes.at(i));
    }
  }

  scene->load(xml.gotoChild("scene"));
}

void MainWindow::save(VXml xml)
{
  {
    VXml coordXml = xml.gotoChild("coord");
    QRect rect = geometry();
    coordXml.setInt("left",   rect.left());
    coordXml.setInt("top",    rect.top());
    coordXml.setInt("width",  rect.width());
    coordXml.setInt("height", rect.height());
  }

  {
    VXml sizesXml = xml.gotoChild("sizes");
    QList<int> sizes;
    QString strList;

    sizes = mainSplitter->sizes();
    strList.clear(); foreach (int size, sizes) strList += QString::number(size) + ",";
    strList = strList.left(strList.count() - 1);
    sizesXml.setStr("mainSplitter", strList);

    sizes = ui->leftSplitter->sizes();
    strList.clear(); foreach (int size, sizes) strList += QString::number(size) + ",";
    strList = strList.left(strList.count() - 1);
    sizesXml.setStr("leftSplitter", strList);

    static const int columnCount = 2;
    strList.clear(); for (int i = 0; i < columnCount; i++) strList += QString::number(ui->treeView->columnWidth(i)) + ",";
    strList = strList.left(strList.count() - 1);
    sizesXml.setStr("treeView", strList);
  }

  scene->save(xml.gotoChild("scene"));
}

void MainWindow::on_actionNewFile_triggered()
{
  fileName = "";
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
    // LOG_DEBUG("count=%d", scene->items().count()); // gilgil tem 2012.06.27
    // scene->removeItem(item);
    if (item->type() == Node::Type)
    {
      Node* node = (Node*)item;
      node->removeArrows();
      // LOG_DEBUG("count=%d", scene->items().count()); // gilgil tem 2012.06.27
      delete node;
      // LOG_DEBUG("count=%d", scene->items().count()); // gilgil tem 2012.06.27
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

  if (optionable->optionDoAll(this)) selectionChanged();
}

void MainWindow::on_actionLogServer_triggered()
{
  runProcess("logserver.exe");
}

void MainWindow::on_actionNetClient_triggered()
{
  runProcess("netclient.exe");
}

void MainWindow::on_actionNetServer_triggered()
{
  runProcess("netserver.exe");
}

void MainWindow::on_actionWebProxy_triggered()
{
  runProcess("webproxy.exe");
}

void MainWindow::on_actionWebSniff_triggered()
{
  runProcess("websniff.exe");
}

void MainWindow::on_actionCapture_Filter_triggered()
{
  QString url = "http://wiki.wireshark.org/CaptureFilters";
  if (!QDesktopServices::openUrl(QUrl(url)))
  {
    LOG_ERROR("can not open url(%s)", qPrintable(url));
  }
}

void MainWindow::on_actionDisplay_Filter_triggered()
{
  QString url = "http://wiki.wireshark.org/DisplayFilters";
  if (!QDesktopServices::openUrl(QUrl(url)))
  {
    LOG_ERROR("can not open url(%s)", qPrintable(url));
  }
}

void MainWindow::on_actionWinDivert_Filter_triggered()
{
  QString url = "http://reqrypt.org/windivert-doc.html#filter_language";
  if (!QDesktopServices::openUrl(QUrl(url)))
  {
    LOG_ERROR("can not open url(%s)", qPrintable(url));
  }
}
