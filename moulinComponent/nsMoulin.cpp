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

#include "plstr.h"
#include "stdio.h"
#include "nsMoulin.h"
#include "nsCOMPtr.h"
#include "nsMemory.h"
#include "prio.h"

#include "nsXPCOM.h"
#include "nsIServiceManager.h"
#include "nsIFile.h"
#include "nsIProperties.h"
#include "nsDirectoryServiceDefs.h"
#include "nsEmbedString.h"

#include <bzlib.h>#define BZREAD_CHUNK_SIZE 131072

// c++ constructor
nsMoulinImpl::nsMoulinImpl( )
{
    NS_INIT_ISUPPORTS();
}
// c++ destructor
nsMoulinImpl::~nsMoulinImpl( )
{

}

NS_IMPL_ISUPPORTS1_CI(nsMoulinImpl, nsIMoulin);

NS_IMETHODIMP
nsMoulinImpl::RetrievePageContent(const char *archivefile, PRUint32 startoffset, PRUint32 length, nsACString & _retval)
{

	FILE*   f;
	BZFILE* b;
	int     nBuf;
	char    buf[ 131072 ];
	int     bzerror;
	int     so = startoffset;
	int     len = length;
	int     rem = 0;
	int     i = 0;
	char    buf2[ len ];

	f = fopen ( archivefile, "r" );
	if ( !f ) {
		// error ; can't open file
	}
        
	b = BZ2_bzReadOpen ( &bzerror, f, 0, 0, NULL, 0 );
	if ( bzerror != BZ_OK ) {
		BZ2_bzReadClose ( &bzerror, b );
		// error ; bzip can't open file.
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
			nBuf = BZ2_bzRead ( &bzerror, b, buf2, len );
			if ( bzerror == BZ_OK ) {
				// add buff2 to output buffer
				_retval.Append(buf2, strlen(buf2));
			}
		}
		i++;
	}
	if ( bzerror != BZ_STREAM_END ) {
		BZ2_bzReadClose ( &bzerror, b );
		// handle error
	} else {
		BZ2_bzReadClose ( &bzerror, b );
	}

	return NS_OK;
}
