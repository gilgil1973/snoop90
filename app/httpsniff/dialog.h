#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include <SnoopArpSpoof>
#include <SnoopWinDivert>
#include <VHttpProxy>
#include <SnoopDump>
#include <SnoopWriteAdapter>

// ----------------------------------------------------------------------------
// Dialog
// ----------------------------------------------------------------------------
namespace Ui {
  class Dialog;
}

class Dialog : public QDialog, public VXmlable
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = 0);
  ~Dialog();

public:
  void initializeControl();
  void finalizeControl();
  void loadControl();
  void saveControl();
  void setControl();

public:
  //
  // Port
  //
  QList<int> httpPortList;
  QList<int> httpsPortList;

  //
  // Capture
  //
  SnoopWinDivert winDivert;
  SnoopArpSpoof  arpSpoof;

  //
  // Proxy
  //
  QStringList proxyProcessNameList;
  VHttpProxy  proxy1;
  VHttpProxy  proxy2;

  //
  // Write
  //
  SnoopDump         dump;
  SnoopWriteAdapter writeAdapter;

public:
  void load(VXml xml);
  void save(VXml xml);

private:
  Ui::Dialog *ui;
};

#endif // DIALOG_H
