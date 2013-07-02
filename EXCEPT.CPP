/**************************************************************** EXCEPT.CPP
 *                                                                         *
 *                            Exception Handler                            *
 *                                                                         *
 *  This rather simple exception handler includes a just-as-simple stack   *
 *  walker, which comes handy sometimes when debugging exceptions.  A far  *
 *  better one can be constructed with the use of Watcom's prologue and    *
 *  epilogue function calls (/ee /ep /en).                                 *
 *                                                                         *
 *  NOTE: For this to work reasonably reliably with the Watcom compiler,   *
 *    the compiler option /of+ should be specified, so as to enforce the   *
 *    usage of stack frames.                                               *
 *                                                                         *
 ***************************************************************************/

#include "System.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Debug.h"
#include "Except.h"
#include "MemSize.h"
#include "Mutex.h"
#include "ReString.h"
#include "Support.h"

#define USEMESSAGEBOX
 
/***************************************************************************
 *                                                                         *
 *  Exception Handler for OS/2                                             *
 *                                                                         *
 ***************************************************************************/

#ifdef __OS2__

extern "C" APIRET APIENTRY DosQueryModFromEIP ( HMODULE *phMod, ULONG *pObjNum,
   ULONG BuffLen, PCHAR pBuff, ULONG *pOffset, PVOID Address );

inline APIRET GetModuleOffset ( PVOID Address, char *Name, int NameSize, ULONG &Object, ULONG &Offset ) {
   HMODULE hModule ( 0 ) ;
   return ( DosQueryModFromEIP ( &hModule, &Object, NameSize, Name, &Offset, Address ) ) ;
}

extern BOOL BreakKey = FALSE;

inline void RegisterDump ( int ThreadNumber, PCONTEXTRECORD pContextRecord ) {

   // Get failing module name.
   ULONG ObjectNumber ( 0 ), Offset ( 0 ) ;
   char ModuleName [ CCHMAXPATH + 1 ] = { 0 } ;
   GetModuleOffset ( (PVOID)pContextRecord->ctx_RegEip, ModuleName, sizeof(ModuleName), ObjectNumber, Offset ) ;

   // Log what was found.
   Log ( "  Module '%s', Object %i, Offset %08lX.", ModuleName, ObjectNumber+1, Offset ) ;
   Log ( "  Thread %i.", ThreadNumber ) ;
   Log ( "  The registers were as follows:" ) ;
   Log ( "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX",
      pContextRecord->ctx_RegEax, pContextRecord->ctx_RegEbx,
      pContextRecord->ctx_RegEcx, pContextRecord->ctx_RegEdx ) ;
   Log ( "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX",
      pContextRecord->ctx_EFlags, pContextRecord->ctx_RegEdi,
      pContextRecord->ctx_RegEsi, pContextRecord->ctx_RegEbp ) ;
   Log ( "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX",
      pContextRecord->ctx_SegCs,  pContextRecord->ctx_RegEip,
      pContextRecord->ctx_SegSs,  pContextRecord->ctx_RegEsp ) ;
   Log ( "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
      pContextRecord->ctx_SegDs,  pContextRecord->ctx_SegEs,
      pContextRecord->ctx_SegFs,  pContextRecord->ctx_SegGs ) ;
}

inline void StackTrace ( PCONTEXTRECORD pContextRecord ) {

   // Generate a WATCOM-style stack trace.
   char LogFile [_MAX_PATH] ;
   if ( HomePath[0] ) {
      strcpy ( LogFile, HomePath ) ;
      strcat ( LogFile, "\\" LOGFILE ) ;
   } else {
      strcpy ( LogFile, LOGFILE ) ;
   } /* endif */
   FILE *File = fopen ( LogFile, "a" ) ;
   if ( File ) {
      SH__DumpStack ( File, 0 ) ;
      fclose ( File );
   } /* endif */

   // Generate an old-fashioned base-pointer-style stack trace.
   if ( pContextRecord->ctx_RegEbp >= pContextRecord->ctx_RegEsp) {
      PLONG Base = (PLONG)pContextRecord->ctx_RegEbp ;
      while ( Base && ( *Base > (LONG)Base ) ) {
         ULONG ObjectNumber ( 0 ), Offset ( 0 ) ;
         char ModuleName [ CCHMAXPATH + 1 ] = { 0 } ;
         if ( !GetModuleOffset ( (PVOID)(*(Base+1)), ModuleName, sizeof(ModuleName), ObjectNumber, Offset ) )
            Log ( "  Calling function was at %08lX (%s,%i:%08X).", *(Base+1), ModuleName, ObjectNumber+1, Offset ) ;
         else 
            break ;
         Base = (PLONG)*Base ;
      } /* endwhile */
   } /* endif */
}

