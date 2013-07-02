/***************************************************************** SYSTEM.CPP
 *                                                                          *
 *                       System-Dependent Functions                         *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#ifndef __IBMCPP__
   #include <dos.h>
#endif // Not IBM C++

#include <stddef.h>

#include "Debug.h"
#include "Mutex.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *                   Sound Playback from Resource (NT)                      *
 *                                                                          *
 ****************************************************************************/

#ifdef __NT__

#include "mmsystem.h"

extern HMODULE LibraryHandle ;
static LPSTR   SoundResourceNames   [3] = { "SOUND1", "SOUND2", "SOUND3" } ;
static HANDLE  SoundResourceHandles [3] = { 0, 0, 0 } ;
static LPSTR   SoundResources       [3] = { 0, 0, 0 } ;
static UINT    SoundDefaults        [3] = { MB_ICONASTERISK, MB_ICONASTERISK, MB_ICONASTERISK } ;

static BOOL SH__PlayResource ( int Sound ) {

   if ( SoundResourceHandles[Sound-1] == 0 ) {

      HANDLE hResInfo = FindResource ( LibraryHandle, SoundResourceNames[Sound-1], "WAVE" ) ;
      if ( hResInfo == NULL ) {
         Log ( "WARNING: Unable to find wave resource '%s'.", SoundResourceNames[Sound-1] ) ;
         return ( FALSE ) ;
      } /* endif */

      SoundResourceHandles[Sound-1] = LoadResource ( LibraryHandle, hResInfo ) ;
      if ( SoundResourceHandles[Sound-1] == NULL ) {
         Log ( "WARNING: Unable to load wave resource '%s'.", SoundResourceNames[Sound-1] ) ;
         return ( FALSE ) ;
      } /* endif */

      SoundResources[Sound-1] = (char*) LockResource ( SoundResourceHandles[Sound-1] ) ;
      if ( SoundResources[Sound-1] == NULL ) {
         Log ( "WARNING: Unable to lock wave resource '%s'.", SoundResourceNames[Sound-1] ) ;
         FreeResource ( SoundResourceHandles[Sound-1] ) ;
         SoundResourceHandles[Sound-1] = 0 ;
         return ( FALSE ) ;
      } /* endif */

   } /* endif */

   if ( ! sndPlaySound ( SoundResources[Sound-1], SND_MEMORY | SND_ASYNC ) ) {
      SoundResources[Sound-1] = 0 ;
      FreeResource ( SoundResourceHandles[Sound-1] ) ;
      SoundResourceHandles[Sound-1] = 0 ;
      MessageBeep ( SoundDefaults[Sound-1] ) ;
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;
}

#endif // __NT__

/****************************************************************************
 *                                                                          *
 *                 Lookalike Functions for OS/2 and Win32                   *
 *                                                                          *
 ****************************************************************************/

   // Wrapper functions.

#ifdef __OS2__
   extern HWND    OWNER ( HWND Window ) { return ( WinQueryWindow ( Window, QW_OWNER ) ) ; }
   extern HWND    PARENT ( HWND Window ) { return ( WinQueryWindow ( Window, QW_PARENT ) ) ; }
   extern HWND    CHILD ( HWND Window, int Id ) { return ( WinWindowFromID ( Window, Id ) ) ; }
   extern void    Sys_Beep ( int Frequency, int Duration ) { DosBeep ( max(0x25,Frequency), Duration ) ; }
   extern void    Sys_BeepWarning ( ) { WinAlarm ( HWND_DESKTOP, WA_NOTE    ) ; }
   extern void    Sys_BeepNote    ( ) { WinAlarm ( HWND_DESKTOP, WA_WARNING ) ; }
   extern void    Sys_BeepError   ( ) { WinAlarm ( HWND_DESKTOP, WA_ERROR   ) ; }
   extern void    Sys_BringWindowToTop ( HWND Window ) { WinSetWindowPos ( PARENT(Window), HWND_TOP, 0, 0, 0, 0, SWP_ZORDER ) ; }
   extern void    Sys_CloseFile  ( FILEHANDLE Handle ) { DosClose ( Handle ) ; }
   extern MRESULT APIENTRY Sys_DefDialogProc ( HWND Window, ULONG Message, MPARAM Parm1, MPARAM Parm2 ) { return ( WinDefDlgProc ( Window, Message, Parm1, Parm2 ) ) ; } 
   extern MRESULT APIENTRY Sys_DefWindowProc ( HWND Window, ULONG Message, MPARAM Parm1, MPARAM Parm2 ) { return ( WinDefWindowProc ( Window, Message, Parm1, Parm2 ) ) ; }
   extern void    Sys_DeleteCriticalSection ( CRITICAL_SECTION *control ) { control++ ; }
   extern void    Sys_DestroyWindow ( HWND Window ) { WinDestroyWindow ( Window ) ; }
   extern void    Sys_EnableWindow ( HWND Window, BOOL fEnable ) { WinEnableWindow ( Window, fEnable ) ; }
   extern int     Sys_EndDialog ( HWND Dialog, BOOL Status ) { return ( (int) WinDismissDlg ( Dialog, Status ) ) ; }
   extern void    Sys_EnterCriticalSection ( CRITICAL_SECTION *control ) { DosEnterCritSec ( ) ; control ++ ; }
   extern void    Sys_ExitThread ( int Status ) { DosExit ( EXIT_THREAD, Status ) ; }
   extern HWND    Sys_GetFocus ( void ) { return ( WinQueryFocus ( HWND_DESKTOP ) ) ; }
   extern HWND    Sys_GetForegroundWindow ( void ) { return ( WinQueryFocus ( HWND_DESKTOP ) ) ; }
   extern ERRORID Sys_GetLastError ( ANCHOR Anchor ) { return ( WinGetLastError ( Anchor ) ) ; }
   extern HMODULE Sys_GetModuleHandle ( char *LibraryName ) { HMODULE Handle ; DosQueryModuleHandle ( (PSZ)LibraryName, &Handle ) ; return ( Handle ) ; }
   extern void   *Sys_GetWindowData ( HWND Window ) { return ( (void*)WinQueryWindowPtr ( Window, QWL_USER ) ) ; }
   extern void    Sys_GetWindowText ( HWND Window, char *Text, int TextSize ) { WinQueryWindowText ( Window, TextSize, (PSZ)Text ) ; }
   extern void    Sys_InitializeCriticalSection ( CRITICAL_SECTION *control ) { control++ ; }
   extern void    Sys_InvalidateWindow ( HWND Window ) { WinInvalidateRect ( Window, 0, TRUE ) ; }
   extern BOOL    Sys_IsMousePresent ( ) { return ( WinQuerySysValue ( HWND_DESKTOP, SV_CMOUSEBUTTONS ) > 0 ) ; }
   extern void    Sys_LeaveCriticalSection ( CRITICAL_SECTION *control ) { DosExitCritSec ( ) ; control ++ ; }
   extern HPOINTER Sys_LoadCursor ( ANCHOR Anchor, long CursorType ) { Anchor ++ ; return ( WinQuerySysPointer ( HWND_DESKTOP, CursorType, FALSE ) ) ; }
   extern int     Sys_PostMessage ( HWND Window, MESG Message, MPARAM1 Parm1, MPARAM2 Parm2 ) { return ( WinPostMsg ( Window, Message, Parm1, Parm2 ) ) ; }
   extern void    Sys_PostQuitMessage ( HWND Window, int Status ) { WinPostMsg ( Window, WM_QUIT, 0, 0 ) ; Status ++ ; }
   extern int     Sys_ReadFile ( FILEHANDLE Handle, void *Buffer, int Count, unsigned long *Read ) { return ( DosRead ( Handle, Buffer, Count, Read ) == 0 ) ; }
   extern void    Sys_ReleaseCapture ( void ) { WinSetCapture ( HWND_DESKTOP, 0 ) ; }
   extern void    Sys_ResumeThread ( int ThreadID ) { DosResumeThread ( ThreadID ) ; }
   extern MRESULT APIENTRY Sys_SendMessage ( HWND Window, ULONG Message, MPARAM Parm1, MPARAM Parm2 ) { return ( WinSendMsg ( Window, Message, Parm1, Parm2 ) ) ; }
   extern void    Sys_SetActiveWindow ( HWND Window ) { WinSetActiveWindow ( HWND_DESKTOP, Window ) ; }
   extern int     Sys_SetCapture ( HWND Window ) { return ( WinSetCapture ( HWND_DESKTOP, Window ) ) ; }
   extern void    Sys_SetCursor ( HPOINTER Cursor ) { WinSetPointer ( HWND_DESKTOP, Cursor ) ; }
   extern void    Sys_SetCursorPos ( int x, int y ) { WinSetPointerPos ( HWND_DESKTOP, x, y ) ; }
   extern BOOL    Sys_SetFocus ( HWND Window ) { return ( WinSetFocus ( HWND_DESKTOP, Window ) ) ; }
   extern void    Sys_SetWindowData ( HWND Window, void *Data ) { WinSetWindowPtr ( Window, QWL_USER, Data ) ; }
   extern void    Sys_SetWindowText ( HWND Window, char *Text ) { WinSetWindowText ( Window, (PSZ)Text ) ; }
   extern void    Sys_ShowCursor ( BOOL Show ) { WinShowPointer ( HWND_DESKTOP, Show ) ; }
   extern int     Sys_ShowWindow ( HWND Window, BOOL Status ) { return ( (int) WinShowWindow ( Window, Status ) ) ; }
   extern void    Sys_Sleep ( int Milliseconds ) { DosSleep ( Milliseconds ) ; }
   extern void    Sys_StartTimer ( HWND Window, int Id, int Milliseconds ) { WinStartTimer ( 0, Window, Id, Milliseconds ) ; }
   extern void    Sys_StopTimer ( HWND Window, int Id ) { WinStopTimer ( 0, Window, Id ) ; }
   extern void    Sys_SuspendThread ( int ThreadID ) { DosSuspendThread ( ThreadID ) ; }
   extern int     Sys_UpdateWindow ( HWND Window ) { return ( (int) WinUpdateWindow ( Window ) ) ; }
   extern void    Sys_WriteFile ( FILEHANDLE Handle, void *Buffer, int Count, unsigned long *Written ) { DosWrite ( Handle, Buffer, Count, Written ) ; }
#else
   extern HWND    OWNER ( HWND Window ) { return ( GetWindow ( Window, GW_OWNER ) ) ; }
   extern HWND    PARENT ( HWND Window ) { return ( GetParent ( Window ) ) ; }
   extern HWND    CHILD ( HWND Window, int Id ) {
      HWND Child = GetWindow ( Window, GW_CHILD ) ;
      while ( Child ) {
         if ( Id == GetWindowLong ( Child, GWL_ID ) ) 
            break ;
         Child = GetWindow ( Child, GW_HWNDNEXT ) ;
      } /* endwhile */
      return ( Child ) ;
   } 
   extern void    Sys_Beep ( int Frequency, int Duration ) { Beep ( max(0x25,Frequency), Duration ) ; }
   extern void    Sys_BeepWarning ( ) { SH__PlayResource ( 2 ) ; }
   extern void    Sys_BeepNote    ( ) { SH__PlayResource ( 1 ) ; }
   extern void    Sys_BeepError   ( ) { SH__PlayResource ( 3 ) ; }
   extern void    Sys_BringWindowToTop ( HWND Window ) { BringWindowToTop ( Window ) ; }
   extern void    Sys_CloseFile  ( FILEHANDLE Handle ) { CloseHandle ( Handle ) ; }
   extern DRESULT CALLBACK Sys_DefDialogProc ( HWND Window, MESG Message, MPARAM1 Parm1, MPARAM2 Parm2 ) { return ( DefDlgProc ( Window, Message, Parm1, Parm2 ) ) ; } 
   extern LRESULT CALLBACK Sys_DefWindowProc ( HWND Window, MESG Message, MPARAM1 Parm1, MPARAM2 Parm2 ) { return ( DefWindowProc ( Window, Message, Parm1, Parm2 ) ) ; }
   extern void    Sys_DeleteCriticalSection ( CRITICAL_SECTION *control ) { DeleteCriticalSection ( control ) ; }
   extern void    Sys_DestroyWindow ( HWND Window ) { DestroyWindow ( Window ) ; }
   extern void    Sys_EnableWindow ( HWND Window, BOOL fEnable ) { EnableWindow ( Window, fEnable ) ; }
   extern int     Sys_EndDialog ( HWND Dialog, BOOL Status ) { return ( EndDialog ( Dialog, Status ) ) ; }
   extern void    Sys_EnterCriticalSection ( CRITICAL_SECTION *control ) { EnterCriticalSection ( control ) ; }
   extern void    Sys_ExitThread ( int Status ) { ExitThread ( Status ) ; }
   extern HWND    Sys_GetFocus ( void ) { return ( GetFocus ( ) ) ; }
   extern HWND    Sys_GetForegroundWindow ( void ) { return ( GetForegroundWindow ( ) ) ; }
   extern ERRORID Sys_GetLastError ( ANCHOR Anchor ) { Anchor = 0 ; return ( GetLastError ( ) ) ; }
   extern HMODULE Sys_GetModuleHandle ( char *LibraryName ) { return ( GetModuleHandle ( LibraryName ) ) ; }
   extern void   *Sys_GetWindowData ( HWND Window ) { return ( (void*)GetWindowLong ( Window, 0 ) ) ; }
   extern void    Sys_GetWindowText ( HWND Window, char *Text, int TextSize ) { GetWindowText ( Window, Text, TextSize ) ; }
   extern void    Sys_InitializeCriticalSection ( CRITICAL_SECTION *control ) { InitializeCriticalSection ( control ) ; }
   extern void    Sys_InvalidateWindow ( HWND Window ) { InvalidateRect ( Window, 0, 0 ) ; }
   extern BOOL    Sys_IsMousePresent ( ) { return ( GetSystemMetrics ( SM_MOUSEPRESENT ) ) ; }
   extern void    Sys_LeaveCriticalSection ( CRITICAL_SECTION *control ) { LeaveCriticalSection ( control ) ; }
   extern HPOINTER Sys_LoadCursor ( ANCHOR Anchor, long CursorType ) { return ( LoadCursor ( Anchor, (char*)CursorType ) ) ; }
   extern int     Sys_PostMessage ( HWND Window, MESG Message, MPARAM1 Parm1, MPARAM2 Parm2 ) { return ( PostMessage ( Window, Message, Parm1, Parm2 ) ) ; }
   extern void    Sys_PostQuitMessage ( HWND Window, int Status ) { PostMessage ( Window, WM_QUIT, MPARAM1(Status), 0 ) ; }
   extern int     Sys_ReadFile ( FILEHANDLE Handle, void *Buffer, int Count, unsigned long *Read ) { return ( ReadFile ( Handle, Buffer, Count, Read, 0 ) ) ; }
   extern void    Sys_ReleaseCapture ( void ) { ReleaseCapture ( ) ; }
   extern void    Sys_ResumeThread ( int ThreadID ) { ResumeThread ( (HANDLE)ThreadID ) ; }
   extern LRESULT CALLBACK Sys_SendMessage ( HWND Window, MESG Message, MPARAM1 Parm1, MPARAM2 Parm2 ) { return ( SendMessage ( Window, Message, Parm1, Parm2 ) ) ; }
   extern void    Sys_SetActiveWindow ( HWND Window ) { SetForegroundWindow ( Window ) ; }
   extern int     Sys_SetCapture ( HWND Window ) { return ( SetCapture ( Window ) != 0 ) ; }
   extern void    Sys_SetCursor ( HPOINTER Cursor ) { SetCursor ( Cursor ) ; }
   extern void    Sys_SetCursorPos ( int x, int y ) { SetCursorPos ( x, y ) ; }
   extern BOOL    Sys_SetFocus ( HWND Window ) { return ( SetFocus ( Window ) != 0 ) ; }
   extern void    Sys_SetWindowData ( HWND Window, void *Data ) { SetWindowLong ( Window, 0, (long)Data ) ; }
   extern void    Sys_SetWindowText ( HWND Window, char *Text ) { SetWindowText ( Window, Text ) ; }
   extern void    Sys_ShowCursor ( BOOL Show ) { ShowCursor ( Show ) ; }
   extern int     Sys_ShowWindow ( HWND Window, BOOL Status ) { return ( ShowWindow ( Window, Status ? SW_SHOW : SW_HIDE ) ) ; }
   extern void    Sys_Sleep ( int Milliseconds ) { Sleep ( Milliseconds ) ; }
   extern void    Sys_StartTimer ( HWND Window, int Id, int Milliseconds ) { SetTimer ( Window, Id, Milliseconds, 0 ) ; }
   extern void    Sys_StopTimer ( HWND Window, int Id ) { KillTimer ( Window, Id ) ; }
   extern void    Sys_SuspendThread ( int ThreadID ) { SuspendThread ( (HANDLE)ThreadID ) ; }
   extern int     Sys_UpdateWindow ( HWND Window ) { return ( UpdateWindow ( Window ) ) ; }
   extern void    Sys_WriteFile ( FILEHANDLE Handle, void *Buffer, int Count, unsigned long *Written ) { WriteFile ( Handle, Buffer, Count, Written, 0 ) ; }
#endif


/****************************************************************************
 *                                                                          *
 *  Determine System Tick-Timer interval.                                   *
 *                                                                          *
 ****************************************************************************/

extern unsigned long Sys_QueryTickInterval ( void ) {

   #define TICKTIMES_PER_SEC (18.206)

   unsigned long TickTime = ( unsigned long ) ( 10000000 / ( TICKTIMES_PER_SEC * 10000 ) ) ;

   return ( TickTime ) ;
}

/****************************************************************************
 *                                                                          *
 *  Determine Window from screen coordinates.                               *
 *                                                                          *
 ****************************************************************************/

extern HWND Sys_WindowFromPoint ( int x, int y ) {

   POINTL Point ;
   Point.x = x ;
   Point.y = y ;

   #ifdef __OS2__
      return ( WinWindowFromPoint ( HWND_DESKTOP, &Point, TRUE ) ) ;
   #else
      return ( WindowFromPoint ( Point ) ) ;
   #endif
}

/****************************************************************************
 *                                                                          *
 *  Register Window Class                                                   *
 *                                                                          *
 ****************************************************************************/

extern int Sys_RegisterClass (
   ANCHOR Anchor,               // Instance or Anchor handle.
   char *Name,                  // Window class name.
   PWINDOWPROC WindowProc,      // -> Window Message Processor function.
   int StyleFlags,              // Class style flags.
   int ExtraClassBytes,         // Number of extra bytes to allocate for class data (WIN only).
   int ExtraWindowBytes,        // Number of extra bytes to allocate for window data.
   char *IconName,              // -> Icon Resource name (WIN only)
   char *MenuName               // -> Menu Resource name (WIN only)
) {

   #ifdef __OS2__
      ExtraClassBytes = 0 ;   // Pacify the compiler.
      IconName = 0 ;
      MenuName = 0 ;
      return ( WinRegisterClass ( Anchor, (PSZ)Name, WindowProc, StyleFlags, ExtraWindowBytes ) ) ;

   #else
      // Local Declarations
      WNDCLASS WindowClass;

      // Set up and register the window class.
      WindowClass.style         = StyleFlags ;
      WindowClass.lpfnWndProc   = (PWINDOWPROC) WindowProc ;
      WindowClass.cbClsExtra    = ExtraClassBytes ;
      WindowClass.cbWndExtra    = ExtraWindowBytes ;
      WindowClass.hInstance     = Anchor ;
      WindowClass.hIcon         = LoadIcon ( (HINSTANCE)Anchor, (PSZ)IconName ) ;
      WindowClass.hCursor       = LoadCursor ( (HINSTANCE)Anchor, CURSOR_ARROW ) ;
      WindowClass.hbrBackground = GetStockObject ( WHITE_BRUSH ) ;
      WindowClass.lpszMenuName  = (PSZ)MenuName ;
      WindowClass.lpszClassName = (PSZ)Name ;

      return ( RegisterClass ( &WindowClass ) ) ;

   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Message Box                                                             *
 *                                                                          *
 ****************************************************************************/

extern int Sys_MessageBox (
   HWND Parent,                 // -> Parent Window on which the message box will be.
   HWND Owner,                  // -> Owner Window to which the message box will report (OS/2 only).
   char *Message,               // -> Message Text
   char *Title,                 // -> Message Title
   int idWindow,                // Window ID (OS/2 only)
   int StyleFlags               // Window Style flags.
) {
   #ifdef __OS2__
      return ( (int) WinMessageBox ( Parent, Owner, (PSZ)Message, (PSZ)Title, idWindow, StyleFlags ) ) ;
   #else
      Owner = 0 ; idWindow = 0 ;  // Pacify the compiler.
      return ( MessageBox ( Parent, Message, Title, StyleFlags ) ) ;
   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Process Messages in Window Queue                                        *
 *                                                                          *
 ****************************************************************************/

extern void Sys_ProcessMessages ( long Anchor, HACCEL AcceleratorTable ) {

   #ifdef __OS2__

      QMSG Message ;
      #ifdef DEBUG
         Log ( "ProcessMessages(Anchor=%08p) Entering GetMsg loop.", Anchor ) ;
      #endif
      while ( WinGetMsg ( Anchor, &Message, 0, 0, 0 ) ) {
         #ifdef DEBUG
            char Name [80] ;
            Log ( "ProcessMessages(Anchor=%08p) About to dispatch message.  Window %08p, Message %s(%08X,%08X).",
               Anchor, Message.hwnd, InterpretMsg(Message.msg,Name), Message.mp1, Message.mp2 ) ;
            MRESULT Result = WinDispatchMsg ( Anchor, &Message ) ;
         #else
            WinDispatchMsg ( Anchor, &Message ) ;
         #endif
         #ifdef DEBUG
            Log ( "ProcessMessages(Anchor=%08p) Message dispatched.  Result %08X.  Waiting for another.",
               Anchor, Result ) ;
         #endif
      } /* endwhile */
      #ifdef DEBUG
         Log ( "ProcessMessages(Anchor=%08p) Leaving GetMsg loop.", Anchor ) ;
      #endif

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

      Anchor = 0 ;  // Pacify the compiler.

   #endif
}

/***************************************************************************
 *                                                                         *
 *  File Time/Date Query & Set                                             *
 *                                                                         *
 ***************************************************************************/

extern void Sys_GetFileTime ( int Handle, unsigned short *FileDate, unsigned short *FileTime ) {

#ifdef __IBMCPP__

   // Under C-Set . . .
   FILESTATUS3 FileStatus ;
   DosQueryFileInfo ( Handle, FIL_STANDARD, &FileStatus, sizeof(FileStatus) ) ;
   *FileDate = *((unsigned short*)&FileStatus.fdateLastWrite) ;
   *FileTime = *((unsigned short*)&FileStatus.ftimeLastWrite) ;

#else

   // Under WATCOM . . .
   _dos_getftime ( Handle, FileDate, FileTime ) ;

#endif

}
 
extern void Sys_SetFileTime ( int Handle, unsigned short FileDate, unsigned short FileTime ) {

#ifdef __IBMCPP__

   // Under C-Set . . .
   FILESTATUS3 FileStatus ;
   DosQueryFileInfo ( Handle, FIL_STANDARD, &FileStatus, sizeof(FileStatus) ) ;
   FileStatus.fdateLastWrite = *((FDATE*)&FileDate) ;
   FileStatus.ftimeLastWrite = *((FTIME*)&FileTime) ;
   DosSetFileInfo ( Handle, FIL_STANDARD, &FileStatus, sizeof(FileStatus) ) ;

#else

   // Under WATCOM . . .
   _dos_getftime ( Handle, &FileDate, &FileTime ) ;

#endif

}


