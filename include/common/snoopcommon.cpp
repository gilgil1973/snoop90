#include <SnoopCommon>

// ----------------------------------------------------------------------------
// Snoop Version
// ----------------------------------------------------------------------------
#ifdef _DEBUG
const char* SNOOP_VERSION = "Snoop 9.0  Debug   Build( "__DATE__ " " __TIME__" )";
#else // RELEASE
const char* SNOOP_VERSION = "Snoop 9.0  Release Build( "__DATE__ " " __TIME__" )";
#endif // _DEBUG

// ----------------------------------------------------------------------------
// SnoopCaptureType
// ----------------------------------------------------------------------------
SnoopCaptureType::SnoopCaptureType(const QString s)
{
  if (s == "None")           value = None;
  else if (s == "InPath")    value = InPath;
  else if (s == "OutOfPath") value = OutOfPath;
  else value = None;
}

QString SnoopCaptureType::str() const
{
  QString res;
  switch (value)
  {
    case None:      res = "None";      break;
    case InPath:    res = "InPath";    break;
    case OutOfPath: res = "OutOfPath"; break;
    default:        res = "None";      break;
  }
  return res;
}

