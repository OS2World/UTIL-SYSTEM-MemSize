/****************************************************************** INIDATA.H
 *                                                                          *
 *                          Profile Data Object                             *
 *                                                                          *
 ****************************************************************************/

#ifndef INIDATA_H
#define INIDATA_H

#include "Items.h"
#include "ReString.h"
#include "Thread.h"

#define MAX_DRIVES (26)

enum {
   ITEM_CLOCK,
   ITEM_ELAPSEDTIME,
   ITEM_MESSAGECOUNT,
   ITEM_MEMORYFREE,
   ITEM_VIRTUALFREE,
   ITEM_SWAPFILESIZE,
   ITEM_SWAPDISKFREE,
   ITEM_SWAPFILESLACK,
   ITEM_SPOOLFILESIZE,
   ITEM_CPULOAD,
   ITEM_AVERAGELOAD,
   ITEM_BATTERY,
   ITEM_TASKCOUNT,
   ITEM_PROCESSCOUNT,
   ITEM_THREADCOUNT,
   ITEM_TOTALFREE,
   ITEM_BASE_COUNT
} ;

typedef struct _IniData {      // Parameters saved to system.

   // The Display Item List - - -
   USHORT ItemCount ;
   Item *Items [ ITEM_BASE_COUNT + MAX_DRIVES ] ;

   // Data required for the display item objects to function.
   ULONG           MemoryFree ;
   time_t          MemoryTime ;
   ULONG           IdleCount ;
   ULONG           MaxCount ;
   BYTE            SwapPath [_MAX_PATH] ;
   ULONG           MinFree ;
   ULONG           InitialSwap ;
   PSZ             SpoolPath ;
   ResourceString *Day ;
   ResourceString *Days ;
   ResourceString *DaysOfWeek ;
   ResourceString *DriveError ;
   ResourceString *BatteryError ;
   ResourceString *BatteryAC ;
   ResourceString *BatteryCharging ;

   // Window size and location
   #ifdef __OS2__
      SWP    Position ;
   #else
      WINDOWPOS Position ;
   #endif
   BOOL   fPosition ;

   // User Options
   BOOL   HideControls ;
   BOOL   fHideControls ;

   BOOL   Float ;
   BOOL   fFloat ;

   BOOL   Animate ;
   BOOL   fAnimate ;

   BOOL   DialogBorder ;
   BOOL   fDialogBorder ;

   BOOL   TableFormat ;
   BOOL   fTableFormat ;

   BOOL   Chime ;
   BOOL   fChime ;

   BOOL   ShowDiskDrives ;
   BOOL   fShowDiskDrives ;

   BOOL   ShowRemoteDrives ;
   BOOL   fShowRemoteDrives ;

   BOOL   ShowFileSystemNames ;
   BOOL   fShowFileSystemNames ;

   BOOL   ShowDiskLabels ;
   BOOL   fShowDiskLabels ;

   BOOL   ShowSeconds ;
   BOOL   fShowSeconds ;

   BOOL   UseTheseus ;
   BOOL   fUseTheseus ;

   BOOL   CenterClock ;
   BOOL   fCenterClock ;

   USHORT ShowK ;
   BOOL   fShowK ;

   USHORT ShowTrueK ;
   BOOL   fShowTrueK ;

   USHORT ShowM ;
   BOOL   fShowM ;

   USHORT ShowG ;
   BOOL   fShowG ;

   Priority MonitorPriority ;
   BOOL   fMonitorPriority ;

   USHORT TimerInterval ;
   BOOL   fTimerInterval ;

   USHORT AveragingInterval ;
   BOOL   fAveragingInterval ;

   USHORT AnchorCorner ;
   BOOL   fAnchorCorner ;

   char   DateFormat [80] ;

   char   PopServer [80] ;
   char   PopUserName [80] ;
   char   PopPassword [32] ;
   char   PopReader [260] ;
   char   PopWavFile [260] ;
   USHORT PopPort ;
   BOOL   PopForceIP ;
   BOOL   PopUseAPOP ;
   int    PopInterval ;
   BOOL   PopAlert ;

   // Presentation Parameters
   BYTE   FontNameSize [80] ;
   BOOL   fFontNameSize ;

   ULONG  BackColor ;
   BOOL   fBackColor ;

   ULONG  TextColor ;
   BOOL   fTextColor ;

   ULONG  WarningBackground, WarningForeground ;
   BOOL   fWarningBackground, fWarningForeground ;

   ULONG  ErrorBackground, ErrorForeground ;
   BOOL   fErrorBackground, fErrorForeground ;

   _IniData ( ) ;
   void Reset ( ) ;

} INIDATA, *PINIDATA ;

typedef struct {
   char *Name ;
   int Offset ;
   int Size ;
} PROFILEITEM ;

extern PROFILEITEM ProfileItems [] ;
extern int ProfileItemCount ;

#endif // INIDATA_H
