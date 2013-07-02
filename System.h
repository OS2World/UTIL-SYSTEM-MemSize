/******************************************************************* SYSTEM.H
 *                                                                          *
 *                System-Dependent Functions & Definitions                  *
 *                                                                          *
 ****************************************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H

   // Special preparations for when C-Set's memory debugging is active.

#ifdef __DEBUG_ALLOC__
   #pragma strings(readonly)
   #include <stdlib.h>
#endif // __DEBUG_ALLOC__

#ifndef __DOS__                                 /* If __DOS__ not defined, */
   #if !defined(__OS2__) && !defined(__NT__)    /*    and we aren't building for OS/2 nor NT, it must be for DOS. */
      #define __DOS__
   #endif
#endif

#if defined(__OS2__) || defined(__NT__)

        /* Get OS Definitions */

   #ifdef __OS2__
      #define INCL_BASE
      #define INCL_DOSDEVIOCTL
      #define INCL_DOSERRORS
      #define INCL_GPI
      #define INCL_NLS
      #define INCL_PM
      #define INCL_SPL
      #define INCL_SPLDOSPRINT
      #define INCL_SPLERRORS
      #define INCL_WINSTDDLGS
      #define INCL_WINWORKPLACE
      #include <os2.h>
   #else
      #ifndef INCLUDE_SYSTEM
         #define NOHELP
         #define NOKERNEL
         #define NOUSER
         #define NONLS
         #define NOSERVICE
         #define NOSYSPARAMSINFO
         #define WIN32_LEAN_AND_MEAN
         #include <windows.h>
         #include <winnls.h>
      #else
         #define WIN32_LEAN_AND_MEAN
         #include <windows.h>
      #endif
   #endif

        /* Redefine equivalent types */

   #ifdef __OS2__
      typedef MRESULT (EXPENTRY WINDOWPROC)(HWND, ULONG, MPARAM, MPARAM);       /* Message Processor */
      typedef WINDOWPROC *PWINDOWPROC ;         /* Pointer to message processor. */
      #define DRESULT      MRESULT              /* Dialog message processor result */
      #define DIALOGPROC   WINDOWPROC           /* Dialog Message Processor */
      #define CALLBACK     EXPENTRY             /* Exported Entry */
      #define MESG         ULONG                /* Message number */
      #define MPARAM1      MPARAM               /* First message parameter */
      #define MPARAM2      MPARAM               /* Second message parameter */
      #define INFINITE     SEM_INDEFINITE_WAIT  /* Infinite timeout (for Event, Mutex, MuxWait) */
      #define FILEHANDLE   HFILE                /* File Handle */
      #define CRITICAL_SECTION int              /* Critical Section object (null for OS/2) */
      #define ANCHOR       HAB                  /* Anchor Block Handle  */
      #define CURSOR_ARROW SPTR_ARROW           /* Standard mouse pointer */
   #else
      typedef LRESULT (CALLBACK WINDOWPROC)(HWND,UINT,WPARAM,LPARAM);           /* Message Processor */
      typedef WINDOWPROC *PWINDOWPROC ;         /* Pointer to above */
      typedef BOOL (CALLBACK DIALOGPROC)(HWND, UINT, WPARAM, LPARAM);           /* Dialog Message Processor */
      typedef DIALOGPROC *PDIALOGPROC ;         /* Pointer to above */
      #define MRESULT      LRESULT              /* Message processor result */
      #define DRESULT      BOOL                 /* Dialog message processor result */
      #define EXPENTRY                          /* Message processor linkage type */
      #define MESG         UINT                 /* Message number */
      #define MPARAM1      WPARAM               /* First message parameter */
      #define MPARAM2      LPARAM               /* Second message parameter */
      #define HEV          HANDLE               /* Event Handle */
      #define HMTX         HANDLE               /* Mutex Handle */
      #define TID          DWORD                /* Thread ID */
      #define HPAL         HPALETTE             /* Palette Handle */
      #define FILEHANDLE   HANDLE               /* File Handle */
      #define HMODULE      HANDLE               /* Module Handle */
      #define HPOINTER     HCURSOR              /* Pointer Handle */
      #define POINTL       POINT                /* Point(x,y) structure */
      #define ANCHOR       HANDLE               /* Anchor Block (Instance) Handle  */
      #define ERRORID      DWORD                /* Return type from GetLastError. */
      #define CURSOR_ARROW IDC_ARROW            /* Standard mouse pointer */
      #define HINI         HKEY                 /* Registry Handle */
   #endif

   #ifdef __OS2__
      #define KF_EXTENDED          0x0100
      #define KF_DLGMODE           0x0800
      #define KF_MENUMODE          0x1000
      #define KF_ALTDOWN           0x2000
      #define KF_REPEAT            0x4000
      #define KF_UP                0x8000
      #define VK_ESCAPE            VK_ESC
      #define VK_CONTROL           VK_CTRL
      #define MP1FROMP(x)          MPFROMP(x)
      #define MP2FROMP(x)          MPFROMP(x)
      #define MP1FROMSHORT(x)      MPFROMSHORT(x)
      #define MP2FROMSHORT(x)      MPFROMSHORT(x)
      #define MP1FROMLONG(x)       MPFROMLONG(x)
      #define MP2FROMLONG(x)       MPFROMLONG(x)
   #else
      #define VK_ALT               VK_MENU
      #define SHORT1FROMMP(x)      LOWORD(x)
      #define SHORT2FROMMP(x)      HIWORD(x)
      #define PVOIDFROMMP(x)       PVOID(x)
      #define PVOIDFROMMR(mr)      ((VOID *)(mr))
      #define SHORT1FROMMR(mr)     ((USHORT)((ULONG)mr))
      #define SHORT2FROMMR(mr)     ((USHORT)((ULONG)mr >> 16))
      #define LONGFROMMR(mr)       ((ULONG)(mr))
      #define MP1FROMP(x)          MPARAM1(x)
      #define MP2FROMP(x)          MPARAM2(x)
      #define MP1FROMSHORT(x)      MPARAM1(x)
      #define MP2FROMSHORT(x)      MPARAM2(x)
      #define MP1FROMLONG(x)       MPARAM1(x)
      #define MP2FROMLONG(x)       MPARAM2(x)
      #define MRFROMSHORT(x)       ((MRESULT)(x))
      #define MRFROMLONG(x)        ((MRESULT)(x))
      #define WM_BUTTON1DOWN       WM_LBUTTONDOWN
      #define WM_BUTTON2DOWN       WM_RBUTTONDOWN
      #define WM_BUTTON3DOWN       WM_MBUTTONDOWN
      #define WM_BUTTON1DBLCLK     WM_LBUTTONDBLCLK
      #define WM_BUTTON2DBLCLK     WM_RBUTTONDBLCLK
      #define WM_BUTTON3DBLCLK     WM_MBUTTONDBLCLK
      #define WM_BUTTON1UP         WM_LBUTTONUP
      #define WM_BUTTON2UP         WM_RBUTTONUP
      #define WM_BUTTON3UP         WM_MBUTTONUP
      #define WM_ERASEBACKGROUND   WM_ERASEBKGND
      #define WM_INITDLG           WM_INITDIALOG
      #define BM_QUERYCHECK        BM_GETCHECK
      #define BN_DBLCLICKED        BN_DOUBLECLICKED
   #endif

        /* Lookalike Functions */

   extern HWND          OWNER ( HWND Window ) ;
   extern HWND          PARENT ( HWND Window ) ;
   extern HWND          CHILD ( HWND Window, int Id ) ;

   extern void          Sys_Beep ( int Frequency, int Duration ) ;
   extern void          Sys_BeepWarning ( ) ;
   extern void          Sys_BeepNote    ( ) ;
   extern void          Sys_BeepError   ( ) ;
   extern void          Sys_BringWindowToTop ( HWND Window ) ;
   extern void          Sys_CloseFile  ( FILEHANDLE Handle ) ;
   extern DIALOGPROC    Sys_DefDialogProc ;
   extern WINDOWPROC    Sys_DefWindowProc ;
   extern void          Sys_DeleteCriticalSection ( CRITICAL_SECTION *control ) ;
   extern void          Sys_DeregisterThread ( void ) ;
   extern void          Sys_DestroyWindow ( HWND Window ) ;
   extern void          Sys_EnableWindow ( HWND Window, BOOL fEnable ) ;
   extern int           Sys_EndDialog ( HWND Dialog, BOOL Status ) ;
   extern void          Sys_EnterCriticalSection ( CRITICAL_SECTION *control ) ;
   extern void          Sys_ExitThread ( int Status ) ;
   extern void          Sys_GetFileTime ( int Handle, unsigned short *FileDate, unsigned short *FileTime ) ;
   extern HWND          Sys_GetFocus ( ) ;
   extern HWND          Sys_GetForegroundWindow ( void ) ;
   extern ERRORID       Sys_GetLastError ( ANCHOR Anchor ) ;
   extern HMODULE       Sys_GetModuleHandle ( char *LibraryName ) ;
   extern void *        Sys_GetWindowData ( HWND Window ) ;
   extern void          Sys_GetWindowText ( HWND Window, char *Text, int TextSize ) ;
   extern void          Sys_InitializeCriticalSection ( CRITICAL_SECTION *control ) ;
   extern void          Sys_InvalidateWindow ( HWND Window ) ;
   extern BOOL          Sys_IsMousePresent ( ) ;
   extern void          Sys_LeaveCriticalSection ( CRITICAL_SECTION *control ) ;
   extern HPOINTER      Sys_LoadCursor ( ANCHOR Anchor, long CursorType ) ;
   extern int           Sys_PostMessage ( HWND Window, MESG Message, MPARAM1 Parm1, MPARAM2 Parm2 ) ;
   extern void          Sys_PostQuitMessage ( HWND Window, int Status ) ;
   extern void          Sys_ProcessMessages ( long Anchor, HACCEL AcceleratorTable = 0 ) ;
   extern unsigned long Sys_QueryTickInterval ( void ) ;
   extern int           Sys_ReadFile ( FILEHANDLE Handle, void *Buffer, int Count, unsigned long *Read ) ;
   extern int           Sys_RegisterThread ( void ) ;
   extern void          Sys_ReleaseCapture ( void ) ;
   extern void          Sys_ResumeThread ( int ThreadID ) ;
   extern WINDOWPROC    Sys_SendMessage ;
   extern void          Sys_SetActiveWindow ( HWND Window ) ;
   extern int           Sys_SetCapture ( HWND Window ) ;
   extern void          Sys_SetCursor ( HPOINTER Cursor ) ;
   extern void          Sys_SetCursorPos ( int x, int y ) ;
   extern void          Sys_SetFileTime ( int Handle, unsigned short FileDate, unsigned short FileTime ) ;
   extern BOOL          Sys_SetFocus ( HWND Window ) ;
   extern void          Sys_SetWindowData ( HWND Window, void *Data ) ;
   extern void          Sys_SetWindowText ( HWND Window, char *Text ) ;
   extern void          Sys_ShowCursor ( BOOL Show ) ;
   extern int           Sys_ShowWindow ( HWND Window, BOOL Status ) ;
   extern void          Sys_Sleep ( int Milliseconds ) ;
   extern void          Sys_StartTimer ( HWND Window, int Id, int Milliseconds ) ;
   extern void          Sys_StopTimer ( HWND Window, int Id ) ;
   extern void          Sys_SuspendThread ( int ThreadID ) ;
   extern int           Sys_ThreadNumber ( register int ThreadID ) ;
   extern int           Sys_UpdateWindow ( HWND Window ) ;
   extern BOOL          Sys_MsgWaitForMultipleObjects ( int EventCount, HEV EventArray[], int Timeout ) ;
   extern HWND          Sys_WindowFromPoint ( int x, int y ) ;
   extern void          Sys_WriteFile ( FILEHANDLE Handle, void *Buffer, int Count, unsigned long *Written ) ;

   extern int Sys_RegisterClass (
      ANCHOR Anchor,               /* Instance or Anchor handle. */
      char *Name,                  /* Window class name. */
      PWINDOWPROC WindowProc,      /* -> Window Message Processor function. */
      int StyleFlags,              /* Class style flags. */
      int ExtraClassBytes,         /* Number of extra bytes to allocate for class data (WIN only). */
      int ExtraWindowBytes,        /* Number of extra bytes to allocate for window data. */
      char *IconName,              /* -> Icon Resource name (WIN only) */
      char *MenuName               /* -> Menu Resource name (WIN only) */
   ) ;

   extern int Sys_MessageBox (
      HWND Parent,                 /* -> Parent Window on which the message box will be. */
      HWND Owner,                  /* -> Owner Window to which the message box will report (OS/2 only). */
      char *Message,               /* -> Message Text */
      char *Title,                 /* -> Message Title */
      int idWindow,                /* Window ID (OS/2 only) */
      int StyleFlags               /* Window Style flags. */
   ) ;

#endif // not __DOS__


   // Define maximum number of threads supported.

#ifdef __DOS__
   #define MAX_THREADS 1
#else
   #define MAX_THREADS 100
#endif


   // WATCOM does not support min/max under C++ for some reason.

#if !defined(min) 
   #define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

#if !defined(max) 
   #define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

   // Some personal preferences.

#define NOT !
#define OR  ||
#define AND &&

#define DATEFMT_MM_DD_YY    (0x0000)
#define DATEFMT_DD_MM_YY    (0x0001)
#define DATEFMT_YY_MM_DD    (0x0002)


#endif  /* If not already included. */
