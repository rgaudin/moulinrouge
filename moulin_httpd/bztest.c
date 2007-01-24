#include <bzlib.h>
// pos: 6377498
// len: 54501
int main()
{
    FILE*   f;
    BZFILE* b;
    int     nBuf, nBuf2;
    char    buf[ 131072 ];
    int     bzerror;
    int     nWritten;
    int     so = 6377498;
    int     len = 54501;
    int     rem = 0;
    int     i = 0;
    char    buf2[ len ];

    f = fopen ( "0-14.bz2", "r" );
    if ( !f ) {
      /* handle error */
    }
    
    b = BZ2_bzReadOpen ( &bzerror, f, 0, 0, NULL, 0 );
    if ( bzerror != BZ_OK ) {
      BZ2_bzReadClose ( &bzerror, b );
      /* handle error */
    }
    
    int nl = so / 131072;
    rem = so % 131072;

    bzerror = BZ_OK;
    while ( bzerror == BZ_OK && i <= nl + 1) {
        //printf("looping %d\n", i);
        if (i < nl) {
            nBuf = BZ2_bzRead ( &bzerror, b, buf, 131072 );
        }
        if (i == nl + 1) {
            nBuf = BZ2_bzRead ( &bzerror, b, buf, rem );
            nBuf = BZ2_bzRead ( &bzerror, b, buf2, len );
            if ( bzerror == BZ_OK ) {
                printf("%s", buf2);
            }
        }
        i++;
    }
    if ( bzerror != BZ_STREAM_END ) {
       BZ2_bzReadClose ( &bzerror, b );
       /* handle error */
    } else {
       BZ2_bzReadClose ( &bzerror, b );
    }
    
    return 0;
}
