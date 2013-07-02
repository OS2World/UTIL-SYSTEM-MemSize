/****************************************************************** EVENT.CPP
 *                                                                          *
 *                   Event Semaphore Functions (OS2/NT)                     *
 *                                                                          *
 ****************************************************************************/

#ifndef __DOS__

#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Event.h"
#include "Mux.h"

/****************************************************************************
 *                                                                          *
 *  Event: Constructor                                                      *
 *                                                                          *
 ****************************************************************************/

Event::Event ( char *tag, char *Name, int OneInstanceOnly ) : Tag(0), DebugFlag(FALSE) {

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
         Status = DosOpenEventSem ( PSZ(FullName), &Handle ) ;
      if ( Status ) {
         Status = DosCreateEventSem ( PSZ(Name?FullName:0), &Handle, DC_SEM_SHARED, FALSE ) ;
         if ( OneInstanceOnly && ( Status == ERROR_DUPLICATE_NAME ) )
            Log ( "Event(%s): Unable to create semaphore for '%s'.  Already exists.", Tag?Tag:"", Name?Name:"" ) ;
         else if ( Status ) 
            Log ( "Event(%s): Unable to create semaphore for '%s'.  Status %i.", Tag?Tag:"", Name?Name:"", Status ) ;
      } /* endif */

   #else

      Handle = CreateEvent ( 0, TRUE, FALSE, Name ) ;
      if ( Handle && OneInstanceOnly && ( GetLastError() == ERROR_ALREADY_EXISTS ) ) {
         Log ( "Event(%s): Unable to create event semaphore '%s'.  Already exists.", Tag?Tag:"", Name?Name:"" ) ;
         CloseHandle ( Handle ) ;
         Handle = 0 ;
      } else if ( !Handle ) {
         DWORD Status = GetLastError ( ) ;
         Log ( "Event(%s): Unable to create event semaphore '%s'.  Status %i.", Tag?Tag:"", Name?Name:"", Status ) ;
      } /* endif */

   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Event: Destructor                                                       *
 *                                                                          *
 ****************************************************************************/

Event::~Event ( ) {

   if ( Handle ) {
      #ifdef __OS2__
         DosCloseEventSem ( Handle ) ;
      #else
         CloseHandle ( Handle ) ;
      #endif
   } /* endif */

   if ( Tag ) free ( Tag ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Event: Post                                                             *
 *                                                                          *
 ****************************************************************************/

unsigned long Event::Post ( ) {

   if ( Handle == 0 )
      return ( 0xFFFFFFFF ) ;

   if ( DebugFlag ) Log ( "Event(%s)::Post.", Tag?Tag:"" ) ;

   BOOL Result = FALSE ;

   #ifdef __OS2__

      APIRET Status = DosPostEventSem ( Handle ) ;
      if ( Status && ( Status != ERROR_ALREADY_POSTED ) )
         Result = FALSE ;
      else
         Result = TRUE ;

   #else

      Result = SetEvent ( Handle ) ;

   #endif

   return ( Result ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Event: Reset                                                            *
 *                                                                          *
 ****************************************************************************/

unsigned long Event::Reset ( ) {

   if ( Handle == 0 )
      return ( 0xFFFFFFFF ) ;

   if ( DebugFlag ) Log ( "Event(%s)::Reset.", Tag?Tag:"" ) ;

   BOOL Result = FALSE ;

   #ifdef __OS2__

      ULONG PostCount ;
      APIRET Status = DosResetEventSem ( Handle, &PostCount ) ;
      if ( Status && ( Status != ERROR_ALREADY_RESET ) )
         Result = FALSE ;
      else
         Result = TRUE ;

   #else

      Result = ResetEvent ( Handle ) ;

   #endif

   return ( Result ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Event: Wait                                                             *
 *                                                                          *
 ****************************************************************************/

unsigned long Event::Wait ( int Timeout ) {

   if ( Handle == 0 )
      return ( 0xFFFFFFFF ) ;

   if ( DebugFlag ) Log ( "Event(%s)::Wait Started.", Tag?Tag:"" ) ;

   unsigned long Result ;

   #ifdef __OS2__

      APIRET Status = WinWaitEventSem ( Handle, Timeout ) ;
      if ( Status && ( Status != ERROR_ALREADY_POSTED ) ) {
         if ( Status != ERROR_TIMEOUT )
            Log ( "Event(%s)::Wait Wait for event %08p failed.  Status %08X.", Tag?Tag:"", Handle, Status ) ;
         Result = FALSE ;
      } else
         Result = TRUE ;

   #else

      HANDLE Handles [1] ;
      Handles [0] = Handle ;
      Result = Sys_MsgWaitForMultipleObjects ( sizeof(Handles)/sizeof(Handles[0]), Handles, Timeout ) ;

   #endif

   if ( DebugFlag ) Log ( "Event(%s)::Wait Done %s.", Tag?Tag:"", Result ? "TRUE" : "FALSE" ) ;

   return ( Result ) ;

} /* endmethod */

#endif // not __DOS__
