/***************************************************************** MODULE.CPP
 *                                                                          *
 *  Class: Resource Module                                                  *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Module.h"

Module::Module ( char *name ) : Handle(0), Lock("Module::Lock") {
   Lock.Request ( ) ;
   if ( name && *name ) {
      Name = (char*) malloc ( strlen(name) + 1 ) ;
      strcpy ( Name, name ) ;
      char Drive[_MAX_DRIVE+1] ;
      char Dir[_MAX_DIR+1] ;
      char Fname[_MAX_FNAME+1] ;
      char Ext[_MAX_EXT+1] ;
      _splitpath ( Name, Drive, Dir, Fname, Ext ) ;
      char ModuleName [_MAX_PATH] ;
      strcpy ( ModuleName, Drive ) ;
      strcat ( ModuleName, Dir ) ;
      strcat ( ModuleName, Fname ) ;
      if ( *Drive || *Dir )
         strcat ( ModuleName, Ext ) ;
      strupr ( ModuleName ) ;
      #ifdef __OS2__
         char MissingModule [_MAX_PATH] ;
         if ( DosLoadModule ( PSZ(MissingModule), sizeof(MissingModule), PSZ(ModuleName), &Handle ) ) {
            Log ( "ERROR: Unable to load module '%s'.\r\n", Name ) ;
            Debug ( HWND_DESKTOP, "ERROR: Unable to load module '%s'.", Name ) ;
            abort ( ) ;
         } /* endif */
      #else
         Handle = LoadLibrary ( ModuleName ) ;
         if ( !Handle ) {
            Log ( "ERROR: Unable to load module '%s'.\r\n", Name ) ;
            Debug ( HWND_DESKTOP, "ERROR: Unable to load module '%s'.", Name ) ;
            abort ( ) ;
         } /* endif */
      #endif 
   } else {
      Name = 0 ;
   } /* endif */
   Lock.Release ( ) ;
} /* endmethod */

Module::Module ( const Module &object ) : Name(0), Handle(0), Lock("Module::Lock") {
   Lock.Request ( ) ;
   Name = (char*) malloc ( strlen(object.Name) + 1 ) ;
   strcpy ( Name, object.Name ) ;
   if ( object.Handle ) {
      char Drive[_MAX_DRIVE+1] ;
      char Dir[_MAX_DIR+1] ;
      char Fname[_MAX_FNAME+1] ;
      char Ext[_MAX_EXT+1] ;
      _splitpath ( Name, Drive, Dir, Fname, Ext ) ;
      char ModuleName [_MAX_PATH] ;
      strcpy ( ModuleName, Drive ) ;
      strcat ( ModuleName, Dir ) ;
      strcat ( ModuleName, Fname ) ;
      if ( *Drive || *Dir )
         strcat ( ModuleName, Ext ) ;
      strupr ( ModuleName ) ;
      #ifdef __OS2__
         char MissingModule [_MAX_PATH] ;
         DosLoadModule ( PSZ(MissingModule), sizeof(MissingModule), PSZ(ModuleName), &Handle ) ;
      #else
         Handle = LoadLibrary ( ModuleName ) ;
      #endif 
   } /* endif */
   Lock.Release ( ) ;
} /* endmethod */

Module::~Module ( ) {
   Lock.Request ( ) ;
   if ( Handle ) {
      #ifdef __OS2__
         DosFreeModule ( Handle ) ;
      #else
         FreeLibrary ( Handle ) ;
      #endif 
      Handle = 0 ;
   } /* endif */
   if ( Name ) 
      free ( Name ) ;
   Lock.Release ( ) ;
} /* endmethod */

#ifdef __OS2__

PFN Module::QueryAddress ( char *Name ) {
   PFN Address ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ(Name), (PFN*)&Address ) )
      return ( 0 ) ;
   return ( Address ) ;
} /* endmethod */

PFN Module::QueryAddress ( int Ordinal ) {
   PFN Address ;
   if ( DosQueryProcAddr ( Handle, Ordinal, 0, (PFN*)&Address ) )
      return ( 0 ) ;
   return ( Address ) ;
} /* endmethod */

#endif // __OS2__
