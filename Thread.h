/******************************************************************* THREAD.H
 *                                                                          *
 *                       Thread Starter Function                            *
 *                                                                          *
 ****************************************************************************/

#ifndef THREAD_H
#define THREAD_H

#include "System.h"

enum Priority {
   PRIORITY_IDLE,
   PRIORITY_LOWEST,
   PRIORITY_BELOW_NORMAL,
   PRIORITY_NORMAL,
   PRIORITY_ABOVE_NORMAL,
   PRIORITY_HIGHEST,
   PRIORITY_TIME_CRITICAL
} ;

extern void Thread_SetPriority ( int Thread, Priority Level ) ;

enum DebugType { DEBUG_OFF, DEBUG_STARTSTOP, DEBUG_FULL } ;
extern enum DebugType ThreadDebug ;

extern TID StartThread ( char *Name, void(*Function)(void*), int StackSize, void *Parameter ) ;

#endif
