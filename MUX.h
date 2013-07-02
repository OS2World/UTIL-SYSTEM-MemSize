/********************************************************************** MUX.H
 *                                                                          *
 *         Wait For Multiple Events, processing messages (OS2/NT)           *
 *                                                                          *
 ****************************************************************************/

#ifndef MUX_H
#define MUX_H

#include "System.h"

extern BOOL Sys_MsgWaitForMultipleObjects ( int EventCount, HEV Events[], int Timeout=INFINITE ) ;

#endif // If not already included.
