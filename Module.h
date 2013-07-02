/******************************************************************* MODULE.H
 *                                                                          *
 *  Class: Resource Module                                                  *
 *                                                                          *
 ****************************************************************************/

#ifndef MODULE_H
#define MODULE_H

#include "Mutex.h"

class Module {

   protected:
      char *Name ;
      HMODULE Handle ;
      Mutex Lock ;

   public:
      Module ( char *Name = "" ) ;
      Module ( const Module &object ) ;
      virtual ~Module ( ) ;
      char *QueryName () { return ( Name ) ; }
      HMODULE QueryHandle () { return ( Handle ) ; }
      #ifdef __OS2__
         PFN QueryAddress ( char *Name ) ;
         PFN QueryAddress ( int Ordinal ) ;
      #endif // __OS2__
} ;

#endif
