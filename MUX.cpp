/******************************************************************** MUX.CPP
 *                                                                          *
 *         Wait For Multiple Events, processing messages (OS2/NT)           *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#include <stdlib.h>

#include "Debug.h"

#ifdef __OS2__

// Class: Multiplex Event Semaphore (OS/2)

class MultiplexEvent {

   private:
      char *Tag ;
      BOOL DebugFlag ;
      HMUX hmux ;
      int Count ;
      PSEMRECORD Semaphores ;
      BOOL CreatedSemaphores ;

   public:
      MultiplexEvent ( char *tag, char *Name, int count, HEV Handles[] ) : Tag(tag), DebugFlag(FALSE), hmux(0), Count(count), CreatedSemaphores(FALSE) {
         Semaphores = PSEMRECORD ( malloc ( Count * sizeof(SEMRECORD) ) ) ;
         if ( Semaphores == NULL )
            return ;
         for ( int i=0; i<Count; i++ ) {
            Semaphores[i].hsemCur = HSEM(Handles[i]) ;
            ULONG PostCount ;
            DosResetEventSem ( HEV(Semaphores[i].hsemCur), &PostCount ) ;
            Semaphores[i].ulUser = i ;
         } /* endfor */
         APIRET Status = DosCreateMuxWaitSem ( (PSZ)Name, &hmux, Count, Semaphores, DCMW_WAIT_ANY ) ;
         if ( Status ) {
            Log ( "ERROR: Unable to create multiplex semaphore.  RC %i.", Status ) ;
            for ( i=0; i<Count; i++ ) 
               DosCloseEventSem ( HEV(Semaphores[i].hsemCur) ) ;
            return ;
         } /* endif */
      } /* endmethod */

      MultiplexEvent ( char *tag, char *Name, int count, char *Names[], int Attrs[] ) : Tag(tag), DebugFlag(FALSE), hmux(0), Count(count), CreatedSemaphores(TRUE) {
         Semaphores = PSEMRECORD ( malloc ( Count * sizeof(SEMRECORD) ) ) ;
         if ( Semaphores == NULL )
            return ;
         for ( int i=0; i<Count; i++ ) {
            Semaphores[i].hsemCur = 0 ;
            APIRET Status = DosCreateEventSem ( Names ? (PSZ)Names[i] : 0,
               PHEV(&Semaphores[i].hsemCur), Attrs ? Attrs[i] : 0 , FALSE ) ;
            if ( Status ) {
               Log ( "ERROR: Unable to create event semaphore %i.  RC %i.", i+1, Status ) ;
               while ( i >= 0 ) 
                   DosCloseEventSem ( HEV(Semaphores[i--].hsemCur) ) ;
               return ;
            } /* endif */
            ULONG PostCount ;
            DosResetEventSem ( HEV(Semaphores[i].hsemCur), &PostCount ) ;
            Semaphores[i].ulUser = i ;
         } /* endfor */
         APIRET Status = DosCreateMuxWaitSem ( (PSZ)Name, &hmux, Count, Semaphores, DCMW_WAIT_ANY ) ;
         if ( Status ) {
            Log ( "ERROR: Unable to create multiplex semaphore.  RC %i.", Status ) ;
            for ( i=0; i<Count; i++ ) 
               DosCloseEventSem ( HEV(Semaphores[i].hsemCur) ) ;
            return ;
         } /* endif */
      } /* endmethod */

      MultiplexEvent ( char *tag, char *Name, int count ) : Tag(tag), DebugFlag(FALSE), hmux(0), Count(count), CreatedSemaphores(TRUE) {
         Semaphores = PSEMRECORD ( malloc ( Count * sizeof(SEMRECORD) ) ) ;
         if ( Semaphores == NULL )
            return ;
         for ( int i=0; i<Count; i++ ) {
            Semaphores[i].hsemCur = 0 ;
            APIRET Status = DosCreateEventSem ( 0, PHEV(&Semaphores[i].hsemCur), 0, FALSE ) ;
            if ( Status ) {
               Log ( "ERROR: Unable to create event semaphore %i.  RC %i.", i+1, Status ) ;
               while ( i >= 0 ) 
                   DosCloseEventSem ( HEV(Semaphores[i--].hsemCur) ) ;
               return ;
            } /* endif */
            ULONG PostCount ;
            DosResetEventSem ( HEV(Semaphores[i].hsemCur), &PostCount ) ;
            Semaphores[i].ulUser = i ;
         } /* endfor */
         APIRET Status = DosCreateMuxWaitSem ( (PSZ)Name, &hmux, Count, Semaphores, DCMW_WAIT_ANY ) ;
         if ( Status ) {
            Log ( "ERROR: Unable to create multiplex semaphore.  RC %i.", Status ) ;
            for ( i=0; i<Count; i++ ) 
               DosCloseEventSem ( HEV(Semaphores[i].hsemCur) ) ;
            return ;
         } /* endif */
      } /* endmethod */

      ~MultiplexEvent ( ) {
         if ( hmux ) {
            DosCloseMuxWaitSem ( hmux ) ;
            if ( CreatedSemaphores ) 
               for ( int i=0; i<Count; i++ ) 
                  DosCloseEventSem ( HEV(Semaphores[i].hsemCur) ) ;
            free ( Semaphores ) ;
         } /* endif */
      } /* endmethod */

      void SetDebug ( BOOL flag ) { DebugFlag = flag ; }

      HMUX QueryHandle ( ) { return ( hmux ) ; }

      HEV QueryEventHandle ( int EventNumber ) { return ( (HEV)Semaphores[EventNumber].hsemCur ) ; }

      APIRET Wait ( int Timeout, ULONG &Cleared ) {
         if ( DebugFlag ) 
            Log ( "MultiplexEvent(%s)::Wait(Timeout=%i) Started.", Tag, Timeout ) ;
         APIRET Status = WinWaitMuxWaitSem ( hmux, Timeout, &Cleared ) ;
         if ( Status ) {
            Log ( "MultiplexEvent(%s)::Wait(Timeout=%i) Error.  Status=%X.", Tag, Timeout, Status ) ;
            return ( Status ) ;
         } /* endif */
         ULONG PostCount ;
         DosResetEventSem ( HEV(Semaphores[Cleared].hsemCur), &PostCount ) ;
         if ( DebugFlag ) 
            Log ( "MultiplexEvent(%s)::Wait(Timeout=%i) Done OK.", Tag, Timeout ) ;
         return ( 0 ) ;
      } /* endmethod */

      APIRET Post ( int EventNumber ) {
         if ( DebugFlag ) 
            Log ( "MultiplexEvent(%s)::Post", Tag ) ;
         return ( DosPostEventSem ( HEV(Semaphores[EventNumber].hsemCur) ) ) ;
      } /* endmethod */
} ;

