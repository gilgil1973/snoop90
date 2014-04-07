#include <SnoopRemote>
#include <VDebugNew>

#ifdef WIN32

REGISTER_METACLASS(SnoopRemote, SnoopCapture)

// ----------------------------------------------------------------------------
// SnoopRemote
// ----------------------------------------------------------------------------
SnoopRemote::SnoopRemote(void* owner) : SnoopPcap(owner)
{
  readTimeout  = 1000;
  host         = "";
  userName     = "";
  password     = "";
  adapterIndex = snoop::INVALID_ADAPTER_INDEX;
}

SnoopRemote::~SnoopRemote()
{
  close();
}

bool SnoopRemote::doOpen()
{ 
  if (!enabled)
  {
    LOG_DEBUG("enabled is false");
    return true;
  }
  
  int i;
  QString source;
  pcap_rmtauth* auth = NULL;
  pcap_if_t *dev;
  char errBuf[PCAP_ERRBUF_SIZE];

  if (host == "")
  {
    SET_ERROR(SnoopError, "host is not specified", VERR_HOST_NOT_SPECIFIED);
    goto _error;
  }
  source = "rpcap://" + host + "/";

  if (this->userName != "" || this->password != "")
  {
    auth = new pcap_rmtauth;
    auth->type = RPCAP_RMTAUTH_PWD;
    char username[MAX_PATH]; strcpy_s(username, qPrintable(this->userName));
    char password[MAX_PATH]; strcpy_s(password, qPrintable(this->password));
    auth->username = username;
    auth->password = password;
  }

  i = pcap_findalldevs_ex((char*)qPrintable(source), auth, &interfaces.allDevs, errBuf);
  if (i != 0) // if error occured
  {
    SET_ERROR(SnoopError, format("error in pcap_findalldevs_ex(%s)", errBuf).c_str(), VERR_IN_PCAP_FINDALLDEVS_EX);
    goto _error;
  }

  {
    interfaces.clear();
    SnoopInterface nullIntf;
    nullIntf.index = 0;
    interfaces.push_back(nullIntf);
  }
  i = 1;
  dev = interfaces.allDevs;
  while (dev != NULL)
  {
    SnoopInterface intf;

    intf.index = i;
    intf.name = dev->name;
    intf.description = dev->description;
    intf.dev = dev;

    interfaces.push_back(intf);

    dev = dev->next;
    i++;
  }

  emit interfacesReceived(this, &interfaces, &adapterIndex);
  if (adapterIndex == snoop::INVALID_ADAPTER_INDEX)
  {
    SET_ERROR(SnoopError, "cancel by user", VERR_CANCELED_BY_USER);
    goto _error;
  }

  const SnoopInterface& intf = interfaces.at(adapterIndex);
  source = intf.name;

  bool res = pcapOpen((char*)qPrintable(source), auth, intf.dev);
  if (!res)
  {
    goto _error;
  }

  res = SnoopPcap::doOpen();
  if (auth != NULL) delete auth;
  return res;

_error:
  if (auth != NULL) delete auth;
  return false;
}

bool SnoopRemote::doClose()
{
  if (!enabled)
  {
    LOG_DEBUG("enabled is false");
    return true;
  }
  
  return SnoopPcap::doClose();
}

void SnoopRemote::load(VXml xml)
{
  SnoopPcap::load(xml);

  host = xml.getStr("host", host);
  userName = xml.getStr("userName", userName);
  password = xml.getStr("password", password);
  adapterIndex = xml.getInt("adapterIndex", adapterIndex);
}

void SnoopRemote::save(VXml xml)
{
  SnoopPcap::save(xml);

  xml.setStr("host", host);
  xml.setStr("userName", userName);
  xml.setStr("password", password);
  xml.setInt("adapterIndex", adapterIndex);
}

#ifdef QT_GUI_LIB
void SnoopRemote::optionAddWidget(QLayout *layout)
{
  SnoopPcap::optionAddWidget(layout);

  VOptionable::addLineEdit(layout, "leHost",         "Host",          host);
  VOptionable::addLineEdit(layout, "leUserName",     "User Name",     userName);
  VOptionable::addLineEdit(layout, "lePassword",     "Password",      password);
  VOptionable::addLineEdit(layout, "leAdapterIndex", "Adapter Index", QString::number(adapterIndex));
}

void SnoopRemote::optionSaveDlg(QDialog *dialog)
{
  SnoopPcap::optionSaveDlg(dialog);

  host         = dialog->findChild<QLineEdit*>("leHost")->text();
  userName     = dialog->findChild<QLineEdit*>("leUserName")->text();
  password     = dialog->findChild<QLineEdit*>("lePassword")->text();
  adapterIndex = dialog->findChild<QLineEdit*>("leAdapterIndex")->text().toInt();
}
#endif // QT_GUI_LIB

#endif // WIN32
