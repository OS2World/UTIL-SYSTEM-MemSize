/****************************************************************** EXCEPT.H
 *                                                                         *
 *                            Exception Handler                            *
 *                                                                         *
 ***************************************************************************/

#ifndef EXCEPT_H
#define EXCEPT_H

#include "System.h"

#ifdef __OS2__

   extern BOOL BreakKey ;
   extern ULONG APIENTRY ExceptionHandler (
       PEXCEPTIONREPORTRECORD pExceptionReportRecord,
       PEXCEPTIONREGISTRATIONRECORD pExceptionRegistrationRecord,
       PCONTEXTRECORD pContextRecord,
       PVOID pDispatcherContext
   ) ;

        // *** IMPORTANT *** IMPORTANT *** IMPORTANT *** IMPORTANT *** IMPORTANT ***
        //
        //     These two macros MUST be invoked within the same function body,
        //           preferably at the highest level function possible.
        //
        // *** IMPORTANT *** IMPORTANT *** IMPORTANT *** IMPORTANT *** IMPORTANT ***

   #define REGISTER_EXCEPTION_HANDLER(x)                                        \
      EXCEPTIONREGISTRATIONRECORD ExceptionRecord = {0, ExceptionHandler};      \
      DosError ( FERR_DISABLEEXCEPTION | FERR_DISABLEHARDERR );                 \
      DosSetExceptionHandler ( &ExceptionRecord );                              

   #define UNREGISTER_EXCEPTION_HANDLER(x)                                      \
      DosUnsetExceptionHandler ( &ExceptionRecord ) ;


#else   // __NT__

   extern EXCEPTION_DISPOSITION __cdecl ExceptionHandler ( 
      struct _EXCEPTION_RECORD *pExceptionReportRecord, 
      void *EstablisherFrame, 
      struct _CONTEXT *pContextRecord, 
      void *pDispatcherContext 
   ) ;

inline void REGISTER_EXCEPTION_HANDLER ( ) {
   _asm { /* assembler block */
      LEA EAX,ExceptionHandler
      push EAX
      push FS:[0]
      mov FS:[0],ESP
   } /* end assembler block */
} /* end inline */

inline void UNREGISTER_EXCEPTION_HANDLER ( ) {
   _asm { /* assembler block */
      mov EAX,[ESP]            
      mov FS:[0],EAX           
      add ESP,8                
   } /* end assembler block */
} /* end inline */

#endif // If OS/2 or NT.

#include <stdio.h>

extern char *SH__RoutineThatCalledMe ( void ) ;
extern void SH__DumpStack ( FILE *File, int StackPtr ) ;
extern "C" void SH__PrintName ( char *Name, int Len, int StackPtr ) ;   // Called from assembler.
extern "C" void SH__EndProc ( int StackPtr ) ;                          // Called from assembler.

#endif // If not already loaded.
