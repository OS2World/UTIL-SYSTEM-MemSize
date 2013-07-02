/**************************************************************** MEMSIZE.CPP
 *                                                                          *
 * System Resources Monitor                                                 *
 *                                                                          *
 * (C) Copyright 1991-2000 by Richard W. Papo.                              *
 *                                                                          *
 * This is 'FreeWare'.  As such, it may be copied and distributed           *
 * freely.  If you want to use part of it in your own program, please       *
 * give credit where credit is due.  If you want to change the              *
 * program, please refer the change request to me or send me the            *
 * modified source code.  I can be reached on the Internet at               *
 * rpapo@mediaone.net, and through my web home page:                        *
 *                                                                          *
 *                  http://people.mw.mediaone.net/rpapo                     *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#ifdef __OS2__
   #define INCL_MACHDR
   #define INCL_MCIOS2
   #include <os2me.h>
#endif 

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Canvas.h"
#include "ChkDrive.h"
#include "Dde.h"
#include "Debug.h"
#include "Dialog.h"
#include "DQPS.h"
#include "Event.h"
#include "Except.h"
#include "HelpWin.h"
#include "Module.h"
#include "Proc.h"
#include "Profile2.h"
#include "ReString.h"
#include "RunHome.h"
#include "RunOnce.h"
#include "Support.h"
#include "SVDisk.h"
#include "Thread.h"
#include "Window.h"

#include "Config.h"

#include "Items.h"

#include "MemSize.h"

#define STATIC          // When left blank, all functions appear in link-map.
// #define DEBUG           // If defined, enables certain debugging messages to MEMSIZE.LOG.
// #define DEBUG_POSITION  // If defined, enables window position debugging.


/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

  // Macros

#define DIM(x) (sizeof(x)/sizeof(x[0]))


  // Data Types

typedef struct {
   BOOL Active ;
   PULONG Counter ;
   PUSHORT Interval ;
   Priority *Priority ;
   HWND Owner ;
} MONITOR_PARMS, *PMONITOR_PARMS ;

typedef struct {
   BOOL Active ;
   ULONG Counter ;
} COUNTER_PARMS, *PCOUNTER_PARMS ;


  // Function Prototypes

extern int main ( int argc, char *argv[] ) ;

extern PSZ ScanSystemConfig ( ANCHOR Anchor, PSZ Keyword ) ;
STATIC char *CopyString ( char *Buffer, char *Original ) ;

STATIC void MonitorLoopThread ( void *Parameter ) ;

STATIC BOOL UpdateDriveList ( HMODULE Library, Profile2 &Prf, PINIDATA IniData,
   Dde_Server *pDdeServer, ULONG OldDrives, ULONG NewDrives, ULONG &ResultDrives, ULONG &Exclude ) ;

STATIC ULONG CalibrateLoadMeter ( PCOUNTER_PARMS Parms ) ;

STATIC void CounterThread ( void *Parameter ) ;


  // Global Data

static RunHome _RunHome ( ) ;                           // Force the program to run home.
static RunOnce _RunOnce ( PROGRAM_NAME, TRUE ) ;        // Prevent multiple instances of the program.
static Process Proc ( PROGRAM_NAME ) ;                  // Establish the anchor block and queue.
static Module Library ( PROGRAM_NAME ) ;                // Establish the basic resource library.

static Event CounterThreadEvent = Event ( ) ;
static Event MonitorThreadEvent = Event ( ) ;

extern char HomePath [_MAX_PATH] = { 0 } ;
extern HMODULE LibraryHandle = 0 ;


/****************************************************************************
 *                                                                          *
 *                      Get Expanded INI Information                        *
 *                                                                          *
 ****************************************************************************/

STATIC int GetIniData ( ANCHOR Anchor, HMODULE Library, Profile2 &Prf, PINIDATA IniData, Dde_Server *pDdeServer, ULONG &Drives, ULONG &Exclude ) {

  /**************************************************************************
   * Get the basic INI information.                                         *
   **************************************************************************/

   if ( Prf.GetIniData ( IniData ) )
      return ( 1 ) ;

  /**************************************************************************
   * Initialize the global resource strings.                                *
   **************************************************************************/

   IniData->Day             = new ResourceString ( Library, IDS_DAY ) ;
   IniData->Days            = new ResourceString ( Library, IDS_DAYS ) ;
   IniData->DaysOfWeek      = new ResourceString ( Library, IDS_DAYSOFWEEK ) ;
   IniData->DriveError      = new ResourceString ( Library, IDS_DRIVEERROR ) ;
   IniData->BatteryError    = new ResourceString ( Library, IDS_APM_ERROR ) ;
   IniData->BatteryAC       = new ResourceString ( Library, IDS_APM_AC ) ;
   IniData->BatteryCharging = new ResourceString ( Library, IDS_APM_CHARGING ) ;

  /**************************************************************************
   * Get the SWAPPATH statement from CONFIG.SYS.                            *
   **************************************************************************/

   PSZ Swappath = ScanSystemConfig ( Anchor, PSZ("SWAPPATH") ) ;

   if ( Swappath == NULL )
      Swappath = PSZ("C:\\OS2\\SYSTEM 0 0") ;

   char *p = CopyString ( PCHAR(IniData->SwapPath), PCHAR(Swappath) ) ;
   IniData->MinFree = 0 ;
   IniData->InitialSwap = 0 ;
   sscanf ( p, " %li %li", &IniData->MinFree, &IniData->InitialSwap ) ;

  /**************************************************************************
   * Find out where the spool work directory is.                            *
   **************************************************************************/

   IniData->SpoolPath = 0 ;

   #ifdef __OS2__

   ULONG Size ;
   if ( PrfQueryProfileSize ( HINI_PROFILE, PSZ("PM_SPOOLER"), PSZ("DIR"), &Size ) ) {
      IniData->SpoolPath = PSZ ( malloc ( (int)Size ) ) ;
      if ( IniData->SpoolPath ) {
         if ( PrfQueryProfileData ( HINI_PROFILE, PSZ("PM_SPOOLER"), PSZ("DIR"), IniData->SpoolPath, &Size ) ) {
            PBYTE p = PBYTE( strchr ( PCHAR(IniData->SpoolPath), ';' ) ) ;
            if ( p ) {
               *p = 0 ;
            }
         } else {
            free ( IniData->SpoolPath ) ;
            IniData->SpoolPath = 0 ;
         } /* endif */
      } /* endif */
   } /* endif */

   #endif // __OS2__

   if ( IniData->SpoolPath == 0 )
      IniData->SpoolPath = PSZ ( "C:\\SPOOL" ) ;

  /**************************************************************************
   * Build the fixed portion of the item list.                              *
   **************************************************************************/

   ResourceString ClockLabel ( Library, IDS_CLOCK ) ;
   IniData->Items[ITEM_CLOCK] = new Clock ( ITEM_CLOCK,
      "ShowClock", ClockLabel, ClockLabel, pDdeServer, "Items",
      IniData->DateFormat, IniData->CenterClock ) ;

   ResourceString ElapsedLabel ( Library, IDS_ELAPSED ) ;
   IniData->Items[ITEM_ELAPSEDTIME] = new ElapsedTime ( ITEM_ELAPSEDTIME,
      "ShowElapsed", ElapsedLabel, ElapsedLabel, pDdeServer, "Items",
      IniData->Day, IniData->Days, IniData->ShowSeconds, IniData->SwapPath ) ;

   ResourceString MesgCountLabel ( Library, IDS_MESSAGECOUNT ) ;
   IniData->Items[ITEM_MESSAGECOUNT] = new MessageCount ( ITEM_MESSAGECOUNT,
      "ShowMesgCount", MesgCountLabel, MesgCountLabel, pDdeServer, "Items", 
      IniData->PopServer, IniData->PopUserName, IniData->PopPassword, 
      IniData->PopPort, IniData->PopForceIP, IniData->PopUseAPOP, 
      IniData->PopInterval, IniData->PopWavFile, IniData->PopAlert ) ;

   ResourceString MemoryLabel ( Library, IDS_MEMORY ) ;
   IniData->Items[ITEM_MEMORYFREE] = new MemoryFree ( ITEM_MEMORYFREE,
      "ShowMemory", MemoryLabel, MemoryLabel, pDdeServer, "Items",
      IniData->UseTheseus, IniData->ShowK, IniData->ShowTrueK, IniData->ShowM, IniData->ShowG ) ;

   ResourceString VirtualLabel ( Library, IDS_VIRTUAL ) ;
   IniData->Items[ITEM_VIRTUALFREE] = new VMFree ( ITEM_VIRTUALFREE,
      "ShowVirtual", VirtualLabel, VirtualLabel, pDdeServer, "Items",
      IniData->ShowK, IniData->ShowTrueK, IniData->ShowM, IniData->ShowG ) ;

   ResourceString SwapSizeLabel ( Library, IDS_SWAPSIZE ) ;
   IniData->Items[ITEM_SWAPFILESIZE] = new SwapSize ( ITEM_SWAPFILESIZE,
      "ShowSwapsize", SwapSizeLabel, SwapSizeLabel, pDdeServer, "Items",
      IniData->ShowK, IniData->ShowTrueK, IniData->ShowM, IniData->ShowG, IniData->SwapPath, IniData->InitialSwap ) ;

   ResourceString SwapFreeLabel ( Library, IDS_SWAPFREE ) ;
   IniData->Items[ITEM_SWAPDISKFREE] = new SwapFree ( ITEM_SWAPDISKFREE,
      "ShowSwapfree", SwapFreeLabel, SwapFreeLabel, pDdeServer, "Items",
      IniData->ShowK, IniData->ShowTrueK, IniData->ShowM, IniData->ShowG, IniData->SwapPath, IniData->MinFree ) ;

   ResourceString SwapSlackLabel ( Library, IDS_SWAPSLACK ) ;
   IniData->Items[ITEM_SWAPFILESLACK] = new SwapSlack ( ITEM_SWAPFILESLACK,
      "ShowSwapSlack", SwapSlackLabel, SwapSlackLabel, pDdeServer, "Items",
      IniData->UseTheseus, IniData->ShowK, IniData->ShowTrueK, IniData->ShowM, IniData->ShowG, 
      (VMFree*)IniData->Items[ITEM_VIRTUALFREE],
      (SwapFree*)IniData->Items[ITEM_SWAPDISKFREE],
      (MemoryFree*)IniData->Items[ITEM_MEMORYFREE] ) ;

   ResourceString SpoolSizeLabel ( Library, IDS_SPOOLSIZE ) ;
   IniData->Items[ITEM_SPOOLFILESIZE] = new SpoolSize ( ITEM_SPOOLFILESIZE,
      "ShowSpoolSize", SpoolSizeLabel, SpoolSizeLabel, pDdeServer, "Items",
      IniData->ShowK, IniData->ShowTrueK, IniData->ShowM, IniData->ShowG, IniData->SpoolPath ) ;

   ResourceString AverageLoadLabel ( Library, IDS_AVERAGELOAD ) ;
   IniData->Items[ITEM_AVERAGELOAD] = new AverageLoad ( ITEM_AVERAGELOAD,
      "ShowAverageLoad", AverageLoadLabel, AverageLoadLabel, pDdeServer, "Items", 
      IniData->AveragingInterval ) ;

   ResourceString CpuLoadLabel ( Library, IDS_CPULOAD ) ;
   IniData->Items[ITEM_CPULOAD] = new CpuLoad ( ITEM_CPULOAD,
      "ShowCpuLoad", CpuLoadLabel, CpuLoadLabel, pDdeServer, "Items",
      IniData->MaxCount, &IniData->IdleCount, IniData->BatteryError, (AverageLoad*)IniData->Items[ITEM_AVERAGELOAD] ) ;

   ((AverageLoad*)IniData->Items[ITEM_AVERAGELOAD])->SetCpuLoadObject ( (CpuLoad*)IniData->Items[ITEM_CPULOAD] ) ;

   ResourceString BatteryLabel ( Library, IDS_BATTERY ) ;
   IniData->Items[ITEM_BATTERY] = new Battery ( ITEM_BATTERY,
      "ShowBattery", BatteryLabel, BatteryLabel, pDdeServer, "Items",
      IniData->BatteryError, IniData->BatteryCharging, IniData->BatteryAC ) ;

   ResourceString TaskCountLabel ( Library, IDS_TASKCOUNT ) ;
   IniData->Items[ITEM_TASKCOUNT] = new TaskCount ( ITEM_TASKCOUNT,
      "ShowTaskCount", TaskCountLabel, TaskCountLabel, pDdeServer, "Items",
      Anchor ) ;

   ResourceString ProcessCountLabel ( Library, IDS_PROCESSCOUNT ) ;
   IniData->Items[ITEM_PROCESSCOUNT] = new ProcessCount ( ITEM_PROCESSCOUNT,
      "ShowProcessCount", ProcessCountLabel, ProcessCountLabel, pDdeServer, "Items" ) ;

   ResourceString ThreadCountLabel ( Library, IDS_THREADCOUNT ) ;
   IniData->Items[ITEM_THREADCOUNT] = new ThreadCount ( ITEM_THREADCOUNT,
      "ShowThreadCount", ThreadCountLabel, ThreadCountLabel, pDdeServer, "Items" ) ;

   ResourceString TotalFreeLabel ( Library, IDS_TOTALFREE ) ;
   IniData->Items[ITEM_TOTALFREE] = new TotalFree ( ITEM_TOTALFREE,
      "ShowTotalFree", TotalFreeLabel, TotalFreeLabel, pDdeServer, "Items",
      IniData->ShowK, IniData->ShowTrueK, IniData->ShowM, IniData->ShowG, 0 ) ;

   for ( int i=0; i<ITEM_BASE_COUNT; i++ ) 
      IniData->Items[i]->GetProfile ( Prf ) ;

  /**************************************************************************
   * Add items for each drive on the system.                                *
   **************************************************************************/

   #ifdef __OS2__
      ULONG Drive ;
      DosQueryCurrentDisk ( &Drive, &Drives ) ;
   #else
      Drives = GetLogicalDrives ( ) ;
   #endif
   Drives &= ( ~Exclude & ( IniData->ShowDiskDrives?0x03FFFFFF:0 ) ) ;
   UpdateDriveList ( Library, Prf, IniData, pDdeServer, 0, Drives, Drives, Exclude ) ;

   return ( 0 ) ;
}


/***************************************************************************
 *                                                                         *
 *  Class: About Dialog                                                    *
 *                                                                         *
 ***************************************************************************/

class About_Dialog : public Dialog {
   public:
      About_Dialog ( HWND Parent, HWND Owner, HMODULE Resource ) :
         Dialog ( Parent, Owner, Resource, IDD_ABOUT ) {
         Load ( ) ;
      } /* endif */
} ;

/***************************************************************************
 *                                                                         *
 *  Class: Client Window                                                   *
 *                                                                         *
 ***************************************************************************/

extern "C" APIRET16 APIENTRY16 Dos16MemAvail ( PULONG pulAvailMem ) ;

class ClientWindow : public Window {

   private:
      Window        *Frame ;             // Frame window object.
      Module        *Library ;           // Language Library object.
      Profile2      *IniFile ;           // Profile object.
      HWND           Menu ;              // Context menu handle.
      INIDATA        IniData ;           // Most of the profile information.
      ULONG          BorderColor ;       // Border/ColumnSeparator color.
      TID            CounterTID ;        // TaskID for the Idle Counter.
      COUNTER_PARMS  CounterParms ;      // Parameters for the Idle Counter.
      TID            MonitorTID ;        // TaskID for the CPU Load monitor.
      MONITOR_PARMS  MonitorParms ;      // Parameters for the CPU Load monitor.
      time_t         PreviousTime ;      // Time when clock was last checked.
      int            WasDischarging ;    // Flag: Battery was discharging.
      int            Rows ;              // Number of table rows being displayed.
      int            Columns ;           // Number of table columns being displayed.
      int            ColumnWidth ;       // Table column width.
      ULONG          Exclude ;           // Drive exclusion mask.
      ULONG          Drives ;            // Current active drive mask.
      long           Width ;             // Font cell width.
      long           Height ;            // Font cell height.
      int            MaxColumns ;        // Maximum text columns per entry.
      FONTINFO       FontInfo ;          // Current Font information.
      USHORT         CodePage ;          // Display Codepage
      Dde_Server    *pDdeServer ;        // -> DDE Server object
      HWND           DialogWindow ;      // Dialog window handle.

