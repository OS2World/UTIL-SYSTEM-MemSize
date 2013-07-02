// Class PROCESS: Encapsulates the startup/shutdown logic for a OS/2-PM process.

#ifndef PROCESS_H
#define PROCESS_H

#include "System.h"

class Process {

   private:

      char *Name ;
      ANCHOR Anchor ;
      #ifdef __OS2__
         HMQ Queue ;
      #endif // __OS2__

   public:

      Process ( char *Name, long QueueSize = 0 ) ;
      ~Process ( ) ;
      inline ANCHOR QueryAnchor    ( ) { return ( Anchor ) ; }
      #ifdef __OS2__
         inline HMQ QueryQueue     ( ) { return ( Queue ) ; }
      #endif // __OS2__
      void Execute ( ) ;
} ;

#endif
