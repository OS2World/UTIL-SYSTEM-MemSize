/****************************************************************** Mutex.CPP
 *                                                                          *
 *                   Mutex Semaphore Functions (OS2/NT)                     *
 *                                                                          *
 ****************************************************************************/

#ifndef __DOS__

#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Mutex.h"
#include "Mux.h"

/****************************************************************************
 *                                                                          *
 *  Mutex: Constructor                                                      *
 *                                                                          *
 ****************************************************************************/

Mutex::Mutex ( char *tag, char *Name, int OneInstanceOnly ) : Tag(0), DebugFlag(FALSE) {

   if ( tag ) {
      Tag = (char*) malloc ( strlen(tag) + 1 ) ;
      strcpy ( Tag, tag ) ;
   } /* endif */

   #ifdef __OS2__

      char FullName [_MAX_PATH] ;
      strcpy ( FullName, "\\SEM32\\" ) ;
      strcat ( FullName, Name?Name:"" ) ;
      unsigned long Status = 1 ;
      Handle = 0 ;
      if ( Name && !OneInstanceOnly ) 
         Status = DosOpenMutexSem ( PSZ(FullName), &Handle ) ;
      if ( Status ) {
         Status = DosCreateMutexSem ( PSZ(Name?FullName:0), &Handle, DC_SEM_SHARED, FALSE ) ;
         if ( OneInstanceOnly && ( Status == ERROR_DUPLICATE_NAME ) ) 
            Log ( "Mutex(%s): Unable to create mutex semaphore '%s'.  Already exists.", Tag?Tag:"", Name?Name:"" ) ;
         else if ( Status ) 
            Log ( "Mutex(%s): Unable to create mutex semaphore '%s'.  Status %i.", Tag?Tag:"", Name?Name:"", Status ) ;
      } /* endif */

   #else

      Handle = CreateMutex ( 0, FALSE, Name ) ;
      if ( Handle && OneInstanceOnly && ( GetLastError() == ERROR_ALREADY_EXISTS ) ) {
         Log ( "Mutex(%s): Unable to create mutex semaphore '%s'.  Already exists.", Tag?Tag:"", Name?Name:"" ) ;
         CloseHandle ( Handle ) ;
         Handle = 0 ;
      } else if ( !Handle ) {
         DWORD Status = GetLastError ( ) ;
         Log ( "Mutex(%s): Unable to create mutex semaphore '%s'.  Status %i.", Tag?Tag:"", Name?Name:"", Status ) ;
      } /* endif */

   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Mutex: Destructor                                                       *
 *                                                                          *
 ****************************************************************************/

Mutex::~Mutex ( ) {

   if ( Handle ) {
      #ifdef __OS2__
         DosCloseMutexSem ( Handle ) ;
      #else
         CloseHandle ( Handle ) ;
      #endif
   } /* endif */

   if ( Tag ) free ( Tag ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Mutex: Request                                                          *
 *                                                                          *
 ****************************************************************************/

unsigned long Mutex::Request ( int Timeout ) {

   if ( Handle == 0 )
      return ( 0xFFFFFFFF ) ;

   if ( DebugFlag ) Log ( "Mutex(%s)::Request Started.", Tag?Tag:"" ) ;

   BOOL Result = FALSE ;

   #ifdef __OS2__

      if ( WinRequestMutexSem ( Handle, Timeout ) )
         Result = FALSE ;
      else
         Result = TRUE ;

   #else

      Result = ( WaitForSingleObject ( Handle, Timeout ) == WAIT_OBJECT_0 ) ? TRUE : FALSE ;

   #endif

   if ( DebugFlag ) Log ( "Mutex(%s)::Request Done %s.", Tag?Tag:"", Result ? "TRUE" : "FALSE" ) ;

   return ( Result ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Mutex: Request (checking window messages)                               *
 *                                                                          *
 ****************************************************************************/

unsigned long Mutex::RequestWin ( int Timeout ) {

   if ( Handle == 0 )
      return ( 0xFFFFFFFF ) ;

   if ( DebugFlag ) Log ( "Mutex(%s)::RequestWin Started.", Tag?Tag:"" ) ;

   BOOL Result = FALSE ;

   #ifdef __OS2__

      if ( WinRequestMutexSem ( Handle, Timeout ) )
         Result = FALSE ;
      else
         Result = TRUE ;

   #else

      HANDLE Handles [1] ;
      Handles [0] = Handle ;
      Result = Sys_MsgWaitForMultipleObjects ( sizeof(Handles)/sizeof(Handles[0]), Handles, Timeout ) ;

   #endif

   if ( DebugFlag ) Log ( "Mutex(%s)::RequestWin Done %s.", Tag?Tag:"", Result ? "TRUE" : "FALSE" ) ;

   return ( Result ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Mutex: Release                                                          *
 *                                                                          *
 ****************************************************************************/

unsigned long Mutex::Release ( ) {

   if ( Handle == 0 )
      return ( 0xFFFFFFFF ) ;

   if ( DebugFlag ) Log ( "Mutex(%s)::Release.", Tag?Tag:"" ) ;

   BOOL Result = FALSE ;

   #ifdef __OS2__

      if ( DosReleaseMutexSem ( Handle ) )
         Result = FALSE ;
      else
         Result = TRUE ;

   #else

      Result = ReleaseMutex ( Handle ) ;

   #endif

   return ( Result ) ;

} /* endmethod */

#endif // not __DOS__