   public:

      ClientWindow ( WindowClass &Class, Window &frame, Module &library, Profile2 &inifile, USHORT codepage, ULONG exclude ) :
         Window ( Class, frame.QueryHandle(), PSZ(""), 0, 0, 0, 0, 0, frame.QueryHandle(), HWND_BOTTOM, FID_CLIENT, this ),
         Frame(&frame), Library(&library), IniFile(&inifile), CodePage(codepage), Exclude(exclude),
         WasDischarging(FALSE), pDdeServer(0) {

         // Determine if we are to use the new CPU Load meter, or the old.
         #ifdef __OS2__
            BOOL UseOldLoadMeter ( CpuLoad::UsePerfAPI ( ) == FALSE ) ;
         #else // __NT__
            BOOL UseOldLoadMeter ( TRUE ) ;
         #endif // __OS2__ vs __NT__

         // Initialize the DDE server object.
         #ifdef __OS2__
            pDdeServer = new Dde_Server ( QueryAnchor(),
               Library->QueryHandle(), QueryHandle(), PROGRAM_NAME ) ;
            pDdeServer->AddTopic ( SZDDESYS_TOPIC, SZDDESYS_ITEM_SYSITEMS ) ;
            char *Topics = SZDDESYS_TOPIC "\t" "Items" ;
            pDdeServer->AddItem ( SZDDESYS_TOPIC, SZDDESYS_ITEM_TOPICS, DDEFMT_TEXT, Topics, strlen(Topics)+1 ) ;
            char *Protocols = "" ;
            pDdeServer->AddItem ( SZDDESYS_TOPIC, SZDDESYS_ITEM_PROTOCOLS, DDEFMT_TEXT, Protocols, strlen(Protocols)+1 ) ;
            pDdeServer->AddTopic ( "Items", "Items" ) ;
         #endif

         // Perform more elaborate initializations.
         #ifdef __OS2__
            BorderColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOWFRAME, 0 ) ;
         #else // __NT__
            BorderColor = GetSysColor ( COLOR_WINDOWFRAME ) ;
         #endif // __OS2__ vs __NT__
         Menu = WinLoadMenu ( HWND_DESKTOP, Library->QueryHandle(), IDM_MENU ) ;
         #ifdef __OS2__
            if ( !UseOldLoadMeter ) 
               Sys_SendMessage ( Menu, MM_DELETEITEM, MPFROM2SHORT(IDM_RESETLOAD,0), 0 ) ;
         #endif
         GetIniData ( QueryAnchor(), Library->QueryHandle(), *IniFile, &IniData, pDdeServer, Drives, Exclude ) ;
         IniData.MaxCount = CalibrateLoadMeter ( &CounterParms ) ;
         IniData.MaxCount = (ULONG) max ( 1, IniData.MaxCount ) ;
         PreviousTime = time ( 0 ) ;

         // Alter the system menu.
         Frame->AddSysMenuSeparator ( ) ;
         ResourceString ResetDefaultsText ( Library->QueryHandle(), IDS_RESET_DEFAULTS ) ;
         Frame->AddSysMenuItem ( PSZ(ResetDefaultsText), IDM_RESET_DEFAULTS ) ;
         #ifdef __OS2__
            ResourceString HideControlsText ( Library->QueryHandle(), IDS_HIDE_CONTROLS ) ;
            Frame->AddSysMenuItem ( PSZ(HideControlsText), IDM_HIDE_CONTROLS ) ;
         #endif // __OS2__
         ResourceString ConfigureText ( Library->QueryHandle(), IDS_CONFIGURE ) ;
         Frame->AddSysMenuItem ( PSZ(ConfigureText), IDM_CONFIGURE ) ;
         if ( UseOldLoadMeter ) {
            ResourceString ResetLoadText ( Library->QueryHandle(), IDS_RESETLOAD ) ;
            Frame->AddSysMenuItem ( PSZ(ResetLoadText), IDM_RESETLOAD ) ;
         } /* endif */
         ResourceString ResetAverageText ( Library->QueryHandle(), IDS_RESETAVERAGE ) ;
         Frame->AddSysMenuItem ( PSZ(ResetAverageText), IDM_RESETAVERAGE ) ;
         ResourceString ResetDrivesText ( Library->QueryHandle(), IDS_RESETDRIVES ) ;
         Frame->AddSysMenuItem ( PSZ(ResetDrivesText), IDM_RESETDRIVES ) ;
         ResourceString LaunchEmailerText ( Library->QueryHandle(), IDS_LAUNCHEMAILER ) ;
         Frame->AddSysMenuItem ( PSZ(LaunchEmailerText), IDM_LAUNCHEMAILER ) ;
         ResourceString CopyText ( Library->QueryHandle(), IDS_COPY ) ;
         Frame->AddSysMenuItem ( PSZ(CopyText), IDM_COPY ) ;
         Frame->AddSysMenuSeparator ( ) ;
         ResourceString AboutText ( Library->QueryHandle(), IDS_ABOUT ) ;
         Frame->AddSysMenuItem ( PSZ(AboutText), IDM_ABOUT ) ;
         ResourceString HelpText ( Library->QueryHandle(), IDS_HELP ) ;
         Frame->AddSysMenuItem ( PSZ(HelpText), 0, MIT_END, MIS_HELP ) ;

         // If necessary, set up the counter thread.
         if ( UseOldLoadMeter ) {
            CounterThreadEvent.Reset ( ) ;
            CounterParms.Active = TRUE ;
            CounterTID = StartThread ( "CounterThread", CounterThread, 0x3000, &CounterParms ) ;
            Sys_SuspendThread ( CounterTID ) ;
            Thread_SetPriority ( CounterTID, PRIORITY_IDLE ) ;
            IniData.IdleCount = 0 ;
            CounterParms.Counter = 0 ;
            if ( IniData.Items[ITEM_CPULOAD]->QueryFlag() )
               Sys_ResumeThread ( CounterTID ) ;
         } else {
            CounterTID = 0 ;
         } /* endif */

         // Set up the monitor thread.
         MonitorThreadEvent.Reset ( ) ;
         MonitorParms.Active = TRUE ;
         MonitorParms.Counter = & CounterParms.Counter ;
         MonitorParms.Interval = & IniData.TimerInterval ;
         MonitorParms.Priority = & IniData.MonitorPriority ;
         MonitorParms.Owner = QueryHandle() ;
         MonitorTID = StartThread ( "MonitorLoopThread", MonitorLoopThread, 0x3000, &MonitorParms ) ;

         // Set up the window.
         Frame->Add2TaskList ( ) ;
         fAnchored = IniData.fAnchorCorner ; AnchorPoint = IniData.AnchorCorner ;
         if ( fAnchored ) Frame->Set_AnchorPoint ( AnchorCorner(AnchorPoint) ) ;
         #ifdef __OS2__
            Frame->HideControls ( IniData.fHideControls ? IniData.HideControls : FALSE ) ;
         #endif // __OS2__
         fBackColor = IniData.fBackColor ; BackColor = IniData.BackColor ;
         if ( fBackColor ) Set_BackColor ( BackColor ) ;
         fTextColor = IniData.fTextColor ; TextColor = IniData.TextColor ;
         if ( fTextColor ) Set_TextColor ( TextColor ) ;
         fFontNameSize = IniData.fFontNameSize ;  strcpy ( FontNameSize, (char*)IniData.FontNameSize ) ;
         if ( fFontNameSize ) Set_FontNameSize ( FontNameSize ) ;
         Frame->Reposition ( int(IniData.Position.x), int(IniData.Position.y),
            int(IniData.Position.cx), int(IniData.Position.cy),
            IniData.Position.fl & SWP_MINIMIZE ? MINIMIZE : RESTORE ) ;

         // Get the selected font's height and width.
         Canvas canvas ( "ClientWindow::ClientWindow::canvas", QueryHandle() ) ;
         RECTANGLE Rectangle = canvas.MeasureText ( " " ) ;
         Width  = Rectangle.xRight - Rectangle.xLeft ;
         Height = Rectangle.yTop - Rectangle.yBottom ;

         // Resize the window to suit the font.
         ResizeWindow ( QueryHandle() ) ;

         // Show the window.
         Frame->Show ( ) ;

         // Activate, then deactivate the window to avoid a bug in FP17.
         Frame->Activate ( ) ;  
         Frame->Deactivate ( ) ;

