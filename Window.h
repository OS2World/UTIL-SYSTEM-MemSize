/****************************************************************** WINDOW.H
 *                                                                         *
 *  Class: Window & Window Class                                           *
 *                                                                         *
 ***************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include "System.h"

#include "Mutex.h"
#include "Proc.h"
#include "Rectangl.h"

#ifdef __NT__
   #define MIT_END     (-1)
   #define MIS_TEXT    MF_STRING
   #define MIS_BITMAP  MF_BITMAP
   #define DID_OK      1
   #define DID_CANCEL  2
#endif

enum PositionClass {
   MINIMIZE,
   RESTORE,
   MAXIMIZE
} ;

enum AnchorCorner {
   ANCHOR_BL,
   ANCHOR_TL,
   ANCHOR_BR,
   ANCHOR_TR
} ;

class WindowClass {

   private:
      char *Name ;
      int Registered ;

   public:
      WindowClass ( Process &Proc, int Style ) ;
      WindowClass ( char *name ) ;

      char *QueryName ( ) { return ( Name ) ; }
      int IsRegistered ( ) { return ( Registered ) ; }
} ;

class Window {

   private:

      HWND Handle ;
      int Created ;
      Mutex Lock ;
      BOOL ControlsHidden ;
      long FrameFlags ;
      int NumberOfHiddenControls ;
      HWND HiddenControls [10] ;

   protected:
      BOOL DebugMessages ;
      int MessageIndent ;
      ULONG BackColor ;  BOOL fBackColor ;
      ULONG TextColor ;  BOOL fTextColor ;
      char  FontNameSize [80] ; BOOL fFontNameSize ;
      USHORT AnchorPoint ; BOOL fAnchored ;

   public:

      Window ( WindowClass &WndClass, HWND Parent, char *Title, int Style,
         int x, int y, int cx, int cy, HWND Owner, HWND Zorder, 
         int Id=0, void *Parms=0 ) ;
      Window ( HWND handle ) ;

      virtual ~Window ( ) ;

      ANCHOR QueryAnchor ( ) ;
      HWND QueryHandle ( ) { return ( Handle ) ; }
      BOOL QueryControlsHidden () { return ( ControlsHidden ) ; }

      void Set_DebugMessages ( BOOL Flag ) { DebugMessages = Flag ; MessageIndent = 0 ; }
      void Set_BackColor ( ULONG color ) ;
      void Set_TextColor ( ULONG color ) ;
      void Set_FontNameSize ( char *fontnamesize ) ;
      void Set_AnchorPoint ( AnchorCorner anchorpoint ) { AnchorPoint = anchorpoint ; }

      void Invalidate ( RECTANGLE &Rectangle ) ;
      void Invalidate ( ) ;
      void Update ( ) ;

      void Reposition ( int x, int y, int cx, int cy, PositionClass State ) ;
      void Show ( ) ;
      void Hide ( ) ;
      void Move ( int x, int y ) ;
      void Size ( int cx, int cy ) ;
      void Minimize ( ) ;
      void Maximize ( ) ;
      void Restore ( ) ;
      void Activate ( ) ;
      void Deactivate ( ) ;
      void Surface ( ) ;
      void Sink ( ) ;

      void SetFocus ( ) ;

      void Enable ( int Flag = TRUE ) ;
      void Disable ( ) ;

      BOOL isVisible ( ) ;

      void SetData ( void *data ) ;
      void *GetData ( ) ;

      void SetText ( char *text ) ;
      void GetText ( char *text, int textsize ) ;

      HWND Child ( int ID ) { return ( CHILD(Handle,ID) ) ; }
      HWND Parent ( ) { return ( PARENT(Handle) ) ; }

      void StartTimer ( int ID, int Milliseconds ) ;
      void StopTimer ( int ID ) ;

      RECTANGLE Rectangle ( ) ;

      void SetWindowText ( char *title ) ;
      char *GetWindowText ( char String[], int StringSize ) ;

      void Add2TaskList ( ) ;

      BOOL AddSysMenuSeparator ( ) ;
      BOOL AddSysMenuItem ( char *Text, USHORT Id, SHORT Position=MIT_END, USHORT Style=MIS_TEXT,
         USHORT Attribute=0, HWND SubMenu=0, ULONG ItemHandle=0 ) ; 

      int MessageBox ( char *Title, char *Message, int Id, int Style ) ;
      int MessageBox ( char *Message, int Id, int Style ) ;

      void PopupMenu ( HWND Menu, int x, int y ) ;

      void HideControls ( BOOL fHide ) ;
      void Resize ( int cx, int cy ) ;
      void Drag ( ) ;

      void SetPresParam ( int ID, int Size, void *Data ) ;
      void RemovePresParam ( int ID ) ;

      MRESULT SendMessage ( MESG Message, MPARAM1 Parm1 = 0, MPARAM2 Parm2 = 0 ) ;
      int PostMessage ( MESG Message, MPARAM1 Parm1 = 0, MPARAM2 Parm2 = 0 ) ;

      virtual WINDOWPROC MessageProcessor ;
} ;

class SubclassedWindow {

   private:
      WINDOWPROC *OldMessageProcessor ;

   protected:
      HWND Handle ;
      BOOL DebugMessages ;
      int MessageIndent ;

   public:
      SubclassedWindow ( HWND handle ) ;
      virtual ~SubclassedWindow ( ) ;

      void Set_DebugMessages ( BOOL Flag ) { DebugMessages = Flag ; MessageIndent = 0 ; }

      virtual WINDOWPROC MessageProcessor ;
} ;

#endif 
