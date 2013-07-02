// Class HELPWIN: Encapsulates the startup/shutdown logic for an OS/2-PM help window.

#ifndef HELPWIN_H
#define HELPWIN_H

#include "System.h"

class HelpWindow {
   private:
      ANCHOR Anchor ;
      HWND Handle ;
      int Created ;
 
   public:
      HelpWindow ( ANCHOR Anchor, HMODULE Module, USHORT Id, PSZ Filename, PSZ Title ) ;
      HelpWindow ( HWND TargetWindow, HWND SourceWindow ) ;
      ~HelpWindow ( ) ;
      BOOL Associate ( HWND Window ) ;
      HWND QueryHandle () { return ( Handle ) ; }
} ;

#endif