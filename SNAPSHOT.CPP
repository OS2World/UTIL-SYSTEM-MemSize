/*************************************************************** SNAPSHOT.CPP
 *                                                                          *
 *           Standalone Text-mode Snapshot of System Resources              *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

// #ifdef __NT__
//    #include "TLHelp32.h"
// #endif // __NT__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ChkDrive.h"
#include "Config.h"
#include "DateFmt.h"
#include "DosQSS.h"
#include "Event.h"
#include "Except.h"
#include "IniData.h"
#include "Items.h"
#include "MemSize.h"
#include "Module.h"
#include "PerfUtil.h"
#include "Profile.h"
#include "ReString.h"
#include "Support.h"
#include "SVDisk.h"

#define DQSS_BUFFER_SIZE 0x40000
 
/****************************************************************************
 *                                                                          *
 *                       Definitions & Declarations                         *
 *                                                                          *
 ****************************************************************************/

static void FormatValue ( ULONG Value, char Text[], CountryInfo &Locale, int ShowK=SHOWK_ABOVE512, int ShowTrueK=TRUE, int ShowM=TRUE, int ShowG=TRUE, int Scale=1 ) ;

#if __OS2__

extern PSZ ScanSystemConfig ( HAB Anchor, PSZ Keyword ) ;
static char *CopyString ( char *Buffer, char *Original ) ;

extern "C" APIRET16 APIENTRY16 Dos16MemAvail ( PULONG pulAvailMem ) ;

static void EXPENTRY TheseusErrorHandler ( T2HANDLE0, ULONG, ULONG, ULONG ) { }

#else // __NT__

#include <tlhelp32.h>
#include <vdmdbg.h>
#include "psapi.h"

typedef BOOL ( CALLBACK *PROCENUMPROC ) ( DWORD, WORD, LPSTR, LPARAM, BOOL ) ;
typedef HANDLE ( WINAPI FN1 ) ( DWORD, DWORD ) ;
typedef BOOL ( WINAPI FN2 ) ( HANDLE, LPPROCESSENTRY32 ) ;
typedef BOOL ( WINAPI FN3 ) ( DWORD*, DWORD, DWORD* ) ;
typedef BOOL ( WINAPI FN4 ) ( HANDLE, HMODULE*, DWORD, LPDWORD ) ;
typedef DWORD ( WINAPI FN5 ) ( HANDLE, HMODULE, LPTSTR, DWORD ) ;
typedef INT ( WINAPI FN6 ) ( DWORD, TASKENUMPROCEX, LPARAM ) ;

#endif // __OS2__ vs __NT__

extern char HomePath [_MAX_PATH] = { 0 } ;
extern HMODULE LibraryHandle = 0 ;
 
/****************************************************************************
 *                                                                          *
 *  General Purpose Counter Thread - Part of fallback for old OS/2 versions.*
 *                                                                          *
 ****************************************************************************/

#ifdef __OS2__

typedef struct {
   BOOL Active ;
   ULONG Counter ;
} COUNTER_PARMS, *PCOUNTER_PARMS ;

static Event CounterThreadEvent = Event ( ) ;

static void CounterThread ( void *Parameter ) {

  /*************************************************************************
   * Count like mad.                                                       *
   *************************************************************************/

   PCOUNTER_PARMS Parms = PCOUNTER_PARMS ( Parameter ) ;
   while ( Parms->Active )
      Parms->Counter ++ ;

   CounterThreadEvent.Post ( ) ;
}

#endif // __OS2__

/****************************************************************************
 *                                                                          *
 *                       Calibrate the Load Meter                           *
 *                                                                          *
 ****************************************************************************/

#ifdef __OS2__

#define LL2F(high,low) ( 4294967296.0*(high) + (low) )

static ULONG CalibrateLoadMeter ( PCOUNTER_PARMS Parms ) {

  /**************************************************************************
   * Increase this thread's priority to the maximum.                        *
   **************************************************************************/

   Thread_SetPriority ( 0, PRIORITY_TIME_CRITICAL ) ;

  /**************************************************************************
   * Create the calibration thread and set its priority next highest.       *
   **************************************************************************/

   CounterThreadEvent.Reset ( ) ;
   Parms->Active = TRUE ;
   TID tidCalibrate = StartThread ( "CounterThread", CounterThread, 0x3000, Parms ) ;
   Sys_SuspendThread ( tidCalibrate ) ;
   Thread_SetPriority ( tidCalibrate, Priority(PRIORITY_TIME_CRITICAL-1) ) ;

  /**************************************************************************
   * Reset the calibration count, get the time, and let the counter go.     *
   **************************************************************************/

   Parms->Counter = 0 ;
   QWORD Time0 ; DosTmrQueryTime ( &Time0 ) ;
   Sys_ResumeThread ( tidCalibrate ) ;

  /**************************************************************************
   * Sleep for one second.                                                  *
   **************************************************************************/

   Sys_Sleep ( 1000 ) ;

  /**************************************************************************
   * Suspend the calibration counter and get the time.                      *
   **************************************************************************/

   Parms->Active = FALSE ;
   CounterThreadEvent.Wait ( 10000 ) ; 
   QWORD Time1 ; DosTmrQueryTime ( &Time1 ) ;

  /**************************************************************************
   * Return priorities to normal.                                           *
   **************************************************************************/

   DosSetPriority ( PRTYS_THREAD, PRTYC_REGULAR, 0, 0 ) ;

  /**************************************************************************
   * Get the elapsed time and adjust the calibration count.                 *
   **************************************************************************/

   __int64 TimerTicks ( ( (__int64)Time1.ulHi << 32 ) + (__int64)Time1.ulLo ) ;
   TimerTicks -= ( (__int64)Time0.ulHi << 32 ) + (__int64)Time0.ulLo ; 

   ULONG TimerFrequency ; DosTmrQueryFreq ( &TimerFrequency ) ; 

   double TimeSpent ( double(TimerTicks) / double(TimerFrequency) ) ; 

   double AdjustedMaxLoad ( double(Parms->Counter) / TimeSpent ) ; 
   
  /**************************************************************************
   * Return the adjusted calibration count.                                 *
   **************************************************************************/

   return ( (ULONG)AdjustedMaxLoad ) ;
}

#endif // __OS2__

