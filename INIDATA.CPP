/**************************************************************** INIDATA.CPP
 *                                                                          *
 *                          Profile Data Object                             *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Config.h"
#include "IniData.h"
#include "Thread.h"

/****************************************************************************
 *                                                                          *
 *  _IniData: Constructor                                                   *
 *                                                                          *
 ****************************************************************************/

_IniData::_IniData ( ) {
   Reset ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  _IniData: Reset Defaults                                                *
 *                                                                          *
 ****************************************************************************/

void _IniData::Reset ( ) {

   fPosition = FALSE ;

   HideControls = FALSE ;
   fHideControls = FALSE ;

   Float = FALSE ;
   fFloat = FALSE ;

   Animate = FALSE ;
   fAnimate = FALSE ;

   DialogBorder = FALSE ;
   fDialogBorder = FALSE ;

   TableFormat = FALSE ;
   fTableFormat = FALSE ;

   Chime = FALSE ;
   fChime = FALSE ;

   ShowDiskDrives = TRUE ;
   fShowDiskDrives = FALSE ;

   ShowRemoteDrives = TRUE ;
   fShowRemoteDrives = FALSE ;

   ShowFileSystemNames = TRUE ;
   fShowFileSystemNames = FALSE ;

   ShowDiskLabels = TRUE ;
   fShowDiskLabels = FALSE ;

   ShowSeconds = FALSE ;
   fShowSeconds = FALSE ;

   UseTheseus = FALSE ;
   fUseTheseus = FALSE ;

   CenterClock = TRUE ;
   fCenterClock = FALSE ;

   ShowK = SHOWK_ABOVE512 ;
   fShowK = FALSE ;

   ShowTrueK = TRUE ;
   fShowTrueK = FALSE ;

   ShowM = TRUE ;
   fShowM = FALSE ;

   ShowG = TRUE ;
   fShowG = FALSE ;

   MonitorPriority = PRIORITY_TIME_CRITICAL ;
   fMonitorPriority = FALSE ;

   TimerInterval = 1000 ;
   fTimerInterval = FALSE ;

   AveragingInterval = 60 ;
   fAveragingInterval = FALSE ;

   AnchorCorner = CORNER_BL ;
   fAnchorCorner = FALSE ;

   strcpy ( PCHAR(FontNameSize), "" ) ;
   fFontNameSize = FALSE ;

   #ifdef __OS2__
      BackColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOW, 0 ) ;
   #else // __NT__
      BackColor = 0xFFFFFF ;
   #endif // __OS2__ vs __NT__
   fBackColor = FALSE ;

   #ifdef __OS2__
      TextColor = WinQuerySysColor ( HWND_DESKTOP, SYSCLR_OUTPUTTEXT, 0 ) ;
   #else // __NT__
      BackColor = 0x000000 ;
   #endif // __OS2__ vs __NT__
   fTextColor = FALSE ;

   WarningBackground = 0xFFFF00 ;
   fWarningBackground = FALSE ;

   WarningForeground = 0x000000 ;
   fWarningForeground = FALSE ;

   ErrorBackground = 0xFF0000 ;
   fErrorBackground = FALSE ;

   ErrorForeground = 0xFFFFFF ;
   fErrorForeground = FALSE ;

   CountryInfo Locale ;
   strcpy ( DateFormat, "%a, " ) ;
   switch ( Locale.fsDateFmt ) {
      case DATEFMT_MM_DD_YY:
         sprintf ( DateFormat+strlen(DateFormat), "%%m%s%%d%s%%y ", 
            Locale.szDateSeparator, Locale.szDateSeparator ) ;
         break ;
      case DATEFMT_DD_MM_YY:
         sprintf ( DateFormat+strlen(DateFormat), "%%d%s%%m%s%%y ", 
            Locale.szDateSeparator, Locale.szDateSeparator ) ;
         break ;
      case DATEFMT_YY_MM_DD:
         sprintf ( DateFormat+strlen(DateFormat), "%%y%s%%m%s%%d ", 
            Locale.szDateSeparator, Locale.szDateSeparator ) ;
         break ;
   } /* endswitch */
   if ( Locale.fsTimeFmt ) {
      sprintf ( DateFormat+strlen(DateFormat), "%%H%s%%M%s%%S %%z", 
         Locale.szTimeSeparator, Locale.szTimeSeparator ) ;
   } else {
      sprintf ( DateFormat+strlen(DateFormat), "%%I%s%%M%s%%S%%p %%z", 
         Locale.szTimeSeparator, Locale.szTimeSeparator ) ;
   } /* endif */

   memset ( PopServer, 0, sizeof(PopServer) ) ;
   memset ( PopUserName, 0, sizeof(PopUserName) ) ;
   memset ( PopPassword, 0, sizeof(PopPassword) ) ;
   memset ( PopReader, 0, sizeof(PopReader) ) ;
   strcpy ( PopReader, "netscape -mail -l en_US" ) ;
   memset ( PopWavFile, 0, sizeof(PopWavFile) ) ;
   #ifdef __OS2__
      ULONG BootDrive ( 3 ) ;
      DosQuerySysInfo ( QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &BootDrive, sizeof(BootDrive) ) ;
      sprintf ( PopWavFile, "%c:\\mmos2\\sounds\\charge.wav", BootDrive+'a'-1 ) ;
   #endif // __OS2__
   PopPort = 0 ;
   PopForceIP = FALSE ;
   PopUseAPOP = TRUE ;
   PopInterval = 60 ;
   PopAlert = TRUE ;

} /* endmethod */

