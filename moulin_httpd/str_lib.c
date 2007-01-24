/*
 * General C String Library (for MUDs)
 * This software is released to the public domain.  2007 Nick "Odis" Cash.
 */

#include "str_lib.h"
#include <string.h>

/*
 * Duration_to_str: Takes an amount of time in seconds and returns long or short format.
 *
 * Return: Returns a pointer to a static character array.
 *
 * Notes: Not thread safe.
 *        Long format (1) = "18 hours, 42 minutes, 38 seconds"
 *        Short format (default) = "18:42:38"
 *        If a negative value is passed for seconds, returns "Permanent"
 */
char *duration_to_str( int secs, int format )
{
  static char buf[MSL+1];
  char seconds[3];
  char minutes[3];
  char hours[8];

  /* check for negative duration */
  if ( secs < 0 )
  {
    sprintf( buf, "Permanent" );
    return buf;
  }
  else
   buf[0] = '\0';

  sprintf( hours, "%d", (secs/3600) );
  secs = secs % 3600;

  sprintf( minutes, "%d", secs / 60 );
  secs = secs % 60;

  sprintf( seconds, "%d", secs );

  /* long format */
  if ( format == 1 )
   sprintf( buf, "%s hours, %s minutes, and %s seconds", hours, minutes, seconds );
  else
   sprintf( buf, "%s:%s:%s", hours, minutes, seconds );

  return buf;
}

/*
 * Int_to_str: Takes an integer and returns it in a specified text format.
 *
 * Return: Returns a pointer to a static character array.
 *
 * Notes: Not thread-safe.
 *        Pass 1 for full integer output. Defaults to (short) numerical version.
 */
char *int_to_str( int num, short full )
{
  static char buf[MSL+1];
  char temp[MSL+1];

  buf[0] = '\0';
  temp[0] = '\0';

  if ( full == 1 )
  {
     int group = 0;
     char  sign[10];
     
     /* string constants */
     char* ones[] = {  "Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
                       "Nine", "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen",
                       "Sixteen", "Seventeen", "Eighteen", "Nineteen" };
     char* tensg[] = { "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" };
     char* groups[] ={ "Hundred", "Thousand", "Million", "Billion" };

     /* if its zero we can bail */
     if ( num == 0 )
     {
      sprintf( buf, "Zero" );
      return buf;
     }

     /* check the sign */
     if ( num < 0 )
     {
       sprintf( sign, "Negative " );
       num = abs(num);
     }
     else
      sign[0] = '\0';

     /* here we go */
     while ( num > 0 )
     {
       char group_desc[MSL];

       int process = (num % 1000);
       num = (num / 1000);

       group_desc[0] = '\0';

       /* figure out the group description */
       {
         int tens = process % 100;
         int hundreds = process / 100;

         if ( hundreds > 0 )
          sprintf( group_desc, "%s %s", ones[hundreds], groups[0] );

         if ( tens > 0 )
         {
           /* anything under twenty is special */
           if ( tens < 20 )
           {
             if ( group_desc[0] != '\0' )
              sprintf( group_desc, "%s %s", group_desc, ones[tens]);
             else
              sprintf( group_desc, "%s", ones[tens] );
           }
           else
           {
                int one = tens % 10;

                tens = (tens / 10) - 2; /* 20's offset */

                strcpy( temp, group_desc );

                if ( temp[0] != '\0' )
                 sprintf( group_desc, "%s %s", temp, tensg[tens] );
                else
                 sprintf( group_desc, "%s", tensg[tens] );

                if ( one > 0 )
                {
                 strcpy( temp, group_desc );

                 if ( temp[0] != '\0' )
                  sprintf( group_desc, "%s %s", temp, ones[one]);
                 else
                  sprintf( group_desc, "%s", ones[one]);
                }
           }
         }
       }

       /* add to the return string */
       if ( group_desc[0] != '\0' )
       {
         if ( group > 0 )
         {
          strcpy(temp, buf);
          sprintf( buf, "%s %s", groups[group], temp );
         }

         if ( buf[0] != '\0' )
         {
          strcpy(temp, buf );
          sprintf( buf, "%s %s", group_desc, temp );
         }
         else
          sprintf( buf, "%s", group_desc );
       }

       group++;
     }

     /* put on the positive or negative */
     sprintf( temp, "%s%s", sign, buf);
     strcpy( buf, temp );
  }
  else
   sprintf( buf, "%d", num );

  return buf;
}