extern BOOL Sys_MsgWaitForMultipleObjects ( int EventCount, HEV Events[], int Timeout ) {
   MultiplexEvent Mux = MultiplexEvent ( "Sys_MsgWaitForMultipleObjects::Mux", 0, EventCount, Events ) ;
   ULONG Cleared ;
   return ( Mux.Wait ( Timeout, Cleared ) ? FALSE : TRUE ) ;
} 

#else

extern BOOL Sys_MsgWaitForMultipleObjects ( int EventCount, HEV Events[], int Timeout ) {

   DWORD Result ;

   do {
      Result = MsgWaitForMultipleObjects ( EventCount, Events, FALSE, Timeout, QS_ALLINPUT ) ;
      if ( Result == WAIT_OBJECT_0 + EventCount ) {
         MSG Message ;
         if ( PeekMessage ( &Message, 0, 0, 0, PM_REMOVE ) ) {
            TranslateMessage ( &Message ) ;
            DispatchMessage ( &Message ) ;
         } /* endif */
      } else if ( Result < WAIT_OBJECT_0 + EventCount ) {
         ;
      } else if ( ( Result >= WAIT_ABANDONED_0 ) && ( Result < WAIT_ABANDONED_0 + EventCount ) ) {
         ;
      } else if ( Result == WAIT_TIMEOUT ) {
         ;
      } /* endif */
   } while ( Result == WAIT_OBJECT_0 + EventCount ); /* enddo */

   switch ( Result ) {
      case WAIT_OBJECT_0:
         Result = TRUE ;
         break;
      case WAIT_ABANDONED_0:
         Result = FALSE ;
         break;
      case WAIT_TIMEOUT:
         Result = FALSE ;
         break;
      case 0xFFFFFFFF:
         Log ( "Sys_MsgWaitForMultipleObjects: Error returned from MsgWaitForMultipleObjects." ) ;
         Result = FALSE ;
         break;
      default: 
         Log ( "Sys_MsgWaitForMultipleObjects: Unknown status %i returned from MsgWaitForMultipleObjects.", Result ) ;
         Result = FALSE ;
         break;
   } /* endswitch */

   return ( Result ) ;
}

#endif // __NT__
