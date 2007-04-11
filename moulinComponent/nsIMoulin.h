/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM nsIMoulin.idl
 */

#ifndef __gen_nsIMoulin_h__
#define __gen_nsIMoulin_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIMoulin */
#define NS_IMOULIN_IID_STR "00536f33-2c53-457c-b74a-893a04a07d1d"

#define NS_IMOULIN_IID \
  {0x00536f33, 0x2c53, 0x457c, \
    { 0xb7, 0x4a, 0x89, 0x3a, 0x04, 0xa0, 0x7d, 0x1d }}

class NS_NO_VTABLE nsIMoulin : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IMOULIN_IID)

  /* AUTF8String retrievePageContent (in string archivefile, in PRUint32 startoffset, in PRUint32 length); */
  NS_IMETHOD RetrievePageContent(const char *archivefile, PRUint32 startoffset, PRUint32 length, nsACString & _retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIMOULIN \
  NS_IMETHOD RetrievePageContent(const char *archivefile, PRUint32 startoffset, PRUint32 length, nsACString & _retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIMOULIN(_to) \
  NS_IMETHOD RetrievePageContent(const char *archivefile, PRUint32 startoffset, PRUint32 length, nsACString & _retval) { return _to RetrievePageContent(archivefile, startoffset, length, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIMOULIN(_to) \
  NS_IMETHOD RetrievePageContent(const char *archivefile, PRUint32 startoffset, PRUint32 length, nsACString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->RetrievePageContent(archivefile, startoffset, length, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsMoulin : public nsIMoulin
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIMOULIN

  nsMoulin();

private:
  ~nsMoulin();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsMoulin, nsIMoulin)

nsMoulin::nsMoulin()
{
  /* member initializers and constructor code */
}

nsMoulin::~nsMoulin()
{
  /* destructor code */
}

/* AUTF8String retrievePageContent (in string archivefile, in PRUint32 startoffset, in PRUint32 length); */
NS_IMETHODIMP nsMoulin::RetrievePageContent(const char *archivefile, PRUint32 startoffset, PRUint32 length, nsACString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIMoulin_h__ */
