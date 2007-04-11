#include "nsIMoulin.h"
// 759132FB-D0C5-42B7-B9AD-A7194A2494D3
#define NS_MOULIN_CID \
{ 0x759132fb, 0xd0c5, 0x42B7, { 0xb9, 0xad, 0xa7, 0x19, 0x4a, 0x24, 0x94, 0xd3 } }
#define NS_MOULIN_CONTRACTID "@kunnafoni.org/cpp_nsMoulin;1"
class nsMoulinImpl : public nsIMoulin
{
public:
    nsMoulinImpl( );
    virtual ~nsMoulinImpl( );
    // nsISupports interface
    NS_DECL_ISUPPORTS
    NS_DECL_NSIMOULIN
};