inline void DisplayMessage ( ) {
   #ifdef USEMESSAGEBOX
   ResourceString Title ( LibraryHandle, IDS_TITLE ) ;
   ResourceString Exception ( LibraryHandle, IDS_EXCEPTION ) ;
   char LogFile [CCHMAXPATH] ;
   strcpy ( LogFile, HomePath ) ;
   strcat ( LogFile, "\\" LOGFILE ) ;
   static char Message [500] ;
   sprintf ( Message, PCHAR(Exception), LogFile ) ;
   Sys_MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title),
      0, MB_ENTER | MB_ICONEXCLAMATION ) ;
   #endif // USEMESSAGEBOX
}

extern ULONG APIENTRY ExceptionHandler (
   PEXCEPTIONREPORTRECORD pExceptionReportRecord,
   PEXCEPTIONREGISTRATIONRECORD,
   PCONTEXTRECORD pContextRecord,
   PVOID
) {

   // If EcoSNAP is installed and active, let it handle all this.
   HMODULE hModule ;
   APIRET rc = DosLoadModule ( 0, 0, "FXSNAP", &hModule ) ;
   if ( !rc ) {
      int (_System *pECO_Hooked) ( ) = 0 ;
      rc = DosQueryProcAddr ( hModule, 0, "ECO_Hooked", (PFN*)&pECO_Hooked ) ;
      if ( !rc && pECO_Hooked ( ) ) {
         return ( XCPT_CONTINUE_SEARCH );
      } /* endif */
   } /* endif */

   // Local Declarations
   int ThreadNumber = Sys_ThreadNumber(*_threadid) ;
   static BOOL Active = FALSE ;

   // Signal Exceptions
   if ( pExceptionReportRecord->ExceptionNum == XCPT_SIGNAL ) {
      if ( pExceptionReportRecord->ExceptionInfo[0] == XCPT_SIGNAL_INTR ) {
         Sys_Beep ( 800, 100 );
         BreakKey = TRUE;
         Log ( "Received interrupt signal." );
         return ( XCPT_CONTINUE_EXECUTION );
      } /* endif */
      if ( pExceptionReportRecord->ExceptionInfo[0] == XCPT_SIGNAL_BREAK ) {
         Sys_Beep ( 800, 100 );
         BreakKey = TRUE;
         Log ( "Received break signal." );
         return ( XCPT_CONTINUE_EXECUTION );
      } /* endif */
      if ( pExceptionReportRecord->ExceptionInfo[0] == XCPT_SIGNAL_KILLPROC ) {
         Log ( "Received KILLPROC signal." );
         return ( XCPT_CONTINUE_SEARCH );
      } /* endif */
   } /* endif */

   // Memory access violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_ACCESS_VIOLATION ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log (
        "ABORT: %s%s%s%s%s%sviolation trying to access address/selector %08lX.",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_READ_ACCESS ) ? "Read " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_WRITE_ACCESS ) ? "Write " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_EXECUTE_ACCESS ) ? "Execute " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_SPACE_ACCESS ) ? "Space " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_LIMIT_ACCESS ) ? "Limit " : "",
        ( pExceptionReportRecord->ExceptionInfo[0] & XCPT_UNKNOWN_ACCESS ) ? "Unknown " : "",
        pExceptionReportRecord->ExceptionInfo[1]
      ) ;

      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Privileged Instruction violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_PRIVILEGED_INSTRUCTION ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: A privileged instruction was encountered." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Illegal instruction violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_ILLEGAL_INSTRUCTION ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: An illegal instruction was encountered." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Integer Divide by Zero violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_INTEGER_DIVIDE_BY_ZERO ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: An integer divide-by-zero error has occurred." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Integer Overflow violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_INTEGER_OVERFLOW ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: An integer overflow has occurred." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Floating Point Divide by Zero violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_FLOAT_DIVIDE_BY_ZERO ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: A floating point divide by zero error has occurred." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Floating Point Overflow violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_FLOAT_OVERFLOW ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: A floating point overflow error has occurred." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   // Floating Point Underflow violation.
   if ( pExceptionReportRecord->ExceptionNum == XCPT_FLOAT_UNDERFLOW ) {
      if ( Active ) {
         Log ( "ABORT: Recursion within exception handler." ) ;
         DosExit ( EXIT_PROCESS, 0 ) ;
      } /* endif */
      Active = TRUE ;
      Log ( "ABORT: A floating point underflow error has occurred." ) ;
      RegisterDump ( ThreadNumber, pContextRecord ) ;
      StackTrace ( pContextRecord ) ;
      DisplayMessage ( ) ;
      DosExit ( EXIT_PROCESS, 0 ) ;
   } /* endif */

   switch ( pExceptionReportRecord->ExceptionNum ) {
      case XCPT_GUARD_PAGE_VIOLATION:
//       Log ( "ExceptionHandler: Guard page violation passed on to system." ) ;
         break;
      case XCPT_UNABLE_TO_GROW_STACK:
         Log ( "ExceptionHandler: Unable to grow stack violation passed on to system." ) ;
         break;
      case XCPT_DATATYPE_MISALIGNMENT:
         Log ( "ExceptionHandler: Datatype misalignment violation passed on to system." ) ;
         break;
      case XCPT_BREAKPOINT:
         Log ( "ExceptionHandler: Breakpoint violation passed on to system." ) ;
         break;
      case XCPT_SINGLE_STEP:
         Log ( "ExceptionHandler: Singlestep violation passed on to system." ) ;
         break;
      case XCPT_FLOAT_DENORMAL_OPERAND:
         Log ( "ExceptionHandler: Floating point denormal operand violation passed on to system." ) ;
         break;
      case XCPT_FLOAT_INEXACT_RESULT:
         Log ( "ExceptionHandler: Floating point inexact result violation passed on to system." ) ;
         break;
      case XCPT_FLOAT_INVALID_OPERATION:
         Log ( "ExceptionHandler: Floating point invalid operation violation passed on to system." ) ;
         break;
      case XCPT_FLOAT_STACK_CHECK:
         Log ( "ExceptionHandler: Floating point stack check violation passed on to system." ) ;
         break;
      case XCPT_IN_PAGE_ERROR:
         Log ( "ExceptionHandler: In-Page violation passed on to system." ) ;
         break;
      case XCPT_PROCESS_TERMINATE:
//       Log ( "ExceptionHandler: Process Terminate violation passed on to system." ) ;
         break;
      case XCPT_ASYNC_PROCESS_TERMINATE:
//       Log ( "ExceptionHandler: Async Process Terminate violation passed on to system.  Terminator TID %i.", pExceptionReportRecord->ExceptionInfo[0] ) ;
         break;
      case XCPT_NONCONTINUABLE_EXCEPTION:
         Log ( "ExceptionHandler: Non-continuable violation passed on to system." ) ;
         break;
      case XCPT_INVALID_DISPOSITION:
         Log ( "ExceptionHandler: Invalid Disposition violation passed on to system." ) ;
         break;
      case XCPT_INVALID_LOCK_SEQUENCE:
         Log ( "ExceptionHandler: Invalid Lock Sequence violation passed on to system." ) ;
         break;
      case XCPT_ARRAY_BOUNDS_EXCEEDED:
         Log ( "ExceptionHandler: Array Bound Exceeded violation passed on to system." ) ;
         break;
      case XCPT_B1NPX_ERRATA_02:
         Log ( "ExceptionHandler: B1NPX_ERRATA_02 violation passed on to system." ) ;
         break;
      case XCPT_UNWIND:
//       Log ( "ExceptionHandler: Unwind violation passed on to system." ) ;
         break;
      case XCPT_BAD_STACK:
         Log ( "ExceptionHandler: Bad Stack violation passed on to system." ) ;
         break;
      case XCPT_INVALID_UNWIND_TARGET:
         Log ( "ExceptionHandler: Invalid Unwind Target violation passed on to system." ) ;
         break;
      default:
         Log ( "Exception %08lX detected and passed on for system handling.", pExceptionReportRecord->ExceptionNum ) ;
         break;
   } /* endswitch */

   return ( XCPT_CONTINUE_SEARCH ) ;
}

