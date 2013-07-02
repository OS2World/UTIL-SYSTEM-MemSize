// Class RESTRING: Encapsulates the load/discard
//   logic for a resource String Table entry.

#define INCLUDE_SYSTEM
#include "System.h"

#include "Debug.h"
#include "ReString.h"


  // Constructor

#ifdef __OS2__

ResourceString::ResourceString ( HMODULE Module, unsigned long Id ) : 
   SavedModule(Module), SavedId(Id), BlockPointer(0), StringPointer(0) {

   APIRET Status = DosGetResource ( Module, RT_STRING, Id/16+1, &BlockPointer ) ;
   if ( Status ) {
      Log ( "ERROR: Unable to get string resource.  Module %lu, id %lu, code %08lX.", SavedModule, SavedId, Status ) ;
      return ;
   } /* endif */

   StringPointer = PSZ(BlockPointer) + sizeof(USHORT) ;

   USHORT Index = (USHORT) ( Id % 16 ) ;
   while ( Index-- ) {
      StringPointer += * PUCHAR ( StringPointer ) ;
      StringPointer ++ ;
   } /* endwhile */

   StringPointer ++ ;

} /* endmethod */


  // Destructor

ResourceString::~ResourceString ( ) {

   if ( BlockPointer ) 
      DosFreeResource ( BlockPointer ) ;

} /* endmethod */

#else

ResourceString::ResourceString ( HMODULE Module, unsigned long Id ) : SavedModule(Module), SavedId(Id), StringPointer(0) {

   char Buffer [200] ;
   if ( ! LoadString ( Module, Id, Buffer, sizeof(Buffer) ) ) {
      Log ( "ERROR: Unable to get string resource.  Module %lu, id %lu.  Status %i.", SavedModule, SavedId, GetLastError() ) ;
      return ;
   } /* endif */

   StringPointer = (char*) malloc ( strlen(Buffer) + 1 ) ;
   strcpy ( StringPointer, Buffer ) ;

} /* endmethod */

ResourceString::~ResourceString ( ) {

   if ( StringPointer ) 
      free ( StringPointer ) ;

} /* endmethod */

#endif