         // Start the drive reset timer.
         StartTimer ( 1, 60000 ) ;
      } /* endmethod */

      ~ClientWindow ( ) {
         StopTimer ( 1 ) ;
         Sys_ResumeThread ( MonitorTID ) ;
         MonitorParms.Active = FALSE ;
         MonitorThreadEvent.Wait ( 10000 ) ;
         if ( CounterTID ) {
            Sys_ResumeThread ( CounterTID ) ;
            CounterParms.Active = FALSE ;
            Thread_SetPriority ( CounterTID, PRIORITY_TIME_CRITICAL ) ;
            CounterThreadEvent.Wait ( 10000 ) ;
            CounterTID = 0 ;
         } /* endif */
         if ( pDdeServer ) delete pDdeServer ;
         SaveApplication ( ) ;
         if ( Menu )
            Sys_DestroyWindow ( Menu ) ;
      } /* endmethod */

      void UpdateWindow ( BOOL All = FALSE ) {

         // Determine how many items will be displayed.
         int Count = 0 ;
         for ( int i=0; i<IniData.ItemCount; i++ )
            if ( IniData.Items[i]->QueryFlag() )
               Count ++ ;

         // Set up the graphic space.
         Canvas canvas ( "ClientWindow::UpdateWindow::canvas", QueryHandle() ) ;
         canvas.SetCodePage ( CodePage ) ;

         // Update all items.
         int Row = 0, Column = 0 ;
         for ( i=0; i<IniData.ItemCount; i++ ) {
            RECTANGLE Box ;
            Box.xLeft = ColumnWidth*Column + Height/(IniData.TableFormat?2:4) ;
            Box.xRight = Box.xLeft + ColumnWidth - Height/(IniData.TableFormat?1:2) ;
            Box.yBottom = Height * ( Rows - Row - 1 ) ;
            Box.yTop = Box.yBottom + Height ;
            Item *pItem = IniData.Items[i] ;
            if ( pItem && pItem->QueryFlag() ) {
               ULONG Value ; Dos16MemAvail ( &Value ) ;
               pItem->Repaint ( canvas, Box, All ) ;
               Row ++ ;
               if ( Row >= Rows ) {
                  Row = 0 ;
                  Column ++ ;
               } /* endif */
            } /* endif */
         } /* endfor */

         // Paint the column separator bars, if needed.
         if ( Columns > 1 ) {
            canvas.SetColor ( BorderColor ) ;
            RECTANGLE Box = Rectangle ( ) ;
            for ( i=1; i<Columns; i++ ) {
               canvas.Move ( ColumnWidth*i-1, Box.yBottom ) ;
               canvas.Line ( ColumnWidth*i-1, Box.yTop ) ;
            } /* endfor */
         } /* endif */

      } /* endmethod */

      void ResizeWindow ( HWND hwnd ) {

         Window Frame ( PARENT(hwnd) ) ;

         // Determine how many columns and rows will be used.
         Canvas canvas ( "ClientWindow::ResizeWindow::canvas", QueryHandle() ) ;
         canvas.SetCodePage ( CodePage ) ;
         int Count = 0 ;
         MaxColumns = 0 ;
         RECTANGLE Rectangles [100] ;
         memset ( Rectangles, 0, sizeof(Rectangles) ) ;
         for ( int i=0; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            if ( pItem->QueryFlag() ) {
               int Columns = pItem->Measure ( canvas, Rectangles[Count] ) ;
               MaxColumns = max ( MaxColumns, Columns ) ;
               Count ++ ;
            } /* endif */
         } /* endfor */

         // If the window was minimized, restore it invisibly.
         BOOL fHadToHide = FALSE ;
         BOOL fHadToRestore = FALSE ;
         if ( IniData.Position.fl & SWP_MINIMIZE ) {
            if ( Frame.isVisible ( ) ) {
               Frame.Hide ( ) ;
               fHadToHide = TRUE ;
            } /* endif */
            Frame.Restore ( ) ;
            fHadToRestore = TRUE ;
         } /* endif */

         // Get the entire window's current size and position.
         SWP Position ;
         WinQueryWindowPos ( Frame.QueryHandle(), &Position ) ;
         RECTL Rectangle = { Position.x, Position.y, Position.x+Position.cx, Position.y+Position.cy } ;
         WinCalcFrameRect ( Frame.QueryHandle(), &Rectangle, TRUE ) ;

         // Determine the window anchor point.
         POINTL Anchor ;
         switch ( IniData.AnchorCorner ) {
            case CORNER_BL:
            default:
               Anchor.x = Rectangle.xLeft ;
               Anchor.y = Rectangle.yBottom ;
               break;
            case CORNER_BR:
               Anchor.x = Rectangle.xRight ;
               Anchor.y = Rectangle.yBottom ;
               break;
            case CORNER_TL:
               Anchor.x = Rectangle.xLeft ;
               Anchor.y = Rectangle.yTop ;
               break;
            case CORNER_TR:
               Anchor.x = Rectangle.xRight ;
               Anchor.y = Rectangle.yTop ;
               break;
         } /* endswitch */

         // Update the window style.
         ULONG Style = WinQueryWindowULong ( Frame.QueryHandle(), QWL_STYLE ) ;
         Style &= ~ ( FS_DLGBORDER | FS_BORDER ) ;
         Style |= IniData.DialogBorder ? FS_DLGBORDER : FS_BORDER ;
         WinSetWindowULong ( Frame.QueryHandle(), QWL_STYLE, Style ) ;

         // Determine the new client window size.
         if ( Count ) {

            ColumnWidth = 0 ;
            for ( i=0; i<Count; i++ )
               ColumnWidth = int ( max ( ColumnWidth, (Rectangles[i].xRight-Rectangles[i].xLeft+1) ) ) ;

            if ( IniData.TableFormat ) {
               ColumnWidth += Height ;
               RECTL Desktop ;
               WinQueryWindowRect ( HWND_DESKTOP, &Desktop ) ;
               Columns = int ( ( Desktop.xRight - Desktop.xLeft ) / ColumnWidth ) ;
               Rows = Count / Columns + ( ( Count % Columns ) ? 1 : 0 ) ;
               Columns = Count / Rows + ( ( Count % Rows ) ? 1 : 0 ) ;

            } else {
               ColumnWidth += Height/2 ;
               Columns = 1 ;
               Rows = Count ;

            } /* endif */

            LONG Height = ( Rectangles[0].yTop - Rectangles[0].yBottom ) * Rows ;
            Rectangle.xLeft = Rectangle.yBottom = 0 ;
            Rectangle.xRight = ColumnWidth * Columns ;
            Rectangle.yTop = Height ;

         } else {
            Rectangle.xLeft = Rectangle.yBottom = 0 ;
            Rectangle.xRight = 200 ;
            Rectangle.yTop = 0 ;

         } /* endif */

         // Determine the new rectangle based on the anchor corner.
         switch ( IniData.AnchorCorner ) {
            case CORNER_BL:
            default:
               Rectangle.xRight = Anchor.x + ( Rectangle.xRight - Rectangle.xLeft ) ;
               Rectangle.xLeft = Anchor.x ;
               Rectangle.yTop = Anchor.y + ( Rectangle.yTop - Rectangle.yBottom ) ;
               Rectangle.yBottom = Anchor.y ;
               break;
            case CORNER_BR:
               Rectangle.xLeft = Anchor.x - ( Rectangle.xRight - Rectangle.xLeft ) ;
               Rectangle.xRight = Anchor.x ;
               Rectangle.yTop = Anchor.y + ( Rectangle.yTop - Rectangle.yBottom ) ;
               Rectangle.yBottom = Anchor.y ;
               break;
            case CORNER_TL:
               Rectangle.xRight = Anchor.x + ( Rectangle.xRight - Rectangle.xLeft ) ;
               Rectangle.xLeft = Anchor.x ;
               Rectangle.yBottom = Anchor.y - ( Rectangle.yTop - Rectangle.yBottom ) ;
               Rectangle.yTop = Anchor.y ;
               break;
            case CORNER_TR:
               Rectangle.xLeft = Anchor.x - ( Rectangle.xRight - Rectangle.xLeft ) ;
               Rectangle.xRight = Anchor.x ;
               Rectangle.yBottom = Anchor.y - ( Rectangle.yTop - Rectangle.yBottom ) ;
               Rectangle.yTop = Anchor.y ;
               break;
         } /* endswitch */

         // Adjust the window size for the frame.
         WinCalcFrameRect ( Frame.QueryHandle(), &Rectangle, FALSE ) ;

         // Move and size the window.
         Frame.Move ( Rectangle.xLeft, Rectangle.yBottom ) ;
         Frame.Size ( Rectangle.xRight-Rectangle.xLeft, Rectangle.yTop-Rectangle.yBottom ) ;

         // If the window had to be restored (because minimized), then minimize it over again.
         if ( fHadToRestore ) {
            Frame.Move ( IniData.Position.x, IniData.Position.y ) ;
            Frame.Size ( IniData.Position.cx, IniData.Position.cy ) ;
            Frame.Minimize ( ) ;
         } /* endif */

         // Show the window now if we had to hide it.
         if ( fHadToHide )
            Frame.Show ( ) ;

         // Cause the window to be repainted.
         Invalidate ( ) ;

      } /* endmethod */

      MRESULT Moved_Sized ( HWND hwnd, BOOL ) { // This may be called before base constructor completes.
         Window Frame ( PARENT(hwnd) ) ;
         SWP NewPosition ;
         WinQueryWindowPos ( Frame.QueryHandle(), &NewPosition ) ;
         if (   NOT ( NewPosition.fl & SWP_HIDE     )
            AND NOT ( NewPosition.fl & SWP_MINIMIZE )
            AND NOT ( NewPosition.fl & SWP_MAXIMIZE ) ) {
            IniData.Position.x = NewPosition.x ;
            IniData.Position.y = NewPosition.y ;
            IniData.Position.cx = NewPosition.cx ;
            IniData.Position.cy = NewPosition.cy ;
         } /* endif */
         #ifdef __OS2__
         if ( IniData.HideControls )
            if ( ( NewPosition.fl & SWP_MINIMIZE ) != ( IniData.Position.fl & SWP_MINIMIZE ) )
               Frame.HideControls ( NOT ( NewPosition.fl & SWP_MINIMIZE ) ) ;
         #endif // __OS2__
         IniData.Position.fl = NewPosition.fl ;
         return ( 0 ) ;
      } /* endmethod */

      MRESULT PresParamChanged ( long ID ) {
         MRESULT Result = Window::MessageProcessor ( QueryHandle(), WM_PRESPARAMCHANGED, MPFROMLONG(ID), 0 ) ;
         switch ( ID ) {
            case PP_FONTNAMESIZE: {
               IniData.fFontNameSize = fFontNameSize ;
               strcpy ( IniData.FontNameSize, FontNameSize ) ;
               Canvas canvas ( "ClientWindow::PresParamChanged::canvas", QueryHandle() ) ;
               RECTANGLE Rectangle = canvas.MeasureText ( " " ) ;
               Width  = Rectangle.xRight - Rectangle.xLeft ;
               Height = Rectangle.yTop - Rectangle.yBottom ;
               ResizeWindow ( QueryHandle() ) ;
               break ;
            } /* endcase */
            case PP_BACKGROUNDCOLOR: {
               IniData.fBackColor = fBackColor ;
               IniData.BackColor = BackColor ;
               for ( int i=0; i<IniData.ItemCount; i++ ) {
                   Item *pItem = IniData.Items[i] ;
                   pItem->SetNormalColors  ( IniData.BackColor, IniData.TextColor ) ;
               } /* endfor */
               Invalidate ( ) ;
               break ;
            } /* endcase */
            case PP_FOREGROUNDCOLOR: {
               IniData.fTextColor = fTextColor ;
               IniData.TextColor = TextColor ;
               for ( int i=0; i<IniData.ItemCount; i++ ) {
                   Item *pItem = IniData.Items[i] ;
                   pItem->SetNormalColors  ( IniData.BackColor, IniData.TextColor ) ;
               } /* endfor */
               Invalidate ( ) ;
               break ;
            } /* endcase */
         } /* endswitch */
         return ( Result ) ;
      } /* endmethod */

      MRESULT SysColorChange ( MPARAM1 mp1, MPARAM2 mp2 ) {
         MRESULT Result = Window::MessageProcessor ( QueryHandle(), WM_SYSCOLORCHANGE, mp1, mp2 ) ;
         IniData.fBackColor = fBackColor ;  IniData.BackColor = BackColor ;
         IniData.fTextColor = fTextColor ;  IniData.TextColor = TextColor ;
         BorderColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOWFRAME, 0 ) ;
         for ( int i=0; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            pItem->SetNormalColors  ( IniData.BackColor, IniData.TextColor ) ;
         } /* endfor */
         return ( Result ) ;
      } /* endmethod */

      MRESULT Timer ( int Id ) {
         if ( Id == 1 ) 
            PostMessage ( WM_COMMAND, MPFROM2SHORT(IDM_RESETDRIVES,0), MPFROM2SHORT(CMDSRC_OTHER,TRUE) ) ;
         return ( 0 ) ;
      } /* endmethod */

      MRESULT Paint ( ) {
         { /* startblock */
            RECTANGLE Clip ;
            Canvas canvas ( "ClientWindow::Paint::canvas", QueryHandle(), Clip ) ;
            canvas.SetCodePage ( CodePage ) ;
            canvas.QueryFontInfo ( FontInfo ) ;
            canvas.SetColor ( BackColor ) ;
            RECTANGLE Box = Rectangle ( ) ;
            canvas.BoxFill ( Box ) ;
         } /* endblock */
         UpdateWindow ( TRUE ) ;
         return ( 0 ) ;
      } /* endmethod */

      MRESULT InitMenu ( MPARAM1 mp1, MPARAM2 mp2 ) {

         // Get the message data.
         int MenuID = SHORT1FROMMP ( mp1 ) ;
         HWND Menu = HWNDFROMMP ( mp2 ) ;

         // Process according to which menu's about to be displayed.
         switch ( MenuID ) {
            case FID_SYSMENU:
            case FID_MENU:
            case IDM_MENU: {
               #ifdef __OS2__
               CheckMenuItem ( Menu, IDM_HIDE_CONTROLS, IniData.HideControls ) ;
               #endif // __OS2__
               break ; }
         } /* endswitch */

         return ( 0 ) ;
      } /* endmethod */

      MRESULT SaveApplication ( ) {
         IniFile->PutIniData ( &IniData ) ;
         return ( Window::MessageProcessor ( QueryHandle(), WM_SAVEAPPLICATION, 0, 0 ) ) ;
      } /* endmethod */

      MRESULT SummonContextMenu ( MPARAM1 mp1 ) {
         Window TheMenu ( Menu ) ;
         TheMenu.SetPresParam ( PP_FONTNAMESIZE,
            fFontNameSize ? strlen(FontNameSize)+1 : 0,
            fFontNameSize ? PSZ(FontNameSize) : PSZ("") ) ;
         PopupMenu ( Menu, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1) ) ;
         return ( 0 ) ;
      } /* endmethod */

      MRESULT Activating ( int GoingActive ) {
         if ( GoingActive )
            Sys_SendMessage ( WinQueryHelpInstance(QueryHandle()), HM_SET_ACTIVE_WINDOW,
               MPFROMHWND(QueryHandle()), MPFROMHWND(QueryHandle()) ) ;
         return ( 0 ) ;
      } /* endmethod */

      void ResetDefaults ( ) {
         IniFile->Reset ( ) ;
         RemovePresParam ( PP_FONTNAMESIZE ) ;
         RemovePresParam ( PP_FOREGROUNDCOLOR ) ;
         RemovePresParam ( PP_BACKGROUNDCOLOR ) ;
         IniData.Reset ( ) ;
         SetPresParam ( PP_BACKGROUNDCOLOR, sizeof(IniData.BackColor), &IniData.BackColor ) ;
         SetPresParam ( PP_FOREGROUNDCOLOR, sizeof(IniData.TextColor), &IniData.TextColor ) ;
         #ifdef __OS2__
         if ( NOT ( IniData.Position.fl & SWP_MINIMIZE ) )
            Frame->HideControls ( IniData.HideControls ) ;
         #endif // __OS2__
         if ( IniData.Items[ITEM_CPULOAD]->QueryFlag() ) {
            if ( CounterTID ) Sys_ResumeThread ( CounterTID ) ;
         } else { 
            if ( CounterTID ) Sys_SuspendThread ( CounterTID ) ;
         } /* endif */
         ((Clock*)IniData.Items[ITEM_CLOCK])->SetDateFormat ( IniData.DateFormat ) ;
         ((ElapsedTime*)IniData.Items[ITEM_ELAPSEDTIME])->SetShowSeconds ( IniData.ShowSeconds ) ;
         ((AverageLoad*)IniData.Items[ITEM_AVERAGELOAD])->SetInterval ( IniData.AveragingInterval ) ;
         for ( int i=0; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            pItem->SetFlag ( ) ;
            pItem->SetLabel ( pItem->QueryDefaultLabel() ) ;
            pItem->SetSenseEnabled  ( pItem->QueryLevelSense() ? TRUE : FALSE ) ;
            __int64 Warning, Error ; pItem->QueryDefaultLevels ( Warning, Error ) ;
            pItem->SetWarningLevel  ( Warning ) ;
            pItem->SetErrorLevel    ( Error ) ;
            pItem->SetNormalColors  ( IniData.BackColor, IniData.TextColor ) ;
            pItem->SetWarningColors ( IniData.WarningBackground, IniData.WarningForeground ) ;
            pItem->SetErrorColors   ( IniData.ErrorBackground, IniData.ErrorForeground ) ;
         } /* endfor */

         ((MemoryFree *)IniData.Items[ITEM_MEMORYFREE   ])->SetShowK ( IniData.ShowK ) ;
         ((VMFree     *)IniData.Items[ITEM_VIRTUALFREE  ])->SetShowK ( IniData.ShowK ) ;
         ((SwapFree   *)IniData.Items[ITEM_SWAPDISKFREE ])->SetShowK ( IniData.ShowK ) ;
         ((SwapSize   *)IniData.Items[ITEM_SWAPFILESIZE ])->SetShowK ( IniData.ShowK ) ;
         ((SwapSlack  *)IniData.Items[ITEM_SWAPFILESLACK])->SetShowK ( IniData.ShowK ) ;
         ((SpoolSize  *)IniData.Items[ITEM_SPOOLFILESIZE])->SetShowK ( IniData.ShowK ) ;
         ((TotalFree  *)IniData.Items[ITEM_TOTALFREE    ])->SetShowK ( IniData.ShowK ) ;

         ((MemoryFree *)IniData.Items[ITEM_MEMORYFREE   ])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((VMFree     *)IniData.Items[ITEM_VIRTUALFREE  ])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((SwapFree   *)IniData.Items[ITEM_SWAPDISKFREE ])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((SwapSize   *)IniData.Items[ITEM_SWAPFILESIZE ])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((SwapSlack  *)IniData.Items[ITEM_SWAPFILESLACK])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((SpoolSize  *)IniData.Items[ITEM_SPOOLFILESIZE])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((TotalFree  *)IniData.Items[ITEM_TOTALFREE    ])->SetShowTrueK ( IniData.ShowTrueK ) ;

         ((MemoryFree *)IniData.Items[ITEM_MEMORYFREE   ])->SetShowM ( IniData.ShowM ) ;
         ((VMFree     *)IniData.Items[ITEM_VIRTUALFREE  ])->SetShowM ( IniData.ShowM ) ;
         ((SwapFree   *)IniData.Items[ITEM_SWAPDISKFREE ])->SetShowM ( IniData.ShowM ) ;
         ((SwapSize   *)IniData.Items[ITEM_SWAPFILESIZE ])->SetShowM ( IniData.ShowM ) ;
         ((SwapSlack  *)IniData.Items[ITEM_SWAPFILESLACK])->SetShowM ( IniData.ShowM ) ;
         ((SpoolSize  *)IniData.Items[ITEM_SPOOLFILESIZE])->SetShowM ( IniData.ShowM ) ;
         ((TotalFree  *)IniData.Items[ITEM_TOTALFREE    ])->SetShowM ( IniData.ShowM ) ;

         ((MemoryFree *)IniData.Items[ITEM_MEMORYFREE   ])->SetShowG ( IniData.ShowG ) ;
         ((VMFree     *)IniData.Items[ITEM_VIRTUALFREE  ])->SetShowG ( IniData.ShowG ) ;
         ((SwapFree   *)IniData.Items[ITEM_SWAPDISKFREE ])->SetShowG ( IniData.ShowG ) ;
         ((SwapSize   *)IniData.Items[ITEM_SWAPFILESIZE ])->SetShowG ( IniData.ShowG ) ;
         ((SwapSlack  *)IniData.Items[ITEM_SWAPFILESLACK])->SetShowG ( IniData.ShowG ) ;
         ((SpoolSize  *)IniData.Items[ITEM_SPOOLFILESIZE])->SetShowG ( IniData.ShowG ) ;
         ((TotalFree  *)IniData.Items[ITEM_TOTALFREE    ])->SetShowG ( IniData.ShowG ) ;

         for ( i=ITEM_BASE_COUNT; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            ((DriveFree*)pItem)->SetShowFileSystemName ( IniData.ShowFileSystemNames ) ;
            ((DriveFree*)pItem)->SetShowDiskLabel ( IniData.ShowDiskLabels ) ;
            ((DriveFree*)pItem)->SetShowK ( IniData.ShowK ) ;
            ((DriveFree*)pItem)->SetShowTrueK ( IniData.ShowTrueK ) ;
            ((DriveFree*)pItem)->SetShowM ( IniData.ShowM ) ;
            ((DriveFree*)pItem)->SetShowG ( IniData.ShowG ) ;
         } /* endfor */

         Drives = 0x03FFFFFF ;
         Drives &= ( ~Exclude & ( IniData.ShowDiskDrives?0x03FFFFFF:0 ) ) ;
         if ( UpdateDriveList ( Library->QueryHandle(), *IniFile,
            &IniData, pDdeServer, Drives, Drives, Drives, Exclude ) )
            if ( DialogWindow )
               Sys_SendMessage ( DialogWindow, WM_UPDATEDRIVES, MPFROMP(&IniData), 0 ) ;

         ResizeWindow ( QueryHandle() ) ;
         SaveApplication ( ) ;
      } /* endmethod */

      void HideControlsCmd ( ) {
         #ifdef __OS2__
         Frame->HideControls ( NOT Frame->QueryControlsHidden() ) ;
         IniData.HideControls = Frame->QueryControlsHidden() ;
         IniData.fHideControls = TRUE ;
         #endif // __OS2__
      } /* endmethod */

      void Configure ( ) {
         CONFIG_PARMS Parms = { 0 } ;
         Parms.HideControls        = IniData.HideControls ;
         Parms.Float               = IniData.Float ;
         Parms.Animate             = IniData.Animate ;
         Parms.DialogBorder        = IniData.DialogBorder ;
         Parms.TableFormat         = IniData.TableFormat ;
         Parms.Chime               = IniData.Chime ;
         Parms.ShowDiskDrives      = IniData.ShowDiskDrives ;
         Parms.ShowRemoteDrives    = IniData.ShowRemoteDrives ;
         Parms.ShowFileSystemNames = IniData.ShowFileSystemNames ;
         Parms.ShowDiskLabels      = IniData.ShowDiskLabels ;
         Parms.ShowSeconds         = IniData.ShowSeconds ;
         Parms.UseTheseus          = IniData.UseTheseus ;
         Parms.CenterClock         = IniData.CenterClock ;
         Parms.ShowK               = IniData.ShowK ;
         Parms.ShowTrueK           = IniData.ShowTrueK ;
         Parms.ShowM               = IniData.ShowM ;
         Parms.ShowG               = IniData.ShowG ;
         Parms.MonitorPriority     = IniData.MonitorPriority ;
         Parms.TimerInterval       = IniData.TimerInterval ;
         Parms.AveragingInterval   = IniData.AveragingInterval ;
         Parms.AnchorCorner        = IniData.AnchorCorner ;
         Parms.NormalBackground    = IniData.BackColor ;
         Parms.NormalForeground    = IniData.TextColor ;
         Parms.WarningBackground   = IniData.WarningBackground ;
         Parms.WarningForeground   = IniData.WarningForeground ;
         Parms.ErrorBackground     = IniData.ErrorBackground ;
         Parms.ErrorForeground     = IniData.ErrorForeground ;
         Parms.PopPort             = IniData.PopPort ;
         Parms.PopForceIP          = IniData.PopForceIP ;
         Parms.PopUseAPOP          = IniData.PopUseAPOP ;
         Parms.PopInterval         = IniData.PopInterval ;
         Parms.PopAlert            = IniData.PopAlert ;
         Parms.ItemCount           = IniData.ItemCount ;
         strcpy ( Parms.Font, IniData.FontNameSize ) ;
         strcpy ( Parms.DateFormat, IniData.DateFormat ) ;
         strcpy ( Parms.PopServer, IniData.PopServer ) ;
         strcpy ( Parms.PopUserName, IniData.PopUserName ) ;
         strcpy ( Parms.PopPassword, IniData.PopPassword ) ;
         strcpy ( Parms.PopReader, IniData.PopReader ) ;
         strcpy ( Parms.PopWavFile, IniData.PopWavFile ) ;
         for ( int i=0; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            Parms.ItemFlags[i] = pItem->QueryFlag () ;
            strcpy ( Parms.CurrentLabels[i], PCHAR(pItem->QueryCurrentLabel()) ) ;
            strcpy ( Parms.DefaultLabels[i], PCHAR(pItem->QueryDefaultLabel()) ) ;
            pItem->QueryDefaultLevels ( Parms.DefaultLevels[i][0], Parms.DefaultLevels[i][1] ) ;
            Parms.WarningLevel[i] = pItem->QueryWarningLevel() ;
            Parms.ErrorLevel[i] = pItem->QueryErrorLevel() ;
            Parms.LevelSense[i] = pItem->QueryLevelSense() ;
            Parms.SenseEnabled[i] = pItem->QuerySenseEnabled() ;
            Parms.MinLevel[i] = pItem->QueryMinLevel() ;
            Parms.MaxLevel[i] = pItem->QueryMaxLevel() ;
         } /* endfor */
         Parms.MainWindow = QueryHandle() ;
         Configure_Dialog ThisDialog ( HWND_DESKTOP, QueryHandle(), Library->QueryHandle(), Parms ) ;
         DialogWindow = ThisDialog.QueryHandle() ;
         if ( DialogWindow ) {
            if ( ThisDialog.Process ( ) ) 
               UpdateParms ( &Parms ) ;
            DialogWindow = 0 ;
         } /* endif */
      } /* endmethod */
       
      void ResetLoad ( ) {

         // Shut down the CPU load meter threads.
         Sys_ResumeThread ( MonitorTID ) ;
         MonitorParms.Active = FALSE ;
         MonitorThreadEvent.Wait ( 10000 ) ;
         if ( CounterTID ) {
            Sys_ResumeThread ( CounterTID ) ;
            CounterParms.Active = FALSE ;
            Thread_SetPriority ( CounterTID, PRIORITY_TIME_CRITICAL ) ;
            CounterThreadEvent.Wait ( 10000 ) ;
         } /* endif */

         // Reset the load meter.
         IniData.MaxCount = CalibrateLoadMeter ( &CounterParms ) ;
         IniData.MaxCount = (ULONG) max ( 1, IniData.MaxCount ) ;

         // Restart the CPU load meter threads.
         if ( CounterTID ) {
            CounterThreadEvent.Reset ( ) ;
            CounterParms.Active = TRUE ;
            CounterTID = StartThread ( "CounterThread", CounterThread, 0x3000, &CounterParms ) ;
            Sys_SuspendThread ( CounterTID ) ;
            Thread_SetPriority ( CounterTID, PRIORITY_IDLE ) ;
            IniData.IdleCount = 0 ;
            CounterParms.Counter = 0 ;
            if ( IniData.Items[ITEM_CPULOAD]->QueryFlag() )
               Sys_ResumeThread ( CounterTID ) ;
         } /* endif */
         MonitorThreadEvent.Reset ( ) ;
         MonitorParms.Active = TRUE ;
         MonitorParms.Counter = & CounterParms.Counter ;
         MonitorParms.Interval = & IniData.TimerInterval ;
         MonitorParms.Priority = & IniData.MonitorPriority ;
         MonitorParms.Owner = QueryHandle() ;
         MonitorTID = StartThread ( "MonitorLoopThread", MonitorLoopThread, 0x3000, &MonitorParms ) ;
      } /* endmethod */

      void ResetAverage ( ) {
         ((AverageLoad*)IniData.Items[ITEM_AVERAGELOAD])->Reset ( ) ;
      } /* endmethod */

      void ResetDrives ( ) {
         // Reset all the drive error flags.
         for ( int i=ITEM_BASE_COUNT; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            ((DriveFree*)pItem)->ResetError ( ) ;
         } /* endfor */
         // Recheck all drives, except those specifically excluded.               
         ULONG NewDrives = 0x03FFFFFF ;
         Drives &= ( ~Exclude & ( IniData.ShowDiskDrives?0x03FFFFFF:0 ) ) ;
         if ( UpdateDriveList ( Library->QueryHandle(), *IniFile,
            &IniData, pDdeServer, Drives, NewDrives, Drives, Exclude ) )
            if ( DialogWindow )
               Sys_SendMessage ( DialogWindow, WM_UPDATEDRIVES, MPFROMP(&IniData), 0 ) ;
      } /* endmethod */

      void LaunchEmailer ( ) {
         char Command [500] ;
         sprintf ( Command, "/C %s", IniData.PopReader ) ;
         STARTDATA StartData = { 0 } ;
         StartData.Length        = sizeof(StartData) ;
         StartData.Related       = SSF_RELATED_INDEPENDENT ;
         StartData.FgBg          = SSF_FGBG_FORE ;
         StartData.TraceOpt      = SSF_TRACEOPT_NONE ;
         StartData.PgmTitle      = "E-Mail Reader" ;
         StartData.PgmName       = "CMD.EXE" ;
         StartData.PgmInputs     = Command ;
         StartData.InheritOpt    = SSF_INHERTOPT_SHELL ;
         StartData.SessionType   = SSF_TYPE_DEFAULT ;
         StartData.PgmControl    = SSF_CONTROL_INVISIBLE ;
         ULONG Session ; PID Process ;
         DosStartSession ( &StartData, &Session, &Process ) ;
      } /* endmethod */

      void Copy ( ) {
         // Review all items.  Display those changed, or all.                    
         char *Buffer = (char*) malloc ( ( ITEM_BASE_COUNT + MAX_DRIVES ) * ( MaxColumns + 1 ) ) ;
         Buffer [0] = 0 ;
         for ( int i=0; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            if ( pItem->QueryFlag() ) {
               pItem->FormatLine ( Buffer, MaxColumns ) ;
            } /* endif */
         } /* endfor */
         char *ClipText ;
         DosAllocSharedMem ( PPVOID(&ClipText), 0, strlen(Buffer)+1,
            PAG_WRITE | PAG_COMMIT | OBJ_GIVEABLE ) ;
         strcpy ( ClipText, Buffer ) ;
         free ( Buffer ) ;
         // Create bitmap image of current window.                                
         Canvas canvas ( "ClientWindow::Copy::canvas" ) ;
         canvas.SetCodePage ( CodePage ) ;
         RECTANGLE Box = Rectangle ( ) ;
         HBITMAP Bitmap = canvas.CreateBitmap ( Box.xRight-Box.xLeft, Box.yTop-Box.yBottom ) ;
         canvas.SetBitmap ( Bitmap ) ;
         canvas.SetFont ( FontInfo ) ;
         canvas.SetColor ( IniData.BackColor ) ;
         canvas.BoxFill ( Box ) ;
         int Count = 0 ;
         for ( i=0; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            if ( pItem->QueryFlag() ) {
               Count ++ ;
            } /* endif */
         } /* endfor */
         Box.xLeft += Height / 2 ;
         Box.xRight -= Height / 2 ;
         Box.yBottom = Height * ( Count - 1 ) ;
         Box.yTop = Box.yBottom + Height ;
         int Row = 0, Column = 0 ;
         for ( i=0; i<IniData.ItemCount; i++ ) {
            RECTANGLE Rectangle ;
            Rectangle.xLeft = ColumnWidth*Column + Height/2 ;
            Rectangle.xRight = Rectangle.xLeft + ColumnWidth - Height ;
            Rectangle.yBottom = Height * ( Rows - Row - 1 ) ;
            Rectangle.yTop = Rectangle.yBottom + Height ;
            Item *pItem = IniData.Items[i] ;
            if ( pItem->QueryFlag() ) {
               pItem->Repaint ( canvas, Rectangle, TRUE ) ;
               Row ++ ;
               if ( Row >= Rows ) {
                  Row = 0 ;
                  Column ++ ;
               } /* endif */
            } /* endif */
         } /* endfor */
         if ( Columns > 1 ) {
            canvas.SetColor ( BorderColor ) ;
            for ( i=1; i<Columns; i++ ) {
               POINTL Point = { ColumnWidth*i-1, Box.yBottom } ;
               canvas.Move ( Point ) ;
               Point.y = Box.yTop ;
               canvas.Line ( Point ) ;
            } /* endfor */
         } /* endif */
         // Empty the clipboard and give it new data.                            
         if ( WinOpenClipbrd ( QueryAnchor() ) ) {
            WinEmptyClipbrd ( QueryAnchor() ) ;
            WinSetClipbrdData ( QueryAnchor(), ULONG(ClipText), CF_TEXT, CFI_POINTER ) ;
            WinSetClipbrdData ( QueryAnchor(), ULONG(Bitmap), CF_BITMAP, CFI_HANDLE ) ;
            WinCloseClipbrd ( QueryAnchor() ) ;
         } else {
            DosFreeMem ( ClipText ) ;
         } /* endif */
      } /* endmethod */

      void Exit ( ) {
         SendMessage ( WM_CLOSE ) ;
      } /* endmethod */

      void About ( ) {
         About_Dialog ThisDialog ( HWND_DESKTOP, QueryHandle(), Library->QueryHandle() ) ;
         ThisDialog.Process ( ) ;
      } /* endmethod */

      MRESULT Command ( MPARAM1 mp1, MPARAM2 ) {
         switch ( SHORT1FROMMP(mp1) ) {
            case IDM_RESET_DEFAULTS:    ResetDefaults ( ) ;     break ;
            case IDM_HIDE_CONTROLS:     HideControlsCmd ( ) ;   break ;
            case IDM_CONFIGURE:         Configure ( ) ;         break ;
            case IDM_RESETLOAD:         ResetLoad ( ) ;         break ;
            case IDM_RESETAVERAGE:      ResetAverage ( ) ;      break ;
            case IDM_RESETDRIVES:       ResetDrives ( ) ;       break ;
            case IDM_LAUNCHEMAILER:     LaunchEmailer ( ) ;     break ;
            case IDM_COPY:              Copy ( ) ;              break ;
            case IDM_EXIT:              Exit ( ) ;              break ;
            case IDM_ABOUT:             About ( ) ;             break ;
         } /* endswitch */
         return ( 0 ) ;
      } /* endmethod */

      MRESULT BeginDrag ( ) {
         // Determine the new window position.                                     
         TRACKINFO TrackInfo ;
         memset ( &TrackInfo, 0, sizeof(TrackInfo) ) ;
         TrackInfo.cxBorder = 1 ;
         TrackInfo.cyBorder = 1 ;
         TrackInfo.cxGrid = 1 ;
         TrackInfo.cyGrid = 1 ;
         TrackInfo.cxKeyboard = 8 ;
         TrackInfo.cyKeyboard = 8 ;
         SWP Position ;
         WinQueryWindowPos ( Frame->QueryHandle(), &Position ) ;
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
            Frame->Move ( TrackInfo.rclTrack.xLeft, TrackInfo.rclTrack.yBottom ) ;
         return ( 0 ) ;
      } /* endmethod */

      MRESULT ButtonDoubleClick ( MPARAM1, MPARAM2 ) {
         PostMessage ( WM_COMMAND, MPFROM2SHORT(IDM_HIDE_CONTROLS,0), MPFROM2SHORT(CMDSRC_OTHER,TRUE) ) ;
         return ( 0 ) ;
      } /* endmethod */

      MRESULT HelpError ( MPARAM1 mp1, MPARAM2 ) {
         static struct {
            ULONG Error ;
            USHORT StringId ;
         } HelpErrors [] = {
            { HMERR_NO_FRAME_WND_IN_CHAIN,     IDS_HMERR_NO_FRAME_WND_IN_CHAIN },
            { HMERR_INVALID_ASSOC_APP_WND,     IDS_HMERR_INVALID_ASSOC_APP_WND },
            { HMERR_INVALID_ASSOC_HELP_INST,   IDS_HMERR_INVALID_ASSOC_HELP_IN },
            { HMERR_INVALID_DESTROY_HELP_INST, IDS_HMERR_INVALID_DESTROY_HELP_ },
            { HMERR_NO_HELP_INST_IN_CHAIN,     IDS_HMERR_NO_HELP_INST_IN_CHAIN },
            { HMERR_INVALID_HELP_INSTANCE_HDL, IDS_HMERR_INVALID_HELP_INSTANCE },
            { HMERR_INVALID_QUERY_APP_WND,     IDS_HMERR_INVALID_QUERY_APP_WND },
            { HMERR_HELP_INST_CALLED_INVALID,  IDS_HMERR_HELP_INST_CALLED_INVA },
            { HMERR_HELPTABLE_UNDEFINE,        IDS_HMERR_HELPTABLE_UNDEFINE    },
            { HMERR_HELP_INSTANCE_UNDEFINE,    IDS_HMERR_HELP_INSTANCE_UNDEFIN },
            { HMERR_HELPITEM_NOT_FOUND,        IDS_HMERR_HELPITEM_NOT_FOUND    },
            { HMERR_INVALID_HELPSUBITEM_SIZE,  IDS_HMERR_INVALID_HELPSUBITEM_S },
            { HMERR_HELPSUBITEM_NOT_FOUND,     IDS_HMERR_HELPSUBITEM_NOT_FOUND },
            { HMERR_INDEX_NOT_FOUND,           IDS_HMERR_INDEX_NOT_FOUND       },
            { HMERR_CONTENT_NOT_FOUND,         IDS_HMERR_CONTENT_NOT_FOUND     },
            { HMERR_OPEN_LIB_FILE,             IDS_HMERR_OPEN_LIB_FILE         },
            { HMERR_READ_LIB_FILE,             IDS_HMERR_READ_LIB_FILE         },
            { HMERR_CLOSE_LIB_FILE,            IDS_HMERR_CLOSE_LIB_FILE        },
            { HMERR_INVALID_LIB_FILE,          IDS_HMERR_INVALID_LIB_FILE      },
            { HMERR_NO_MEMORY,                 IDS_HMERR_NO_MEMORY             },
            { HMERR_ALLOCATE_SEGMENT,          IDS_HMERR_ALLOCATE_SEGMENT      },
            { HMERR_FREE_MEMORY,               IDS_HMERR_FREE_MEMORY           },
            { HMERR_PANEL_NOT_FOUND,           IDS_HMERR_PANEL_NOT_FOUND       },
            { HMERR_DATABASE_NOT_OPEN,         IDS_HMERR_DATABASE_NOT_OPEN     },
            { 0,                               IDS_HMERR_UNKNOWN               }
         } ;
         ULONG ErrorCode = (ULONG) LONGFROMMP ( mp1 ) ;
         int Index = 0 ;
         while ( HelpErrors[Index].Error AND ( HelpErrors[Index].Error != ErrorCode ) )
            Index ++ ;
         ResourceString Title ( Library->QueryHandle(), IDS_HMERR ) ;
         ResourceString Message ( Library->QueryHandle(), HelpErrors[Index].StringId ) ;
         MessageBox ( PSZ(Title), PSZ(Message), 0, MB_OK | MB_WARNING ) ;
         return ( MRFROMSHORT ( 0 ) ) ;
      } /* endmethod */

      MRESULT ExtHelpUndefined ( MPARAM1, MPARAM2 ) {
         ResourceString Title ( Library->QueryHandle(), IDS_HMERR ) ;
         ResourceString Message ( Library->QueryHandle(), IDS_HMERR_EXTHELPUNDEFINED ) ;
         MessageBox ( PSZ(Title), PSZ(Message), 0, MB_OK | MB_WARNING ) ;
         return ( MRFROMSHORT ( 0 ) ) ;
      } /* endmethod */

      MRESULT HelpSubitemNotFound ( MPARAM1 mp1, MPARAM2 mp2 ) {
         ResourceString Title ( Library->QueryHandle(), IDS_HMERR ) ;
         USHORT Topic = (USHORT) SHORT1FROMMP ( mp2 ) ;
         USHORT Subtopic = (USHORT) SHORT2FROMMP ( mp2 ) ;
         ResourceString Frame   ( Library->QueryHandle(), IDS_HELPMODE_FRAME ) ;
         ResourceString Menu    ( Library->QueryHandle(), IDS_HELPMODE_MENU ) ;
         ResourceString Window  ( Library->QueryHandle(), IDS_HELPMODE_WINDOW ) ;
         ResourceString Unknown ( Library->QueryHandle(), IDS_HELPMODE_UNKNOWN ) ;
         PBYTE Mode ;
         switch ( SHORT1FROMMP ( mp1 ) ) {
            case HLPM_FRAME:
               Mode = PSZ(Frame) ;
               break ;
            case HLPM_MENU:
               Mode = PSZ(Menu) ;
               break ;
            case HLPM_WINDOW:
               Mode = PSZ(Window) ;
               break ;
            default:
               Mode = PSZ(Unknown) ;
         } /* endswitch */
         ResourceString Format ( Library->QueryHandle(), IDS_HELPSUBITEMNOTFOUND ) ;
         char Message [200] ;
         sprintf ( Message, PCHAR(Format), Mode, Topic, Subtopic ) ;
         MessageBox ( PSZ(Title), Message, 0, MB_OK | MB_WARNING ) ;
         return ( MRFROMSHORT ( 0 ) ) ;
      } /* endmethod */

      MRESULT Refresh ( MPARAM1 mp1 ) {

         // Surface the window if so configured.
         if ( IniData.Float )
            Frame->Surface ( ) ;

         // Update the current idle counter.
         IniData.IdleCount = LONGFROMMP ( mp1 ) ;

         // Update the battery status.
         Battery *BatteryItem = (Battery*) IniData.Items[ITEM_BATTERY] ;
         if ( BatteryItem->QueryFlag() ) {
            if ( WasDischarging != BatteryItem->Discharging ( ) ) {
               if ( BatteryItem->Discharging ( ) ) {
                  if ( CounterTID ) Sys_SuspendThread ( CounterTID ) ;
                  IniData.IdleCount = 0xFFFFFFFF ;
               } else {
                  IniData.IdleCount = 0 ;
                  if ( CounterTID ) Sys_ResumeThread ( CounterTID ) ;
               } /* endif */
               WasDischarging = BatteryItem->Discharging ( ) ;
            } /* endif */
         } else {
            WasDischarging = BatteryItem->Discharging ( ) ;
         } /* endif */

         // Update the drive list.
         #ifdef __OS2__
            ULONG Drive, NewDrives ;
            DosQueryCurrentDisk ( &Drive, &NewDrives ) ;
         #else
            DWORD NewDrives ( GetLogicalDrives ( ) ) ;
         #endif
         Drives &= ( ~Exclude & ( IniData.ShowDiskDrives?0x03FFFFFF:0 ) ) ;
         NewDrives &= ( ~Exclude & ( IniData.ShowDiskDrives?0x03FFFFFF:0 ) ) ;
         if ( NewDrives != Drives ) {
            if ( UpdateDriveList ( Library->QueryHandle(), *IniFile,
               &IniData, pDdeServer, Drives, NewDrives, Drives, Exclude ) ) {
               if ( DialogWindow )
                  Sys_SendMessage ( DialogWindow, WM_UPDATEDRIVES, MPFROMP(&IniData), 0 ) ;
               ResizeWindow ( QueryHandle() ) ;
            } /* endif */
         } /* endif */

         // Update the display.
         UpdateWindow ( ) ;

         // Hourly Chime.
         time_t Time = time(0) ;
         struct tm Now ; memcpy ( &Now, localtime(&Time), sizeof(Now) ) ;
         struct tm Then ; memcpy ( &Then, localtime(&PreviousTime), sizeof(Now) ) ;
         if ( IniData.Chime AND ( Now.tm_hour != Then.tm_hour ) ) {
           if ( !WinAlarm ( HWND_DESKTOP, 11 ) ) {     // If no cuckoo clock, just beep.
              DosBeep ( 400, 100 ) ;
              Sys_Sleep ( 100 ) ;
              DosBeep ( 400, 100 ) ;
           } /* endif */
         } /* endif */

         // The date field may change size from day to day.  Adjust for this.
         if ( Now.tm_mday != Then.tm_mday ) 
            ResizeWindow ( QueryHandle() ) ;

         // Reset the last time we did all this.
         PreviousTime = Time ;

         // Done.
         return ( 0 ) ;
      } /* endmethod */

      MRESULT UpdateParms ( CONFIG_PARMS *Parms ) {

         // Save the new monitor priority. 
         IniData.fMonitorPriority = TRUE ;
         IniData.MonitorPriority = Priority ( Parms->MonitorPriority ) ;

         // Save the new timer interval.                                         
         IniData.fTimerInterval = TRUE ;
         IniData.TimerInterval = USHORT ( Parms->TimerInterval ) ;

         // Save the float-to-top flag.                                            
         IniData.fFloat = TRUE ;
         IniData.Float = Parms->Float ;

         // Save the window animate flag.
         IniData.fAnimate = TRUE ;
         IniData.Animate = Parms->Animate ;

         // Save the anchor corner flag.
         IniData.fAnchorCorner = TRUE ;
         IniData.AnchorCorner = Parms->AnchorCorner ;

         // Save the hourly chime flag.
         IniData.fChime = TRUE ;
         IniData.Chime = Parms->Chime ;

         // Save the hide controls flag, and adjust the window if it changed.
         IniData.fHideControls = TRUE ;
         if ( IniData.HideControls != Parms->HideControls ) {
            IniData.HideControls = Parms->HideControls ;
            #ifdef __OS2__
            if ( NOT ( IniData.Position.fl & SWP_MINIMIZE ) )
               Frame->HideControls ( IniData.HideControls ) ;
            #endif // __OS2__
         } /* endif */

         // If CPU load monitoring has changed, start/stop the monitoring thread.
         if ( Parms->ItemFlags[ITEM_CPULOAD] != IniData.Items[ITEM_CPULOAD]->QueryFlag() )
            if ( Parms->ItemFlags[ITEM_CPULOAD] ) {
               if ( CounterTID ) Sys_ResumeThread ( CounterTID ) ;
            } else { 
               if ( CounterTID ) Sys_SuspendThread ( CounterTID ) ;
            } /* endif */
      
         // Save the new averaging interval.                                         
         if ( IniData.AveragingInterval != Parms->AveragingInterval ) {
            IniData.fAveragingInterval = TRUE ;
            IniData.AveragingInterval = USHORT ( Parms->AveragingInterval ) ;
            ((AverageLoad*)IniData.Items[ITEM_AVERAGELOAD])->SetInterval ( Parms->AveragingInterval ) ;
         } /* endif */

         // Save new POP parameters.
         if ( strcmp ( IniData.PopServer, Parms->PopServer ) ) {
            memset ( IniData.PopServer, 0, sizeof(IniData.PopServer) ) ;
            strcpy ( IniData.PopServer, Parms->PopServer ) ;
            ((MessageCount*)IniData.Items[ITEM_MESSAGECOUNT])->SetServer ( Parms->PopServer ) ;
         } /* endif */
         if ( strcmp ( IniData.PopUserName, Parms->PopUserName ) ) {
            memset ( IniData.PopUserName, 0, sizeof(IniData.PopUserName) ) ;
            strcpy ( IniData.PopUserName, Parms->PopUserName ) ;
            ((MessageCount*)IniData.Items[ITEM_MESSAGECOUNT])->SetUserName ( Parms->PopUserName ) ;
         } /* endif */
         if ( strcmp ( IniData.PopPassword, Parms->PopPassword ) ) {
            memset ( IniData.PopPassword, 0, sizeof(IniData.PopPassword) ) ;
            strcpy ( IniData.PopPassword, Parms->PopPassword ) ;
            ((MessageCount*)IniData.Items[ITEM_MESSAGECOUNT])->SetPassword ( Parms->PopPassword ) ;
         } /* endif */
         if ( IniData.PopPort != Parms->PopPort ) {
            IniData.PopPort = Parms->PopPort ;
            ((MessageCount*)IniData.Items[ITEM_MESSAGECOUNT])->SetPort ( Parms->PopPort ) ;
         } /* endif */
         if ( IniData.PopForceIP != Parms->PopForceIP ) {
            IniData.PopForceIP = Parms->PopForceIP ;
            ((MessageCount*)IniData.Items[ITEM_MESSAGECOUNT])->SetForceIP ( Parms->PopForceIP ) ;
         } /* endif */
         if ( IniData.PopUseAPOP != Parms->PopUseAPOP ) {
            IniData.PopUseAPOP = Parms->PopUseAPOP ;
            ((MessageCount*)IniData.Items[ITEM_MESSAGECOUNT])->SetUseAPOP ( Parms->PopUseAPOP ) ;
         } /* endif */
         if ( IniData.PopInterval != Parms->PopInterval ) {
            IniData.PopInterval = Parms->PopInterval ;
            ((MessageCount*)IniData.Items[ITEM_MESSAGECOUNT])->SetInterval ( Parms->PopInterval ) ;
         } /* endif */
         if ( strcmp ( IniData.PopReader, Parms->PopReader ) ) {
            memset ( IniData.PopReader, 0, sizeof(IniData.PopReader) ) ;
            strcpy ( IniData.PopReader, Parms->PopReader ) ;
         } /* endif */
         if ( IniData.PopAlert != Parms->PopAlert ) {
            IniData.PopAlert = Parms->PopAlert ;
            ((MessageCount*)IniData.Items[ITEM_MESSAGECOUNT])->SetAlert ( Parms->PopAlert ) ;
         } /* endif */
         if ( strcmp ( IniData.PopWavFile, Parms->PopWavFile ) ) {
            memset ( IniData.PopWavFile, 0, sizeof(IniData.PopWavFile) ) ;
            strcpy ( IniData.PopWavFile, Parms->PopWavFile ) ;
            ((MessageCount*)IniData.Items[ITEM_MESSAGECOUNT])->SetWavFile ( Parms->PopWavFile ) ;
         } /* endif */

         // Determine if the display item list or its appearance has changed.
         //   If not, return.                                                    

         BOOL ItemsChanged = FALSE ;
         for ( int i=0; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            if ( Parms->ItemFlags[i] != pItem->QueryFlag() ) {
               ItemsChanged = TRUE ;
               break ;
            } /* endif */
            if ( strcmp ( Parms->CurrentLabels[i], PCHAR(pItem->QueryCurrentLabel()) ) ) {
               ItemsChanged = TRUE ;
               break ;
            } /* endif */
            if ( Parms->SenseEnabled[i] != pItem->QuerySenseEnabled() ) {
               ItemsChanged = TRUE ;
               break ;
            } /* endif */
            if ( Parms->WarningLevel[i] != pItem->QueryWarningLevel() ) {
               ItemsChanged = TRUE ;
               break ;
            } /* endif */
            if ( Parms->ErrorLevel[i] != pItem->QueryErrorLevel() ) {
               ItemsChanged = TRUE ;
               break ;
            } /* endif */
         } /* endfor */

         if ( NOT ItemsChanged
            AND ( Parms->NormalBackground == IniData.BackColor )
            AND ( Parms->NormalForeground == IniData.TextColor )
            AND ( Parms->WarningBackground == IniData.WarningBackground )
            AND ( Parms->WarningForeground == IniData.WarningForeground )
            AND ( Parms->ErrorBackground == IniData.ErrorBackground )
            AND ( Parms->ErrorForeground == IniData.ErrorForeground )
            AND !strcmp ( Parms->Font, IniData.FontNameSize )
            AND ( IniData.ShowFileSystemNames == Parms->ShowFileSystemNames )
            AND ( IniData.ShowDiskLabels == Parms->ShowDiskLabels )
            AND ( IniData.ShowSeconds == Parms->ShowSeconds )
            AND ( IniData.UseTheseus == Parms->UseTheseus )
            AND ( IniData.CenterClock == Parms->CenterClock ) 
            AND ( IniData.ShowK == Parms->ShowK )
            AND ( IniData.ShowTrueK == Parms->ShowTrueK )
            AND ( IniData.ShowM == Parms->ShowM )
            AND ( IniData.ShowG == Parms->ShowG )
            AND ( IniData.DialogBorder == Parms->DialogBorder )
            AND ( IniData.TableFormat == Parms->TableFormat )
            AND ( IniData.ShowDiskDrives == Parms->ShowDiskDrives )
            AND ( IniData.ShowRemoteDrives == Parms->ShowRemoteDrives )
            AND !strcmp ( IniData.DateFormat, Parms->DateFormat ) ) {
            SaveApplication ( ) ;
            return ( MRFROMSHORT ( 0 ) ) ;
         } /* endif */

         // Update the colors.                                                  
         IniData.BackColor = Parms->NormalBackground ;
         IniData.TextColor = Parms->NormalForeground ;
         IniData.WarningBackground = Parms->WarningBackground ;
         IniData.WarningForeground = Parms->WarningForeground ;
         IniData.ErrorBackground = Parms->ErrorBackground ;
         IniData.ErrorForeground = Parms->ErrorForeground ;
         strcpy ( IniData.FontNameSize, Parms->Font ) ;
         SetPresParam ( PP_FONTNAMESIZE, strlen(IniData.FontNameSize)+1, IniData.FontNameSize ) ;
         SetPresParam ( PP_BACKGROUNDCOLOR, sizeof(IniData.BackColor), &IniData.BackColor ) ;
         SetPresParam ( PP_FOREGROUNDCOLOR, sizeof(IniData.TextColor), &IniData.TextColor ) ;
      
         // Give all the items their levels and colors.                        
         for ( i=0; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            pItem->SetSenseEnabled  ( Parms->SenseEnabled[i] ) ;
            pItem->SetWarningLevel  ( Parms->WarningLevel[i] ) ;
            pItem->SetErrorLevel    ( Parms->ErrorLevel[i] ) ;
            pItem->SetNormalColors  ( IniData.BackColor, IniData.TextColor ) ;
            pItem->SetWarningColors ( IniData.WarningBackground, IniData.WarningForeground ) ;
            pItem->SetErrorColors   ( IniData.ErrorBackground, IniData.ErrorForeground ) ;
         } /* endfor */

         // Save the show file-system names flag.                                
         IniData.fShowFileSystemNames = TRUE ;
         IniData.ShowFileSystemNames = Parms->ShowFileSystemNames ;
         for ( i=ITEM_BASE_COUNT; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            ((DriveFree*)pItem)->SetShowFileSystemName ( IniData.ShowFileSystemNames ) ;
         } /* endfor */

         // Save the show disk labels flag.                                      
         IniData.fShowDiskLabels = TRUE ;
         IniData.ShowDiskLabels = Parms->ShowDiskLabels ;
         for ( i=ITEM_BASE_COUNT; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            ((DriveFree*)pItem)->SetShowDiskLabel ( IniData.ShowDiskLabels ) ;
         } /* endfor */

         // Save the show seconds flag.                                         
         IniData.fShowSeconds = TRUE ;
         IniData.ShowSeconds = Parms->ShowSeconds ;
         ((ElapsedTime*)IniData.Items[ITEM_ELAPSEDTIME])->SetShowSeconds ( IniData.ShowSeconds ) ;

         // Save the 'Use Theseus' flag.                                         
         IniData.fUseTheseus = TRUE ;
         IniData.UseTheseus = Parms->UseTheseus ;
         ((MemoryFree*)IniData.Items[ITEM_MEMORYFREE])->SetUseTheseus ( IniData.UseTheseus ) ;
         ((SwapSlack*)IniData.Items[ITEM_SWAPFILESLACK])->SetUseTheseus ( IniData.UseTheseus ) ;

         // Save the 'Center Clock' flag.                                         
         IniData.fCenterClock = TRUE ;
         IniData.CenterClock = Parms->CenterClock ;
         ((Clock*)IniData.Items[ITEM_CLOCK])->SetCenterClock ( IniData.CenterClock ) ;

         // Save the date format.
         strcpy ( IniData.DateFormat, Parms->DateFormat ) ;
         ((Clock*)IniData.Items[ITEM_CLOCK])->SetDateFormat ( IniData.DateFormat ) ;

         // Save the show 'K' flag.                                             
         IniData.fShowK = TRUE ;
         IniData.ShowK = Parms->ShowK ;
         ((MemoryFree *)IniData.Items[ITEM_MEMORYFREE   ])->SetShowK ( IniData.ShowK ) ;
         ((VMFree     *)IniData.Items[ITEM_VIRTUALFREE  ])->SetShowK ( IniData.ShowK ) ;
         ((SwapFree   *)IniData.Items[ITEM_SWAPDISKFREE ])->SetShowK ( IniData.ShowK ) ;
         ((SwapSize   *)IniData.Items[ITEM_SWAPFILESIZE ])->SetShowK ( IniData.ShowK ) ;
         ((SwapSlack  *)IniData.Items[ITEM_SWAPFILESLACK])->SetShowK ( IniData.ShowK ) ;
         ((SpoolSize  *)IniData.Items[ITEM_SPOOLFILESIZE])->SetShowK ( IniData.ShowK ) ;
         ((TotalFree  *)IniData.Items[ITEM_TOTALFREE    ])->SetShowK ( IniData.ShowK ) ;
         for ( i=ITEM_BASE_COUNT; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            ((DriveFree*)pItem)->SetShowK ( IniData.ShowK ) ;
         } /* endfor */

         // Save the show true 'K' flag.                                        
         IniData.fShowTrueK = TRUE ;
         IniData.ShowTrueK = Parms->ShowTrueK ;
         ((MemoryFree *)IniData.Items[ITEM_MEMORYFREE   ])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((VMFree     *)IniData.Items[ITEM_VIRTUALFREE  ])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((SwapFree   *)IniData.Items[ITEM_SWAPDISKFREE ])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((SwapSize   *)IniData.Items[ITEM_SWAPFILESIZE ])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((SwapSlack  *)IniData.Items[ITEM_SWAPFILESLACK])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((SpoolSize  *)IniData.Items[ITEM_SPOOLFILESIZE])->SetShowTrueK ( IniData.ShowTrueK ) ;
         ((TotalFree  *)IniData.Items[ITEM_TOTALFREE    ])->SetShowTrueK ( IniData.ShowTrueK ) ;
         for ( i=ITEM_BASE_COUNT; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            ((DriveFree*)pItem)->SetShowTrueK ( IniData.ShowTrueK ) ;
         } /* endfor */

         // Save the show 'M' flag.                                                
         IniData.fShowM = TRUE ;
         IniData.ShowM = Parms->ShowM ;
         ((MemoryFree *)IniData.Items[ITEM_MEMORYFREE   ])->SetShowM ( IniData.ShowM ) ;
         ((VMFree     *)IniData.Items[ITEM_VIRTUALFREE  ])->SetShowM ( IniData.ShowM ) ;
         ((SwapFree   *)IniData.Items[ITEM_SWAPDISKFREE ])->SetShowM ( IniData.ShowM ) ;
         ((SwapSize   *)IniData.Items[ITEM_SWAPFILESIZE ])->SetShowM ( IniData.ShowM ) ;
         ((SwapSlack  *)IniData.Items[ITEM_SWAPFILESLACK])->SetShowM ( IniData.ShowM ) ;
         ((SpoolSize  *)IniData.Items[ITEM_SPOOLFILESIZE])->SetShowM ( IniData.ShowM ) ;
         ((TotalFree  *)IniData.Items[ITEM_TOTALFREE    ])->SetShowM ( IniData.ShowM ) ;
         for ( i=ITEM_BASE_COUNT; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            ((DriveFree*)pItem)->SetShowM ( IniData.ShowM ) ;
         } /* endfor */

         // Save the show 'G' flag.                                                
         IniData.fShowG = TRUE ;
         IniData.ShowG = Parms->ShowG ;
         ((MemoryFree *)IniData.Items[ITEM_MEMORYFREE   ])->SetShowG ( IniData.ShowG ) ;
         ((VMFree     *)IniData.Items[ITEM_VIRTUALFREE  ])->SetShowG ( IniData.ShowG ) ;
         ((SwapFree   *)IniData.Items[ITEM_SWAPDISKFREE ])->SetShowG ( IniData.ShowG ) ;
         ((SwapSize   *)IniData.Items[ITEM_SWAPFILESIZE ])->SetShowG ( IniData.ShowG ) ;
         ((SwapSlack  *)IniData.Items[ITEM_SWAPFILESLACK])->SetShowG ( IniData.ShowG ) ;
         ((SpoolSize  *)IniData.Items[ITEM_SPOOLFILESIZE])->SetShowG ( IniData.ShowG ) ;
         ((TotalFree  *)IniData.Items[ITEM_TOTALFREE    ])->SetShowG ( IniData.ShowG ) ;
         for ( i=ITEM_BASE_COUNT; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            ((DriveFree*)pItem)->SetShowG ( IniData.ShowG ) ;
         } /* endfor */

         // Save the new item flags.                                              
         for ( i=0; i<IniData.ItemCount; i++ ) {
            Item *pItem = IniData.Items[i] ;
            if ( Parms->ItemFlags[i] )
               pItem->SetFlag ( ) ;
            else
               pItem->ResetFlag ( ) ;
            pItem->SetLabel ( Parms->CurrentLabels[i] ) ;
         } /* endif */

         // Save the dialog border flag.                                          
         if ( IniData.DialogBorder != Parms->DialogBorder ) {
            IniData.fDialogBorder = TRUE ;
            IniData.DialogBorder = Parms->DialogBorder ;
         } /* endif */

         // Save the table format flag.                                          
         IniData.fTableFormat = TRUE ;
         IniData.TableFormat = Parms->TableFormat ;

         // Save the 'Show Disk Drives' flag.
         if ( IniData.ShowDiskDrives != Parms->ShowDiskDrives ) {
            IniData.fShowDiskDrives = TRUE ;
            IniData.ShowDiskDrives = Parms->ShowDiskDrives ;
            Drives = 0x03FFFFFF ;
            Drives &= ( ~Exclude & ( IniData.ShowDiskDrives?0x03FFFFFF:0 ) ) ;
            if ( UpdateDriveList ( Library->QueryHandle(), *IniFile,
               &IniData, pDdeServer, Drives, Drives, Drives, Exclude ) )
               if ( DialogWindow )
                  Sys_SendMessage ( DialogWindow, WM_UPDATEDRIVES, MPFROMP(&IniData), 0 ) ;
         } /* endif */

         // Save the 'Show Remote Drives' flag.                                  
         if ( IniData.ShowRemoteDrives != Parms->ShowRemoteDrives ) {
            IniData.fShowRemoteDrives = TRUE ;
            IniData.ShowRemoteDrives = Parms->ShowRemoteDrives ;
            Drives = 0x03FFFFFF ;
            Drives &= ( ~Exclude & ( IniData.ShowDiskDrives?0x03FFFFFF:0 ) ) ;
            if ( UpdateDriveList ( Library->QueryHandle(), *IniFile,
               &IniData, pDdeServer, Drives, Drives, Drives, Exclude ) )
               if ( DialogWindow )
                  Sys_SendMessage ( DialogWindow, WM_UPDATEDRIVES, MPFROMP(&IniData), 0 ) ;
         } /* endif */

         // Resize the display window.
         ResizeWindow ( QueryHandle() ) ;
         SaveApplication ( ) ;
         return ( 0 ) ;
      } /* endmethod */

      #ifdef __OS2__
      MRESULT DdeInitiate ( HWND Window, PDDEINIT Parms ) {
         pDdeServer->Initiate ( QueryHandle(), Window, Parms ) ;
         return ( 0 ) ;
      } /* endmethod */
      #endif // __OS2__

      virtual MRESULT APIENTRY MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {
         if ( DebugMessages ) {
            LogMsgRcvd ( MessageIndent, "ClientWindow", msg, mp1, mp2 ) ;
            MessageIndent += 2 ;
         } /* endif */
         MRESULT Result ;
         switch ( msg ) {
            case WM_SIZE:                  Result = Moved_Sized ( hwnd, FALSE ) ;          break ;
            case WM_MOVE:                  Result = Moved_Sized ( hwnd, TRUE ) ;           break ;
            case WM_TIMER:                 Result = Timer ( SHORT1FROMMP(mp1) ) ;          break ;
            case WM_SAVEAPPLICATION:       Result = SaveApplication ( ) ;                  break ;
            case WM_PAINT:                 Result = Paint ( ) ;                            break ;
            case WM_INITMENU:              Result = InitMenu ( mp1, mp2 ) ;                break ;
            case WM_ACTIVATE:              Result = Activating ( SHORT1FROMMP(mp1) ) ;     break ;
            case WM_COMMAND:               Result = Command ( mp1, mp2 ) ;                 break ;
            case WM_BEGINSELECT:           Result = BeginDrag ( ) ;                        break ;
            case WM_BEGINDRAG:             Result = BeginDrag ( ) ;                        break ;
            case WM_BUTTON1DBLCLK:         Result = ButtonDoubleClick ( mp1, mp2 ) ;       break ;
            case WM_CONTEXTMENU:           Result = SummonContextMenu ( mp1 ) ;            break ;
            case WM_PRESPARAMCHANGED:      Result = PresParamChanged ( LONGFROMMP(mp1) ) ; break ;
            case WM_SYSCOLORCHANGE:        Result = SysColorChange ( mp1, mp2 ) ;          break ;
            case HM_QUERY_KEYS_HELP:       Result = (MRESULT) IDM_KEYS_HELP ;              break ;
            case HM_ERROR:                 Result = HelpError ( mp1, mp2 ) ;               break ;
            case HM_EXT_HELP_UNDEFINED:    Result = ExtHelpUndefined ( mp1, mp2 ) ;        break ;
            case HM_HELPSUBITEM_NOT_FOUND: Result = HelpSubitemNotFound ( mp1, mp2 ) ;     break ;
            case WM_REFRESH:               Result = Refresh ( mp1 ) ;                      break ;
            case WM_UPDATEPARMS:           Result = UpdateParms ( (CONFIG_PARMS*) mp1 ) ;  break ; 
            #ifdef __OS2__
            case WM_DDE_INITIATE:          Result = DdeInitiate ( HWNDFROMMP(mp1), PDDEINIT(PVOIDFROMMP(mp2)) ) ; break ;
            #endif // __OS2__
            default:                       Result = Window::MessageProcessor ( hwnd, msg, mp1, mp2 ) ;
         } /* endswitch */
         if ( DebugMessages ) {
            MessageIndent -= 2 ;
            LogMsgDone ( MessageIndent, "ClientWindow", msg, Result ) ;
         } /* endif */
         return ( Result ) ;
      } /* endmethod */

} ;