/***************************************************************************
 *                                                                         *
 *  Exception Handler for Win32                                            *
 *                                                                         *
 ***************************************************************************/

#else // __NT__

#include "Excpt.h"

extern EXCEPTION_DISPOSITION __cdecl ExceptionHandler ( 
   struct _EXCEPTION_RECORD * /* pExceptionReportRecord */, 
   void * /* EstablisherFrame */, 
   struct _CONTEXT *pContextRecord, 
   void * /* pDispatcherContext */ 
) {

   Log ( 
      "WARNING: An unhandled exception has occurred at %08X.\n"
      "  The registers were as follows:\n"
      "    AX:%08lX  BX:%08lX  CX:%08lX  DX:%08lX\n"
      "    FL:%08lX  DI:%08lX  SI:%08lX  BP:%08lX\n"
      "    CS:%08lX  IP:%08lX  SS:%08lX  SP:%08lX\n"
      "    DS:%08lX  ES:%08lX  FS:%08lX  GS:%08lX",
      pContextRecord->Eip,
      pContextRecord->Eax,
      pContextRecord->Ebx,
      pContextRecord->Ecx,
      pContextRecord->Edx,
      pContextRecord->EFlags,
      pContextRecord->Edi,
      pContextRecord->Esi,
      pContextRecord->Ebp,
      pContextRecord->SegCs,
      pContextRecord->Eip,
      pContextRecord->SegSs,
      pContextRecord->Esp,
      pContextRecord->SegDs,
      pContextRecord->SegEs,
      pContextRecord->SegFs,
      pContextRecord->SegGs ) ;

   PLONG Base = ( PLONG ) pContextRecord->Ebp;
   while ( Base && ( *Base > ( LONG ) Base ) ) {
      Log ( "  Calling function was at %08lX.", *( Base + 1 ) );
      Base = ( PLONG ) * Base;
   } /* endwhile */

   return ( ExceptionContinueSearch ) ;
}

