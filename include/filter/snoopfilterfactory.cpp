#include <SnoopFilterFactory>

#include <SnoopBpFilter>
#include <SnoopProcessFilter>
#include <VDebugNew>

// ----------------------------------------------------------------------------
// SnoopFilterFactory
// ----------------------------------------------------------------------------
void SnoopFilterFactory::explicitLink()
{
  SnoopBpFilter      bpFilter;
#ifdef WIN32
  SnoopProcessFilter processFilter;
#endif // WIN32
}
