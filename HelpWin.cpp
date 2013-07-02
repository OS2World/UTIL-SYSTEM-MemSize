// Class HELPWIN: Encapsulates the startup/shutdown logic for an OS/2-PM help window.

#include "System.h"

#include <string.h>

#include "Debug.h"
#include "HelpWin.h"


  // Constructor

HelpWindow::HelpWindow ( ANCHOR anchor, HMODULE Module, USHORT Id, PSZ Filename, PSZ Title ) : Anchor(anchor), Handle(0), Created(FALSE) {

#ifdef __OS2__

   HELPINIT HelpInit ;
   memset ( &HelpInit, 0, sizeof(HelpInit) ) ;

   HelpInit.cb = sizeof(HelpInit) ;
   HelpInit.phtHelpTable = (PHELPTABLE)MAKEULONG(Id,0xFFFF);
   HelpInit.hmodHelpTableModule = Module ;
   HelpInit.pszHelpWindowTitle = Title ;
   HelpInit.fShowPanelId = CMIC_HIDE_PANEL_ID ;
   HelpInit.pszHelpLibraryName = Filename ;

   Handle = WinCreateHelpInstance ( Anchor, &HelpInit ) ;

   if ( Handle == 0 ) {
      ERRORID Error = WinGetLastError ( Anchor ) ;
      if ( ( Error & 0xFFFF ) != PMERR_NO_MSG_QUEUE ) {
         char Message [512] ;
         Log ( "ERROR: Unable to create help instance from file '%s'.  %s", Filename, InterpretWinError(Anchor,Message) ) ;
      } /* endif */
   } else {
      Created = TRUE ;
   } /* endif */

#else

   anchor = 0 ; Module = 0 ; Id = 0 ; Filename = 0 ; Title = 0 ; // Pacify the compiler.

#endif

} /* endmethod */


  // Alternate Constructor (Wrapper for preexisting window).

HelpWindow::HelpWindow ( HWND TargetWindow, HWND SourceWindow ) : Anchor(0), Handle(0), Created(FALSE) {
   #ifdef __OS2__
      Anchor = WinQueryAnchorBlock ( TargetWindow ) ;
      Handle = WinQueryHelpInstance ( SourceWindow ) ; 
   #else 
      TargetWindow = SourceWindow = 0 ;  // Pacify the compiler.
   #endif
} /* endmethod */


  // Destructor

HelpWindow::~HelpWindow ( ) {

   if ( Handle && Created ) {
      #ifdef __OS2__
         WinDestroyHelpInstance ( Handle ) ;
      #endif
      Handle = 0 ;
   } /* endif */

} /* endmethod */


  // Associate with a window.

BOOL HelpWindow::Associate ( HWND Window ) {

   if ( Handle == 0 ) 
      return ( FALSE ) ;

#ifdef __OS2__

   if ( !WinIsWindow ( Anchor, Handle ) ) {
      Log ( "WARNING: Attempt to associate window %08p with invalid help instance %08p.", Window, Handle ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( !WinIsWindow ( Anchor, Window ) ) {
      Log ( "WARNING: Attempt to associate help instance with invalid window %08p.", Window ) ;
      return ( FALSE ) ;
   } /* endif */

   if ( !WinAssociateHelpInstance ( Handle, Window ) ) {
      char Message [512] ;
      Log ( "ERROR: Unable to associate help instance %08p with window %08p.  %s", Handle, Window, InterpretWinError(0,Message) ) ;
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;

#else

   Window = 0 ;  // Pacify the compiler.

   return ( FALSE ) ;

#endif

} /* endmethod */

