/****************************************************************** DATEFMT.H
 *                                                                          *
 *                             Date Formatter                               *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Country.h"
#include "DateFmt.h"
#include "MemSize.h"
#include "ReString.h"

extern char *FormatDate ( char *Format, char *Result, int ResultSize, time_t Time ) {

   // Get the local time information.
   if ( Time == 0 ) 
      Time = time ( 0 ) ;
   struct tm Tm ; memcpy ( &Tm, localtime(&Time), sizeof(Tm) ) ;

   // Get some language-specific supporting data.
   CountryInfo Locale ;
   ResourceString DaysOfWeek ( LibraryHandle, IDS_DAYSOFWEEK ) ;
   ResourceString WeekdayNames [7] = {
      ResourceString ( LibraryHandle, IDS_DATEFMT_DAY0 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_DAY1 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_DAY2 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_DAY3 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_DAY4 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_DAY5 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_DAY6 ),
   } ;
   ResourceString MonthsOfYear ( LibraryHandle, IDS_DATEFMT_MONS ) ;
   ResourceString MonthNames [12] = {
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH1 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH2 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH3 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH4 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH5 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH6 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH7 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH8 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH9 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH10 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH11 ),
      ResourceString ( LibraryHandle, IDS_DATEFMT_MONTH12 ),
   } ;                                                  
                                                        
   // Format the result.                                
   memset ( Result, 0, ResultSize ) ;                   
   char *p1 ( Format ), *p2 ( Result ) ;
   while ( *p1 && ( p2-Result < ResultSize-1 ) ) {

      if ( *p1 == '%' ) {
         char Text [80] ;

         switch ( *(p1+1) ) {

            // Replace the 'a' (abbreviated weekday name) function.
            case 'a': {
               sprintf ( Text, "%0.*s", strlen(PCHAR(DaysOfWeek))/7, PSZ(DaysOfWeek)+Tm.tm_wday*(strlen(PCHAR(DaysOfWeek))/7) ) ;
               for ( int i=0; Text[i] && p2-Result<ResultSize-1; i++ ) 
                  *p2++ = Text[i] ;
               p1 += 2 ;
               break; }

            // Replace the 'A' (full weekday name) function.
            case 'A': {
               strcpy ( Text, PCHAR(WeekdayNames[Tm.tm_wday]) ) ;
               for ( int i=0; Text[i] && p2-Result<ResultSize-1; i++ ) 
                  *p2++ = Text[i] ;
               p1 += 2 ;
               break; }

            // Replace the 'b' (abbreviated month name) function.
            case 'b': case 'h': {
               sprintf ( Text, "%0.*s", strlen(PCHAR(MonthsOfYear))/12, PSZ(MonthsOfYear)+Tm.tm_mon*(strlen(PCHAR(MonthsOfYear))/12) ) ;
               for ( int i=0; Text[i] && p2-Result<ResultSize-1; i++ ) 
                  *p2++ = Text[i] ;
               p1 += 2 ;
               break; }

            // Replace the 'B' (full month name) function.
            case 'B': {
               strcpy ( Text, PCHAR(MonthNames[Tm.tm_mon]) ) ;
               for ( int i=0; Text[i] && p2-Result<ResultSize-1; i++ ) 
                  *p2++ = Text[i] ;
               p1 += 2 ;
               break; }

            // Replace the 'p' (AM/PM) function.
            case 'p': {
               if ( Tm.tm_hour ) {
                  if ( Tm.tm_hour < 12 ) {
                     strcpy ( Text, Locale.szAm ) ;
                  } else if ( Tm.tm_hour == 12 ) {
                     if ( Tm.tm_min )
                        strcpy ( Text, Locale.szPm ) ;
                     else
                        strcpy ( Text, Locale.szAm ) ;
                  } else if ( Tm.tm_hour > 12 ) {
                     strcpy ( Text, Locale.szPm ) ;
                  } /* endif */
               } else {
                  if ( Tm.tm_min )
                     strcpy ( Text, Locale.szAm ) ;
                  else
                     strcpy ( Text, Locale.szPm ) ;
               } /* endif */
               for ( int i=0; Text[i] && p2-Result<ResultSize-1; i++ ) 
                  *p2++ = Text[i] ;
               p1 += 2 ;
               break; }

            // Replace the 'r' (HH:MM:SS AM/PM) function.
            case 'r': {
               int Hour = Tm.tm_hour % 12 ;  Hour = Hour ? Hour : 12 ;
               sprintf ( Text, "%02u:%02u:%02u ", Hour, Tm.tm_min, Tm.tm_sec ) ;
               if ( Tm.tm_hour ) {
                  if ( Tm.tm_hour < 12 ) {
                     strcat ( Text, Locale.szAm ) ;
                  } else if ( Tm.tm_hour == 12 ) {
                     if ( Tm.tm_min )
                        strcat ( Text, Locale.szPm ) ;
                     else
                        strcat ( Text, Locale.szAm ) ;
                  } else if ( Tm.tm_hour > 12 ) {
                     strcat ( Text, Locale.szPm ) ;
                  } /* endif */
               } else {
                  if ( Tm.tm_min )
                     strcat ( Text, Locale.szAm ) ;
                  else
                     strcat ( Text, Locale.szPm ) ;
               } /* endif */
               for ( int i=0; Text[i] && p2-Result<ResultSize-1; i++ ) 
                  *p2++ = Text[i] ;
               p1 += 2 ;
               break; }

            // Let all other valid functions be handled by strftime itself.
            case 'c':
            case 'D': case 'd':
            case 'H': 
            case 'I': 
            case 'j': 
            case 'S': 
            case 'T': 
            case 'U': 
            case 'M': case 'm': 
            case 'W': case 'w': 
            case 'X': case 'x': 
            case 'Y': case 'y': 
            case 'Z': case 'z': 
            case '%': {
               char Source [3] = { *p1, *(p1+1), 0 } ;
               strftime ( Text, sizeof(Text), Source, &Tm ) ;
               for ( int i=0; Text[i] && p2-Result<ResultSize-1; i++ ) 
                  *p2++ = Text[i] ;
               p1 += 2 ;
               break; }

            // Replace default (invalid) function handling.
            default: {
               *p2++ = *p1++ ;
               *p2++ = *p1++ ;
               break; }

         } /* endswitch */

      } else {
         *p2++ = *p1++ ;

      } /* endif */

   } /* endwhile */

   // Return the pointer to the result, just to be nice.
   return ( Result ) ;
}
