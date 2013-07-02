// Class RESTRING: Encapsulates the load/discard
//   logic for a resource String Table entry.

#ifndef RESTRING_H
#define RESTRING_H

#include "System.h"

class ResourceString {

   private:
      HMODULE SavedModule ;
      unsigned long SavedId ;

      #ifdef __OS2__
      void *BlockPointer ;
      #endif

      PSZ StringPointer ;

   public:
      // Constructor
      ResourceString ( HMODULE Module, unsigned long Id ) ;

      // Destructor
      ~ResourceString ( ) ;

      // Typecast Operators
      operator unsigned char * () {
         return ( (unsigned char *) (StringPointer?StringPointer:"") ) ;
      } /* endmethod */

      operator char * () {
         return ( (char *) (StringPointer?StringPointer:"") ) ;
      } /* endmethod */

} ;

#endif
