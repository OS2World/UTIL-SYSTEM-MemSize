/***************************************************************** THREAD.CPP
 *                                                                          *
 *                            Threading Support                             *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <process.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Except.h"
#include "Thread.h"

//#define DEBUG

 
/****************************************************************************
 *                                                                          *
 *                         Definitions & Declarations                       *
 *                                                                          *
 ****************************************************************************/

  // Macros

#ifndef __IBMCPP__
   #define _Optlink 
#endif


  // Type Definitions

typedef struct {
   char *Name ;
   void *Stack ;
   void (*Function)(void*) ;
   void *Parameter ;
} THREAD_PARMS, *PTHREAD_PARMS ;


  // Global Data

extern DebugType ThreadDebug = DEBUG_OFF ;


  // Private Data

static void *ThreadStacks [MAX_THREADS+1] ;

 
/****************************************************************************
 *                                                                          *
 *                            Generic Thread                                *
 *                                                                          *
 ****************************************************************************/

static void _Optlink Thread ( void *Parameter ) {

  /**************************************************************************
   * Get parameters.                                                        *
   **************************************************************************/

   PTHREAD_PARMS Parms = PTHREAD_PARMS ( Parameter ) ;

  /**************************************************************************
   * Set a default exception filter.                                        *
   **************************************************************************/

   REGISTER_EXCEPTION_HANDLER(0)

  /**************************************************************************
   * Register self and adjust the stack pool.                               *
   **************************************************************************/

   int ThreadNumber = Sys_RegisterThread ( ) ;

   if ( ThreadStacks[ThreadNumber-1] ) 
      free ( ThreadStacks[ThreadNumber-1] ) ;

   ThreadStacks[ThreadNumber-1] = Parms->Stack ;

  /**************************************************************************
   * Execute the function requested.                                        *
   **************************************************************************/

   if ( ThreadDebug >= DEBUG_STARTSTOP )
      Log ( "Thread %i (%s) starting.\n", ThreadNumber, Parms->Name ) ;

   Parms->Function ( Parms->Parameter ) ;

   if ( ThreadDebug >= DEBUG_STARTSTOP )
      Log ( "Thread %i (%s) ending.\n", ThreadNumber, Parms->Name ) ;

  /**************************************************************************
   * Release the memory allocated for the thread parameters.                *
   **************************************************************************/

   free ( Parms->Name ) ;
   free ( Parms ) ;

  /**************************************************************************
   * De-register self.                                                      *
   **************************************************************************/

   Sys_DeregisterThread ( ) ;

  /**************************************************************************
   * Unhook the exception handler.                                          *
   **************************************************************************/

   UNREGISTER_EXCEPTION_HANDLER(0)
}

/****************************************************************************
 *                                                                          *
 *                            Start Thread                                  *
 *                                                                          *
 ****************************************************************************/

extern TID StartThread ( char *Name, void(*Function)(void*), int StackSize, void *Parameter ) {

   // Local Declarations
   PTHREAD_PARMS Parms ;
   TID ThreadID ;

   // Allocate a stack for the new thread.
   void *Stack = malloc ( StackSize ) ;       // This memory is never freed!

   // Set up thread parameters.
   Parms = (PTHREAD_PARMS) malloc ( sizeof(THREAD_PARMS) ) ;
   Parms->Name = (PCHAR) malloc ( strlen(Name) + 1 ) ;
   strcpy ( Parms->Name, Name ) ;
   Parms->Stack = Stack ;
   Parms->Function = Function ;
   Parms->Parameter = Parameter ;

   // If full debugging, log attempt to start thread.
   if ( ThreadDebug == DEBUG_FULL )
      Log ( "Attempting to start thread (%s).\n", Name ) ;

   // Try to start the thread.
   #ifdef __NT__
      ThreadID = _beginthread ( Thread, StackSize, Parms ) ;
   #else
      ThreadID = _beginthread ( Thread, Stack, StackSize, Parms ) ;
   #endif

   // If failed to begin thread, log the error and exit.
   if ( ThreadID == -1 ) {
      Log ( "ERROR: Unable to start thread '%s'.", Name ) ;
      return ( -1 ) ;
   } /* endif */

   // If full debugging, log thread start.
   if ( ThreadDebug == DEBUG_FULL )
      Log ( "Thread (%s) started.", Name ) ;

   // Return thread ID.
   return ( ThreadID ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                           Set Thread Priority                            *
 *                                                                          *
 ****************************************************************************/

extern void Thread_SetPriority ( int Thread, Priority Level ) {

   #ifdef __OS2__

      switch ( Level ) {
         case PRIORITY_IDLE: {              // Absolute idle priority.
            DosSetPriority ( PRTYS_THREAD, PRTYC_IDLETIME, PRTYD_MINIMUM, Thread ) ;
            break; }
         case PRIORITY_LOWEST: {            // Lowest for process class.
            DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, PRTYD_MINIMUM, Thread ) ;
            break; }
         case PRIORITY_BELOW_NORMAL: {      // Lower than normal for process class.
            DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, PRTYD_MINIMUM, Thread ) ;
            DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +8, Thread ) ;
            break; }
         case PRIORITY_NORMAL: {            // Normal for process class.
            DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, PRTYD_MINIMUM, Thread ) ;
            DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, +16, Thread ) ;
            break; }
         case PRIORITY_ABOVE_NORMAL: {      // Higher than normal for process class.
            DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, PRTYD_MAXIMUM, Thread ) ;
            DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, -8, Thread ) ;
            break; }
         case PRIORITY_HIGHEST: {           // Highest for process class.
            DosSetPriority ( PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, PRTYD_MAXIMUM, Thread ) ;
            break; }
         case PRIORITY_TIME_CRITICAL: {     // Absolute highest priority.
            DosSetPriority ( PRTYS_THREAD, PRTYC_TIMECRITICAL, PRTYD_MAXIMUM, 0 );
            break; }
      } /* endswitch */

   #else

      if ( Thread == 0 )
         Thread = (int) GetCurrentThread ( ) ;

      switch ( Level ) {
         case PRIORITY_IDLE: {              // Absolute idle priority.
            SetThreadPriority ( (HANDLE)Thread, THREAD_PRIORITY_IDLE ) ;
            break; }
         case PRIORITY_LOWEST: {            // Lowest for process class.
            SetThreadPriority ( (HANDLE)Thread, THREAD_PRIORITY_LOWEST ) ;
            break; }
         case PRIORITY_BELOW_NORMAL: {      // Lower than normal for process class.
            SetThreadPriority ( (HANDLE)Thread, THREAD_PRIORITY_BELOW_NORMAL ) ;
            break; }
         case PRIORITY_NORMAL: {            // Normal for process class.
            SetThreadPriority ( (HANDLE)Thread, THREAD_PRIORITY_NORMAL ) ;
            break; }
         case PRIORITY_ABOVE_NORMAL: {      // Higher than normal for process class.
            SetThreadPriority ( (HANDLE)Thread, THREAD_PRIORITY_ABOVE_NORMAL ) ;
            break; }
         case PRIORITY_HIGHEST: {           // Highest for process class.
            SetThreadPriority ( (HANDLE)Thread, THREAD_PRIORITY_HIGHEST ) ;
            break; }
         case PRIORITY_TIME_CRITICAL: {     // Absolute highest priority.
            SetThreadPriority ( (HANDLE)Thread, THREAD_PRIORITY_TIME_CRITICAL ) ;
            break; }
      } /* endswitch */

   #endif
}

