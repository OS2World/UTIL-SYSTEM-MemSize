/***************************************************************** PROFILE2.H
 *                                                                          *
 *                        Profile Object (for GUI)                          *
 *                                                                          *
 ****************************************************************************/

#ifndef PROFILE2_H
#define PROFILE2_H

#include "Profile.h"

class Profile2 : public Profile {

   public:

      Profile2 ( char *Name, ANCHOR Anchor, HMODULE Library, int DialogID, HelpWindow *Help, BOOL ResetFlag ) ;
      Profile2 ( char *Name, ANCHOR Anchor, char *HomePath, int MustExist=FALSE ) : Profile ( Name, Anchor, HomePath, MustExist ) { } ;
      Profile2 ( char *Name ) : Profile ( Name ) { } ;
      virtual ~Profile2 ( ) { } ;

      void PutIniData ( PINIDATA IniData ) ;
} ;

#endif // PROFILE2_H
