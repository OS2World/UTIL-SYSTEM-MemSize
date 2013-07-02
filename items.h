/******************************************************************** ITEMS.H
 *                                                                          *
 *                     Display Item Class Definition                        *
 *                                                                          *
 ****************************************************************************/

#ifndef ITEMS_H
#define ITEMS_H

#include "System.h"

#include <time.h>

#include "Canvas.h"
#include "Country.h"
#include "Dde.h"
#include "Debug.h"
#include "PerfUtil.h"
#include "ReString.h"

#ifdef __OS2__
   extern "C" {
      #include "Theseus0.h"
   }
#endif // __OS2__

#define MAX_AVERAGE_COUNT (60*60) 

class Profile ;

class Item {

   private:
      USHORT Id ;                  // Item ID.
      BOOL   Flag ;                // Flag: Show this item at this time?
      char   Name [80] ;           // Text for items profile name.
      char   DefaultLabel [80] ;   // Text to display on left part of line (default).
      void  *pDdeServer ;          // -> DDE Server object
      char   Topic [80] ;          // DDE Topic name
      #ifdef __OS2__
         Dde_Item *pDdeItem ;      // -> DDE Item object
      #endif // __OS2__

   protected:
      char    CurrentLabel [80] ;  // Text to display on left part of line.
      __int64 Value ;              // Current value.
      int     SenseEnabled ;       // Flag: Does user want level sensing enabled?
      int     LevelSense ;         // Flag: If +1, level sensing has Error > Warning > Normal.
      __int64 MinLevel, MaxLevel ; //       If -1, Error < Warning < Normal.  If zero, level checks are disabled.
      __int64 WarningLevel ;
      __int64 ErrorLevel ;
      __int64 DefaultLevels[2] ;
      ULONG   NormalColors[2] ;
      ULONG   WarningColors[2] ;
      ULONG   ErrorColors[2] ;

   public:
      Item ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel, void *ddeserver, char *topic ) ;
      virtual ~Item ( ) ;

      void GetProfile ( Profile &Prf ) ;
      void PutProfile ( Profile &Prf ) ;

      USHORT QueryId           ( void ) { return ( Id   ) ; }
      BOOL   QueryFlag         ( void ) { return ( Flag ) ; }
      PCHAR  QueryName         ( void ) { return ( Name ) ; }
      PCHAR  QueryCurrentLabel ( void ) { return ( CurrentLabel ) ; }
      PCHAR  QueryDefaultLabel ( void ) { return ( DefaultLabel ) ; }
      __int64 QueryValue       ( void ) { return ( Value ) ; }

      void SetLabel ( char *label ) { 
         if ( strcmp ( CurrentLabel, label ) ) {
            strcpy ( CurrentLabel, label ) ;  
            Value = 0 ; 
         } /* endif */
      } /* endmethod */

      void SetNormalColors ( ULONG Background, ULONG Foreground ) {
         NormalColors[0] = Background ;
         NormalColors[1] = Foreground ;
      } /* endmethod */

      __int64 QueryMinLevel   ( ) { return ( MinLevel ) ; }
      __int64 QueryMaxLevel   ( ) { return ( MaxLevel ) ; }
      int   QueryLevelSense ( ) { return ( LevelSense ) ; }
      int   QuerySenseEnabled ( ) { return ( SenseEnabled ) ; }

      void SetMinLevel ( __int64 Level ) { MinLevel = Level ; }
      void SetMaxLevel ( __int64 Level ) { MaxLevel = Level ; }
      void SetLevelSense ( int Sense ) { LevelSense = Sense ; }
      void SetSenseEnabled ( int Flag ) { SenseEnabled = Flag ; }

      void QueryDefaultLevels ( __int64 &Warning, __int64 &Error ) {
         Warning = DefaultLevels [0] ;
         Error = DefaultLevels [1] ;
      } /* endmethod */

      __int64 QueryWarningLevel ( ) { return ( WarningLevel ) ; }
      void SetWarningLevel ( __int64 Level ) { WarningLevel = Level ; }
      void SetWarningColors ( ULONG Background, ULONG Foreground ) {
         WarningColors[0] = Background ;
         WarningColors[1] = Foreground ;
      } /* endmethod */

      __int64 QueryErrorLevel ( ) { return ( ErrorLevel ) ; }
      void SetErrorLevel ( __int64 Level ) { ErrorLevel = Level ; }
      void SetErrorColors ( ULONG Background, ULONG Foreground ) {
         ErrorColors[0] = Background ;
         ErrorColors[1] = Foreground ;
      } /* endmethod */

      void SetFlag   ( void ) { Flag = TRUE ; }
      void ResetFlag ( void ) { Flag = FALSE ; }

      void DdeUpdate ( char *Text ) ;

      void Paint ( Canvas &canvas, RECTANGLE &Rectangle,
         char *Label, char *Text, __int64 NewValue, int Scale=1 ) ;

      // The following functions must be provided by all subclasses.

      virtual int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) = 0 ;

      virtual __int64 NewValue ( void ) = 0 ;

      virtual void FormatText ( char *Label, char *Text, __int64 Value ) = 0 ;

      virtual void FormatLine ( char *Buffer, int MaxWidth ) ;

      virtual void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) = 0 ;
} ;

