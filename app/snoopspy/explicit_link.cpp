// #include "explicit_link.h" // gilgil temp 2013.12.06
#include <VMyObject> // gilgil temp 2012.07.30
#include <VNetFactory> // gilgil temp 2012.07.30
#include <SnoopCaptureFactory>
#include <SnoopFilterFactory>
#include <SnoopProcessFactory>
#include <VLog>

void showList()
{
  VMetaClassMap& map = VMetaClassMap::instance();
  for (VMetaClassMap::iterator it = map.begin(); it != map.end(); it++)
  {
    VMetaClassList& list = it->second;
    foreach(VMetaClass* metaClass, list)
    {
      LOG_DEBUG("className=%s categoryName=%s", metaClass->className(), metaClass->categoryName());
    }
    LOG_DEBUG("");
  }
}

void explicitLink()
{
  VMyObject::explicitLink(); // gilgil temp 2012.07.30
  VNetFactory::explicitLink(); // gilgil temp 2012.07.30
  SnoopCaptureFactory::explicitLink();
  SnoopFilterFactory::explicitLink();
  SnoopProcessFactory::explicitLink();
  //showList();
}