#endif

/****************************************************************************
 *                                                                          *
 *            Call Tracing Functions (borrowed from GLDS::SHELL)            *
 *                                                                          *
 ****************************************************************************/

  // Constants
 
#define DSCSIZE 64              /* Size of name, not counting sentinal. */
#define FSTSIZE 32              /* Number of names. */


  // Private Data, shared over the next few functions.

static char FunctionStack       [MAX_THREADS+1] [FSTSIZE] [DSCSIZE + 1] ;
static int  FunctionStackTop    [MAX_THREADS+1] ;

/****************************************************************************
 *                                                                          *
 *                   Utility function: Who Called Me?                       *
 *                                                                          *
 ****************************************************************************/

extern char *SH__RoutineThatCalledMe ( void ) {

   int Thread = Sys_ThreadNumber ( *_threadid ) ;
   int StackDepth = FunctionStackTop[Thread-1] ;

   if ( Thread > MAX_THREADS ) 
      return ( "???? Unregistered Thread  " ) ;

   if ( StackDepth <= 1 )
      return ( "???? Underflowed          " ) ;

   if ( StackDepth > FSTSIZE )
      return ( "???? Overflowed           " ) ;

   return ( FunctionStack[Thread-1][StackDepth-2] );
}


/****************************************************************************
 *                                                                          *
 *                   Utility function: Dump Call Stack.                     *
 *                                                                          *
 ****************************************************************************/

extern void SH__DumpStack ( FILE *File, int StackPtr ) {

   int Thread = Sys_ThreadNumber ( *_threadid ) ;
   char Date[9], Time[9] ;
   int StackDepth = FunctionStackTop[Thread-1] ;

   // If bad thread number, exit.
   if ( Thread > MAX_THREADS ) 
      return ;

   if ( StackPtr ) {
      fprintf ( File,
         "%s %s %02i: SP=%08lX, BP=%08lX, Return=%08lX.\n",
         _strtime(Time), _strdate(Date), Thread,
         StackPtr, *((int*)StackPtr+0), *((int*)StackPtr+1) ) ;
   } /* endif */

   if ( StackDepth >= 0  ) {
      do {
         if ( StackDepth < FSTSIZE ) {
            fprintf ( File, "%s %s %02i: Called by function '%s'.\n",
               _strtime(Time), _strdate(Date), Thread,
               FunctionStack     [Thread-1] [StackDepth] ) ;
         } /* endif */
      } while ( StackDepth-- ); /* enddo */
   } /* endif */
}