/****************************************************************************
 *                                                                          *
 *      Program Mainline                                                    *
 *                                                                          *
 ****************************************************************************/

extern int main ( int argc, char *argv[] ) {

  /**************************************************************************
   * Save the resource library handle for the exception handler to use.     *
   **************************************************************************/

   LibraryHandle = Library.QueryHandle() ;

  /**************************************************************************
   * Set up the exception handler.                                          *
   **************************************************************************/

   REGISTER_EXCEPTION_HANDLER();

  /**************************************************************************
   * Determine the home directory.                                          *
   **************************************************************************/

   char Drive[_MAX_DRIVE+1] ;
   char Dir[_MAX_DIR+1] ;
   char Fname[_MAX_FNAME+1] ;
   char Ext[_MAX_EXT+1] ;
   strupr ( argv[0] ) ;
   _fullpath ( HomePath, argv[0], sizeof(HomePath) ) ;
   _splitpath ( HomePath, Drive, Dir, Fname, Ext ) ;
   if ( Dir[strlen(Dir)-1] == '\\' )
      Dir[strlen(Dir)-1] = 0 ;
   strcpy ( HomePath, Drive ) ;
   strcat ( HomePath, Dir ) ;

  /**************************************************************************
   * Get the program title.                                                 *
   **************************************************************************/

   ResourceString Title ( Library.QueryHandle(), IDS_TITLE ) ;

  /**************************************************************************
   * Set the codepage.  Abort if unable to do so.                           *
   **************************************************************************/

   #ifdef __OS2__

   void *Offset ;
   DosGetResource ( Library.QueryHandle(), RT_RCDATA, 1, &Offset ) ;

   PUSHORT pCodePage = PUSHORT ( Offset ) ;
   USHORT DisplayCodePage = *pCodePage ;

   { /* startblock */
      Canvas canvas ( "main::canvas" ) ;
      while ( *pCodePage ) {
         if ( canvas.SetCodePage ( *pCodePage ) )
            DisplayCodePage = *pCodePage ;
         if ( !DosSetProcessCp ( *pCodePage ) )
            if ( WinSetCp ( Proc.QueryQueue(), *pCodePage ) )
               break ;
         pCodePage ++ ;
      } /* endwhile */
   } /* endblock */

   if ( *pCodePage == 0 ) {
      ResourceString Format ( Library.QueryHandle(), IDS_ERROR_BADCODEPAGE ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), *pCodePage ) ;
      Log ( "%s", Message ) ;
      Sys_MessageBox ( HWND_DESKTOP, HWND_DESKTOP, Message, PSZ(Title), 0, MB_OK | MB_ICONEXCLAMATION ) ;
      return ( 1 ) ;
   } /* endif */

   #endif // __OS2__

  /**************************************************************************
   * Decipher command-line parameters.                                      *
   **************************************************************************/

   BOOL Reset = FALSE ;
   ULONG ExcludeDrives = 0 ;

   ResourceString ResetCommand ( Library.QueryHandle(), IDS_PARMS_RESET ) ;
   ResourceString ExcludeCommand ( Library.QueryHandle(), IDS_PARMS_EXCLUDE ) ;

   while ( --argc ) {

      argv ++ ;

      if ( *argv[0] == '?' ) {
         ResourceString Message ( Library.QueryHandle(), IDS_PARAMETERLIST ) ;
         Sys_MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title), 0, MB_OK ) ;
         return ( 0 ) ;
      } /* endif */

      if ( !stricmp ( *argv, PCHAR(ResetCommand) ) ) {
         Reset = TRUE ;
         continue ;
      } /* endif */

      if ( !strnicmp ( *argv, PCHAR(ExcludeCommand), strlen(PCHAR(ExcludeCommand)) ) ) {
         char *p = *argv+strlen(PCHAR(ExcludeCommand)) ;
         while ( *p ) {
            ExcludeDrives |= 0x0001 << ( *p - 'A' ) ;
            p ++ ;
         } /* endfor */
         continue ;
      } /* endif */

      #if 0
         ResourceString Format ( Library.QueryHandle(), IDS_ERROR_INVALIDPARM ) ;
         char Message [200] ;
         sprintf ( Message, PCHAR(Format), *argv ) ;
         Sys_MessageBox ( HWND_DESKTOP, HWND_DESKTOP, Message, PSZ(Title), 0, MB_OK | MB_ICONEXCLAMATION ) ;
         return ( 2 ) ;
      #endif

   } /* endwhile */

  /**************************************************************************
   * Create the help instance.                                              *
   **************************************************************************/

   ResourceString HelpTitle ( Library.QueryHandle(), IDS_HELPTITLE ) ;

   HelpWindow Help ( Proc.QueryAnchor(), 0,
      ID_MAIN, PSZ(PROGRAM_NAME ".hlp"), PSZ(HelpTitle) ) ;

   if ( Help.QueryHandle() == 0 ) {
      ERRORID Error = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      ResourceString Format ( Library.QueryHandle(), IDS_ERROR_CREATEHELP ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), Error ) ;
      Log ( "%s", Message ) ;
      Sys_MessageBox ( HWND_DESKTOP, HWND_DESKTOP, Message, PSZ(Title), 0, MB_OK | MB_ICONEXCLAMATION ) ;
   } /* endif */

  /**************************************************************************
   * Open/create the profile file.  Reset if requested.                     *
   **************************************************************************/

   Profile2 IniFile ( PROGRAM_NAME, Proc.QueryAnchor(), Library.QueryHandle(), IDD_PROFILE_PATH, &Help, Reset ) ;
   if ( !IniFile.IsReady() ) {
      ResourceString Message ( Library.QueryHandle(), IDS_ERROR_PRFOPENPROFILE ) ;
      Log ( "%s", PSZ(Message) ) ;
      Sys_MessageBox ( HWND_DESKTOP, HWND_DESKTOP, PSZ(Message), PSZ(Title), 0, MB_OK | MB_ICONEXCLAMATION ) ;
      return ( 3 ) ;
   } /* endif */

  /**************************************************************************
   * Read the profile to find out if we're to animate the frame window.     *
   **************************************************************************/

   INIDATA IniData ;

   // If we're looking at an ancient INI (before MemSize 2.00) . . .
   Profile2 OldIniFile ( "MEMSIZE" ) ;
   if ( !OldIniFile.GetIniData ( &IniData ) ) {

      // Save the INI information in a private INI.
      IniFile.Copy ( OldIniFile ) ;

      // Destroy the old INI information.
      OldIniFile.Reset ( ) ;

   } else {

      // See if we have an INI from MemSize 2.x or 3.x.
      Profile2 OldIniFile ( "MEMSIZE", Proc.QueryAnchor(), HomePath, TRUE ) ;

      // If we do, grab the information from it.
      if ( OldIniFile.IsReady() && !OldIniFile.GetIniData ( &IniData ) ) {

         // Save the INI information in a private INI.
         IniFile.Copy ( OldIniFile ) ;

         // Rename the old profile so it doesn't interfere anymore.
         OldIniFile.PhaseOut ( ) ;

      // Otherwise just get the current information from the new-style INI.
      } else {

         // Get the new-style INI information.
         IniFile.GetIniData ( &IniData ) ;

      } /* endif */

   } /* endif */

  /**************************************************************************
   * Declare the frame window class.                                        *
   **************************************************************************/

   WindowClass FrameClass ( WC_FRAME ) ;

  /**************************************************************************
   * Declare the frame window.                                              *
   **************************************************************************/

   FRAMECDATA FrameControlData ;
   memset ( &FrameControlData, 0, sizeof(FrameControlData) ) ;
   FrameControlData.cb = sizeof(FrameControlData) ;
   FrameControlData.flCreateFlags = FCF_TITLEBAR | FCF_SYSMENU | FCF_ICON | FCF_MINBUTTON | FCF_NOBYTEALIGN | FCF_ACCELTABLE ;
   FrameControlData.idResources = ID_MAIN ;

   Window Frame ( FrameClass, HWND_DESKTOP, PSZ(Title),
      ( IniData.fAnimate AND IniData.Animate ? WS_ANIMATE : 0 ) | ( IniData.DialogBorder ? FS_DLGBORDER : FS_BORDER ),
      0, 0, 0, 0, HWND_DESKTOP, HWND_TOP, ID_MAIN, &FrameControlData ) ;
   if ( Frame.QueryHandle() == 0 ) {
      ERRORID Error = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      ResourceString Format ( Library.QueryHandle(), IDS_ERROR_CREATEFRAME ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), Error ) ;
      Log ( "%s", Message ) ;
      Sys_MessageBox ( HWND_DESKTOP, HWND_DESKTOP, Message, PSZ(Title), 0, MB_OK | MB_ICONEXCLAMATION ) ;
      return ( 4 ) ;
   } /* endif */

  /**************************************************************************
   * Associate the help instance with the frame window.                     *
   **************************************************************************/

   Help.Associate ( Frame.QueryHandle() ) ;

  /**************************************************************************
   * Declare the client window class.                                       *
   **************************************************************************/

   WindowClass ClientClass ( Proc, CS_MOVENOTIFY ) ;
   if ( !ClientClass.IsRegistered() ) {
      ERRORID Error = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      ResourceString Format ( Library.QueryHandle(), IDS_ERROR_WINREGISTERCLASS ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), CLASS_NAME, Error ) ;
      Log ( "%s", Message ) ;
      Sys_MessageBox ( HWND_DESKTOP, HWND_DESKTOP, Message, PSZ(Title), 0, MB_OK | MB_ICONEXCLAMATION ) ;
      return ( 5 ) ;
   } /* endif */

  /**************************************************************************
   * Create the client window.                                              *
   **************************************************************************/

   ClientWindow Client ( ClientClass, Frame, Library, IniFile, DisplayCodePage, ExcludeDrives ) ;
   if ( Client.QueryHandle() == 0 ) {
      ERRORID Error = Sys_GetLastError ( Proc.QueryAnchor() ) ;
      ResourceString Format ( Library.QueryHandle(), IDS_ERROR_CREATECLIENT ) ;
      char Message [200] ;
      sprintf ( Message, PCHAR(Format), Error ) ;
      Log ( "%s", Message ) ;
      Sys_MessageBox ( HWND_DESKTOP, HWND_DESKTOP, Message, PSZ(Title), 0, MB_OK | MB_ICONEXCLAMATION ) ;
      return ( 6 ) ;
   } /* endif */

   #ifdef DEBUG
      Client.Set_DebugMessages ( TRUE ) ;
   #endif // DEBUG

  /**************************************************************************
   * Wait for and process messages to the window's queue.  Terminate        *
   *   when the WM_QUIT message is received.                                *
   **************************************************************************/

   Proc.Execute ( ) ;

  /**************************************************************************
   * Release the exception handler.                                         *
   **************************************************************************/

   UNREGISTER_EXCEPTION_HANDLER();

  /**************************************************************************
   * Discard all that was requested of the system and terminate.            *
   **************************************************************************/

   return ( 0 ) ;
}