/*
 * Change_case_str: Converts a string to upper or lower case.
 *
 * Return: -1 on bad input, 1 otherwise. Modifies the original string.
 *
 * Notes: Pass 0 to convert to lower, 1 for upper
 */
short change_case_str( char* str, short new_case )
{
  if ( !str )
   return STR_BAD_INPUT;

  /* lower case */
  if ( new_case == 0 )
  {
    for ( ; *str != '\0'; str++ )
     *str = tolower(*str);
  }
  else if ( new_case == 1 )
  {
    for ( ; *str != '\0'; str++ )
     *str = toupper(*str);
  }
  else
   return STR_BAD_INPUT;

  /* no error */
  return STR_SUCCESS;
}

/*
 * Find_between: Finds the string between two specified characters.
 *
 * Return: A pointer to a static buffer containing the string between
 *         the two specified characters.
 *         Returns NULL on error, if supplied bad data, or missing specified characters.
 *
 * Notes: Not thread safe.
 */
char *find_between( char *src, char a, char b )
{
  short found_a = 0, found_b = 0; /* flags */
  static char buf[MSL+1];
  char *buf_ptr = buf;

  /* check input */
  if ( !src )
   return NULL;

  /* loop while we have data */
  while ( *src != '\0' )
  {
    if ( found_a == 0 )
    {
      if ( *src == a ) /* did we find the first character? */
       found_a = 1;
    }
    else
    {
      if ( *src == b ) /* found b, exit */
      {
       found_b = 1;
       break;
      }

     *buf_ptr = *src;
      buf_ptr++;
    }
    src++;
  }

  /* only return data if we found the last delimiter */
  if ( found_b == 1 )
  {
    /* add the null */
    *buf_ptr = '\0';
    return buf;
  }

  return NULL;
}

/*
 * Compare_tokens: Checks to see if all of the tokens in list a are in list b.
 *                 Accepts character to determine what delimiter to use.
 *                 Accepts argument to determine use of prefix checking.
 *
 * Return:  returns 1 of list matches, 0 otherwise
 *          reutrns -1 if supplied bad data
 *
 * Notes:   Doesnt check using prefixes by default. Pass 1 for prefixes for prefix checking.
 *          Both lists must use the same delimiter.
 *          Max of fifty tokens per string.
 *          Case-insensitive checking.
 */
short compare_tokens( char* list_a, char* list_b, char delimiter, short prefixes )
{
 /* check input */
 if ( !list_a || !list_b )
  return STR_BAD_INPUT;

 /* check number of tokens */
 if ( total_tokens( list_a, delimiter) > total_tokens( list_b, delimiter ) )
  return STR_FAILURE;

 /* read in tokens and compare */
 {
   char token_list_a[50][50]; /* max of fifty tokens */
   char token_list_b[50][50];
   int ax = 0, bx = 0, i;

   /* read in list a */
   while ( list_a && list_a[0] != '\0' )
   {
    list_a = grab_token( list_a, token_list_a[ax++], delimiter );

    if ( ax >= 50 )
     break;
   }

   /* read in list b */
   while ( list_b && list_b[0] != '\0' )
   {
    list_b = grab_token( list_b, token_list_b[bx++], delimiter );

    if ( bx >= 50 )
     break;
   }

   /* compare list a to list b*/
   for ( i = 0; i < ax; i++ )
   {
     int ib;
     short found = 0; /* flag */

     /* compare this string against all of the strings in list b */
     for ( ib = 0; ib < bx; ib++ )
     {
      if ( compare_str(token_list_a[i],token_list_b[ib]) )
      {
       found = 1;
       break;
      }
      else if ( prefixes == 1 ) /* check prefixes if applicable */
       if ( prefix_str(token_list_a[i],token_list_b[ib]) )
       {
        found = 1;
        break;
       }
     }

     /* did we find a token in list a that wasnt in list b? */
     if ( found != 1 )
      return STR_FAILURE;
   }
 }

 /* all tokens in list a were in list b */
 return STR_SUCCESS;
}

