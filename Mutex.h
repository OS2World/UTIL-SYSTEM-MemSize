/******************************************************************** Mutex.H
 *                                                                          *
 *                   Mutex Semaphore Functions (OS2/NT)                     *
 *                                                                          *
 ****************************************************************************/

#ifndef MUTEX_H
#define MUTEX_H

#ifndef __DOS__

#include "System.h"

class Mutex {

   private:
      HMTX Handle ;
      char *Tag ;
      int DebugFlag ;

   public:
      Mutex ( char *tag = 0, char *Name = 0, int OneInstanceOnly=TRUE ) ;
      ~Mutex ( ) ;

      HMTX QueryHandle ( ) { return ( Handle ) ; }
      void SetDebug ( int flag ) { DebugFlag = flag ; }

      unsigned long Request ( int Timeout = INFINITE ) ;
      unsigned long RequestWin ( int Timeout = INFINITE ) ;
      unsigned long Release ( ) ;
} ;

#endif // not __DOS__

#endif // If not already included.
