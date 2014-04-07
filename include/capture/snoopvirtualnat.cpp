#include <SnoopVirtualNat>

REGISTER_METACLASS(SnoopVirtualNat, SnoopCapture)

// ----------------------------------------------------------------------------
// SnoopVirtualNat
// ----------------------------------------------------------------------------
SnoopVirtualNat::SnoopVirtualNat(void* owner) : SnoopCapture(owner)
{ 
  virAdapterIndex = snoop::INVALID_ADAPTER_INDEX;
  VObject::connect(&virAdapter, SIGNAL(captured(SnoopPacket*)), this, SLOT(myVirtualRecv(SnoopPacket*)), Qt::DirectConnection);

  realAdapterIndex    = snoop::DEFAULT_ADAPTER_INDEX;
  VObject::connect(&realAdapter, SIGNAL(captured(SnoopPacket*)), this, SLOT(myRealRecv(SnoopPacket*)), Qt::DirectConnection);
}

SnoopVirtualNat::~SnoopVirtualNat()
{
  close();
}

bool SnoopVirtualNat::doOpen()
{
  if (!enabled)
  {
    LOG_DEBUG("enabled is false");
    return true;
  }
  
  virAdapter.adapterIndex = virAdapterIndex;
  if (!virAdapter.open())
  {
    error = virAdapter.error;
    return false;
  }
  virNetInfo.adapterIndex = virAdapterIndex;

  realAdapter.adapterIndex = realAdapterIndex;
  if (!realAdapter.open())
  {
    error = realAdapter.error;
    return false;
  }
  realNetInfo.adapterIndex = realAdapterIndex;

  if (!changeRouteTable())
  {
    SET_ERROR(VError, "error in change route info", VERR_UNKNOWN);
    return false;
  }

  return true;
}

bool SnoopVirtualNat::doClose()
{
  if (!enabled)
  {
    LOG_DEBUG("enabled is false");
    return true;
  }
  
  virAdapter.close();
  realAdapter.close();
  recoverRouteTable();
  return true;
}

//
// real : 192.168.10.2 255.255.255.0 192.168.10.1
// vir  : 5.1.1.2      255.0.0.0     5.1.1.1
//
// [change]
// C-1 : route change 192.168.10.0 mask 255.255.255.0 5.1.1.2
// C-2 : route change 0.0.0.0      mask 0.0.0.0       5.1.1.1
//
// [recover]
// R-1 : route change 192.168.10.0 mask 255.255.255.0 192.168.10.3
// R-2 : route add 0.0.0.0         mask 0.0.0.0       192.168.10.1
//

bool SnoopVirtualNat::changeRouteTable()
{
  QString command;
  bool res = true;
  
  //
  // C-1
  //
  Ip dst = realNetInfo.ip & realNetInfo.subnet;
  command = "route change " + dst.str() + " mask " + realNetInfo.subnet.str() + " " + virNetInfo.ip.str();
  LOG_DEBUG("C-1 %s", qPrintable(command));
  res = VProcess::run(qPrintable(command));
  if (!res)
  {
    SET_ERROR(VError, qformat("can not run %s", qPrintable(command)), VERR_RUN_PROCESS);
    return false;
  }

  //
  // C-2
  //
  command = "route change 0.0.0.0 mask 0.0.0.0 " + virNetInfo.gateway.str();
  LOG_DEBUG("C-2 %s", qPrintable(command));
  res = VProcess::run(qPrintable(command));
  if (!res)
  {
    SET_ERROR(VError, qformat("can not run %s", qPrintable(command)), VERR_RUN_PROCESS);
    return false;
  }

  return res;

}

bool SnoopVirtualNat::recoverRouteTable()
{
  QString command;
  bool res = true;
  
  //
  // R-1
  //
  Ip dst = realNetInfo.ip & realNetInfo.subnet;
  command = "route change " + dst.str() + " mask " + realNetInfo.subnet.str() + " " + realNetInfo.ip.str();
  LOG_DEBUG("R-1 %s", qPrintable(command));
  res = VProcess::run(qPrintable(command));
  if (!res)
  {
    SET_ERROR(VError, qformat("can not run %s", qPrintable(command)), VERR_RUN_PROCESS);
    return false;
  }

  //
  // R-2 ( // R-2 : route add 0.0.0.0         mask 0.0.0.0       192.168.10.1)
  //
  command = "route add 0.0.0.0 mask 0.0.0.0 " + realNetInfo.gateway.str();
  LOG_DEBUG("R-1 %s", qPrintable(command));
  res = VProcess::run(qPrintable(command));
  if (!res)
  {
    SET_ERROR(VError, qformat("can not run %s", qPrintable(command)), VERR_RUN_PROCESS);
    return false;
  }

  return res;
}

void SnoopVirtualNat::myVirtualRecv(SnoopPacket* packet)
{
  Q_UNUSED(packet) // gilgil temp 2012.06.11
}


void SnoopVirtualNat::myRealRecv(SnoopPacket* packet)
{
  Q_UNUSED(packet) // gilgil temp 2012.06.11
}

void SnoopVirtualNat::load(VXml xml)
{
  virAdapter.load(xml.gotoChild("virtual"));
  virAdapterIndex = virAdapter.adapterIndex;

  realAdapter.load(xml.gotoChild("real"));
  realAdapterIndex = realAdapter.adapterIndex;
}

void SnoopVirtualNat::save(VXml xml)
{
  virAdapter.adapterIndex = virAdapterIndex;
  virAdapter.save(xml.gotoChild("virtual"));

  realAdapter.adapterIndex = realAdapterIndex;
  realAdapter.save(xml.gotoChild("real"));
}

