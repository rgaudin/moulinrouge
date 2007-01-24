/* moulin.c
**
*/

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <bzlib.h>
#include "str_lib.h"
#define SQLITE_OK           0
//#define DEBUG

typedef struct wiki_pages {
    short unsigned int archive;
    unsigned int startoff;
    int length;
    } wiki_page;
                    
static char ARCHIVE_LOCATION[50]        = "../datas/ark/%s.bz2";
static char MATH_ARCHIVE_LOCATION[50]   = "../datas/math_ark/%s";
static char INDEX_LOCATION[50]          = "../datas/index.db";
static char MATH_INDEX_LOCATION[50]     = "../datas/math_index.db";
//static char INDEX_LOCATION[50]          = "/tmp/.moulin_index.db";
//static char MATH_INDEX_LOCATION[50]     = "/tmp/.moulin_math_index.db";
static char SINGLE_REQ[180]             = "SELECT a.title, a.archive, a.startoff, b.title, b.archive, b.startoff, a.id, a.redirect FROM windex a, windex b WHERE b.id = (a.id + 1) AND a.title = \"%s\";";
static char MATH_REQ[180]               = "SELECT a.md5, a.archive, a.startoff, b.md5, b.archive, b.startoff, a.id FROM mindex a, mindex b WHERE b.id = (a.id + 1) AND a.md5 = \"%s\";";
static char SEARCH_REQ[180]             = "SELECT title FROM windex WHERE stdtitle LIKE \"%s%s%s\" LIMIT 0, 20;";
char headd[1024]                        = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"fr\" lang=\"fr\" dir=\"ltr\">\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n<title>%s</title>\n<style type=\"text/css\" media=\"screen,projection\">@import \"main.css\";</style>\n<link rel=\"stylesheet\" type=\"text/css\" media=\"print\" href=\"print.css\" />\n<script type=\"text/javascript\" src=\"wikibits.js\"></script>\n<style type=\"text/css\">\n@import \"wikipedia.css\";\n@import \"moulin.css\";\n</style>\n</head>\n<body  class=\"ns-0 ltr\">\n<div id=\"globalWrapper\">\n<div id=\"column-content\">\n";
char searchhead[100]                    = "%s<h1>Resultats de la recherche (%s) :</h1>\n<ul>";
char result_row[100]                    = "<li><a href='_WikiPage:%s'>%s</a></li>";
char *sp_array[50]                      = {"Â","Á","À","È","É","Ê","Ë","Í","Ì","Î","Ò","Ó","Ô","Œ","Ñ","Ú","Ù","Ü","Û","Ÿ","Ç","«","»","¿","¡","—","â","á","à","è","é","ê","ë","í","ì","ï","î","ò","ó","ô","œ","ñ","ú","ü","ù","û","ÿ","ç"};
char *std_array[50]                     = {"A","A","A","E","E","E","E","I","I","I","O","O","O","OE","N","U","U","U","U","Y","C","<",">","?","!","-","a","a","a","e","e","e","e","i","i","i","i","o","o","o","oe","n","u","u","u","u","y","c"};
char *searchstr;

wiki_page   current_page;

char html_content[131072];
sqlite3 *db;
char *zErrMsg = 0;
int rc;
char req[400];

int wiki_callback (void *NotUsed, int argc, char **argv, char **azColName)
    {
        char path[100];
        char *archive = argv[1];
        unsigned int startoff = atoi(argv[2]);
        int length;
        if ( atoi(argv[5]) == 0 )
            length = -1;
        else
            length = atoi(argv[5]) - atoi(argv[2]);

        if (strlen(argv[7]) > 1) {
            char *newtitle = argv[7];
            newtitle = replace_str( "", "#REDIRECT[[", newtitle );
            newtitle = replace_str( "", "]]", newtitle );
#ifdef DEBUG
            printf( "goto: %s\n", newtitle);
#endif
            char redir[300] = "Location: /_WikiPage:";
            strncat( redir, newtitle, strlen(newtitle) );
#ifdef DEBUG
            printf("header-- %s", redir);
#endif
            add_headers( 301, "Moved Permanently", redir, "UTF-8", "text/html", 0, 1293753600 );
            send_response();
            return 0;
        }
        
        if (length == 0) {
            send_error( 404, argv[0], "", "Cet article est vide ou n'a pas pu etre lu." );
        }

#ifdef DEBUG        
        printf( "%s: %s, %d, %d\n", argv[0], archive, startoff, length );
#endif

        sprintf( path, ARCHIVE_LOCATION, archive );
#ifdef DEBUG
        printf( "%s\n", path );
#endif

        FILE*   f;
        BZFILE* b;
        int     nBuf;
        char    buf[ 131072 ];
        int     bzerror;
        int     so = startoff;
        int     len = length;
        int     rem = 0;
        int     i = 0;
        char    buf2[ len ];

        f = fopen ( path, "r" );
        if ( !f ) {
          send_error( 404, argv[0], "", "Cette page est indisponible : Le fichier source est illisible (Not Found)." );
        }
        
        sprintf( html_content, headd, replace_str( " ", "_", argv[0]) );
        
        b = BZ2_bzReadOpen ( &bzerror, f, 0, 0, NULL, 0 );
        if ( bzerror != BZ_OK ) {
          BZ2_bzReadClose ( &bzerror, b );
          /* handle error */
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
                    strncat( html_content, buf2, len );
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
        

        add_headers( 200, "Ok", "", "UTF-8", "text/html", strlen(html_content), 1293753600 );
        send_response();

        my_write( html_content, strlen(html_content) );
        
        return 0;
    }

int wiki_callback_search (void *NotUsed, int argc, char **argv, char **azColName)
    {
        char *title = replace_str( " ", "_", argv[0]);
        char *url = argv[0];
        char html_row[200];
        sprintf( html_row, result_row, url, title );
#ifdef DEBUG
        printf( "result: %s\n", html_row );
#endif
        strncat( html_content, html_row, strlen(html_row) );
        return 0;
    }

void char_sp2std( )
    {
        int i;
        for (i=0;i<47;i++) {
            searchstr = replace_str( std_array[i], sp_array[i], searchstr );
        }
    }
    
int wiki_math_callback (void *NotUsed, int argc, char **argv, char **azColName)
    {
        char path[100];
        char *archive = argv[1];
        unsigned int startoff = atoi(argv[2]);
        int length;
        if ( atoi(argv[5]) == 0 )
            length = -1;
        else
            length = atoi(argv[5]) - atoi(argv[2]);
#ifdef DEBUG
        printf( "MathImage: %s, %s, %d, %d\n", argv[0], archive, startoff, length );
#endif

        sprintf( path, MATH_ARCHIVE_LOCATION, archive );
#ifdef DEBUG        
        printf( "%s\n", path );
#endif

        FILE*   f;
        int     so = startoff;
        int     len = length;
        char    buf2[ len ];
        
        f = fopen ( path, "r" );
        if ( !f ) {
          printf( "Unable to open math archive.\n");
        }
        
        add_headers( 200, "Ok", "", "", "image/png", len, 1293753600 );
        send_response();
        
        fseek(f, so, SEEK_SET);

        fread(buf2, sizeof(buf2), 1, f);
        my_write( buf2, sizeof(buf2) );
        
        return 0;
    }