/****************************************************************************
 *                                                                          *
 *      Scan CONFIG.SYS for a keyword.  Return the value.                   *
 *                                                                          *
 ****************************************************************************/

extern PSZ ScanSystemConfig ( ANCHOR Anchor, PSZ Keyword ) {

  /**************************************************************************
   * Get the boot drive number from the global information segment.         *
   **************************************************************************/

   ULONG BootDrive ( 3 ) ; // Under Win32 this can only be C:.
   #ifdef __OS2__
      DosQuerySysInfo ( QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &BootDrive, sizeof(BootDrive) ) ;
   #endif // __OS2__

  /**************************************************************************
   * Convert the keyword to upper case.                                     *
   **************************************************************************/

   #ifdef __OS2__
      WinUpper ( Anchor, 0, 0, Keyword ) ;
   #else // __NT__
      Anchor = 0 ;  // Pacify the compiler.
      CharUpper ( Keyword ) ;
   #endif // __OS2__ vs __NT__

  /**************************************************************************
   * Build the CONFIG.SYS path.                                             *
   **************************************************************************/

   char Path [_MAX_PATH] ;
   Path[0] = (char) ( BootDrive + 'A' - 1 ) ;
   Path[1] = 0 ;
   strcat ( Path, ":\\CONFIG.SYS" ) ;

  /**************************************************************************
   * Open CONFIG.SYS for reading.                                           *
   **************************************************************************/

   FILE *File = fopen ( Path, "r" ) ;
   if ( NOT File )
      return ( 0 ) ;

  /**************************************************************************
   * While there're more lines in CONFIG.SYS, read a line and check it.     *
   **************************************************************************/

   static char Buffer [500] ;
   while ( fgets ( Buffer, sizeof(Buffer), File ) ) {

     /***********************************************************************
      * Clean any trailing newline character from the input string.         *
      ***********************************************************************/

      if ( Buffer[strlen(Buffer)-1] == '\n' )
         Buffer[strlen(Buffer)-1] = 0 ;

     /***********************************************************************
      * If keyword starts the line, we've found the line we want.  Close    *
      *   the file and return a pointer to the parameter text.              *
      ***********************************************************************/

      #ifdef __OS2__
         WinUpper ( Anchor, 0, 0, PSZ(Buffer) ) ;
      #else // __NT__
         CharUpper ( Buffer ) ;
      #endif // __OS2__ vs __NT__

      char *p = Buffer ;
      while ( *p AND ( ( *p == ' ' ) OR ( *p == '\t' ) ) ) p++ ;

      if ( strncmp ( p, PCHAR(Keyword), strlen(PCHAR(Keyword)) ) )
         continue ;

      p += strlen(PCHAR(Keyword)) ;
      while ( *p AND ( ( *p == ' ' ) OR ( *p == '\t' ) ) ) p++ ;

      if ( *p++ != '=' )
         continue ;

      while ( *p AND ( ( *p == ' ' ) OR ( *p == '\t' ) ) ) p++ ;

      fclose ( File ) ;
      return ( PSZ(p) ) ;

   } /* endwhile */

  /**************************************************************************
   * Close the file.  We haven't found the line we wanted.                  *
   **************************************************************************/

   fclose ( File ) ;

   return ( 0 ) ;
}

