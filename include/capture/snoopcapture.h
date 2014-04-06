// ----------------------------------------------------------------------------
//
// Snoop Component Suite version 9.0
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#ifndef __SNOOP_CAPTURE_H__
#define __SNOOP_CAPTURE_H__

#include <Snoop>
#include <VThread>
#include <VObjectWidget>
#include <VGraph>

// ----------------------------------------------------------------------------
// SnoopCapture
// ----------------------------------------------------------------------------
/// Base class of all capture classes
class SnoopCapture : public VObject, protected VRunnable, public VOptionable
{
  Q_OBJECT

public:
  SnoopCapture(void* owner = NULL);
  virtual ~SnoopCapture();

protected:
  virtual bool doOpen();
  virtual bool doClose();

public:
  virtual int read(SnoopPacket* packet);
  virtual int write(SnoopPacket* packet);
  virtual int write(u_char* buf, int size, WINDIVERT_ADDRESS* divertAddr = NULL);

public:
  void parse(SnoopPacket* packet);

public:
  virtual SnoopCaptureType captureType() { return SnoopCaptureType::None; }
  virtual int              dataLink()    { return DLT_NULL; }
  virtual bool             relay(SnoopPacket* packet);

  //
  // Properties
  //
public:
  bool enabled;
  bool autoRead;
  bool autoParse;

protected:
  virtual void run();

signals:
  void captured(SnoopPacket* packet);

protected:
  SnoopPacket packet;

public:
  virtual void load(VXml xml);
  virtual void save(VXml xml);

#ifdef QT_GUI_LIB
public: // for VOptionable
  virtual void optionAddWidget(QLayout* layout);
  virtual void optionSaveDlg(QDialog* dialog);
#endif // QT_GUI_LIB
};

#endif // __SNOOP_CAPTURE_H__