/*
 * Grab_token: Grabs the first token off a string as noted by a delimiter.
 *
 * Return: Puts token in end_buf string. Returns advanced src pointer.
 *         Returns NULL if supplied bad data or if the src string is exausted of tokens.
 *
 */
char *grab_token( char *src, char* end_buf, char delimiter )
{
  char buf[MSL+1];
  char* buf_ptr = buf;
  char flag = 0;

  /* check data */
  if ( !src || !end_buf )
   return NULL;

  /* copy token */
  while ( *src != '\0' )
  {
    /* check for the end of the token */
    if (*src == delimiter )
    {
      src++; /* chop off delimiter */
      flag = 1;
      break;
    }

    /* copy */
    *buf_ptr++ = *src++;
  }

  /* close end_buf */
  *buf_ptr = '\0';

  /* copy string if delimiter was found */
  if ( flag == 1 )
   strcpy( end_buf, buf );

  /* return advanced src ptr */
  if ( src && src[0] != '\0' )
    return src;
  else
    return NULL;
}

/*
 * Total_tokens: Find the total number of tokens within a string (src)
 *
 * Return: The number of total tokens in src
 *
 * Notes: Uses grab_token for simplicity
 */
int total_tokens( char *src, char delimiter )
{
  int occur = 0;
  char token[MSL+1];

  /* loop to tally the tokens */
  while ( 1 )
  {
    src = grab_token( src, token, delimiter );

    /* if we returned token data */
    if ( token[0] != '\0' )
    {
     occur++;

     /* reset */
     token[0] = '\0';
    }
    else
     break;

    /* check validity of source string */
    if ( !src || src[0] == '\0' )
     break;
  }

  return occur;
}

/*
 * Sentence_from_tokens: Forms a sentence from a list of tokens. Must provide a delimiter.
 *                       Accepts argument to determine use of AND or OR.
 *
 * Return: Pointer to static character array with the sentence.
 *         Returns NULL if supplied bad data
 *
 * Notes: Uses grab_token and total_tokens. Not thread safe.
 *        Pass 1 for OR. Defaults to AND.
 */
char *sentence_from_tokens( char *tokens, char delim, short word )
{
  static char buf[MSL+1];
  char tok[MSL/2];
  int i, bi = 0;

  /* check input */
  if ( !tokens )
   return NULL;

  i = total_tokens( tokens, delim );

  /* make sure we have at least one token */
  if ( i < 1 )
   return NULL;

  /* put this string togeather */
  for ( ; i > 1; i-- )
  {
    tokens = grab_token( tokens, tok, delim );

    /* put the token into buf */
    strcat( buf, tok );

    bi += strlen(tok);
    buf[bi] = ',';
    buf[++bi] = ' ';
    
    /* close string in preperation for the next strcat */
    buf[++bi] = '\0';
  }

  /* check for OR */
  if ( word == 1 )
   strcat( buf, "or " );
  else /* AND */
   strcat( buf, "and " );

  /* put in last token */
  grab_token( tokens, tok, delim );
  strcat( buf, tok );

  return buf;
}

/*
 * Replace_str: Replace all instances of "replace" with "new_str" within "src"
 *
 * Return: Pointer to static character array with new string.
 *         Returns NULL if supplied bad data.
 *
 * Notes: Not thread safe.
 */
