/**************************************************************** COUNTRY.CPP
 *                                                                          *
 *                        Country Information Object                        *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Country.h"

CountryInfo::CountryInfo ( ) {

   #ifdef __OS2__

      COUNTRYINFO Info ;
      COUNTRYCODE Code = { 0, 0 } ;
      ULONG Count ;
      ULONG Status = DosGetCtryInfo ( sizeof(Info), &Code, &Info, &Count ) ;
      if ( !Status ) {
         fsDateFmt = Info.fsDateFmt ;
         fsTimeFmt = Info.fsTimeFmt ;
         strcpy ( szDateSeparator, Info.szDateSeparator ) ;
         strcpy ( szTimeSeparator, Info.szTimeSeparator ) ;
         strcpy ( szThousandsSeparator, Info.szThousandsSeparator ) ;
      } else {
         fsDateFmt = DATEFMT_MM_DD_YY ;
         fsTimeFmt = 0 ;
         strcpy ( szDateSeparator, "/" ) ;
         strcpy ( szTimeSeparator, ":" ) ;
         strcpy ( szThousandsSeparator, "," ) ;
      } /* endif */

      char Text [10] ;
      ULONG Size = 10 ;
      if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iDate"), Text, &Size ) )
         fsDateFmt = atoi ( Text ) ;

      Size = 10 ;
      if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iTime"), Text, &Size ) )
         fsTimeFmt = UCHAR ( atoi ( Text ) ) ;

      Size = 10 ;
      if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iCountry"), Text, &Size ) )
         idCountry = atoi ( Text ) ;

      Size = 2 ;
      PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sDate"), szDateSeparator, &Size ) ;

      Size = 2 ;
      PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sTime"), szTimeSeparator, &Size ) ;

      Size = 8 ; strcpy ( szAm, "am" ) ;
      PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("s1159"), szAm, &Size ) ;

      Size = 8 ; strcpy ( szPm, "pm" ) ;
      PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("s2359"), szPm, &Size ) ;

      Size = 2 ;
      PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("sThousand"), szThousandsSeparator, &Size ) ;

   #else

		char Text [10] ;
      GetLocaleInfo ( 0, LOCALE_IDATE,     Text,                 sizeof(Text)                 ) ;
		fsDateFmt = atoi ( Text ) ;
      GetLocaleInfo ( 0, LOCALE_ITIME,     Text,                 sizeof(Text)                 ) ;
		fsTimeFmt = atoi ( Text ) ;
      GetLocaleInfo ( 0, LOCALE_STHOUSAND, szThousandsSeparator, sizeof(szThousandsSeparator) ) ;
      GetLocaleInfo ( 0, LOCALE_SDATE,     szDateSeparator,      sizeof(szDateSeparator)      ) ;
      GetLocaleInfo ( 0, LOCALE_STIME,     szTimeSeparator,      sizeof(szTimeSeparator)      ) ;
      GetLocaleInfo ( 0, LOCALE_S1159,     szAm,                 sizeof(szAm)                 ) ;
      GetLocaleInfo ( 0, LOCALE_S2359,     szPm,                 sizeof(szPm)                 ) ;

   #endif // __OS2__

} /* endmethod */