/****************************************************************************
 *                                                                          *
 *                              Mainline                                    *
 *                                                                          *
 ****************************************************************************/

extern int main ( int, char *argv[] ) {

   // Initialization

   REGISTER_EXCEPTION_HANDLER();
   Module Library ( PSZ(PROGRAM_NAME) ) ;
   LibraryHandle = Library.QueryHandle() ;
   CountryInfo Locale ;

   strupr ( argv[0] ) ; _fullpath ( HomePath, argv[0], sizeof(HomePath) ) ;
   char DriveName[_MAX_DRIVE+1] ; char Dir[_MAX_DIR+1] ; char Fname[_MAX_FNAME+1] ; char Ext[_MAX_EXT+1] ;
   _splitpath ( HomePath, DriveName, Dir, Fname, Ext ) ;
   if ( Dir[strlen(Dir)-1] == '\\' )
      Dir[strlen(Dir)-1] = 0 ;
   strcpy ( HomePath, DriveName ) ;
   strcat ( HomePath, Dir ) ;

   Profile IniFile ( PROGRAM_NAME, 0, HomePath ) ;
   INIDATA IniData ;
   if ( IniFile.IsReady() ) 
      IniFile.GetIniData ( &IniData ) ;

#ifdef __OS2__

   ULONG PathSize ;
   DosQuerySysInfo ( QSV_MAX_PATH_LENGTH, QSV_MAX_PATH_LENGTH, &PathSize, sizeof(PathSize) ) ;

   PSZ Swappath = ScanSystemConfig ( 0, PSZ("SWAPPATH") ) ;
   if ( Swappath == NULL ) 
      Swappath = PSZ("C:\\OS2\\SYSTEM 0") ;
   char SwapPath [_MAX_PATH] ;
   char *p = CopyString ( SwapPath, PCHAR(Swappath) ) ;
   ULONG MinFree ;
   sscanf ( p, " %li", &MinFree ) ;

   PSZ SpoolPath = 0 ; ULONG Size ;
   if ( PrfQueryProfileSize ( HINI_PROFILE, PSZ("PM_SPOOLER"), PSZ("DIR"), &Size ) ) {
      SpoolPath = PSZ ( malloc ( (int)Size ) ) ;
      if ( SpoolPath ) {
         if ( PrfQueryProfileData ( HINI_PROFILE, PSZ("PM_SPOOLER"), PSZ("DIR"), SpoolPath, &Size ) ) {
            PBYTE p = PBYTE( strchr ( PCHAR(SpoolPath), ';' ) ) ;
            if ( p ) {
               *p = 0 ;
            } /* endif */
         } else {
            free ( SpoolPath ) ;
            SpoolPath = 0 ;
         } /* endif */
      } /* endif */
   } /* endif */
   if ( SpoolPath == 0 )
      SpoolPath = PSZ ( "C:\\SPOOL" ) ;

   // Enable use of Theseus/2.
   HMODULE hTheseus ( 0 ) ;
   T2HANDLE0 T2Handle0 ( 0 ) ;
   ULONG (EXPENTRY *pT2Initialize0)(ULONG*,ERROR_HANDLER0) = 0 ;
   VOID (EXPENTRY *pT2Terminate0)(VOID) = 0 ;
   T2HANDLE0 (EXPENTRY *pT2GetHandle0)(ERROR_HANDLER0) = 0 ;
   VOID (EXPENTRY *pT2ReleaseHandle0)(T2HANDLE0*) = 0 ;
   VOID (EXPENTRY *pT2FreeMemory)(T2HANDLE0,PULONG,PULONG,PULONG,PULONG,PULONG,PULONG) = 0 ;
   VOID (EXPENTRY *pT2AnalyzeSwapper)(T2HANDLE0,PULONG,PULONG) = 0 ;
   if ( !DosLoadModule ( 0, 0, "THESEUS0.DLL", &hTheseus ) ) {
      DosQueryProcAddr ( hTheseus, 0, "T2Initialize0", (PFN*)&pT2Initialize0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2Terminate0", (PFN*)&pT2Terminate0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2GetHandle0", (PFN*)&pT2GetHandle0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2ReleaseHandle0", (PFN*)&pT2ReleaseHandle0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2FreeMemory", (PFN*)&pT2FreeMemory ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2AnalyzeSwapper", (PFN*)&pT2AnalyzeSwapper ) ;
      ULONG ulDDVersion ;
      if ( pT2Initialize0 ( &ulDDVersion, TheseusErrorHandler ) == Theseus2OK ) {
         T2Handle0 = pT2GetHandle0 ( TheseusErrorHandler ) ;
      } /* endif */
   } /* endif */

#else // __NT__

   // Get the ToolHelp 32-bit API interface, if possible.
   HMODULE hKernel32 ( LoadLibrary ( "KERNEL32.DLL" ) ) ;
   FN1 *pCreateToolhelp32Snapshot ( hKernel32 ? (FN1*)GetProcAddress(hKernel32,"CreateToolhelp32Snapshot") : 0 ) ;
   FN2 *pProcess32First ( hKernel32 ? (FN2*)GetProcAddress(hKernel32,"Process32First") : 0 ) ;
   FN2 *pProcess32Next ( hKernel32 ? (FN2*)GetProcAddress(hKernel32,"Process32Next") : 0 ) ;

   // Get Program Scheduler API interface, if available.
   HMODULE hPSAPI ( LoadLibrary ( "PSAPI.DLL" ) ) ;
   FN3 *pEnumProcesses ( hPSAPI ? (FN3*)GetProcAddress(hPSAPI,"EnumProcesses") : 0 ) ;
   FN4 *pEnumProcessModules ( hPSAPI ? (FN4*)GetProcAddress(hPSAPI,"EnumProcessModules") : 0 ) ;
   FN5 *pGetModuleFileNameEx ( hPSAPI ? (FN5*)GetProcAddress(hPSAPI,"GetModuleFileNameExA") : 0 ) ;

   // Get the Windows on Windows API interface, if available.
   HMODULE hWOW ( LoadLibrary ( "VDMDBG.DLL" ) ) ;
   FN6 *pVDMEnumTaskWOWEx ( hWOW ? (FN6*)GetProcAddress(hWOW,"VDMEnumTaskWOWEx") : 0 ) ;

#endif // __OS2__ vs __NT__

   // Date and Time

   ULONG Time = ULONG ( time ( 0 ) ) ;
   struct tm Tm ; memcpy ( &Tm, localtime(&Time), sizeof(Tm) ) ;
   ULONG Dow    = Tm.tm_wday ;
   ULONG Month  = Tm.tm_mon + 1 ;
   ULONG Day    = Tm.tm_mday ;
   ULONG Hour   = Tm.tm_hour ;
   ULONG Minute = Tm.tm_min ;
   ULONG Second = Tm.tm_sec ;
   ResourceString DaysOfWeek ( Library.QueryHandle(), IDS_DAYSOFWEEK ) ;
   BOOL ShowSeconds ( IniData.ShowSeconds ) ;
   char Text [100] ;
   FormatDate ( IniData.DateFormat, Text, sizeof(Text) ) ;
   printf ( "%*s%s", 40-strlen(Text)/2, "", Text ) ;
   printf ( "\n" ) ;


   // Elapsed Time

   ResourceString ElapsedLabel ( Library.QueryHandle(), IDS_ELAPSED ) ;
   int Column = printf ( "%s", PSZ(ElapsedLabel) ) ;
   strcpy ( Text, "" ) ;

   ULONG Milliseconds ;
   #ifdef __OS2__
      DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &Milliseconds, sizeof(Milliseconds) ) ;
   #else // __NT__
      Milliseconds = GetTickCount ( ) ;
   #endif // __OS2__ vs __NT__

   Time = Milliseconds / 1000L ;

   ULONG NumberOfDays = ULONG ( Time / ( 60 * 60 * 24 ) ) ;
   Time -= NumberOfDays * 60 * 60 * 24 ;
   ULONG Hours   = ULONG ( Time / ( 60 * 60 ) ) ;
   Time -= Hours * 60 * 60 ;
   ULONG Minutes = ULONG ( Time / 60 ) ;
   ULONG Seconds = ULONG ( Time % 60 ) ;

   if ( NumberOfDays ) {
      ResourceString Day ( Library.QueryHandle(), IDS_DAY ) ;
      ResourceString Days ( Library.QueryHandle(), IDS_DAYS ) ;
      sprintf ( Text+strlen(Text), "%lu %s, ", NumberOfDays, NumberOfDays > 1 ? PSZ(Days) : PSZ(Day) ) ;
   } /* endif */

   if ( ShowSeconds )
      sprintf ( Text+strlen(Text), "%lu%s%02lu%s%02lu", Hours, Locale.szTimeSeparator, Minutes, Locale.szTimeSeparator, Seconds ) ;
   else
      sprintf ( Text+strlen(Text), "%lu%s%02lu", Hours, Locale.szTimeSeparator, Minutes ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;


   // Physical Memory Free (verified on OS/2 and NT)

   ResourceString PhysicalMemoryFree ( Library.QueryHandle(), IDS_MEMORY ) ;
   Column = printf ( "%s", PSZ(PhysicalMemoryFree) ) ;
   ULONG PhysMemFree ( 0 ) ;
   #ifdef __OS2__
   { /* block */
      Dos16MemAvail ( &PhysMemFree ) ;
      if ( ( PhysMemFree <= 512 * 1024 ) && hTheseus && pT2FreeMemory ) {
         ULONG FreeRam, IdleRam, DirtyRam, LLockRam, SLockRam, BLockRam ;
         pT2FreeMemory ( T2Handle0, &FreeRam, &IdleRam, &DirtyRam, &LLockRam, &SLockRam, &BLockRam ) ;
         PhysMemFree = FreeRam * 0x1000 ;
      } /* endif */
   } /* endblock */
   #else // __NT__
   { /* block */
      MEMORYSTATUS MemoryStatus ;
      GlobalMemoryStatus ( &MemoryStatus ) ;
      PhysMemFree = MemoryStatus.dwAvailPhys ;
   } /* endblock */
   #endif // __OS2__ vs __NT__
   BOOL ShowK(IniData.ShowK), ShowTrueK(IniData.ShowTrueK), ShowM(IniData.ShowM), ShowG(IniData.ShowG) ;
   FormatValue ( PhysMemFree, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;


   // Virtual Memory Free

   ResourceString VirtualMemoryFree ( Library.QueryHandle(), IDS_VIRTUAL ) ;
   Column = printf ( "%s", PSZ(VirtualMemoryFree) ) ;
   ULONG VirtMemFree ( 0 ) ;
   #ifdef __OS2__
      DosQuerySysInfo ( QSV_TOTAVAILMEM, QSV_TOTAVAILMEM, &VirtMemFree, sizeof(VirtMemFree) ) ;
   #else // __NT__
   { /* block */
      MEMORYSTATUS MemoryStatus ;
      GlobalMemoryStatus ( &MemoryStatus ) ;
      VirtMemFree = MemoryStatus.dwAvailPageFile + MemoryStatus.dwAvailPhys ;
   } /* endblock */
   #endif // __OS2__ vs __NT__
   FormatValue ( VirtMemFree, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;

   // Swapfile Size (Verified on OS/2 and NT)

   ResourceString SwapfileSize ( Library.QueryHandle(), IDS_SWAPSIZE ) ;
   Column = printf ( "%s", PSZ(SwapfileSize) ) ;
 	ULONG SwapSize ( 0 ) ;
	char Path [_MAX_PATH] = { 0 } ;
   #ifdef __OS2__
   { /* block */
   	strcpy ( PCHAR(Path), PCHAR(SwapPath) ) ;
   	if ( Path[strlen(PCHAR(Path))-1] != '\\' ) {
      	strcat ( PCHAR(Path), "\\" ) ;
   	} /* endif */
   	strcat ( PCHAR(Path), "SWAPPER.DAT" ) ;
   	FILESTATUS3 FileStatus ;
   	if ( DosQueryPathInfo ( (PSZ)Path, FIL_STANDARD, &FileStatus, sizeof(FileStatus) ) == 0 ) 
      	SwapSize = FileStatus.cbFileAlloc ;
   } /* endblock */
   #else // __NT__
   { /* block */
		// Under Win 9x, registry section 386enh, entry PagingFile contains the path, null terminated.
		// Under Win NT, registry key "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management\Paging Files"
		//   has the path, terminated by a space.
		OSVERSIONINFO Version = { sizeof(OSVERSIONINFO) } ;
		GetVersionEx ( &Version ) ;
		if ( Version.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ) {
			GetProfileString ( "386enh", "PagingFile", "N/A", Path, sizeof(Path) ) ;
		} else {
			HKEY hKey ;
			if ( RegOpenKeyEx ( HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management", 0, KEY_READ, &hKey ) == ERROR_SUCCESS ) {
				DWORD dwSize = sizeof(Path) ;
				if ( RegQueryValueEx ( hKey, "PagingFiles", 0, 0, (unsigned char*)Path, &dwSize ) == ERROR_SUCCESS ) {
					char *p = strchr ( Path, ' ' ) ;
					if ( p ) *p = 0 ;
					WIN32_FIND_DATA Found ;
					HANDLE hDir = FindFirstFile ( Path, &Found ) ;
					if ( hDir ) {
						FindClose ( hDir ) ;
						SwapSize = Found.nFileSizeLow ;
					} else {
						Path[0] = 0 ;
					} /* endif */
				} else {
					Path[0] = 0 ;
				} /* endif */
				RegCloseKey ( hKey ) ;
			} /* endif */
		} /* endif */
   } /* endblock */
   #endif // __OS2__ vs __NT__
   FormatValue ( SwapSize, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   printf ( " (%s)%*s%s", Path, 79-Column-strlen(Text)-strlen(Path)-3, "", Text ) ;
   printf ( "\n" ) ;

   // Available Swap Space (Verified on OS/2 and NT)

   ResourceString SwapspaceFree ( Library.QueryHandle(), IDS_SWAPFREE ) ;
   Column = printf ( "%s", PSZ(SwapspaceFree) ) ;
   ULONG SwapFree ( 0 ) ;
   #ifdef __OS2__
   { /* block */
   	PBYTE Path = PBYTE ( malloc ( size_t(PathSize) ) ) ;
   	strcpy ( PCHAR(Path), (PCHAR)SwapPath ) ;
   	strcat ( PCHAR(Path), "\\SWAPPER.DAT" ) ;
   	if ( Path[0] ) {
      	DosError ( FERR_DISABLEHARDERR ) ;
      	FSALLOCATE Allocation ;
      	DosQueryFSInfo ( Path[0]-'A'+1, FSIL_ALLOC, (PBYTE)&Allocation, sizeof(Allocation) ) ;
      	DosError ( FERR_ENABLEHARDERR ) ;
      	SwapFree = Allocation.cUnitAvail*Allocation.cSectorUnit*Allocation.cbSector ;
   	} /* endif */
   	if ( SwapFree < ULONG(MinFree*1024L) )
      	SwapFree = 0 ;
   	else
      	SwapFree = SwapFree - ULONG(MinFree*1024L) ;
   	free ( Path ) ;
   } /* endblock */
   #else // __NT__
   { /* block */
      MEMORYSTATUS MemoryStatus ;
      GlobalMemoryStatus ( &MemoryStatus ) ;
      SwapFree = MemoryStatus.dwTotalPageFile - ( SwapSize - MemoryStatus.dwAvailPageFile ) ;
   } /* endblock */
   #endif // __OS2__ vs __NT__
   FormatValue ( SwapFree, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;

   // Swapfile Slack Space (Verified on OS/2 and NT)

   ResourceString SwapslackFree ( Library.QueryHandle(), IDS_SWAPSLACK ) ;
   Column = printf ( "%s", PSZ(SwapslackFree) ) ;
   LONG SwapSlack ( 0 ) ;
   #ifdef __OS2__
      SwapSlack = LONG ( max ( 0, LONG(VirtMemFree-SwapFree-PhysMemFree) ) ) ;
      if ( hTheseus && pT2AnalyzeSwapper ) {
         ULONG SwapUsed, SwapFree ;
         pT2AnalyzeSwapper ( T2Handle0, &SwapUsed, &SwapFree ) ;
         SwapSlack = SwapFree * 0x1000 ;
      } /* endif */
   #else // __NT__
   { /* block */
      MEMORYSTATUS MemoryStatus ;
      GlobalMemoryStatus ( &MemoryStatus ) ;
      SwapSlack = MemoryStatus.dwAvailPageFile ;
   } /* endblock */
   #endif // __OS2__ vs __NT__
   FormatValue ( SwapSlack, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;

#ifdef __OS2__

   // Spoolfile Size

   ResourceString SpoolSize ( Library.QueryHandle(), IDS_SPOOLSIZE ) ;
   Column = printf ( "%s", PSZ(SpoolSize) ) ;
   PFILEFINDBUF3 Found = PFILEFINDBUF3 ( malloc ( size_t(PathSize+sizeof(FILEFINDBUF3)) ) ) ;
   strcpy ( (PCHAR)Path, (PCHAR)SpoolPath ) ;
   strcat ( (PCHAR)Path, "\\*.*" ) ;
   HDIR hDir = (HDIR) HDIR_CREATE ;
   ULONG Count = 1 ;
   ULONG TotalSize = 0 ;
   if ( !DosFindFirst2 ( Path, &hDir,
      FILE_NORMAL | FILE_READONLY | FILE_DIRECTORY | FILE_ARCHIVED,
      Found, PathSize+sizeof(FILEFINDBUF3), &Count, FIL_STANDARD ) ) {
      do {
         if ( !strcmp ( (PCHAR)Found->achName, "." ) OR !strcmp ( (PCHAR)Found->achName, ".." ) ) {
            continue ;
         } /* endif */
         if ( Found->attrFile & FILE_DIRECTORY ) {
            HDIR hDir2 = (HDIR) HDIR_CREATE ;
            strcpy ( (PCHAR)Path, (PCHAR)SpoolPath ) ;
            strcat ( (PCHAR)Path, "\\" ) ;
            strcat ( (PCHAR)Path, (PCHAR)Found->achName ) ;
            strcat ( (PCHAR)Path, "\\*.*" ) ;
            ULONG Count2 = 1 ;
            if ( !DosFindFirst2 ( Path, &hDir2,
               FILE_NORMAL | FILE_READONLY | FILE_ARCHIVED,
               Found, PathSize+sizeof(FILEFINDBUF3), &Count2, FIL_STANDARD ) ) {
               do {
                  TotalSize += Found->cbFileAlloc ;
               } while ( !DosFindNext ( hDir2, Found, PathSize+sizeof(FILEFINDBUF3), &Count2 ) ) ;
               DosFindClose ( hDir2 ) ;
            } /* endif */
         } else {
            TotalSize += Found->cbFileAlloc ;
         } /* endif */
      } while ( !DosFindNext ( hDir, Found, PathSize+sizeof(FILEFINDBUF3), &Count ) ) ;
      DosFindClose ( hDir ) ;
   } /* endif */
   free ( Found ) ;
   FormatValue ( TotalSize, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;

#endif // __OS2__

#ifdef __OS2__

   // CPU Load
   ResourceString CpuLoadLabel ( Library.QueryHandle(), IDS_CPULOAD ) ;
   Column = printf ( "%s", PSZ(CpuLoadLabel) ) ;
   #ifndef QSV_NUMPROCESSORS
      #define QSV_NUMPROCESSORS (26)
   #endif
   ULONG CpuCount ( 1 ) ;
   if ( DosQuerySysInfo ( QSV_NUMPROCESSORS, QSV_NUMPROCESSORS, &CpuCount, sizeof(CpuCount) ) )
      if ( ( CpuCount < 1 ) || ( CpuCount > 64 ) )
         CpuCount = 1 ;
   HMODULE ModuleHandle ;
   APIRET Status = DosLoadModule ( 0, 0, "DOSCALL1.DLL", &ModuleHandle ) ;
   APIRET APIENTRY (*DosPerfSysCall)(ULONG ulCommand, ULONG ulParm1, ULONG ulParm2, ULONG ulParm3) = 0 ;
   if ( !Status ) {
      Status = DosQueryProcAddr ( ModuleHandle, ORD_DOS32PERFSYSCALL, "", (PFN*)&DosPerfSysCall ) ;
      if ( Status ) 
         DosPerfSysCall = 0 ;
   } /* endif */
   ULONG Load ( 0 ) ;
   if ( DosPerfSysCall ) {
      CPUUTIL *Previous ( (CPUUTIL*) malloc ( sizeof(CPUUTIL) * CpuCount ) ) ;
      CPUUTIL *Current ( (CPUUTIL*) malloc ( sizeof(CPUUTIL) * CpuCount ) ) ;
      DosPerfSysCall ( CMD_KI_ENABLE, 0, 0, 0 ) ;
      DosPerfSysCall ( CMD_KI_RDCNT, (ULONG)Previous, 0, 0 ) ;
      DosSleep ( 1000 ) ;
      DosPerfSysCall ( CMD_KI_RDCNT, (ULONG)Current, 0, 0 ) ;
      double DeltaTime(0.0), IdleTime(0.0) ;
      for ( int i=0; i<CpuCount; i++ ) {
         DeltaTime += LL2F(Current[i].ulTimeHigh,Current[i].ulTimeLow) - LL2F(Previous[i].ulTimeHigh,Previous[i].ulTimeLow) ;
         IdleTime += LL2F(Current[i].ulIdleHigh,Current[i].ulIdleLow) - LL2F(Previous[i].ulIdleHigh,Previous[i].ulIdleLow) ;
      } /* endfor */
      Load = ULONG ( DeltaTime ? ULONG ( ( (DeltaTime-IdleTime) / DeltaTime ) * 100.0 + 0.49 ) : 0L ) ;
   	if ( Previous )
      	free ( Previous ), Previous = 0 ;
   	if ( Current )
      	free ( Current ), Current = 0 ;
  	} else {
      // If we have an old version of OS/2, we have to do this the hard way.
      COUNTER_PARMS Parms ;
      ULONG MaxLoad = CalibrateLoadMeter ( &Parms ) ; 
      CounterThreadEvent.Reset ( ) ;
      Parms.Active = TRUE ;
      TID tidCounter = StartThread ( "CounterThread", CounterThread, 0x3000, &Parms ) ;
      Sys_SuspendThread ( tidCounter ) ;
      Thread_SetPriority ( tidCounter, Priority(PRIORITY_IDLE) ) ;
      Parms.Counter = 0 ;
      QWORD Time0 ; DosTmrQueryTime ( &Time0 ) ;
      Sys_ResumeThread ( tidCounter ) ;
      Sys_Sleep ( 1000 ) ;
      Parms.Active = FALSE ;
      Thread_SetPriority ( tidCounter, Priority(PRIORITY_HIGHEST) ) ;
      CounterThreadEvent.Wait ( 10000 ) ; 
      QWORD Time1 ; DosTmrQueryTime ( &Time1 ) ;
      __int64 TimerTicks ( ( (__int64)Time1.ulHi << 32 ) + (__int64)Time1.ulLo ) ;
      TimerTicks -= ( (__int64)Time0.ulHi << 32 ) + (__int64)Time0.ulLo ; 
      ULONG TimerFrequency ; DosTmrQueryFreq ( &TimerFrequency ) ; 
      double TimeSpent ( double(TimerTicks) / double(TimerFrequency) ) ; 
      ULONG AdjustedLoad ( ULONG ( double(Parms.Counter) / TimeSpent ) ) ; 
      Load = ( ( MaxLoad - AdjustedLoad ) * 100 ) / (MaxLoad?MaxLoad:1) ;
   } /* endif */
   sprintf ( Text, "%lu%%", Load ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;

#endif // __OS2__

#ifdef __OS2__

   // Battery Status

   HFILE Handle ;  ULONG Action ;
   if ( !DosOpen ( (PSZ) "APM$", &Handle, &Action, 0, FILE_NORMAL, FILE_OPEN,
         OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYREADWRITE | OPEN_FLAGS_FAIL_ON_ERROR, 0 ) ) {

      ResourceString BatteryStatus ( Library.QueryHandle(), IDS_BATTERY ) ;
      Column = printf ( "%s", PSZ(BatteryStatus) ) ;

      struct {
         USHORT ParmLength ;
         USHORT PowerFlags ;
         UCHAR  ACStatus ;
         UCHAR  BatteryStatus ;
         UCHAR  BatteryLife ;
      } Parm ;
      Parm.ParmLength = sizeof(Parm) ;
      ULONG ParmLen = sizeof(Parm) ;
      USHORT Data ;
      ULONG DataLen = sizeof(Data) ;
      if ( !DosDevIOCtl ( Handle, IOCTL_POWER, POWER_GETPOWERSTATUS, &Parm, sizeof(Parm), &ParmLen, &Data, sizeof(Data), &DataLen ) ) {
         if ( Parm.PowerFlags & 1 ) {                       
            if ( Parm.BatteryStatus == 3 ) {                
               if ( Parm.BatteryLife < 100 ) {               
                  ResourceString Charging ( Library.QueryHandle(),IDS_APM_CHARGING ) ;
                  sprintf ( Text, "%s ", PCHAR(Charging) ) ;
               } else {                                        
                  sprintf ( Text, "%lu%%", 100 ) ;
               } /* endif */
            } else if ( Parm.ACStatus == 1 ) {              
               ResourceString AC ( Library.QueryHandle(),IDS_APM_AC ) ;
               sprintf ( Text, "%s ", PCHAR(AC) ) ;
            } else if ( Parm.BatteryLife <= 100 ) {         
               sprintf ( Text, "%lu%%", Parm.BatteryLife ) ;
            } else {                                        
               ResourceString Error ( Library.QueryHandle(),IDS_APM_ERROR ) ;
               sprintf ( Text, "%s ", PCHAR(Error) ) ;
            } /* endif */                                   
         } else {
            ResourceString Error ( Library.QueryHandle(),IDS_APM_ERROR ) ;
            sprintf ( Text, "%s ", PCHAR(Error) ) ;
         } /* endif */
      } else {
         ResourceString Error ( Library.QueryHandle(),IDS_APM_ERROR ) ;
         sprintf ( Text, "%s ", PCHAR(Error) ) ;
      } /* endif */

      DosClose ( Handle ) ;

      printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
      printf ( "\n" ) ;

   } /* endif */

#endif // __OS2__

#ifdef __OS2__

   // Active Task Count

   ResourceString ActiveTaskCount ( Library.QueryHandle(), IDS_TASKCOUNT ) ;
   Column = printf ( "%s", PSZ(ActiveTaskCount) ) ;
   ULONG TaskCount = WinQuerySwitchList ( 0, 0, 0 ) ;
   sprintf ( Text, "%i ", TaskCount ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;

#endif // __OS2__

   // Process Count

   ResourceString ProcessCount ( Library.QueryHandle(), IDS_PROCESSCOUNT ) ;
   Column = printf ( "%s", PSZ(ProcessCount) ) ;
   int ProcCount ( 0 ) ;

#ifdef __OS2__

   PULONG DQSS_Buffer ;
   DosAllocMem ( (PPVOID)&DQSS_Buffer, DQSS_BUFFER_SIZE, PAG_READ | PAG_WRITE | PAG_COMMIT ) ;
   if ( !DosQuerySysState ( 0x17, 0, 0, 0, DQSS_Buffer, DQSS_BUFFER_SIZE ) ) {
      QPROCESS *pProcRecord = ((QTOPLEVEL*)DQSS_Buffer)->procdata ;
      while ( pProcRecord->rectype == 1 ) {
         ProcCount ++ ;
         char *p ( (char*) ( pProcRecord->threads+pProcRecord->threadcnt ) ) ;
         pProcRecord = (QPROCESS*) p ;
      } /* endfor */
   } /* endif */
   DosFreeMem ( DQSS_Buffer ) ;

#else // __NT__

   // Try to make use of ToolHelp32 . . .
   if ( pCreateToolhelp32Snapshot && pProcess32First && pProcess32Next ) {
      HANDLE hSnapShot ( pCreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 ) ) ;
      if ( hSnapShot != INVALID_HANDLE_VALUE ) {
      	PROCESSENTRY32 Entry = { 0 } ;
      	Entry.dwSize = sizeof(Entry) ;
      	if ( pProcess32First ( hSnapShot, &Entry ) ) {
         	do {
					ProcCount ++ ;
            	Entry.dwSize = sizeof(Entry) ;
         	} while ( pProcess32Next ( hSnapShot, &Entry ) ) ;
      	} /* endif */
	      CloseHandle ( hSnapShot ) ;
      } /* endif */
      
   // Else, try for PSAPI (NT 4.0 and earlier).
   } else if ( pEnumProcesses && pEnumProcessModules && pGetModuleFileNameEx && pVDMEnumTaskWOWEx ) {
      DWORD *pPIDs ( 0 ) ;
      DWORD SizeOffered ( 256 * sizeof(DWORD) ) ;
      DWORD SizeReturned ;
      do {
         if ( pPIDs ) {
            HeapFree ( GetProcessHeap(), 0, pPIDs ) ;
            SizeOffered *= 2 ;
         } /* endif */
         pPIDs = (DWORD*) HeapAlloc ( GetProcessHeap(), 0, SizeOffered ) ;
         if ( pPIDs == NULL ) 
				break ;
         if ( !pEnumProcesses ( pPIDs, SizeOffered, &SizeReturned ) ) 
            break ;
      } while ( SizeOffered == SizeReturned ) ;
      HeapFree ( GetProcessHeap(), 0, pPIDs ) ;
	   ProcCount = SizeReturned / sizeof(DWORD) ;
	} /* endif */

#endif // __OS2__ vs __NT__

   sprintf ( Text, "%i ", ProcCount ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;

#ifdef __OS2__

   // Thread Count

   ResourceString ThreadCount ( Library.QueryHandle(), IDS_THREADCOUNT ) ;
   Column = printf ( "%s", PSZ(ThreadCount) ) ;
   int ThrdCount ( 0 ) ;
   DosAllocMem ( (PPVOID)&DQSS_Buffer, DQSS_BUFFER_SIZE, PAG_READ | PAG_WRITE | PAG_COMMIT ) ;
   if ( !DosQuerySysState ( 0x17, 0, 0, 0, DQSS_Buffer, DQSS_BUFFER_SIZE ) ) 
      ThrdCount = int ( ((QTOPLEVEL*)DQSS_Buffer)->gbldata->threadcnt ) ;
   DosFreeMem ( DQSS_Buffer ) ;
   sprintf ( Text, "%i ", ThrdCount ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;

#endif // __OS2__


   // Total Free Space

   ResourceString TotalFree ( Library.QueryHandle(), IDS_TOTALFREE ) ;
   Column = printf ( "%s", PSZ(TotalFree) ) ;
   ULONG Drive, Drives ;
   #ifdef __OS2__
      DosQueryCurrentDisk ( &Drive, &Drives ) ;
   #else // __NT__
      Drives = GetLogicalDrives ( ) ;
   #endif // __OS2__ vs __NT__
   ULONG Free = 0 ;
   ULONG Mask = Drives >> 2 ;
   for ( Drive=3; Drive<=26; Drive++ ) {
      if ( Mask & 1 ) {
         char FileSystem [80] = { 0 } ;
         char DiskLabel [80] = { 0 } ;
         if ( CheckDrive ( Drive, FileSystem, sizeof(FileSystem), DiskLabel, sizeof(DiskLabel) ) > 0 ) {
            unsigned long SectorsPerCluster, BytesPerSector, FreeClusters ;
            #ifdef __OS2__
               DosError ( FERR_DISABLEHARDERR ) ;
               FSALLOCATE Allocation ;
               APIRET Status = DosQueryFSInfo ( Drive, FSIL_ALLOC, (PBYTE)&Allocation, sizeof(Allocation) ) ;
               DosError ( FERR_ENABLEHARDERR ) ;
               SectorsPerCluster = Allocation.cSectorUnit ;
               BytesPerSector = Allocation.cbSector ;
               FreeClusters = Allocation.cUnitAvail ;
            #else // __NT__
               char DriveName [10] ; unsigned long TotalClusters ;
               sprintf ( DriveName, "%c:\\", Drive+'A'-1 ) ;
               int Status = !GetDiskFreeSpace ( DriveName, &SectorsPerCluster, &BytesPerSector, &FreeClusters, &TotalClusters ) ;
            #endif // __OS2__ vs __NT__
            if ( Status ) {
               Drives &= ~ ( 1 << (Drive-1) ) ;
            } else {
               Free += FreeClusters * SectorsPerCluster * ( BytesPerSector / 256 ) ;
            } /* endif */
         } else {
            Drives &= ~ ( 1 << (Drive-1) ) ;
         } /* endif */
      } /* endif */
      Mask >>= 1 ;
   } /* endfor */
   FormatValue ( Free, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG, 256 ) ;
   printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
   printf ( "\n" ) ;


   // Drive Space Free

   #ifdef __OS2__
      DosQueryCurrentDisk ( &Drive, &Drives ) ;
   #else // __NT__
      Drives = GetLogicalDrives ( ) ;
   #endif // __OS2__ vs __NT__
   ResourceString DriveFree ( Library.QueryHandle(), IDS_DRIVE_FREE ) ;
   BOOL ShowFileSystemName ( IniData.ShowFileSystemNames ) ;
   BOOL ShowDiskLabel ( IniData.ShowDiskLabels ) ;
   Mask = Drives >> 2 ;
   for ( Drive=3; Drive<=26; Drive++ ) {
      if ( Mask & 1 ) {
         char FileSystem [80] = { 0 } ;
         char DiskLabel [80] = { 0 } ;
         if ( CheckDrive ( Drive, FileSystem, sizeof(FileSystem), DiskLabel, sizeof(DiskLabel) ) ) {
            unsigned long SectorsPerCluster, BytesPerSector, FreeClusters ;
            #ifdef __OS2__
               DosError ( FERR_DISABLEHARDERR ) ;
               FSALLOCATE Allocation ;
               APIRET Status = DosQueryFSInfo ( Drive, FSIL_ALLOC, (PBYTE)&Allocation, sizeof(Allocation) ) ;
               DosError ( FERR_ENABLEHARDERR ) ;
               SectorsPerCluster = Allocation.cSectorUnit ;
               BytesPerSector = Allocation.cbSector ;
               FreeClusters = Allocation.cUnitAvail ;
            #else // __NT__
               char DriveName [10] ; unsigned long TotalClusters ;
               sprintf ( DriveName, "%c:\\", Drive+'A'-1 ) ;
               int Status = !GetDiskFreeSpace ( DriveName, &SectorsPerCluster, &BytesPerSector, &FreeClusters, &TotalClusters ) ;
            #endif // __OS2__ vs __NT__
            if ( Status ) {
               Drives &= ~ ( 1 << (Drive-1) ) ;
            } else {
               char Label [80] ;
               sprintf ( Label, PCHAR(DriveFree), Drive+'A'-1 ) ;
               if ( ShowFileSystemName && *FileSystem ) {
                  if ( ShowDiskLabel && *DiskLabel ) {
                     Column = printf ( "%s (%s,%s)", Label, DiskLabel, FileSystem ) ;
                  } else {
                     Column = printf ( "%s (%s)", Label, FileSystem ) ;
                  } /* endif */
               } else {
                  if ( ShowDiskLabel && *DiskLabel ) {
                     Column = printf ( "%s (%s)", Label, DiskLabel ) ;
                  } else {
                     Column = printf ( "%s", Label ) ;
                  } /* endif */
               } /* endif */
               ULONG Free = FreeClusters * SectorsPerCluster * ( BytesPerSector / 256 ) ;
               FormatValue ( Free, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG, 256 ) ;
               printf ( "%*s%s", 79-Column-strlen(Text), "", Text ) ;
               printf ( "\n" ) ;
            } /* endif */
         } else {
            Drives &= ~ ( 1 << (Drive-1) ) ;
         } /* endif */
      } /* endif */
      Mask >>= 1 ;
   } /* endfor */

   // Close up system services previously opened up.
   #ifdef __OS2__
      if ( hTheseus ) {
         pT2ReleaseHandle0 ( &T2Handle0 ) ;
         pT2Terminate0 ( ) ;
         DosFreeModule ( hTheseus ) ;
      } /* endif */
   #else // __NT__
   	if ( hKernel32 ) 
      	FreeLibrary ( hKernel32 ) ;
	   if ( hPSAPI )
   	   FreeLibrary ( hPSAPI ) ;
	   if ( hWOW ) 
   	   FreeLibrary ( hWOW ) ;
   #endif // __OS2__

   UNREGISTER_EXCEPTION_HANDLER();

   return ( 0 ) ;
}

inline ULONG KiloScale ( ULONG Value, int ShowTrueK=TRUE, int Scale=1 ) {
   if ( ShowTrueK ) {
      return ( (Value+(0x200/Scale))/(0x400/Scale) ) ;
   } else {
      return ( (Value+(500/Scale))/(1000/Scale) ) ;
   } /* endif */
}

inline ULONG MegaScale ( ULONG Value, int ShowTrueK=TRUE, int Scale=1 ) {
   if ( ShowTrueK ) {
      return ( (Value+(0x80000/Scale))/(0x100000/Scale) ) ;
   } else {
      return ( (Value+(500000/Scale))/(1000000/Scale) ) ;
   } /* endif */
}

inline ULONG KiloScale ( __int64 Value, int ShowTrueK=TRUE, int Scale=1 ) {
   if ( ShowTrueK ) {
      return ( ULONG ( (Value+(0x200/Scale))/(0x400/Scale) ) ) ;
   } else {
      return ( ULONG ( (Value+(500/Scale))/(1000/Scale) ) ) ;
   } /* endif */
}

inline ULONG MegaScale ( __int64 Value, int ShowTrueK=TRUE, int Scale=1 ) {
   if ( ShowTrueK ) {
      return ( ULONG ( (Value+(0x80000/Scale))/(0x100000/Scale) ) ) ;
   } else {
      return ( ULONG ( (Value+(500000/Scale))/(1000000/Scale) ) ) ;
   } /* endif */
}

inline ULONG GigaScale ( __int64 Value, int ShowTrueK=TRUE, int Scale=1 ) {
   if ( ShowTrueK ) {
      return ( ULONG ( (Value+(0x20000000/Scale))/(0x40000000/Scale) ) ) ;
   } else {
      return ( ULONG ( (Value+(500000000/Scale))/(1000000000/Scale) ) ) ;
   } /* endif */
}

static void FormatValue ( ULONG Value, char Text[], CountryInfo &Locale, int ShowK, int ShowTrueK, int ShowM, int ShowG, int Scale ) {

   __int64 GigaBreak = ShowTrueK ? __int64(0x100000000)/Scale : __int64(4000000000)/Scale ;
   __int64 MegaBreak = ShowTrueK ? __int64(0x800000)/Scale : __int64(8000000)/Scale ;
   __int64 KiloBreak = ShowTrueK ? __int64(0x80000)/Scale : __int64(500000)/Scale ;
   __int64 FullValue = Value * __int64(Scale) ;

   switch ( ShowK ) {
      case SHOWK_NEVER:
         sprintf ( Text, "%Lu", FullValue ) ;
         break;
      case SHOWK_ALWAYS:
         if ( !ShowM || ( Value < MegaBreak ) )
            sprintf ( Text, "%lu", KiloScale(Value,ShowTrueK,Scale) ) ;
         else if ( !ShowG || ( Value < GigaBreak ) ) 
            sprintf ( Text, "%lu", MegaScale(Value,ShowTrueK,Scale) ) ;
         else
            sprintf ( Text, "%lu", GigaScale(Value,ShowTrueK,Scale) ) ;
         break;
      case SHOWK_ABOVE512:
      default:
         if ( Value < KiloBreak )
            sprintf ( Text, "%Lu", FullValue ) ;
         else if ( !ShowM || ( Value < MegaBreak ) )
            sprintf ( Text, "%lu", KiloScale(Value,ShowTrueK,Scale) ) ;
         else if ( !ShowG || ( Value < GigaBreak ) ) 
            sprintf ( Text, "%lu", MegaScale(Value,ShowTrueK,Scale) ) ;
         else
            sprintf ( Text, "%lu", GigaScale(Value,ShowTrueK,Scale) ) ;
         break;
   } /* endswitch */

   char Work[100] ;
   char *p1 = Text ;
   char *p2 = Work ;
   while ( *p1 ) {
      *p2 = *p1 ;
      p1 ++ ;
      p2 ++ ;
      if ( *p1 ) {
         if ( strlen(p1) % 3 == 0 ) {
            *p2 = Locale.szThousandsSeparator [0] ;
            p2 ++ ;
         } /* endif */
      } /* endif */
   } /* endwhile */
   *p2 = 0 ;
   strcpy ( Text, Work ) ;

   p2 = Text + strlen(Text) ;
   *(p2+1) = 0 ;
   switch ( ShowK ) {
      case SHOWK_NEVER:
         *p2 = ' ' ;
         break;
      case SHOWK_ALWAYS:
         if ( !ShowM || ( Value < MegaBreak ) )
            *p2 = (char) ( ShowTrueK ? 'K' : 'k' ) ;
         else if ( !ShowG || ( Value < GigaBreak ) )
            *p2 = (char) ( ShowTrueK ? 'M' : 'm' ) ;
         else
            *p2 = (char) ( ShowTrueK ? 'G' : 'g' ) ;
         break;
      case SHOWK_ABOVE512:
      default:
         if ( Value < KiloBreak )
            *p2 = ' ' ;
         else if ( !ShowM || ( Value < MegaBreak ) )
            *p2 = (char) ( ShowTrueK ? 'K' : 'k' ) ;
         else if ( !ShowG || ( Value < GigaBreak ) ) 
            *p2 = (char) ( ShowTrueK ? 'M' : 'm' ) ;
         else
            *p2 = (char) ( ShowTrueK ? 'G' : 'g' ) ;
         break;
   } /* endswitch */
}

#ifdef __OS2__

extern PSZ ScanSystemConfig ( HAB Anchor, PSZ Keyword ) {

   ULONG BootDrive ;
   DosQuerySysInfo ( QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &BootDrive, sizeof(BootDrive) ) ;

   WinUpper ( Anchor, 0, 0, Keyword ) ;

   char Path [_MAX_PATH] ;
   Path[0] = (char) ( BootDrive + 'A' - 1 ) ;
   Path[1] = 0 ;
   strcat ( Path, ":\\CONFIG.SYS" ) ;

   FILE *File = fopen ( Path, "r" ) ;
   if ( NOT File ) {
      return ( 0 ) ;
   } /* endif */

   static char Buffer [500] ;
   while ( fgets ( Buffer, sizeof(Buffer), File ) ) {

      if ( Buffer[strlen(Buffer)-1] == '\n' ) 
         Buffer[strlen(Buffer)-1] = 0 ;

      WinUpper ( Anchor, 0, 0, PSZ(Buffer) ) ;

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

   fclose ( File ) ;

   return ( 0 ) ;
}

static char *CopyString ( char *Buffer, char *Original ) {
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

#endif // __OS2__

