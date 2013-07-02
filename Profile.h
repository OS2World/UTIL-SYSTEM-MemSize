/****************************************************************** PROFILE.H
 *                                                                          *
 *                      Profile Object definitions                          *
 *                                                                          *
 ****************************************************************************/

#ifndef PROFILE_H
#define PROFILE_H

#include "HelpWin.h"
#include "IniData.h"
#include "ReString.h"

class Profile {

   protected:

      char *Name ;                  // Application/Section Name.
      char ProfileName [_MAX_PATH] ;// The name of the profile file, if private.
      int Ready ;                   // Flag: Is ready for use?
      #ifdef __OS2__
         int Opened ;               // Flag: Was the handle opened, or already open?
         HINI Handle ;              // INI file handle.
      #endif // __OS2__ vs __NT__

   public:

      Profile ( char *Name, ANCHOR Anchor, char *HomePath, int MustExist=FALSE ) ;
      Profile ( char *Name ) ;
      virtual ~Profile ( ) ;

      BOOL IsReady ( ) { return ( Ready ) ; }
      char *QueryName ( ) { return ( Name ) ; }

      BOOL GetItem ( char *ItemName, void *Item, int Size ) ;
      BOOL PutItem ( char *ItemName, void *Item, int Size ) ;

      BOOL GetString ( char *ItemName, char *Item, int MaxSize ) ;
      BOOL GetString ( char *ItemName, char* &Item ) ;
      BOOL PutString ( char *ItemName, char *Item ) ;

      void Reset ( char *ItemName ) ;
      void Reset ( ) ;

      void Copy ( Profile &Source ) ;
      void PhaseOut ( ) ;

      int GetIniData ( PINIDATA IniData ) ;
} ;

#endif
