/****************************************************************** COUNTRY.H
 *                                                                          *
 *                        Country Information Object                        *
 *                                                                          *
 ****************************************************************************/

#ifndef COUNTRY_H
#define COUNTRY_H

#include "System.h"

struct CountryInfo {
   int  fsDateFmt ;
   int  fsTimeFmt ;
   int  idCountry ;
   char szThousandsSeparator [10] ;
   char szDateSeparator [10] ;
   char szTimeSeparator [10] ;
   char szAm [10] ;
   char szPm [10] ;
   CountryInfo ( ) ;
} ;

#endif // COUNTRY_H