class Clock : public Item {

   private:
      CountryInfo Locale ;
      ResourceString *DaysOfWeek ;
      char DateFormat [80] ;
      BOOL CenterClock ;

   public:
      Clock ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, char *dateformat, BOOL centered ) ;

      void SetDateFormat ( char *dateformat ) { strcpy ( DateFormat, dateformat ) ; }
      void SetCenterClock ( BOOL value ) { CenterClock = value ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void FormatLine ( char *Buffer, int MaxWidth ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class ElapsedTime : public Item {

   private:
      CountryInfo Locale ;
      ResourceString *Day ;
      ResourceString *Days ;
      BOOL ShowSeconds ;
      __int64 Offset ;
      int Wrapped ;

   public:
      ElapsedTime ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, ResourceString *day, ResourceString *days, BOOL showseconds, PSZ swappath ) ;

      void SetShowSeconds ( BOOL showseconds ) { ShowSeconds = showseconds ;  Value = 0 ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class MessageCount : public Item {

   private:
      clock_t LastClock ;
      char Server [80] ;
      char UserName [80] ;
      char Password [32] ;
      char WavFile [260] ;
      USHORT Port ;
      BOOL ForceIP ;
      BOOL UseAPOP ;
      int Interval ;
      BOOL Alert ;

   public:
      MessageCount ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, char *Server, char *UserName, 
         char *Password, USHORT Port, BOOL ForceIP, BOOL UseAPOP, int Interval, char *WavFile, BOOL Alert ) ;

      void SetServer   ( char *server   ) { strcpy ( Server, server ) ; LastClock = 0 ; }
      void SetUserName ( char *username ) { strcpy ( UserName, username ) ; LastClock = 0 ; }
      void SetPassword ( char *password ) { strcpy ( Password, password ) ; LastClock = 0 ; }
      void SetPort     ( USHORT port    ) { Port = port ; LastClock = 0 ; }
      void SetForceIP  ( BOOL forceip   ) { ForceIP = forceip ; LastClock = 0 ; }
      void SetUseAPOP  ( BOOL useapop   ) { UseAPOP = useapop ; LastClock = 0 ; }
      void SetInterval ( int interval   ) { Interval = interval ; LastClock = 0 ; }
      void SetWavFile  ( char *wavfile  ) { strcpy ( WavFile, wavfile ) ; }
      void SetAlert    ( BOOL alert     ) { Alert = alert ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class MemoryFree : public Item {

   private:
      CountryInfo Locale ;
      BOOL UseTheseus ;
      USHORT ShowK, ShowTrueK, ShowM, ShowG ;
      #ifdef __OS2__
         HMODULE hTheseus ; clock_t LastClock ;
         T2HANDLE0 T2Handle0 ;
         ULONG (EXPENTRY *pT2Initialize0)(ULONG*,ERROR_HANDLER0);
         VOID (EXPENTRY *pT2Terminate0)(VOID);
         T2HANDLE0 (EXPENTRY *pT2GetHandle0)(ERROR_HANDLER0);
         VOID (EXPENTRY *pT2ReleaseHandle0)(T2HANDLE0*);
         VOID (EXPENTRY *pT2FreeMemory)(T2HANDLE0,PULONG,PULONG,PULONG,PULONG,PULONG,PULONG);
      #endif // __OS2__

   public:
      MemoryFree ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, BOOL ut, USHORT sk, USHORT stk, USHORT sm, USHORT sg ) ;
      ~MemoryFree ( ) ;

      void SetUseTheseus ( BOOL flag ) ;
      void SetShowK      ( USHORT flag ) { ShowK = flag ;  Value = 0 ; }
      void SetShowTrueK  ( USHORT flag ) { ShowTrueK = flag ;  Value = 0 ; }
      void SetShowM      ( USHORT flag ) { ShowM = flag ;  Value = 0 ; }
      void SetShowG      ( USHORT flag ) { ShowG = flag ;  Value = 0 ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class VMFree : public Item {

   private:
      CountryInfo Locale ;
      USHORT ShowK, ShowTrueK, ShowM, ShowG ;

   public:
      VMFree ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, USHORT sk, USHORT stk, USHORT sm, USHORT sg ) ;

      void SetShowK     ( USHORT flag ) { ShowK = flag ;  Value = 0 ; }
      void SetShowTrueK ( USHORT flag ) { ShowTrueK = flag ;  Value = 0 ; }
      void SetShowM     ( USHORT flag ) { ShowM = flag ;  Value = 0 ; }
      void SetShowG     ( USHORT flag ) { ShowG = flag ;  Value = 0 ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class SwapSize : public Item {

   private:
      CountryInfo Locale ;
      USHORT ShowK, ShowTrueK, ShowM, ShowG ;
      BYTE *SwapPath ;

   public:
      SwapSize ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, 
         USHORT sk, USHORT stk, USHORT sm, USHORT sg, BYTE *swappath, ULONG initialsize ) ;
      ~SwapSize ( void ) ;

      void SetShowK     ( USHORT flag ) { ShowK = flag ;  Value = 0 ; }
      void SetShowTrueK ( USHORT flag ) { ShowTrueK = flag ;  Value = 0 ; }
      void SetShowM     ( USHORT flag ) { ShowM = flag ;  Value = 0 ; }
      void SetShowG     ( USHORT flag ) { ShowG = flag ;  Value = 0 ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class SwapFree : public Item {

   private:
      CountryInfo Locale ;
      USHORT ShowK, ShowTrueK, ShowM, ShowG ;
      BYTE *SwapPath ;
      ULONG MinFree ;

   public:
      SwapFree ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, 
         USHORT sk, USHORT stk, USHORT sm, USHORT sg, BYTE *swappath, ULONG minfree ) ;
      ~SwapFree ( void ) ;

      void SetShowK     ( USHORT flag ) { ShowK = flag ;  Value = 0 ; }
      void SetShowTrueK ( USHORT flag ) { ShowTrueK = flag ;  Value = 0 ; }
      void SetShowM     ( USHORT flag ) { ShowM = flag ;  Value = 0 ; }
      void SetShowG     ( USHORT flag ) { ShowG = flag ;  Value = 0 ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class SpoolSize : public Item {

   private:
      CountryInfo Locale ;
      USHORT ShowK, ShowTrueK, ShowM, ShowG ;
      PSZ SpoolPath ;

   public:
      SpoolSize ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, 
         USHORT sk, USHORT stk, USHORT sm, USHORT sg, PSZ spoolpath ) ;
      ~SpoolSize ( void ) ;

      void SetShowK     ( USHORT flag ) { ShowK = flag ;  Value = 0 ; }
      void SetShowTrueK ( USHORT flag ) { ShowTrueK = flag ;  Value = 0 ; }
      void SetShowM     ( USHORT flag ) { ShowM = flag ;  Value = 0 ; }
      void SetShowG     ( USHORT flag ) { ShowG = flag ;  Value = 0 ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class CpuLoad ;

class AverageLoad : public Item {

   private:
      int Interval ;
      ULONG Total ;
      int NumberOfCounts ;
      ULONG Counts [MAX_AVERAGE_COUNT] ;
      int NextIndex ;
      void StoreCount ( ULONG Count ) ;
      CpuLoad *pCpuLoad ;

   public:
      AverageLoad ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel, 
         void *pDdeServer, char *Topic, int Interval, CpuLoad *pCpuLoad=0 ) ;

      void SetCpuLoadObject ( CpuLoad *pcpuload ) { pCpuLoad = pcpuload ; }
      void Reset ( ) ;
      void SetInterval ( int NewInterval ) ;
      void UpdateHistory ( int Seconds, ULONG Count ) ;

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class CpuLoad : public Item {

   private:
      PULONG IdleCount ;
      ULONG MaxCount ;
      ResourceString *Error ;
      time_t LastTime ;
      AverageLoad *pAverageLoad ;
      #ifdef __OS2__
         ULONG CpuCount ;
         APIRET (APIENTRY *DosPerfSysCall)(ULONG ulCommand, ULONG ulParm1, ULONG ulParm2, ULONG ulParm3);
         CPUUTIL *Previous, *Current ;
      #endif // __OS2__

   public:
      CpuLoad ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, ULONG maxcount, PULONG idlecount, ResourceString *error, AverageLoad *paverageload ) ;
      ~CpuLoad ( ) ;

      #ifdef __OS2__
         static BOOL UsePerfAPI ( HMODULE &ModuleHandle, PFN *DosPerfSysCall, ULONG CpuCount, CPUUTIL *Previous, CPUUTIL *Current ) ;
         static BOOL UsePerfAPI ( ) ;
      #endif // __OS2__

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class TaskCount : public Item {

   private:
      ANCHOR Anchor ;

   public:
      TaskCount ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, ANCHOR anchor ) ;

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class DriveFree : public Item {

   private:
      CountryInfo Locale ;
      USHORT ShowK, ShowTrueK, ShowM, ShowG ;
      ResourceString *DriveError ;
      USHORT DriveNumber ;
      BOOL ShowFileSystemName ;
      BYTE FileSystem [80] ;
      BOOL ShowDiskLabel ;
      BYTE DiskLabel [80] ;
      BOOL Error ;

   public:
      DriveFree ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, 
         USHORT sk, USHORT stk, USHORT sm, USHORT sg, USHORT drivenumber, ResourceString *driveerror, BOOL showfilesystemname,
         PSZ filesystem, BOOL showdisklabel, PSZ disklabel ) ;

      void SetShowK     ( USHORT flag ) { ShowK = flag ;  Value = 0 ; }
      void SetShowTrueK ( USHORT flag ) { ShowTrueK = flag ;  Value = 0 ; }
      void SetShowM     ( USHORT flag ) { ShowM = flag ;  Value = 0 ; }
      void SetShowG     ( USHORT flag ) { ShowG = flag ;  Value = 0 ; }
      void SetShowFileSystemName ( BOOL showfilesystemname ) { ShowFileSystemName = showfilesystemname ;  Value = 0 ; }
      void SetShowDiskLabel ( BOOL showdisklabel ) { ShowDiskLabel = showdisklabel ;  Value = 0 ; }
      void ResetError ( ) { Error = FALSE ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class TotalFree : public Item {

   private:
      CountryInfo Locale ;
      USHORT ShowK, ShowTrueK, ShowM, ShowG ;
      ULONG Drives ;

   public:
      TotalFree ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, 
         USHORT sk, USHORT stk, USHORT sm, USHORT sg, ULONG drives ) ;

      void SetShowK     ( USHORT flag ) { ShowK = flag ;  Value = 0 ; }
      void SetShowTrueK ( USHORT flag ) { ShowTrueK = flag ;  Value = 0 ; }
      void SetShowM     ( USHORT flag ) { ShowM = flag ;  Value = 0 ; }
      void SetShowG     ( USHORT flag ) { ShowG = flag ;  Value = 0 ; }
      void ResetMask ( ULONG drives ) { Drives = drives ; }
      ULONG QueryMask ( ) { return ( Drives ) ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class SwapSlack : public Item {

   private:
      CountryInfo Locale ;     
      BOOL UseTheseus ;
      USHORT ShowK, ShowTrueK, ShowM, ShowG ;
      VMFree *pVMFree ;
      SwapFree    *pSwapFree ;
      MemoryFree  *pMemFree ;
      #ifdef __OS2__
         HMODULE hTheseus ; clock_t LastClock ;
         T2HANDLE0 T2Handle0 ;
         ULONG (EXPENTRY *pT2Initialize0)(ULONG*,ERROR_HANDLER0);
         VOID (EXPENTRY *pT2Terminate0)(VOID);
         T2HANDLE0 (EXPENTRY *pT2GetHandle0)(ERROR_HANDLER0);
         VOID (EXPENTRY *pT2ReleaseHandle0)(T2HANDLE0*);
         VOID (EXPENTRY *pT2AnalyzeSwapper)(T2HANDLE0,PULONG,PULONG);
      #endif // __OS2__

   public:
      SwapSlack ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic,
         BOOL ut, USHORT sk, USHORT stk, USHORT sm, USHORT sg, VMFree *pvf, SwapFree *psf, MemoryFree *pmf ) ;
      virtual ~SwapSlack ( ) ;

      void SetUseTheseus ( BOOL flag ) ;
      void SetShowK      ( USHORT flag ) { ShowK = flag ;  Value = 0 ; }
      void SetShowTrueK  ( USHORT flag ) { ShowTrueK = flag ;  Value = 0 ; }
      void SetShowM      ( USHORT flag ) { ShowM = flag ;  Value = 0 ; }
      void SetShowG     ( USHORT flag ) { ShowG = flag ;  Value = 0 ; }

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class ProcessCount : public Item {

   private:
      #ifdef __OS2__
         PULONG DQSS_Buffer ;
      #endif // __OS2__

   public:
      ProcessCount ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic ) ;
      ~ProcessCount ( ) ;

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

extern PSZ ScanSystemConfig ( ANCHOR Anchor, PSZ Keyword ) ;

class ThreadCount : public Item {

   private:
      #ifdef __OS2__
         PULONG DQSS_Buffer ;
      #endif // __OS2__

   public:
      ThreadCount ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic ) ;
      ~ThreadCount ( ) ;

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;
} ;

class Battery : public Item {

   private:
      HFILE Handle ;
      ResourceString *Error ;
      ResourceString *Charging ;
      ResourceString *AC ;

   public:
      Battery ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
         void *pDdeServer, char *Topic, ResourceString *error,
         ResourceString *charging, ResourceString *ac ) ;
      ~Battery ( ) ;

      int Measure ( Canvas &canvas, RECTANGLE &Rectangle ) ;
      __int64 NewValue ( void ) ;
      void FormatText ( char *Label, char *Text, __int64 Value ) ;
      void Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) ;

      int Discharging ( ) ;
} ;

#endif