/****************************************************************************
 *                                                                          *
 *                          Copy Quoted String                              *
 *                                                                          *
 ****************************************************************************/

STATIC char *CopyString ( char *Buffer, char *Original ) {
   char *p1 = PCHAR(Original), *p2 = Buffer ;
   BOOL Quoted = FALSE ;
   *p2 = 0 ;
   while ( *p1 ) {
      if ( Quoted ) {
         if ( *p1 == '"' ) {
            Quoted = FALSE ;
         } else {
            *p2++ = *p1 ;
            *p2 = 0 ;
         } /* endif */
      } else {
         if ( *p1 == '"' ) {
            Quoted = TRUE ;
         } else if ( ( *p1 == ' ' ) OR ( *p1 == '\t' ) ) {
            break ;
         } else {
            *p2++ = *p1 ;
            *p2 = 0 ;
         } /* endif */
      } /* endif */
      p1 ++ ;
   } /* endwhile */
   return ( p1 ) ;
}

/****************************************************************************
 *                                                                          *
 *    Monitor Loop Thread                                                   *
 *                                                                          *
 ****************************************************************************/

STATIC void MonitorLoopThread ( void *Parameter ) {

  /**************************************************************************
   * Get the thread parameter.                                              *
   **************************************************************************/

   PMONITOR_PARMS Parms = PMONITOR_PARMS ( Parameter ) ;

  /**************************************************************************
   * Set this thread's priority.                                            *
   **************************************************************************/

   Thread_SetPriority ( 0, *Parms->Priority ) ;

  /**************************************************************************
   * Start up the high resolution timer, if it is available.                *
   **************************************************************************/

   #ifdef __OS2__
      BOOL HiResTimer = OpenTimer ( ) ;
   #endif // __OS2__

  /**************************************************************************
   * Loop while active . . .                                                *
   **************************************************************************/

   while ( Parms->Active ) {

     /***********************************************************************
      * Reset the last time and count seen.                                 *
      ***********************************************************************/

      ULONG LastMilliseconds ;

      #ifdef __OS2__
         TIMESTAMP Time [2] ;
         if ( HiResTimer )
            GetTime ( &Time[0] ) ;
         else
            DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &LastMilliseconds, sizeof(LastMilliseconds) ) ;
      #else
         LastMilliseconds = GetTickCount ( ) ;
      #endif

      ULONG LastCounter = *Parms->Counter ;

     /***********************************************************************
      * Let the counter count.                                              *
      ***********************************************************************/

      Sys_Sleep ( *Parms->Interval ) ;

     /***********************************************************************
      * Find out how much time and counts went by.                          *
      ***********************************************************************/

      ULONG CurrentCounter = *Parms->Counter ;

      ULONG DeltaMilliseconds ;

      #ifdef __OS2__
         if ( HiResTimer ) {
            GetTime ( &Time[1] ) ;
            ULONG Nanoseconds ;
            DeltaMilliseconds = ComputeElapsedTime ( &Time[0], &Time[1], &Nanoseconds ) ;
            if ( Nanoseconds >= 500000 )
               DeltaMilliseconds ++ ;
         } else {
            ULONG Milliseconds ;
            DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &Milliseconds, sizeof(Milliseconds) ) ;
            DeltaMilliseconds = Milliseconds - LastMilliseconds ;
         } /* endif */
      #else
         DeltaMilliseconds = GetTickCount() - LastMilliseconds ;
      #endif

     /***********************************************************************
      * Find out how much idle time was counted.  Adjust it to persecond.   *
      *   Tell the owner window to refresh its statistics.                  *
      ***********************************************************************/

      if ( DeltaMilliseconds ) {
         ULONG Counter = (ULONG) ( ( (double)(CurrentCounter-LastCounter) * 1000 ) / (double)DeltaMilliseconds ) ;
         Sys_PostMessage ( Parms->Owner, WM_REFRESH, MP1FROMLONG(Counter), 0 ) ;
      } /* endif */

   } /* endwhile */

   MonitorThreadEvent.Post ( ) ;
}

