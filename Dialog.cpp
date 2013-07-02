/***************************************************************** DIALOG.CPP
 *                                                                          *
 *                         Subclassed Basic Dialog                          *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#include "Debug.h"
#include "Dialog.h"
#include "HelpWin.h"
#include "Rectangl.h"
#include "Window.h"

/***************************************************************************
 *                                                                         *
 *  Dialog Message Processor                                               *
 *                                                                         *
 ***************************************************************************/

static DRESULT APIENTRY DialogMessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   switch ( msg ) {

      case WM_INITDLG: {
         Dialog *This = (class Dialog *) PVOIDFROMMP ( mp2 ) ;
         Sys_SetWindowData ( hwnd, This ) ;
         break ; }

   } /* endswitch */

   Dialog *This = (class Dialog *) Sys_GetWindowData ( hwnd ) ;
   return ( This ? This->MessageProcessor ( hwnd, msg, mp1, mp2 ) : Sys_DefDialogProc ( hwnd, msg, mp1, mp2 ) ) ;
} 

/***************************************************************************
 *                                                                         *
 *  Dialog: Constructor                                                    *
 *                                                                         *
 ***************************************************************************/

Dialog::Dialog ( HWND parent, HWND owner, HMODULE resource, int id ) : 
   Parent(parent), Owner(owner), Resource(resource), Id(id), Result(0), 
   Handle ( 0 ), DebugMessages(FALSE), MessageIndent(0) {

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: Destructor                                                     *
 *                                                                         *
 ***************************************************************************/

Dialog::~Dialog ( ) {

   if ( Handle ) 
      Sys_DestroyWindow ( Handle ),
      Handle = 0 ;

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: Load Dialog.  Must be called after all constructors.           *
 *                                                                         *
 ***************************************************************************/

int Dialog::Load ( ) {
   #ifdef __OS2__
      Handle = WinLoadDlg ( Parent, Owner, DialogMessageProcessor, Resource, Id, this ) ;
   #else
      Handle = CreateDialog ( Resource, (char*)Id, Owner, (FARPROC)DialogMessageProcessor ) ;
   #endif
   return ( Handle != 0 ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: Process Dialog                                                 *
 *                                                                         *
 ***************************************************************************/

int Dialog::Process ( HACCEL AcceleratorTable ) {

   Result = FALSE ;

   if ( Handle ) {

      #ifdef __OS2__

         Result = int ( WinProcessDlg ( Handle ) ) ;
         AcceleratorTable = 0 ;  // Pacify the compiler.

      #else

         MSG Message ;
         while ( GetMessage ( &Message, 0, 0, 0 ) ) {
            if ( AcceleratorTable ) {
               if ( !TranslateAccelerator ( Message.hwnd, AcceleratorTable, &Message ) ) {
                  TranslateMessage ( &Message ) ;
                  DispatchMessage ( &Message ) ;
               } /* endif */
            } else {
               TranslateMessage ( &Message ) ;
               DispatchMessage ( &Message ) ;
            } /* endif */
         } /* endwhile */
         
      #endif

   } /* endif */

   return ( Result ) ;

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: Dismiss Dialog                                                 *
 *                                                                         *
 ***************************************************************************/

void Dialog::Dismiss ( int Status ) {

   Result = Status ;

   #ifdef __OS2__
      Sys_EndDialog ( Handle, Status ) ;
   #else
      PostQuitMessage ( Status ) ;
   #endif

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: INITDLG message processor                                      *
 *                                                                         *
 ***************************************************************************/

DRESULT Dialog::InitDialog ( HWND hwnd ) {

   // Build a window object for utility.
   Window ThisWindow ( hwnd ) ;

   // Associate dialog with help instance of owner window.
   HelpWindow TheHelpWindow ( hwnd, OWNER(hwnd) ) ;
   TheHelpWindow.Associate ( hwnd ) ;

   // Center the dialog over its owner.
   RECTANGLE OwnerRectangle(OWNER(hwnd)), DialogRectangle(hwnd), DesktopRectangle(Parent) ;
   POINTL Position ;
   Position.x = ( ( OwnerRectangle.xRight - OwnerRectangle.xLeft ) - ( DialogRectangle.xRight - DialogRectangle.xLeft ) ) / 2 + OwnerRectangle.xLeft ;
   Position.y = ( ( OwnerRectangle.yTop - OwnerRectangle.yBottom ) - ( DialogRectangle.yTop - DialogRectangle.yBottom ) ) / 2 + OwnerRectangle.yBottom ;
   #ifdef __OS2__
      WinMapWindowPoints ( OWNER(hwnd), Parent, &Position, 1 ) ;
   #else
      MapWindowPoints ( OWNER(hwnd), Parent, &Position, 1 ) ;
   #endif

   // Keep the dialog fully on the desktop, if possible.
   if ( Position.x+DialogRectangle.xRight-DialogRectangle.xLeft >= DesktopRectangle.xRight )
      Position.x = DesktopRectangle.xRight - ( DialogRectangle.xRight - DialogRectangle.xLeft ) ;
   if ( Position.y+DialogRectangle.yTop-DialogRectangle.yBottom >= DesktopRectangle.yTop )
      Position.y = DesktopRectangle.yTop - ( DialogRectangle.yTop - DialogRectangle.yBottom ) ;
   if ( Position.x < DesktopRectangle.xLeft ) 
      Position.x = DesktopRectangle.xLeft ;
   if ( Position.y < DesktopRectangle.yBottom ) 
      Position.y = DesktopRectangle.yBottom ;
   ThisWindow.Move ( Position.x, Position.y ) ;

   return ( Sys_DefDialogProc ( hwnd, WM_INITDLG, 0, 0 ) ) ; 

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: COMMAND message processor                                      *
 *                                                                         *
 ***************************************************************************/

DRESULT Dialog::Command ( MPARAM1 mp1, MPARAM2 ) {

   switch ( SHORT1FROMMP(mp1) ) {

      case DID_OK: {
         Dismiss ( TRUE ) ;
         break; }

      case DID_CANCEL: {
         Dismiss ( FALSE ) ;
         break; }

   } /* endswitch */

   return ( 0 ) ;

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Dialog: Message Processor                                              *
 *                                                                         *
 ***************************************************************************/

DRESULT Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Dialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   DRESULT Result ;

   switch ( msg ) {
      case WM_INITDLG:  Result = InitDialog ( hwnd ) ;                          break ;
      case WM_COMMAND:  Result = Command ( mp1, mp2 ) ;                         break ;
      default:          Result = Sys_DefDialogProc ( hwnd, msg, mp1, mp2 ) ;    break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Dialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  PageDialog: Constructor                                                *
 *                                                                         *
 ***************************************************************************/

PageDialog::PageDialog ( HWND parent, HWND owner, HMODULE resource, int id ) : Dialog(parent,owner,resource,id) {
   Load ( ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  PageDialog: Destructor                                                 *
 *                                                                         *
 ***************************************************************************/

PageDialog::~PageDialog ( ) {
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  PageDialog: Bind to Notebook Page                                      *
 *                                                                         *
 ***************************************************************************/

void PageDialog::BindToPage ( HWND notebook, ULONG pagehandle ) {
   #ifdef __OS2__
      Notebook = notebook ; PageHandle = pagehandle ;
      Sys_SendMessage ( Notebook, BKM_SETPAGEWINDOWHWND, MPFROMLONG(PageHandle), MPFROMLONG(Handle) ) ;
   #else
      // ELABORATE 
      notebook = 0 ;
      pagehandle = 0 ;
   #endif
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  PageDialog: Turn to this page                                          *
 *                                                                         *
 ***************************************************************************/

void PageDialog::TurnTo ( ) {
   #ifdef __OS2__
      Sys_SendMessage ( Notebook, BKM_TURNTOPAGE, MPFROMLONG(PageHandle), 0 ) ;
   #else
      // ELABORATE 
   #endif 
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  PageDialog: INITDLG message processor                                  *
 *                                                                         *
 ***************************************************************************/

DRESULT PageDialog::InitDialog ( HWND hwnd ) {

   // Associate dialog with help instance of owner window.
   #ifdef __OS2__
      WinAssociateHelpInstance ( WinQueryHelpInstance(OWNER(hwnd)), hwnd ) ;
   #endif // __OS2__

   // Return through the system default dialog processor.
   return ( Sys_DefDialogProc ( hwnd, WM_INITDLG, 0, 0 ) ) ; 

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  PageDialog: COMMAND message processor                                  *
 *                                                                         *
 ***************************************************************************/

DRESULT PageDialog::Command ( MPARAM1, MPARAM2 ) {
   return ( 0 ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  PageDialog: Message Processor                                          *
 *                                                                         *
 ***************************************************************************/

DRESULT PageDialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "PageDialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   DRESULT Result ;

   switch ( msg ) {
      case WM_INITDLG:  Result = InitDialog ( hwnd ) ;                          break ;
      case WM_COMMAND:  Result = Command ( mp1, mp2 ) ;                         break ;
      default:          Result = Dialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "PageDialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */


