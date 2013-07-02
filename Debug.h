/******************************************************************** DEBUG.H
 *                                                                          *
 *  Debugging Aids                                                          *
 *                                                                          *
 ****************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#include <stdlib.h> // Need definition of 'size_t'.

extern void Debug ( HWND hwnd, char *Message, ... ) ;
extern void Log ( char *Message, ... ) ;
extern void DumpMemory ( char *Note, void *pMemory, int Count ) ;

#ifdef __OS2__
   #include "hrtimer.h"
   extern HFILE Timer ;
   extern BOOL OpenTimer ( void ) ;
   extern void CloseTimer ( void ) ;
   extern BOOL GetTime ( PTIMESTAMP pts ) ;
   extern ULONG ComputeElapsedTime ( PTIMESTAMP ptsStart, PTIMESTAMP ptsStop, PULONG pulNs ) ;
#endif // __OS2__

extern void *AllocateMemory ( size_t ByteCount ) ;
extern void FreeMemory ( void *Memory ) ;

extern char *InterpretWinError ( ANCHOR Anchor, char *MessageBuffer ) ;

extern char *InterpretMsg ( MESG msg, char *Buffer ) ;
extern void LogMsgRcvd ( int Indent, char *Prefix, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
extern void LogMsgDone ( int Indent, char *Prefix, MESG msg, MRESULT Result ) ;

#endif