/****************************************************************************
 *                                                                          *
 *      Update the Item List to reflect changes in the available drives.    *
 *                                                                          *
 ****************************************************************************/

STATIC BOOL UpdateDriveList ( HMODULE Library, Profile2 &Prf,
   PINIDATA IniData, Dde_Server *pDdeServer, ULONG OldDrives, ULONG NewDrives,
   ULONG &ResultDrives, ULONG &Exclude ) {

   #ifdef DEBUG
      Log ( "UpdateDriveList() Started.  OldDrives %08X, NewDrives %08X, Exclude %08X.", OldDrives, NewDrives, Exclude ) ;
   #endif // DEBUG

  /**************************************************************************
   * Get format strings.                                                    *
   **************************************************************************/

   ResourceString LabelFormat ( Library, IDS_DRIVE_FREE ) ;

  /**************************************************************************
   * Save the old item list for comparison.                                 *
   **************************************************************************/

   Item *OldItems [ ITEM_BASE_COUNT + MAX_DRIVES ] ;

   memset ( OldItems, 0, sizeof(OldItems) ) ;

   USHORT OldCount = 0 ;
   if ( OldDrives ) {
      OldCount = USHORT ( IniData->ItemCount ) ;
      memcpy ( OldItems, IniData->Items, sizeof(OldItems) ) ;
   } /* endif */

  /**************************************************************************
   * Add items for each drive on the system.                                *
   **************************************************************************/

   USHORT Count = ITEM_BASE_COUNT ;
   USHORT OldIndex = ITEM_BASE_COUNT ;

   ULONG LocalDrives = ( (TotalFree*) IniData->Items [ ITEM_TOTALFREE ] ) -> QueryMask ( ) ;
   ResultDrives = NewDrives ;
   NewDrives >>= 2 ;
   OldDrives >>= 2 ;

   BOOL Changed = FALSE ;

   for ( int Drive=3; Drive<=MAX_DRIVES; Drive++ ) {

      #ifdef DEBUG
         Log ( "Checking drive %c", Drive+'A'-1 ) ;
      #endif // DEBUG

      while ( ( OldIndex < OldCount )
         AND ( (SHORT)OldItems[OldIndex]->QueryId() < ITEM_BASE_COUNT + Drive ) ) {
         OldIndex ++ ;
         #ifdef DEBUG
            if ( OldIndex < OldCount ) {
               Log ( "OldIndex -> Drive %c", OldItems[OldIndex]->QueryId()-ITEM_BASE_COUNT+'A'-1 ) ;
            } else {
               Log ( "OldIndex -> EOL" ) ;
            } /* endif */
         #endif // DEBUG
      } /* endwhile */

      // If this drive exists currently . . .
      if ( NewDrives & 1 ) {

         // If this drive was in the old list . . .
         if ( OldDrives & 1 ) {

            // If we're no longer showing remote drives, and this drive is remote . . .
            if ( !IniData->ShowRemoteDrives && !(LocalDrives&(1<<(Drive-1))) ) {
               // If the drive is in the list, remove it.
               if ( ( OldIndex < OldCount ) AND ( (SHORT)OldItems[OldIndex]->QueryId() == ITEM_BASE_COUNT + Drive ) ) {
                  #ifdef DEBUG
                     Log ( "Drive %c removed from list because no longer allowing remote drives.", Drive+'A'-1 ) ;
                  #endif // DEBUG
                  ResultDrives &= ~( 1 << (Drive-1) ) ;
                  OldItems[OldIndex]->PutProfile ( Prf ) ;
                  delete OldItems[OldIndex++] ;
                  Changed = TRUE ;
               } /* endif */

            // Else, if we're now showing remote drives, and the drive is remote . . .
            } else if ( IniData->ShowRemoteDrives && !(LocalDrives&(1<<(Drive-1))) ) {
               // If the drive is not in the list, add it.
               if ( ( OldIndex >= OldCount ) OR ( (SHORT)OldItems[OldIndex]->QueryId() != ITEM_BASE_COUNT + Drive ) ) {
                  char FileSystem [80] = { 0 } ;
                  char DiskLabel [80] = { 0 } ;
                  int DriveType = CheckDrive ( Drive, FileSystem, sizeof(FileSystem), DiskLabel, sizeof(DiskLabel) ) ;
                  if ( ( DriveType > 0 ) OR ( ( DriveType < 0 ) AND IniData->ShowRemoteDrives ) ) {

                     if ( DriveType > 0 ) // OR IniData->TotalRemoteDrives 
                        LocalDrives |= ( 1 << (Drive-1) ) ;

                     #ifdef DEBUG
                        Log ( "Drive %c restored to list because now allowing remote drives.", Drive+'A'-1 ) ;
                     #endif // DEBUG

                     char Name [80] ;
                     sprintf ( Name,   "ShowDrive%c:", Drive+'A'-1 ) ;

                     char Label [80] ;
                     sprintf ( Label,  PCHAR(LabelFormat),  Drive+'A'-1 ) ;

                     IniData->Items[Count] = new DriveFree ( USHORT(ITEM_BASE_COUNT+Drive),
                        Name, Label, Label,  pDdeServer, "Items",
                        IniData->ShowK, IniData->ShowTrueK, IniData->ShowM, IniData->ShowG,
                        USHORT(Drive), IniData->DriveError,
                        IniData->ShowFileSystemNames, FileSystem,
                        IniData->ShowDiskLabels, DiskLabel ) ;
                     IniData->Items[Count++]->GetProfile ( Prf ) ;
                     Changed = TRUE ;
                  } else {
                     #ifdef DEBUG
                        Log ( "Drive %c not added to list because %s.", Drive+'A'-1,
                           DriveType==0?"drive is not fixed":(DriveType<0?"drive is remote":"") ) ;
                     #endif // DEBUG
                     ResultDrives &= ~( 1 << (Drive-1) ) ;
                     if ( !strcmpi(FileSystem,"CDFS") || !strcmpi(FileSystem,"REMV") )
                        Exclude |= ( 1 << (Drive-1) ) ;
                  } /* endif */
               // Else if it was in the list, preserve it.
               } else {
                  #ifdef DEBUG
                     Log ( "Drive %c preserved in list.", Drive+'A'-1 ) ;
                  #endif // DEBUG
                  IniData->Items[Count++] = OldItems[OldIndex++] ;
               } /* endif */

            // Else, preserve the drive in the list, if it is there.
            } else {
               if ( ( OldIndex < OldCount ) AND ( (SHORT)OldItems[OldIndex]->QueryId() == ITEM_BASE_COUNT + Drive ) ) {
                  #ifdef DEBUG
                     Log ( "Drive %c preserved in list.", Drive+'A'-1 ) ;
                  #endif // DEBUG
                  IniData->Items[Count++] = OldItems[OldIndex++] ;
               } /* endif */

            } /* endif */

         // Else, if this drive didn't exist last time around . . .
         } else {
            char FileSystem [80] = { 0 } ;
            char DiskLabel [80] = { 0 } ;
            int DriveType = CheckDrive ( Drive, FileSystem, sizeof(FileSystem), DiskLabel, sizeof(DiskLabel) ) ;
            if ( ( DriveType > 0 ) OR ( ( DriveType < 0 ) AND IniData->ShowRemoteDrives ) ) {

               if ( DriveType > 0 ) // OR IniData->TotalRemoteDrives 
                  LocalDrives |= ( 1 << (Drive-1) ) ;

               #ifdef DEBUG
                  Log ( "Drive %c added to list.", Drive+'A'-1 ) ;
               #endif // DEBUG

               char Name [80] ;
               sprintf ( Name,   "ShowDrive%c:", Drive+'A'-1 ) ;

               char Label [80] ;
               sprintf ( Label,  PCHAR(LabelFormat),  Drive+'A'-1 ) ;

               IniData->Items[Count] = new DriveFree ( USHORT(ITEM_BASE_COUNT+Drive),
                  Name, Label, Label,  pDdeServer, "Items",
                  IniData->ShowK, IniData->ShowTrueK, IniData->ShowM, IniData->ShowG,
                  USHORT(Drive), IniData->DriveError,
                  IniData->ShowFileSystemNames, FileSystem,
                  IniData->ShowDiskLabels, DiskLabel ) ;
               IniData->Items[Count++]->GetProfile ( Prf ) ;
               Changed = TRUE ;
            } else {
               #ifdef DEBUG
                  Log ( "Drive %c not added to list because %s.", Drive+'A'-1,
                     DriveType==0?"drive is not fixed":(DriveType<0?"drive is remote":"") ) ;
               #endif // DEBUG
               ResultDrives &= ~( 1 << (Drive-1) ) ;
               if ( !strcmpi(FileSystem,"CDFS") || !strcmpi(FileSystem,"REMV") )
                  Exclude |= ( 1 << (Drive-1) ) ;
            } /* endif */

         } /* endif */

      // Else, this drive no longer exists.
      } else {
         LocalDrives &= ~ ( 1 << (Drive-1) ) ;
         if ( OldDrives & 1 ) {
            if ( ( OldIndex < OldCount ) && ( OldItems[OldIndex]->QueryId() == ITEM_BASE_COUNT + Drive ) ) {
               #ifdef DEBUG
                  Log ( "Drive %c removed from list because it has disappeared.", Drive+'A'-1 ) ;
               #endif // DEBUG
               OldItems[OldIndex]->PutProfile ( Prf ) ;
               delete OldItems[OldIndex++] ;
               Changed = TRUE ;
            } else if ( OldIndex >= OldCount ) {
               #ifdef DEBUG
                  Log ( "Drive %c not removed from list because it was not there (short list).", Drive+'A'-1 ) ;
               #endif // DEBUG
            } else if ( OldItems[OldIndex]->QueryId() != ITEM_BASE_COUNT + Drive ) {
               #ifdef DEBUG
                  Log ( "Drive %c not removed from list because it was not there (missing entry).", Drive+'A'-1 ) ;
               #endif // DEBUG
            } /* endif */
         } else {
            // Do nothing.
         } /* endif */

      } /* endif */

      NewDrives >>= 1 ;
      OldDrives >>= 1 ;

   } /* endfor */

  /**************************************************************************
   * Save the new item count.                                               *
   **************************************************************************/

   IniData->ItemCount = Count ;

  /**************************************************************************
   * Update the total free space object.                                    *
   **************************************************************************/

   ( (TotalFree*) IniData->Items [ ITEM_TOTALFREE ] ) -> ResetMask ( LocalDrives ) ;

  /**************************************************************************
   * Give everyone their colors.                                            *
   **************************************************************************/

   for ( int i=0; i<IniData->ItemCount; i++ ) {
      Item *pItem = IniData->Items[i] ;
      pItem->SetWarningLevel  ( pItem->QueryWarningLevel() ) ;
      pItem->SetErrorLevel    ( pItem->QueryErrorLevel() ) ;
      pItem->SetNormalColors  ( IniData->BackColor, IniData->TextColor ) ;
      pItem->SetWarningColors ( IniData->WarningBackground, IniData->WarningForeground ) ;
      pItem->SetErrorColors   ( IniData->ErrorBackground, IniData->ErrorForeground ) ;
   } /* endfor */

  /**************************************************************************
   * Return final status: Did the list really change?                       *
   **************************************************************************/

   #ifdef DEBUG
      Log ( "UpdateDriveList() Done.  Exclude %08X, ResultDrives %08X.", Exclude, ResultDrives ) ;
   #endif // DEBUG

   return ( Changed ) ;
}