/****************************************************************************
 *                                                                          *
 *        Prologue Function, called at entry to functions by __PRO.         *
 *                                                                          *
 ****************************************************************************/

extern void SH__PrintName ( char *Name, int Len, int /*StackPtr*/ ) {

   // If bad thread number, exit.
   int Thread = Sys_ThreadNumber(*_threadid) ;
   if ( Thread > MAX_THREADS ) 
      return ;

   int StackDepth = FunctionStackTop[Thread-1] ;
   if ( ( StackDepth >= 0 ) && ( StackDepth < FSTSIZE ) ) {
      char Temp[260]; sprintf ( Temp, "%0.*s", Len, Name ) ;
      strcpy ( FunctionStack[Thread-1][StackDepth], Temp );
   } /* endif */

   FunctionStackTop [Thread-1] ++;
}

/****************************************************************************
 *                                                                          *
 *        Epilogue Function, called at exit from functions by __EPI.        *
 *                                                                          *
 ****************************************************************************/

extern void SH__EndProc ( int /*StackPtr*/ ) {

   // If bad thread number, exit.
   int Thread = Sys_ThreadNumber(*_threadid) ;
   if ( Thread > MAX_THREADS ) 
      return ;

   // Reduce the stack by one level.
   int StackDepth = --(FunctionStackTop[Thread-1]) ;
}

/****************************************************************************
 *                                                                          *
 *                          Register New Thread                             *
 *                                                                          *
 ****************************************************************************/

extern unsigned __MaxThreads = MAX_THREADS ;

#ifdef __NT__
   static Mutex ThreadListLock ( "System::ThreadListLock" ) ;
   static int ThreadList       [MAX_THREADS] ;
   static int Overflowed       [MAX_THREADS] ;
#endif // __NT__

extern int Sys_RegisterThread ( void ) {

   #ifdef __DOS__

      return ( 1 ) ;

   #else

      #ifdef __OS2__

         return ( *_threadid ) ;

      #else

         int ThreadNumber ;

         ThreadListLock.Request ( ) ;
         for ( ThreadNumber=0; ThreadNumber<__MaxThreads; ThreadNumber++ )
            if ( ThreadList[ThreadNumber] == 0 ) 
               break ;
         ThreadNumber ++ ;
         ThreadList[ThreadNumber-1] = *_threadid ;
         ThreadListLock.Release ( ) ;

         FunctionStackTop[ThreadNumber-1] = 0 ;
         Overflowed[ThreadNumber-1] = FALSE ;

         return ( ThreadNumber ) ;

      #endif

   #endif

}


/****************************************************************************
 *                                                                          *
 *                         Deregister Old Thread                            *
 *                                                                          *
 ****************************************************************************/

extern void Sys_DeregisterThread ( void ) {

   #ifdef __NT__

      int ThreadNumber ;
      ThreadListLock.Request ( ) ;                         // Lock the thread list.
      ThreadNumber = Sys_ThreadNumber ( *_threadid ) ;     // Get the current thread number.
      ThreadList[ThreadNumber-1] = 0 ;                     // Clear the list entry.
      ThreadListLock.Release ( ) ;                         // Unlock the thread list.

   #endif
}


/****************************************************************************
 *                                                                          *
 *                           Get Thread Number                              *
 *                                                                          *
 ****************************************************************************/

extern int Sys_ThreadNumber ( register int ThreadID ) {

#ifdef __DOS__

   return ( 1 ) ;

#else

   #ifdef __OS2__

      return ( ThreadID ) ;

   #else

      register int ThreadNumber ;

      ThreadListLock.Request ( INFINITE ) ;
      for ( ThreadNumber=0; ThreadNumber<__MaxThreads; ThreadNumber++ )
         if ( ThreadList[ThreadNumber] == ThreadID )
            break ;
      ThreadNumber ++ ;
      ThreadListLock.Release ( ) ;

      return ( ThreadNumber ) ;

   #endif

#endif

}

#ifdef __SW_EP
   You shouldn't compile this module with /ep flag!
#endif

