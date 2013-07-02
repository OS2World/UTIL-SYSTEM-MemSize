/******************************************************************** EVENT.H
 *                                                                          *
 *                   Event Semaphore Functions (OS2/NT)                     *
 *                                                                          *
 ****************************************************************************/

#ifndef EVENT_H
#define EVENT_H

#ifndef __DOS__

#include "System.h"

class Event {

   private:
      HEV Handle ;
      char *Tag ;
      int DebugFlag ;

   public:
      Event ( char *tag = 0, char *Name = 0, int OneInstanceOnly=FALSE ) ;
      ~Event ( ) ;

      HEV QueryHandle ( ) { return ( Handle ) ; }
      void SetDebug ( int flag ) { DebugFlag = flag ; }

      unsigned long Wait ( int Timeout = INFINITE ) ;
      unsigned long Post ( ) ;
      unsigned long Reset ( ) ;
} ;

#endif // not __DOS__

#endif // If not already included.