/****************************************************************************
 *                                                                          *
 *                       Calibrate the Load Meter                           *
 *                                                                          *
 ****************************************************************************/

STATIC ULONG CalibrateLoadMeter ( PCOUNTER_PARMS Parms ) {

   #ifdef __OS2__

  /**************************************************************************
   * Set result to zero as a default.                                       *
   **************************************************************************/

   double AdjustedMaxLoad = 0.0 ;

  /**************************************************************************
   * If HRTIMER.SYS has been installed . . .                                *
   **************************************************************************/

   if ( OpenTimer ( ) ) {

     /***********************************************************************
      * Increase this thread's priority to the maximum.                     *
      ***********************************************************************/

      Thread_SetPriority ( 0, PRIORITY_TIME_CRITICAL ) ;

     /***********************************************************************
      * Create the calibration thread and set its priority next highest.    *
      ***********************************************************************/

      CounterThreadEvent.Reset ( ) ;
      Parms->Active = TRUE ;
      TID tidCalibrate = StartThread ( "CounterThread", CounterThread, 0x3000, Parms ) ;
      Sys_SuspendThread ( tidCalibrate ) ;
      Thread_SetPriority ( tidCalibrate, Priority(PRIORITY_TIME_CRITICAL-1) ) ;

     /***********************************************************************
      * Reset the calibration count, get the time, and let the counter go.  *
      ***********************************************************************/

      Parms->Counter = 0 ;
      TIMESTAMP Time[2] ;
      GetTime ( &Time[0] ) ;
      Sys_ResumeThread ( tidCalibrate ) ;

     /***********************************************************************
      * Sleep for one second.                                               *
      ***********************************************************************/

      Sys_Sleep ( 1000 ) ;

     /***********************************************************************
      * Suspend the calibration counter and get the time.                   *
      ***********************************************************************/

      Parms->Active = FALSE ;
      CounterThreadEvent.Wait ( 10000 ) ;
      GetTime ( &Time[1] ) ;

     /***********************************************************************
      * Return priorities to normal.                                        *
      ***********************************************************************/

      DosSetPriority ( PRTYS_THREAD, PRTYC_REGULAR, 0, 0 ) ;

     /***********************************************************************
      * Get the elapsed time and adjust the calibration count.              *
      ***********************************************************************/

      ULONG Milliseconds ;
      ULONG Nanoseconds ;
      Milliseconds = ComputeElapsedTime ( &Time[0], &Time[1], &Nanoseconds ) ;

      AdjustedMaxLoad = (double)Parms->Counter * 1.0E9 ;
      AdjustedMaxLoad /= (double)Milliseconds*1.0E6L + (double)Nanoseconds ;

     /***********************************************************************
      * Close down the connection to HRTIMER.SYS.                           *
      ***********************************************************************/

      CloseTimer ( ) ;

   } /* endif */

  /**************************************************************************
   * Return the adjusted calibration count.  If HRTIMER was not there, it   *
   *   will be zero.                                                        *
   **************************************************************************/

   return ( (ULONG)AdjustedMaxLoad ) ;

   #else // __NT__

   Parms = 0 ; // Pacify the compiler.
   return ( 0 ) ; // HRTIMER is -never- here under Win32.

   #endif // __OS2__ vs __NT__

}

/****************************************************************************
 *                                                                          *
 *                    General Purpose Counter Thread                        *
 *                                                                          *
 ****************************************************************************/

STATIC void CounterThread ( void *Parameter ) {

  /*************************************************************************
   * Count like mad.                                                       *
   *************************************************************************/

   PCOUNTER_PARMS Parms = PCOUNTER_PARMS ( Parameter ) ;
   while ( Parms->Active )
      Parms->Counter ++ ;

   CounterThreadEvent.Post ( ) ;
}

