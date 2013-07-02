/*************************************************************** PROCESS.CPP
 *                                                                         *
 *  Class: Process                                                         *
 *                                                                         *
 ***************************************************************************/

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Proc.h"
#include "ReString.h"
#include "Support.h"

/***************************************************************************
 *                                                                         *
 *  Process: Constructor                                                   *
 *                                                                         *
 ***************************************************************************/
 
Process::Process ( char *name, long QueueSize ) : Name(0), Anchor(0), Queue(0) {

   // Save the process name, if given.
   if ( name ) {
      Name = PCHAR ( malloc ( strlen(name)+1 ) ) ;
      strcpy ( Name, name ) ;
   } /* endif */

   // Create thread anchor block.
   Anchor = WinInitialize ( 0 ) ;
   if ( Anchor == 0 ) {
      // This error message cannot be displayed, only logged, because we have no queue yet.
      Log ( "ERROR: Unable to initialize for windowing." ) ;
      return ;
   } /* endif */

   // Create thread message queue.
   Queue = WinCreateMsgQueue ( Anchor, QueueSize ) ;
   if ( Queue == 0 ) {
      // This error message cannot be displayed, only logged, because we have no queue yet.
      Log ( "ERROR: Unable to create process message queue." ) ;
      WinTerminate ( Anchor ) ;  Anchor = 0 ;
      return ;
   } /* endif */

} /* endmethod */


/***************************************************************************
 *                                                                         *
 *  Process: Destructor                                                    *
 *                                                                         *
 ***************************************************************************/

Process::~Process ( ) {

   if ( Queue ) { 
      if ( !WinDestroyMsgQueue ( Queue ) ) {
         char Message [512] ;
         Log ( "Process::~Process(%s) WinDestroyMsgQueue failed.  %s", Name, InterpretWinError(0,Message) ) ;
      } /* endif */
   } /* endif */

   if ( Anchor ) { 
      if ( !WinTerminate ( Anchor ) ) {
         char Message [512] ;
         Log ( "Process::~Process(%s) WinTerminate failed.  %s", Name, InterpretWinError(0,Message) ) ;
      } /* endif */
   } /* endif */

   if ( Name ) free ( Name ) ;

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Process: Execute (process messages)                                    *
 *                                                                         *
 ***************************************************************************/

void Process::Execute ( ) {
   Sys_ProcessMessages ( Anchor ) ;
} /* endmethod */

