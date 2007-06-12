/*  moulin - offline wikipedia distro
    Copyright (C) 2006-2007, Kunnafoni Foundation and contributors
    
    Contributor(s):
        reg <reg@nurv.fr>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "nsIGenericFactory.h"
#include "bzlib.h"
#include "zlib.h"
#include "nsIMoulin.h"

#include "plstr.h"
#include "stdio.h"
#include "nsCOMPtr.h"
#include "nsMemory.h"
#include "prio.h"

#include "nsXPCOM.h"
#include "nsIServiceManager.h"
#include "nsIFile.h"
#include "nsIProperties.h"
#include "nsDirectoryServiceDefs.h"
#include "nsEmbedString.h"

// 759132FB-D0C5-42B7-B9AD-A7194A2494D3
#define NS_MOULIN_CID \
{ 0x759132fb, 0xd0c5, 0x42B7, { 0xb9, 0xad, 0xa7, 0x19, 0x4a, 0x24, 0x94, 0xd3 } }
#define NS_MOULIN_CONTRACTID "@kunnafoni.org/cpp_nsMoulin;1"
class nsMoulinImpl : public nsIMoulin
{
public:
    nsMoulinImpl( );
    virtual ~nsMoulinImpl( );
    NS_DECL_ISUPPORTS
    NS_DECL_NSIMOULIN
};

nsMoulinImpl::nsMoulinImpl( )
{
    NS_INIT_ISUPPORTS();
}
nsMoulinImpl::~nsMoulinImpl( )
{

}

NS_IMPL_ISUPPORTS1_CI(nsMoulinImpl, nsIMoulin);

NS_IMETHODIMP
nsMoulinImpl::RetrieveGzipContent(const char *archivefile, PRUint32 startoffset, PRInt32 length, nsACString & _retval)
{
	long     so = startoffset; //startoffset;
	long     len = length; //length;
	int     rem = 0;
	int     i = 0;

	int nl = so / 131072;
	rem = so % 131072;

	gzFile infile = gzopen(archivefile, "rb");

	char buffer[131072];
	char *buffer2;
	if (len == -1) {
		buffer2 = new char[131072];
	 } else {
		buffer2 = new char[len];
	}
	int num_read = 0;

	while ( i <= nl + 1) {
		if (i < nl) {
			num_read = gzread(infile, buffer, sizeof(buffer));
		}
		if (i == nl + 1) {
			num_read = gzread(infile, buffer, rem);
		}
		i++;
	}
	
	if (len == -1) { // last article in ark
		while (num_read != 0) {
			num_read = gzread(infile, buffer2, 131072);
			_retval.Append(buffer2, num_read);
		}
	} else {
		num_read = gzread(infile, buffer2, len);
		_retval.Append(buffer2, num_read);
	}
	
	gzclose(infile);
    
	return NS_OK;
}

NS_IMETHODIMP

nsMoulinImpl::RetrieveBzip2Content(const char *archivefile, PRUint32 startoffset, PRInt32 length, nsACString & _retval)
{
	FILE*   f;
	BZFILE* b;
	long     nBuf;
	char    buf[ 131072 ];
	int     bzerror;
	long     so = startoffset;
	long     len = length;
	int     rem = 0;
	int     i = 0;

	char *buf2;
	if (len == -1) {
		buf2 = new char[131072];
	 } else {
		buf2 = new char[len];
	}

	f = fopen ( archivefile, "rb" );
	if ( !f ) {
		// error ; can't open file
        _retval.Assign(NS_LITERAL_CSTRING("can not fopen"));
        return NS_OK;
	}

	b = BZ2_bzReadOpen ( &bzerror, f, 0, 0, NULL, 0 );
	if ( bzerror != BZ_OK ) {
		BZ2_bzReadClose ( &bzerror, b );
		_retval.Assign(NS_LITERAL_CSTRING("can not BZ2_bzReadOpen"));
        return NS_OK;
	}

	int nl = so / 131072;
	rem = so % 131072;

	bzerror = BZ_OK;
	while ( bzerror == BZ_OK && i <= nl + 1) {
		if (i < nl) {
			nBuf = BZ2_bzRead ( &bzerror, b, buf, 131072 );
		}
		if (i == nl + 1) {
			nBuf = BZ2_bzRead ( &bzerror, b, buf, rem );
		}
		i++;
	}
	
	if (len == -1) { // last article in ark
		while (bzerror == BZ_OK) {
			nBuf = BZ2_bzRead ( &bzerror, b, buf2, 131072 );
			_retval.Append(buf2, nBuf);
		}
	} else {
		nBuf = BZ2_bzRead ( &bzerror, b, buf2, len );
		_retval.Append(buf2, nBuf);
	}
	
	if ( bzerror != BZ_STREAM_END ) {
		BZ2_bzReadClose ( &bzerror, b );
		// handle error
	} else {
		BZ2_bzReadClose ( &bzerror, b );
	}
    //_retval.Assign(NS_LITERAL_CSTRING("shit"));
	return NS_OK;
}

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
