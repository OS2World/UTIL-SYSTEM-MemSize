/**************************************************************** WINDOW.CPP
 *                                                                         *
 *  Class: Window & Window Class                                           *
 *                                                                         *
 ***************************************************************************/

#include "System.h"

#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "Window.h"

/***************************************************************************
 *                                                                         *
 *  Window Message Processor                                               *
 *                                                                         *
 ***************************************************************************/

static MRESULT APIENTRY WindowMessageProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( msg ) {

      case WM_CREATE: {
         Window *This = (class Window *) PVOIDFROMMP ( mp1 ) ;
         Sys_SetWindowData ( hwnd, This ) ;
         return ( This->MessageProcessor ( hwnd, msg, mp1, mp2 ) ) ; }

   } /* endswitch */

   Window *This = (class Window *) Sys_GetWindowData ( hwnd ) ;
   return ( This->MessageProcessor ( hwnd, msg, mp1, mp2 ) ) ;
} 

/***************************************************************************
 *                                                                         *
 *  WindowClass: Constructors                                              *
 *                                                                         *
 ***************************************************************************/

WindowClass::WindowClass ( Process &Proc, int Style ) : Name ( "Window" ) {

   Registered = WinRegisterClass ( Proc.QueryAnchor(), (PSZ)"Window", WindowMessageProcessor, Style, sizeof(PVOID) ) ;

} /* endmethod */

WindowClass::WindowClass ( char *name ) : Name ( name ), Registered(TRUE) {

   // Do nothing.

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Constructors                                                   *
 *                                                                         *
 ***************************************************************************/

Window::Window ( WindowClass &WndClass, HWND Parent, char *Title, int Style,
   int x, int y, int cx, int cy, HWND Owner, HWND Zorder, int Id, void *Parms ) : 
   Handle(0), Created(TRUE), Lock("Window::Lock"), 
   DebugMessages(FALSE), MessageIndent(0), ControlsHidden(FALSE), NumberOfHiddenControls(0),
   fTextColor(FALSE), fBackColor(FALSE), fFontNameSize(FALSE),
   AnchorPoint(ANCHOR_BL), fAnchored(FALSE) {

   BackColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOW, 0 ) ;
   TextColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0 ) ;
   strcpy ( FontNameSize, "" ) ;

   Lock.Request ( ) ;

   Handle = WinCreateWindow ( Parent, WndClass.QueryName(), Title, Style,
      x, y, cx, cy, Owner, Zorder, Id, Parms, 0 ) ;

   Lock.Release ( ) ;

} /* endmethod */