char *replace_str( const char *new_str, const char *replace, const char *src )
{
  static char buf[MSL+1];
  char *ri = (char *)replace;
  int bi = 0; /* buf index */
  int ns_length = strlen(new_str); /* new string length */

  /* check input */
  if ( !new_str || !replace || !src )
   return NULL;

  /* iterate while we have data */
  while ( *src != '\0' && bi <= MSL )
  {
    if ( *src == *ri ) /* initiate sub scan? */
    {
      short found = 0; /* flag is set if the string turns not to match */
      char *si_sub = (char *)src;

      si_sub++;
      ri++;

      /* serach for the rest of replace */
      for ( ; *ri != '\0'; ri++, si_sub++ )
      {
        if ( *ri != *si_sub )
        {
          found = 1; /* no match */
          break;
        }
      }

      /* matched */
      if ( found == 0 )
      {
        int x;

        /* put in the new string to replace the old string */
        for ( x = 0; x < ns_length; x++ )
        {
          buf[bi++] = new_str[x];

          /* buffer overflow protection */
          if ( bi == (MSL+1) )
          {
            buf[bi] = '\0';
            return buf;
          }
        }

        src = si_sub; /* advance src ptr to skip the matched portion */
      }

      /* reset ri */
      ri = (char *)replace;
    }

    buf[bi++] = *src;
    src++;
  } /* end while */

  buf[bi] = '\0';
  return buf;
}

/*
 * Prefix_str: Checks to see if string a is a complete prefix to string b
 *
 * Return: Returns true (1) if a is a prefix of b, false (0) if not.
 *         Returns -1 on bad input.
 *
 * Notes: Case-insensitive
 */
short prefix_str(const char *str_a, const char *str_b)
{
  /* check input */
  if ( !str_a || !str_b )
   return STR_BAD_INPUT;

  /* check for empty strings */
  if (str_a[0] == '\0' || str_b[0] == '\0')
   return STR_BAD_INPUT;

  /* analyze input */
  while (*str_a != '\0')
  {
    if (tolower(*str_a++) != tolower(*str_b++))
      return STR_FAILURE;
  }

  return STR_SUCCESS;
}

/*
 * Preix_str_case: Checks to see if string a is a complete prefix to string b
 *
 * Return: Returns true (1) if a is a prefix of b, false (0) if not.
 *         Returns -1 on bad input.
 *
 * Notes: Case-sensitive
 */
short prefix_str_case(const char *str_a, const char *str_b)
{
  /* check input */
  if ( !str_a || !str_b )
   return STR_BAD_INPUT;

  /* check for empty strings */
  if (str_a[0] == '\0' || str_b[0] == '\0')
   return STR_BAD_INPUT;

  /* analyze input */
  while (*str_a != '\0')
  {
    if ((*str_a++) != (*str_b++))
      return STR_FAILURE;
  }

  return STR_SUCCESS;
}

/*
 * Compare_str: Compares string a to string b
 *
 * Return: Returns true (1) if match, false (0) if not a match.
 *         Returns -1 on bad input.
 *
 * Notes: Case-insensitive
 */
short compare_str( const char* str_a, const char* str_b )
{
  /* check input */
  if ( !str_a || !str_b )
   return STR_BAD_INPUT;

  for ( ; *str_a != '\0' && *str_b != '\0'; str_a++, str_b++ )
   if ( tolower(*str_a) != tolower(*str_b) )
    return STR_FAILURE;

  /* check for premature ending */
  if ( tolower(*str_a) != tolower(*str_b) )
   return STR_FAILURE;

  return STR_SUCCESS;
}

/*
 * Compare_str_case: Compares string a to string b
 *
 * Return: Returns true (1) if match, false (0) if not a match.
 *         Returns -1 on bad input.
 *
 * Notes: Case-sensitive
 */
short compare_str_case( const char* str_a, const char* str_b )
{
  /* check input */
  if ( !str_a || !str_b )
   return STR_BAD_INPUT;

  for ( ; *str_a != '\0' && *str_b != '\0'; str_a++, str_b++ )
   if ( *str_a != *str_b )
    return STR_FAILURE;

  /* check for premature ending */
  if ( *str_a != *str_b )
   return STR_FAILURE;

  return STR_SUCCESS;
}

/*
 * Find_str: Seraches a string (search) for a string (find). Case determined by parameter
 *
 * Return:  Returns the number of occurances of "find" within "search".
 *          Returns -1 on error.
 *
 * Notes: Case-insensitive = 0 (FALSE)
 *        Case-sensitive   = 1 (TRUE)
 */
int find_str( const char* find, const char* search, short sense )
{
  int occur = 0; /* the times the string was found within string */
  char *fi; /* find index */
  char *si; /* serach index */
  char *si_sub; /* sub-search index */
  char search_buf[strlen(search)+1]; /* to possibly store a copy of search but case smashed */
  char find_buf[strlen(find)+1]; /* to possibly store a copy of find but case smashed */

  /* check input */
  if ( !find || !search )
   return STR_BAD_INPUT;

  /* prepare for case insensitivity if need be */
  if ( sense == 0 )
  {
    strcpy( search_buf, search );

    /* smash case */
    for ( si = search_buf; *si != '\0'; si++ )
     *si = tolower(*si);

    si = search_buf; /* reset position */

    strcpy( find_buf, find );
    fi = find_buf;

    for ( fi = find_buf; *fi != '\0'; fi++ )
     *fi = tolower(*fi);

    fi = find_buf; /* reset position */
  }
  else
  {
   si = (char *)search;
   fi = (char *)find;
  }


  /* iterate while we have data to search */
  while ( *si != '\0' )
  {
    if ( *si == *fi ) /* initiate sub scan? */
    {
      short found = 0; /* flag is set if the string turns not to match */

      si_sub = si;
      si_sub++;
      fi++;

      /* serach the rest of find */
      for ( ; *fi != '\0'; fi++, si_sub++ )
      {
        if ( *fi != *si_sub )
        {
          found = 1;
          break;
        }
      }


      if ( found == 0 )
       occur++;

      /* reset fi */
      if ( sense == 0 )
       fi = find_buf;
      else
       fi = (char *)find;
    }

    si++;
  } /* end while */

  return occur;
}

/*
 * Wordwrap_str: Formats src for output by adding a string (nstr) every num_columns.
 *
 * Return: pointer to static buffer containing modified output. NULL on error
 *
 * Notes: num_columns defaults to 75 if it is less than or equal to 0. Not thread-safe.
 */
char* wordwrap_str( const char* src, short num_columns, const char* nstr )
{
  /* to avoid buffer overruns use a special buffer */
  char str_buf[MSL+1];
  char *str = str_buf;

  /* return buffer */
  static char buf[MSL+1];
  char* buf_ptr = buf;

  short counter = 0; /* keep track of when to add new string */
  short nstr_len = strlen(nstr);

  /* check input */
  if ( !src || !nstr )
   return NULL;

  if ( num_columns <= 0 )
   num_columns = 75;

  /* copy up to max length of MSL */
  strncpy( str_buf, src, MSL );

  /* loop while we have string data */
  while ( *str != '\0' )
  {
    *buf_ptr = *str; /* copy character */

    /* insert new string? */
    if ( ++counter == num_columns )
    {
      short i;

      /* append new string to the end */
      strcat( buf, nstr );

      /* correct buf_ptr position */
      for ( i = 0; i < nstr_len; i++ )
       buf_ptr++;

      /* skip spaces after newlines to avoid bad looking output */
      if ( nstr[nstr_len-1] == '\n' )
      {
        while (isspace(*(++str))); /* while *(++str) is a space, loop again */
        str--; /* correct pointer */
      }

      counter = 0; /* reset counter */
    }

    buf_ptr++;
    str++;
  }

  return buf;
}
