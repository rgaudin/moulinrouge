# Makefile for moulin_httpd

BINDIR =	/usr/local/sbin
MANDIR =	/usr/local/man
CDEFS =		${SSL_DEFS} ${SSL_INC}
CFLAGS =	-O ${CDEFS}
LDFLAGS =	-s
### Linux i386 + Windows (cygin)
#CFLAGS =	-O ${CDEFS} -pthread
#CC =		gcc
#LDLIBS =	/usr/lib/libcrypt.a /usr/lib/libsqlite3.a /usr/lib/libbz2.a
### MAC OS X
CC = gcc -Os -Wall -framework AppKit -isysroot /Developer/SDKs/MacOSX10.4u.sdk
LDLIBS =	/usr/lib/libcrypto.dylib /Volumes/Hobbes/sqlite-build/compiled/release-universal/lib/libsqlite3.0.dylib /usr/lib/libbz2.dylib

all:		moulin_httpd

moulin_httpd:   mini_httpd.o match.o tdate_parse.o str_lib.o
	${CC} ${CFLAGS} ${LDFLAGS} mini_httpd.o match.o str_lib.o tdate_parse.o ${LDLIBS} -o moulin_httpd

mini_httpd.o:	mini_httpd.c version.h port.h match.h tdate_parse.h mime_encodings.h mime_types.h str_lib.h moulin.c
	${CC} ${CFLAGS} -c mini_httpd.c

match.o:	match.c match.h
	${CC} ${CFLAGS} -c match.c

str_lib.o:	str_lib.c str_lib.h
	${CC} ${CFLAGS} -c str_lib.c

tdate_parse.o:	tdate_parse.c tdate_parse.h
	${CC} ${CFLAGS} -c tdate_parse.c

mime_encodings.h: mime_encodings.txt
	rm -f mime_encodings.h
	sed < mime_encodings.txt > mime_encodings.h \
	  -e 's/#.*//' -e 's/[ 	]*$$//' -e '/^$$/d' \
	  -e 's/[ 	][ 	]*/", 0, "/' -e 's/^/{ "/' -e 's/$$/", 0 },/'

mime_types.h: mime_types.txt
	rm -f mime_types.h
	sed < mime_types.txt > mime_types.h \
	  -e 's/#.*//' -e 's/[ 	]*$$//' -e '/^$$/d' \
	  -e 's/[ 	][ 	]*/", 0, "/' -e 's/^/{ "/' -e 's/$$/", 0 },/'

clean:
	rm -f moulin_httpd moulin_httpd.exe mime_encodings.h mime_types.h htpasswd mini_httpd.rnd *.o core core.* *.core

universal: moulin_httpd
	${CC} ${CFLAGS} -arch i386 -c mini_httpd.c
	${CC} ${CFLAGS} -arch i386 -c match.c
	${CC} ${CFLAGS} -arch i386 -c tdate_parse.c
	${CC} ${CFLAGS} -arch i386 -c str_lib.c
	${CC} -arch i386 mini_httpd.o match.o str_lib.o tdate_parse.o ${LDLIBS} -o moulin_httpd-i386
	${CC} ${CFLAGS} -arch ppc -c mini_httpd.c
	${CC} ${CFLAGS} -arch ppc -c match.c
	${CC} ${CFLAGS} -arch ppc -c tdate_parse.c
	${CC} ${CFLAGS} -arch ppc -c str_lib.c	
	${CC} -arch ppc mini_httpd.o match.o str_lib.o tdate_parse.o ${LDLIBS} -o moulin_httpd-ppc
	lipo -create -output moulin_httpd moulin_httpd-ppc moulin_httpd-i386	
	rm moulin_httpd-i386
	rm moulin_httpd-ppc