Window::Window ( HWND handle ) : Handle(handle), Created(FALSE) {
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Destructor                                                     *
 *                                                                         *
 ***************************************************************************/

Window::~Window ( ) {
   if ( Created && Handle ) {
      Lock.Request ( ) ;
      WinDestroyWindow ( Handle ) ;
      Handle = 0 ;
      Lock.Release ( ) ;
   } /* endif */
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Query Anchor Block Handle (OS/2)                               *
 *                                                                         *
 ***************************************************************************/

unsigned long Window::QueryAnchor ( ) { 
   #ifdef __OS2__
      return ( WinQueryAnchorBlock ( Handle ) ) ; 
   #else
      return ( 0 ) ;
   #endif
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Set Window Background Color                                    *
 *                                                                         *
 ***************************************************************************/

void Window::Set_BackColor ( ULONG color ) {
   fBackColor = TRUE ;
   BackColor = color ;
   #ifdef __OS2__
      SetPresParam ( PP_BACKGROUNDCOLOR, sizeof(BackColor), &BackColor ) ;
   #endif // __OS2__
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Set Window Text Color                                          *
 *                                                                         *
 ***************************************************************************/

void Window::Set_TextColor ( ULONG color )  {
   fTextColor = TRUE ;
   TextColor = color ;
   #ifdef __OS2__
      SetPresParam ( PP_FOREGROUNDCOLOR, sizeof(TextColor), &TextColor ) ;
   #endif // __OS2__
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Set Window Font                                                *
 *                                                                         *
 ***************************************************************************/

void Window::Set_FontNameSize ( char *fontnamesize ) {
   fFontNameSize = TRUE ;
   strcpy ( FontNameSize, fontnamesize ) ;
   #ifdef __OS2__
      SetPresParam ( PP_FONTNAMESIZE, strlen(FontNameSize)+1, FontNameSize ) ;
   #endif // __OS2__
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Methods for causing repaint, both immediate and deferred.      *
 *                                                                         *
 ***************************************************************************/

void Window::Invalidate ( RECTANGLE &Rectangle ) {
   RECTL rcl ( Rectangle.Get ( ) ) ;
   WinInvalidateRect ( Handle, &rcl, 0 ) ;
} /* endmethod */

void Window::Invalidate ( ) {
   Sys_InvalidateWindow ( Handle ) ;
} /* endmethod */

void Window::Update ( ) {
   Sys_UpdateWindow ( Handle ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Positioning Methods                                            *
 *                                                                         *
 ***************************************************************************/

void Window::Reposition ( int x, int y, int cx, int cy, PositionClass State ) {
   WinSetWindowPos ( Handle, 0, x, y, cx, cy, SWP_MOVE | SWP_SIZE | 
      (State==MINIMIZE?SWP_MINIMIZE:(State==MAXIMIZE?SWP_MAXIMIZE:SWP_RESTORE)) ) ;
} /* endmethod */

void Window::Show ( ) {
   Sys_ShowWindow ( Handle, TRUE ) ;
} /* endmethod */

void Window::Hide ( ) {
   Sys_ShowWindow ( Handle, FALSE ) ;
} /* endmethod */

void Window::Move ( int x, int y ) {
   WinSetWindowPos ( Handle, 0, x, y, 0, 0, SWP_MOVE ) ;
} /* endmethod */

void Window::Size ( int cx, int cy ) {
   WinSetWindowPos ( Handle, 0, 0, 0, cx, cy, SWP_SIZE ) ;
} /* endmethod */

void Window::Minimize ( ) {
   WinSetWindowPos ( Handle, 0, 0, 0, 0, 0, SWP_MINIMIZE ) ;
} /* endmethod */

void Window::Maximize ( ) {
   WinSetWindowPos ( Handle, 0, 0, 0, 0, 0, SWP_MAXIMIZE ) ;
} /* endmethod */

void Window::Restore ( ) {
   WinSetWindowPos ( Handle, 0, 0, 0, 0, 0, SWP_RESTORE ) ;
} /* endmethod */

void Window::Activate ( ) {
   WinSetWindowPos ( Handle, 0, 0, 0, 0, 0, SWP_ACTIVATE ) ;
} /* endmethod */

void Window::Deactivate ( )  {
   WinSetWindowPos ( Handle, 0, 0, 0, 0, 0, SWP_DEACTIVATE ) ;
} /* endmethod */

void Window::Surface ( ) {
   WinSetWindowPos ( Handle, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER ) ;
} /* endmethod */

void Window::Sink ( ) {
   WinSetWindowPos ( Handle, HWND_BOTTOM, 0, 0, 0, 0, SWP_ZORDER ) ;
} /* endmethod */

void Window::SetFocus ( ) {
   Sys_SetFocus ( Handle ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Enable/Disable Input                                           *
 *                                                                         *
 ***************************************************************************/

void Window::Enable ( int Flag ) {
   Sys_EnableWindow ( Handle, Flag ) ;
} /* endmethod */

void Window::Disable ( ) {
   Sys_EnableWindow ( Handle, FALSE ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Is the window visible (though not necessarily showing)?        *
 *                                                                         *
 ***************************************************************************/

BOOL Window::isVisible ( ) {
   #ifdef __OS2__
      return ( WinIsWindowVisible ( Handle ) ) ;
   #else // __NT__
      return ( IsWindowVisible ( Handle ) ) ;
   #endif // __OS2__ vs __NT__
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Set pointer to private window data.                            *
 *                                                                         *
 ***************************************************************************/

void Window::SetData ( void *data ) {
   Sys_SetWindowData ( Handle, data ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Get pointer to private window data.                            *
 *                                                                         *
 ***************************************************************************/

void *Window::GetData ( ) {
   return ( Sys_GetWindowData ( Handle ) ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Set/Get Window Text                                            *
 *                                                                         *
 ***************************************************************************/

void Window::SetText ( char *Text ) {
   Sys_SetWindowText ( Handle, Text ) ;
} /* endmethod */

void Window::GetText ( char *Text, int Size ) {
   Sys_GetWindowText ( Handle, Text, Size ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Start Timer                                                    *
 *                                                                         *
 ***************************************************************************/

void Window::StartTimer ( int ID, int Milliseconds ) {
   Sys_StartTimer ( Handle, ID, Milliseconds ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Stop Timer                                                     *
 *                                                                         *
 ***************************************************************************/

void Window::StopTimer ( int ID ) {
   Sys_StopTimer ( Handle, ID ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Get Window Rectangle                                           *
 *                                                                         *
 ***************************************************************************/

RECTANGLE Window::Rectangle ( ) {
   return ( RECTANGLE ( Handle ) ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Set Window Text                                                *
 *                                                                         *
 ***************************************************************************/

void Window::SetWindowText ( char *Title ) {

   Sys_SetWindowText ( Handle, Title ) ;

   HSWITCH SwitchHandle = WinQuerySwitchHandle ( Handle, 0 ) ;
   if ( SwitchHandle ) {
      SWCNTRL Entry ;
      if ( !WinQuerySwitchEntry ( SwitchHandle, &Entry ) ) {
         strcpy ( Entry.szSwtitle, Title ) ;
         WinChangeSwitchEntry ( SwitchHandle, &Entry ) ;
      } /* endif */
   } /* endif */

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Get Window Text                                                *
 *                                                                         *
 ***************************************************************************/

char *Window::GetWindowText ( char String[], int StringSize ) {
   String[0] = 0 ;
   Sys_GetWindowText ( Handle, String, StringSize ) ;
   return ( String ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Add Window to Task List                                        *
 *                                                                         *
 ***************************************************************************/

void Window::Add2TaskList ( ) {

   char Title [80] ;
   GetWindowText ( Title, sizeof(Title) ) ;

   PID pid ;
   WinQueryWindowProcess ( Handle, &pid, 0 ) ;

   SWCNTRL swctl ;
   swctl.hwnd = Handle ;
   swctl.hwndIcon = 0 ;
   swctl.hprog = 0 ;
   swctl.idProcess = pid ;
   swctl.idSession = 0 ;
   swctl.uchVisibility = SWL_VISIBLE ;
   swctl.fbJump = SWL_JUMPABLE ;
   strcpy ( swctl.szSwtitle, Title ) ;
   WinAddSwitchEntry ( &swctl ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *                         Add Item to System Menu                          *
 *                                                                          *
 ****************************************************************************/

BOOL Window::AddSysMenuSeparator ( ) {
   return ( AddSysMenuItem ( 0, 0, MIT_END, MIS_SEPARATOR ) ) ;
} /* endmethod */

BOOL Window::AddSysMenuItem ( char *Text, USHORT Id, SHORT Position, USHORT Style, USHORT Attribute, HWND SubMenu, ULONG ItemHandle ) {

  /**************************************************************************
   * Construct the menu item descriptor.                                    *
   **************************************************************************/

   MENUITEM Item ;
   Item.iPosition = Position ;
   Item.afStyle = Style ;
   Item.afAttribute = Attribute ;
   Item.id = Id ;
   Item.hwndSubMenu = SubMenu ;
   Item.hItem = ItemHandle ;

  /**************************************************************************
   * Obtain the system menu window handle.                                  *
   **************************************************************************/

   HWND hwndSysMenu = CHILD ( Handle, FID_SYSMENU ) ;
   if ( hwndSysMenu == 0 ) {
      hwndSysMenu = CHILD ( WinQueryWindow(Handle,QW_PARENT), FID_SYSMENU ) ;
      if ( hwndSysMenu == 0 )
         return ( FALSE ) ;
   } /* endif */

  /**************************************************************************
   * Get the system menu's base item and its window handle.                 *
   **************************************************************************/

   USHORT idSysMenu = SHORT1FROMMR ( Sys_SendMessage ( hwndSysMenu, MM_ITEMIDFROMPOSITION, NULL, NULL ) ) ;

   MENUITEM miSysMenu ;
   Sys_SendMessage ( hwndSysMenu, MM_QUERYITEM, MPFROM2SHORT(idSysMenu,FALSE), MPFROMP(&miSysMenu) ) ;

   HWND hwndSysSubMenu = miSysMenu.hwndSubMenu ;

  /**************************************************************************
   * Add the new item to the system menu's submenu, which is what we see.   *
   **************************************************************************/

   Sys_SendMessage ( hwndSysSubMenu, MM_INSERTITEM, MPFROMP(&Item), MPFROMP(Text) ) ;

   return ( TRUE ) ;
}

/***************************************************************************
 *                                                                         *
 *  Window: Message Box                                                    *
 *                                                                         *
 ***************************************************************************/

int Window::MessageBox ( char *Title, char *Message, int Id, int Style ) {
   return ( (int) Sys_MessageBox ( HWND_DESKTOP, Handle, Message, Title, Id, Style ) ) ;
} /* endmethod */

int Window::MessageBox ( char *Message, int Id, int Style ) {
   char Title [80] ;
   GetWindowText ( Title, sizeof(Title) ) ;
   return ( (int) Sys_MessageBox ( HWND_DESKTOP, Handle, Message, Title, Id, Style ) ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Summon Popup Menu                                              *
 *                                                                         *
 ***************************************************************************/

void Window::PopupMenu ( HWND Menu, int x, int y ) {
   WinPopupMenu ( Handle, Handle, Menu, x, y, 0, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1 ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *                         Hide / Show Frame Controls                       *
 *                                                                          *
 ****************************************************************************/

inline void HideControl ( HWND Frame, int FID, int FCF, long &FrameFlags, int &NumberOfHiddenControls, HWND HiddenControls[] ) {
   HWND Control = CHILD ( Frame, FID  ) ;  
   if ( Control ) {
      FrameFlags |= FCF ;
      HiddenControls[NumberOfHiddenControls++] = Control ;
   } /* endif */
}

void Window::HideControls ( BOOL fHide ) {
   if ( fHide == ControlsHidden ) 
      return ;
   ControlsHidden = fHide ;
   #ifdef __OS2__
   if ( fHide ) {
      FrameFlags = 0 ;
      NumberOfHiddenControls = 0 ;
      HideControl ( Handle, FID_SYSMENU,  FCF_SYSMENU,  FrameFlags, NumberOfHiddenControls, HiddenControls ) ;
      HideControl ( Handle, FID_TITLEBAR, FCF_TITLEBAR, FrameFlags, NumberOfHiddenControls, HiddenControls ) ;
      HideControl ( Handle, FID_MINMAX,   FCF_MINMAX,   FrameFlags, NumberOfHiddenControls, HiddenControls ) ;
      HideControl ( Handle, FID_MENU,     FCF_MENU,     FrameFlags, NumberOfHiddenControls, HiddenControls ) ;
   } /* endif */
   SWP OldPosition ;
   WinQueryWindowPos ( Handle, &OldPosition ) ;
   BOOL WasVisible = WinIsWindowVisible ( Handle ) ;
   WinSetWindowPos ( Handle, 0, 0, 0, 0, 0, SWP_RESTORE | SWP_HIDE ) ;
   SWP Position ;
   WinQueryWindowPos ( Handle, &Position ) ;
   RECTL Rectangle = { Position.x, Position.y, Position.x+Position.cx, Position.y+Position.cy } ;
   WinCalcFrameRect ( Handle, &Rectangle, TRUE ) ;
   if ( fHide ) {
      for ( int i=0; i<NumberOfHiddenControls; i++ ) 
         WinSetParent ( HiddenControls[i], HWND_OBJECT, FALSE ) ;
   } else {
      for ( int i=0; i<NumberOfHiddenControls; i++ ) 
         WinSetParent ( HiddenControls[i], Handle, TRUE ) ;
      if ( WinQueryActiveWindow ( WinQueryWindow(Handle,QW_PARENT) ) == Handle ) 
         Sys_SendMessage ( CHILD(Handle,FID_TITLEBAR), TBM_SETHILITE, MPFROMLONG(TRUE), 0 ) ;
   } /* endif */
   SendMessage ( WM_UPDATEFRAME, MPFROMSHORT(FrameFlags), 0 ) ;
   WinCalcFrameRect ( Handle, &Rectangle, FALSE ) ;
   WinSetWindowPos ( Handle, 0, Rectangle.xLeft,  Rectangle.yBottom,
      Rectangle.xRight - Rectangle.xLeft, Rectangle.yTop - Rectangle.yBottom, SWP_SIZE | SWP_MOVE ) ;
   if ( OldPosition.fl & SWP_MAXIMIZE ) {
      WinSetWindowPos ( Handle, 0, Rectangle.xLeft,  Rectangle.yBottom,
         Rectangle.xRight-Rectangle.xLeft, Rectangle.yTop-Rectangle.yBottom,
         SWP_SIZE | SWP_MOVE | ( OldPosition.fl & SWP_MAXIMIZE ) ) ;
   } /* endif */
   if ( WasVisible )
      Show ( ) ;
   #endif // __OS2__
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *                   Resize Window around anchor point                      *
 *                                                                          *
 ****************************************************************************/

void Window::Resize ( int cx, int cy ) {

   SWP Position;
   WinQueryWindowPos ( Handle, &Position ) ;

   BOOL fHadToHide = FALSE ;
   BOOL fHadToRestore = FALSE ;
   if ( Position.fl & SWP_MINIMIZE ) {
      if ( WinIsWindowVisible ( Handle ) ) {
         Hide ( ) ;
         fHadToHide = TRUE ;
      } /* endif */
      Restore ( ) ;
      WinQueryWindowPos ( Handle, &Position ) ;
      fHadToRestore = TRUE ;
   } /* endif */

   RECTL Rectangle = { Position.x, Position.y, Position.x+Position.cx, Position.y+Position.cy } ;
   WinCalcFrameRect ( Handle, &Rectangle, TRUE ) ;

   POINTL Anchor ;
   switch ( AnchorPoint ) {
      case ANCHOR_BL:
         Anchor.x = Rectangle.xLeft ;
         Anchor.y = Rectangle.yBottom ;
         break;
      case ANCHOR_BR:
         Anchor.x = Rectangle.xRight ;
         Anchor.y = Rectangle.yBottom ;
         break;
      case ANCHOR_TL:
         Anchor.x = Rectangle.xLeft ;
         Anchor.y = Rectangle.yTop ;
         break;
      case ANCHOR_TR:
         Anchor.x = Rectangle.xRight ;
         Anchor.y = Rectangle.yTop ;
         break;
   } /* endswitch */

   switch ( AnchorPoint ) {
      case ANCHOR_BL:
         Rectangle.xLeft = Anchor.x ;
         Rectangle.xRight = Anchor.x + cx ;
         Rectangle.yBottom = Anchor.y ;
         Rectangle.yTop = Anchor.y + cy ;
         break;
      case ANCHOR_BR:
         Rectangle.xLeft = Anchor.x - cx ;
         Rectangle.xRight = Anchor.x ;
         Rectangle.yBottom = Anchor.y ;
         Rectangle.yTop = Anchor.y + cy ;
         break;
      case ANCHOR_TL:
         Rectangle.xLeft = Anchor.x ;
         Rectangle.xRight = Anchor.x + cx ;
         Rectangle.yBottom = Anchor.y - cy ;
         Rectangle.yTop = Anchor.y ;
         break;
      case ANCHOR_TR:
         Rectangle.xLeft = Anchor.x - cx ;
         Rectangle.xRight = Anchor.x ;
         Rectangle.yBottom = Anchor.y - cy ;
         Rectangle.yTop = Anchor.y ;
         break;
   } /* endswitch */

   WinCalcFrameRect ( Handle, &Rectangle, FALSE ) ;

   WinSetWindowPos ( Handle, 0, Rectangle.xLeft, Rectangle.yBottom,
      Rectangle.xRight-Rectangle.xLeft, Rectangle.yTop-Rectangle.yBottom,
      SWP_MOVE | SWP_SIZE ) ;

   if ( fHadToRestore ) 
      WinSetWindowPos ( Handle, 0, Position.x, Position.y, Position.cx, Position.cy,
         SWP_MOVE | SWP_SIZE | SWP_MINIMIZE ) ;

   if ( fHadToHide )
      Show ( ) ;

   Invalidate ( ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *                             Drag Entire Window                           *
 *                                                                          *
 ****************************************************************************/

void Window::Drag ( ) {
   TRACKINFO TrackInfo ;
   memset ( &TrackInfo, 0, sizeof(TrackInfo) ) ;
   TrackInfo.cxBorder = 1 ;
   TrackInfo.cyBorder = 1 ;
   TrackInfo.cxGrid = 1 ;
   TrackInfo.cyGrid = 1 ;
   TrackInfo.cxKeyboard = 8 ;
   TrackInfo.cyKeyboard = 8 ;
   SWP Position ;
   WinQueryWindowPos ( Handle, &Position ) ;
   TrackInfo.rclTrack.xLeft   = Position.x ;
   TrackInfo.rclTrack.xRight  = Position.x + Position.cx ;
   TrackInfo.rclTrack.yBottom = Position.y ;
   TrackInfo.rclTrack.yTop    = Position.y + Position.cy ;
   WinQueryWindowPos ( HWND_DESKTOP, &Position ) ;
   TrackInfo.rclBoundary.xLeft   = Position.x ;
   TrackInfo.rclBoundary.xRight  = Position.x + Position.cx ;
   TrackInfo.rclBoundary.yBottom = Position.y ;
   TrackInfo.rclBoundary.yTop    = Position.y + Position.cy ;
   TrackInfo.ptlMinTrackSize.x = 0 ;
   TrackInfo.ptlMinTrackSize.y = 0 ;
   TrackInfo.ptlMaxTrackSize.x = Position.cx ;
   TrackInfo.ptlMaxTrackSize.y = Position.cy ;
   TrackInfo.fs = TF_MOVE | TF_STANDARD | TF_ALLINBOUNDARY ;
   if ( WinTrackRect ( HWND_DESKTOP, 0, &TrackInfo ) ) 
      WinSetWindowPos ( Handle, 0, TrackInfo.rclTrack.xLeft, TrackInfo.rclTrack.yBottom, 0, 0, SWP_MOVE ) ;
}

/***************************************************************************
 *                                                                         *
 *  Window: Set/Remove Presentation Parameters (OS/2)                      *
 *                                                                         *
 ***************************************************************************/

void Window::SetPresParam ( int ID, int Size, void *Data ) { 
   #ifdef __OS2__
      WinSetPresParam ( Handle, ID, Size, Data ) ; 
   #else
      ID = 0 ; Data = 0 ; Size = 0 ;  // Pacify the compiler.
   #endif
} /* endmethod */

void Window::RemovePresParam ( int ID ) { 
   #ifdef __OS2__
      WinRemovePresParam ( Handle, ID ) ; 
   #else
      ID = 0 ; // Pacify the compiler.
   #endif
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Send Message (immediate processing)                            *
 *                                                                         *
 ***************************************************************************/

MRESULT Window::SendMessage ( MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {
   return ( Sys_SendMessage ( Handle, msg, mp1, mp2 ) ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Post Message (deferred processing)                             *
 *                                                                         *
 ***************************************************************************/

int Window::PostMessage ( MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {
   return ( Sys_PostMessage ( Handle, msg, mp1, mp2 ) ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Window: Message Processor                                              *
 *                                                                         *
 ***************************************************************************/

MRESULT Window::MessageProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Window", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   switch ( msg ) {

      case WM_CLOSE: {
         Sys_SendMessage ( hwnd, WM_SAVEAPPLICATION, 0, 0 ) ;
         break ; }

      case WM_SYSCOLORCHANGE: {
         if ( !fBackColor )
            BackColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOW, 0 ) ;
         if ( !fTextColor )
            TextColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0 ) ;
         break ; }

      case WM_PRESPARAMCHANGED: {
         ULONG ppid ;
         switch ( LONGFROMMP(mp1) ) {
            case PP_FONTNAMESIZE: {
               char Name [80] ;
               if ( WinQueryPresParam ( hwnd, PP_FONTNAMESIZE, 0, &ppid, sizeof(Name), Name, 0 ) ) {
                  fFontNameSize = TRUE ;
                  strcpy ( FontNameSize, Name ) ;
               } else {
                  fFontNameSize = FALSE ;
               } /* endif */
               Sys_InvalidateWindow ( hwnd ) ;
               break; }
            case PP_BACKGROUNDCOLOR: {
               if ( WinQueryPresParam ( hwnd, PP_BACKGROUNDCOLOR, 0, &ppid, sizeof(BackColor), &BackColor, 0 ) ) {
                  fBackColor = TRUE ;
               } else {
                  BackColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOW, 0 ) ;
                  fBackColor = FALSE ;
               } /* endif */
               Sys_InvalidateWindow ( hwnd ) ;
               break; }
            case PP_FOREGROUNDCOLOR: {
               if ( WinQueryPresParam ( hwnd, PP_FOREGROUNDCOLOR, 0, &ppid, sizeof(TextColor), &TextColor, 0 ) ) {
                  fTextColor = TRUE ;
               } else {
                  TextColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0 ) ;
                  fTextColor = FALSE ;
               } /* endif */
               Sys_InvalidateWindow ( hwnd ) ;
               break; }
         } /* endswitch */
         break; }

   } /* endswitch */

   MRESULT Result = Sys_DefWindowProc ( hwnd, msg, mp1, mp2 ) ;

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Window", msg, Result ) ;
   } /* endif */

   return ( Result ) ;

} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Subclassed Window Message Processor                                    *
 *                                                                         *
 ***************************************************************************/

static MRESULT APIENTRY SubclassedMessageProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {
   SubclassedWindow *This = (class SubclassedWindow *) Sys_GetWindowData ( hwnd ) ;
   return ( This->MessageProcessor ( hwnd, msg, mp1, mp2 ) ) ;
} 

/***************************************************************************
 *                                                                         *
 *  Subclassed Window: Constructor                                         *
 *                                                                         *
 ***************************************************************************/

SubclassedWindow::SubclassedWindow ( HWND handle ) : 
   Handle(handle), DebugMessages(FALSE), MessageIndent(0) {
   Sys_SetWindowData ( Handle, this ) ;
   OldMessageProcessor = WinSubclassWindow ( Handle, SubclassedMessageProcessor ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Subclassed Window: Destructor                                          *
 *                                                                         *
 ***************************************************************************/

SubclassedWindow::~SubclassedWindow ( ) {
   WinSubclassWindow ( Handle, OldMessageProcessor ) ;
} /* endmethod */

/***************************************************************************
 *                                                                         *
 *  Subclassed Window: Message Processor                                   *
 *                                                                         *
 ***************************************************************************/

MRESULT SubclassedWindow::MessageProcessor ( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "SubclassedWindow", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   MRESULT Result = OldMessageProcessor ( hwnd, msg, mp1, mp2 ) ;

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "SubclassedWindow", msg, Result ) ;
   } /* endif */

   return ( Result ) ;

} /* endmethod */

