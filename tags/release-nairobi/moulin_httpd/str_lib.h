/*
 * General C String Library (for MUDs)
 * This software is released to the public domain.  2007 Nick "Odis" Cash.
 */

#ifndef INC_STR_LIB
#define INC_STR_LIB

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

/* Returns used in the library */
typedef enum { STR_BAD_INPUT = -1, STR_FAILURE = 0, STR_SUCCESS = 1 } str_returns;

#ifndef MSL
 #ifdef MAX_STRING_LENGTH
  #define MSL MAX_STRING_LENGTH
 #else
  #define MSL 4096
 #endif
#endif

/*
 * End-User Functions within str_lib.c
 */
 
/* returns time in a specific format based off of seconds */
char *duration_to_str( int secs, int format );

/* returns a string or english version of the number provided */
char *int_to_str( int num, short full );

/* convert a string totally to upper or lower case */
short change_case_str( char* str, short new_case );

/* find the contents of a string between two specified characters */
char *find_between( char *src, char a, char b );

/* replace all instances of "replace" with "new_str" within "src" */
char *replace_str( const char *new_str, const char *replace, const char *src );

/* returns true (1) if str_a is a prefix of str_b. Case-insensitive */
short prefix_str(const char *str_a, const char *str_b);

/* Case-sensitive version of prefix_str */
short prefix_str_case(const char *str_a, const char *str_b);

/* returns true (1) if str_a is equal to str_b. Case-insensitive */
short compare_str( const char* str_a, const char* str_b );

/* Case-sensitive version of compare_str */
short compare_str_case( const char* str_a, const char* str_b );

/* Searches a string (search) for a string (find). Case determined by parameter */
int find_str( const char* find, const char* search, short sense );

/* adds the string "nstr" every num_columns in src. */
char* wordwrap_str( const char* src, short num_columns, const char* nstr );

/* Token Handling Functions */

/* Checks to see if all of the tokens in list a are in list b */
short compare_tokens( char* list_a, char* list_b, char delimiter, short prefixes );

/* pulls the first token from src and puts it in end_buf */
char *grab_token( char *src, char* end_buf, char delimiter );

/* returns the total number of tokens in a string */
int total_tokens( char *src, char delimiter );

/* creates an english list using the tokens */
char *sentence_from_tokens( char *tokens, char delim, short word );

#endif /* INC_STR_LIB */
