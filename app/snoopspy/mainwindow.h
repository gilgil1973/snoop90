#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDebug>

#include <VObjectWidget>
#include <VXml>
#include <VLog>

#include "scene.h"
#include "view.h"
#include "node.h"
#include "arrow.h"
#include "dommodel.h"
#include "aboutdlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, VXmlable
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public:
  AboutDlg*  aboutDlg;

public:
  QSplitter*      mainSplitter;
  Scene*          scene;
  View*           view;

public:
  DomModel*       model;
  VObject*        selectObject;
  VXmlDoc         selectDoc;

public:
  void initializeControl();
  void finalizeControl();
  void loadControl();
  void saveControl();
  void setControl();

public slots:
  void changed(QList<QRectF> region);
  void selectionChanged();
  void dataChanged(QModelIndex,QModelIndex);

protected:
  bool        m_changed;
  QString     fileName;
  QFileDialog fileDialog;

public:
  void load(VXml xml);
  void save(VXml xml);

private slots:
  void on_actionNewFile_triggered();

  void on_actionOpenFile_triggered();

  void on_actionSaveFile_triggered();

  void on_actionSaveFileAs_triggered();

  void on_actionStart_triggered();

  void on_actionStop_triggered();

  void on_actionOption_triggered();

  void on_actionPointer_triggered();

  void on_actionLinePointer_triggered();

  void on_actionDelete_triggered();

  void on_actionBringToFront_triggered();

  void on_actionSendToBack_triggered();

  void on_actionExit_triggered();

  void on_actionAbout_triggered();

  void on_treeWidget_clicked(const QModelIndex &index);

  void on_actionShowOption_triggered();

  void on_actionCapture_Filter_triggered();

  void on_actionDisplay_Filter_triggered();

  void on_actionWinDivert_Filter_triggered();

private:
  Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
