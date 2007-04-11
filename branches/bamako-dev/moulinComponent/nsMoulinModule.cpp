#include "nsIGenericFactory.h"
#include "nsMoulin.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(nsMoulinImpl)
static NS_METHOD nsMoulinRegistrationProc(nsIComponentManager *aCompMgr,
                                          nsIFile *aPath,
                                          const char *registryLocation,
                                          const char *componentType,
                                          const nsModuleComponentInfo *info)
{
    return NS_OK;
}
static NS_METHOD nsMoulinUnregistrationProc(nsIComponentManager *aCompMgr,
                                            nsIFile *aPath,
                                            const char *registryLocation,
                                            const nsModuleComponentInfo *info)
{
    return NS_OK;
}
// For each class that wishes to support nsIClassInfo, add a line like this
NS_DECL_CLASSINFO(nsMoulinImpl)
static nsModuleComponentInfo components[ ] =
{
  { "moulin Component",    // a message to display when component is loaded
    NS_MOULIN_CID,           // our UUID 
    NS_MOULIN_CONTRACTID,    // our human readable PROGID or CLSID
    nsMoulinImplConstructor,
    nsMoulinRegistrationProc      /* NULL if you dont need one */,
    nsMoulinUnregistrationProc    /* NULL if you dont need one */,
    NULL /* no factory destructor */,
    NS_CI_INTERFACE_GETTER_NAME(nsMoulinImpl),
    NULL /* no language helper */,
    &NS_CLASSINFO_NAME(nsMoulinImpl)
  }
};
NS_IMPL_NSGETMODULE(nsMoulinModule, components)
