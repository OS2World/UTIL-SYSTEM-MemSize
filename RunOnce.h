#ifndef RUNONCE_H
#define RUNONCE_H

#include "Mutex.h"

// Class: Run Once - Prevent multiple concurrent executions of a program.
//
// Note: This is a simple implementation using named semaphores.
//

class RunOnce {

   private:
      Mutex *Semaphore ;

   public:

      RunOnce ( char *Name, int TerminateAtOnce=TRUE ) : Semaphore(0) {
         Semaphore = new Mutex ( "RunOnce", Name, TRUE ) ;
         if ( ( Semaphore == 0 ) || ( Semaphore->QueryHandle() == 0 ) ) 
            if ( TerminateAtOnce ) 
               exit ( 1 ) ;
      } /* endmethod */

      ~RunOnce ( ) {
         if ( Semaphore ) 
            delete Semaphore, Semaphore = 0 ;
      } /* endmethod */

      int isDuplicateInstance ( ) {
         return ( ( Semaphore == 0 ) || ( Semaphore->QueryHandle() == 0 ) ) ;
      } /* endmethod */
} ;

#endif // RUNONCE_H
