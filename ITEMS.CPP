/****************************************************************** ITEMS.CPP
 *                                                                          *
 *                     Display Item Class Functions                         *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#ifdef __OS2__
   #define INCL_MACHDR
   #define INCL_MCIOS2
   #include <os2me.h>
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Config.h"
#include "Country.h"
#include "DateFmt.h"
#include "Debug.h"
#include "DosQSS.h"
#include "Items.h"
#include "MemSize.h"
#include "Pop.h"
#include "Profile.h"
#include "Support.h"
#include "ReString.h"

#ifdef __OS2__
   static void EXPENTRY TheseusErrorHandler ( T2HANDLE0, ULONG, ULONG, ULONG ) { }
#endif // __OS2__

#ifdef __OS2__
static int MeasureValue ( char *Text, char *Label, BOOL ShowK, BOOL ShowM, BOOL /*ShowG*/ ) {
   int Result ;
   if ( ShowK == SHOWK_NEVER ) {
      Result = sprintf ( Text, "%s 1,234,567,890 ", Label ) ;
   } else if ( !ShowM ) {
      Result = sprintf ( Text, "%s 1,234,567K", Label ) ;
   } else {
      Result = sprintf ( Text, "%s 1,234M", Label ) ;
   } /* endif */
   return ( Result ) ;
}
#endif // __OS2__

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

#ifdef __OS2__
static void FormatValue ( __int64 Value, char Text[], CountryInfo &Locale, int ShowK=SHOWK_ABOVE512, int ShowTrueK=TRUE, int ShowM=TRUE, int ShowG=TRUE, int Scale=1 ) {

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
#endif // __OS2__

Item::Item ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel, void *ddeserver, char *topic ) :
   Id(id), Flag(TRUE), Value(0), pDdeServer(ddeserver) {
   strcpy ( Name, pName ) ;
   strcpy ( CurrentLabel, pCurrentLabel ) ;
   strcpy ( DefaultLabel, pDefaultLabel ) ;
   strcpy ( Topic, topic ) ;
   #ifdef __OS2__
      ((Dde_Server*)pDdeServer)->AddItem ( Topic, DefaultLabel, DDEFMT_TEXT, "", 1 ) ;
      pDdeItem = ((Dde_Server*)pDdeServer)->FindItem ( Topic, DefaultLabel ) ;
   #endif // __OS2__
   NormalColors [0] = 0xFFFFFF ;
   NormalColors [1] = 0x000000 ;
   WarningColors[0] = 0xFFFF00 ;
   WarningColors[1] = 0x000000 ;
   ErrorColors  [0] = 0xFF0000 ;
   ErrorColors  [1] = 0xFFFFFF ;
} /* endmethod */

Item::~Item ( ) {
   #ifdef __OS2__
      ((Dde_Server*)pDdeServer)->RemoveItem ( Topic, DefaultLabel ) ;
   #endif // __OS2__
} /* endmethod */

void Item::GetProfile ( Profile &Prf ) {

   Flag = TRUE ;
   Prf.GetItem ( Name, &Flag, sizeof(Flag) ) ;

   char Tag [80] ; strcpy ( Tag, PCHAR(Name) ) ; strcat ( Tag, " Label" ) ;
   Prf.GetItem ( Tag, CurrentLabel, sizeof(CurrentLabel) ) ;

   strcpy ( Tag, PCHAR(Name) ) ; strcat ( Tag, " Sense Enabled" ) ;
   Prf.GetItem ( Tag, &SenseEnabled, sizeof(SenseEnabled) ) ;

   strcpy ( Tag, PCHAR(Name) ) ; strcat ( Tag, " Warning" ) ;
   Prf.GetItem ( Tag, &WarningLevel, sizeof(WarningLevel) ) ;

   strcpy ( Tag, PCHAR(Name) ) ; strcat ( Tag, " Error" ) ;
   Prf.GetItem ( Tag, &ErrorLevel, sizeof(ErrorLevel) ) ;

} /* endmethod */

void Item::PutProfile ( Profile &Prf ) {

   Prf.PutItem ( Name, &Flag, sizeof(Flag) ) ;

   // Save custom label, if any.
   char Tag [80] ; strcpy ( Tag, PCHAR(Name) ) ; strcat ( Tag, " Label" ) ;
   if ( strcmp ( CurrentLabel, DefaultLabel ) ) {
      Prf.PutItem ( Tag, CurrentLabel, sizeof(CurrentLabel) ) ;
   } else {
      Prf.Reset ( Tag ) ;
   } /* endif */

   // If the item uses a warning/error threshold at all . . .
   if ( LevelSense ) {

      // Save the warning threshold value.
      strcpy ( Tag, PCHAR(Name) ) ; strcat ( Tag, " Sense Enabled" ) ;
      Prf.PutItem ( Tag, &SenseEnabled, sizeof(SenseEnabled) ) ;

      // Save the warning threshold value.
      __int64 Threshold = WarningLevel ; strcpy ( Tag, PCHAR(Name) ) ; strcat ( Tag, " Warning" ) ;
      Prf.PutItem ( Tag, &Threshold, sizeof(Threshold) ) ;

      // Save the error threshold value.
      Threshold = ErrorLevel ; strcpy ( Tag, PCHAR(Name) ) ; strcat ( Tag, " Error" ) ;
      Prf.PutItem ( Tag, &Threshold, sizeof(Threshold) ) ;

   } /* endif */

} /* endmethod */

void Item::FormatLine ( char *Buffer, int MaxWidth ) {

   // Get the left and right texts for the line.
   char Label[100], Text[100] ;
   FormatText ( Label, Text, QueryValue() ) ;

   // Format the line.
   strcat ( Buffer, Label ) ;
   sprintf ( Buffer+strlen(Buffer), "%*s", MaxWidth-strlen(Label)-strlen(Text), "" ) ;
   strcat ( Buffer, Text ) ;
   strcat ( Buffer, "\r\n" ) ;
}


void Item::DdeUpdate ( char *Text ) {

#ifdef __OS2__

   int Format, Size ; PVOID Data ;
   pDdeItem->Query ( Format, Data, Size ) ;

   int NewSize = strlen(Text) + 1 ;
   if ( Data && memcmp ( Text, Data, NewSize ) )
      pDdeItem->Update ( DDEFMT_TEXT, Text, NewSize ) ;

#else

   Text = 0 ;  // Pacify the compiler.

#endif // __OS2__

}


void Item::Paint ( Canvas &canvas, RECTANGLE &Rectangle,
   char *Label, char *Text, __int64 NewValue, int Scale ) {

   ULONG *Colors = NormalColors ;
   if ( SenseEnabled ) {
      if ( LevelSense > 0 ) {
         if ( NewValue >= ErrorLevel/Scale ) {
            Colors = ErrorColors ;
         } else if ( NewValue >= WarningLevel/Scale ) {
            Colors = WarningColors ;
         } /* endif */
      } else if ( LevelSense < 0 ) {
         if ( NewValue /* *Scale */ <= ErrorLevel/Scale ) {
            Colors = ErrorColors ;
         } else if ( NewValue <= WarningLevel/Scale ) {
            Colors = WarningColors ;
         } /* endif */
      } /* endif */
   } /* endif */

   canvas.DrawText ( Text, Rectangle, Colors[1], Colors[0], canvas.HP_RIGHT, canvas.VP_BOTTOM, TRUE ) ;
   canvas.DrawText ( Label, Rectangle, Colors[1], Colors[0], canvas.HP_LEFT, canvas.VP_BOTTOM ) ;

   Value = NewValue ;
}


Clock::Clock ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic, char *dateformat, BOOL centered )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ) {
   LevelSense = 0 ;
   SenseEnabled = FALSE ;
   strcpy ( DateFormat, dateformat ) ;
   CenterClock = centered ;
} /* endmethod */

int Clock::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   char Text [100] ;
   FormatDate ( DateFormat, Text, sizeof(Text) ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( strlen(Text) ) ;
}

__int64 Clock::NewValue ( void ) {
   return ( time ( 0 ) ) ;
}


void Clock::FormatText ( char *Label, char *Text, __int64 value ) {
   strcpy ( Label, CurrentLabel ) ;
   FormatDate ( DateFormat, Text, 100, (time_t)value ) ;
   DdeUpdate ( Text ) ;
}


void Clock::FormatLine ( char *Buffer, int MaxWidth ) {

   // Get the left and right texts for the line.
   char Label[100], Text[100] ;
   FormatText ( Label, Text, QueryValue() ) ;

   // Format the line.
   if ( CenterClock ) {
      sprintf ( Buffer+strlen(Buffer), "%*s%s%*s",
         (MaxWidth-strlen(Text))/2, "", Text,
         MaxWidth-strlen(Text)-(MaxWidth-strlen(Text))/2, "" ) ;
   } else {
      sprintf ( Buffer+strlen(Buffer), "%s%*s",
         Text, MaxWidth-strlen(Text), "" ) ;
   } /* endif */
   strcat ( Buffer, "\r\n" ) ;
}


void Clock::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Time = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Time != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Time ) ;
      canvas.DrawText ( Text, Rectangle, NormalColors[1], NormalColors[0], CenterClock?canvas.HP_CENTER:canvas.HP_LEFT, canvas.VP_BOTTOM, TRUE ) ;
      Value = Time ;
   } /* endif */
}


#ifdef __OS2__

ElapsedTime::ElapsedTime ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic, ResourceString *day, ResourceString *days, BOOL showseconds, PSZ SwapPath )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   Day(day), Days(days), ShowSeconds(showseconds), Offset(0), Wrapped(0) {
   LevelSense = 0 ;
   SenseEnabled = FALSE ;

   char Path [_MAX_PATH+1] ;
   strcpy ( Path, (PCHAR)SwapPath ) ;
   if ( Path[strlen(Path)-1] != '\\' )
      strcat ( Path, "\\" ) ;
   strcat ( Path, "SWAPPER.DAT" ) ;
   ULONG SwapSize = 0 ;
   FILESTATUS3 Status ;
   if ( DosQueryPathInfo ( (PSZ)Path, FIL_STANDARD, &Status, sizeof(Status) ) == 0 ) {
      struct tm Tm = { 0 } ;
      Tm.tm_year = Status.fdateLastWrite.year + 80 ;
      Tm.tm_mon = Status.fdateLastWrite.month - 1 ;
      Tm.tm_mday = Status.fdateLastWrite.day ;
      Tm.tm_hour = Status.ftimeLastWrite.hours ;
      Tm.tm_min = Status.ftimeLastWrite.minutes ;
      Tm.tm_sec = Status.ftimeLastWrite.twosecs * 2 ;
      Tm.tm_isdst = -1 ;
      time_t Time = mktime ( &Tm ) ;
      Offset = time(0) - Time ;
      Offset *= 1000 ;
      ULONG Milliseconds ;
      DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &Milliseconds, sizeof(Milliseconds) ) ;
      Offset -= Milliseconds ;
      Offset /= 1000 ;
   } /* endif */
} /* endmethod */

int ElapsedTime::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   int Result ;
   char Text [100] ;
   Result = sprintf ( Text, "%s XX %s, XX:XX%s", QueryCurrentLabel(), PSZ(*Days), ShowSeconds?":XX":"" ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 ElapsedTime::NewValue ( void ) {
   ULONG Milliseconds ;
   DosQuerySysInfo ( QSV_MS_COUNT, QSV_MS_COUNT, &Milliseconds, sizeof(Milliseconds) ) ;
   __int64 _Value = Milliseconds / 1000 ;
   if ( _Value < Value*1000 ) 
      Wrapped ++ ;
   _Value += Wrapped * 0x100000000 ;
   _Value += Offset ;
   return ( _Value ) ;
}

void ElapsedTime::FormatText ( char *Label, char *Text, __int64 Time ) {

   strcpy ( Label, CurrentLabel ) ;

   *Text = 0 ;

   ULONG NumberOfDays = ULONG ( Time / ( 60 * 60 * 24 ) ) ;
   Time -= NumberOfDays * 60 * 60 * 24 ;
   ULONG Hours   = ULONG ( Time / ( 60 * 60 ) ) ;
   Time -= Hours * 60 * 60 ;
   ULONG Minutes = ULONG ( Time / 60 ) ;
   ULONG Seconds = ULONG ( Time % 60 ) ;

   if ( NumberOfDays ) {
      sprintf ( Text, "%lu %s, ",
         NumberOfDays, NumberOfDays > 1 ? PSZ(*Days) : PSZ(*Day) ) ;
   } /* endif */

   if ( ShowSeconds )
      sprintf ( Text+strlen(Text), "%lu%s%02lu%s%02lu",
         Hours, Locale.szTimeSeparator, Minutes,
         Locale.szTimeSeparator, Seconds ) ;
   else
      sprintf ( Text+strlen(Text), "%lu%s%02lu",
         Hours, Locale.szTimeSeparator, Minutes ) ;

   DdeUpdate ( Text ) ;
}

void ElapsedTime::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Time = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Time != Value ) ) {
      char Label[100], Text[100] ;
      FormatText ( Label, Text, Time ) ;
      Paint ( canvas, Rectangle, Label, Text, Time ) ;
   } /* endif */
}

MessageCount::MessageCount ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic, char *server, char *username, char *password, USHORT port, BOOL forceip, BOOL useapop, int interval, char *wavfile, BOOL alert )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ), 
   LastClock(0), Port(port), ForceIP(forceip), UseAPOP(useapop), Interval(interval), Alert(alert) {
   strcpy ( Server, server ) ;
   strcpy ( UserName, username ) ;
   strcpy ( Password, password ) ;
   strcpy ( WavFile, wavfile ) ;
   LevelSense = +1 ;
   MinLevel = 0 ;
   MaxLevel = 64 ;
   DefaultLevels[0] = WarningLevel = 1 ;
   DefaultLevels[1] = ErrorLevel = 32 ;
   SenseEnabled = TRUE ;
   InitPop3 ( ) ;
} /* endmethod */

int MessageCount::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   char Text [100] ;
   int Result = sprintf ( Text, "%s 000", QueryCurrentLabel() ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
} /* endmethod */

__int64 MessageCount::NewValue ( void ) {
   if ( !LastClock || ( ( ( clock()-LastClock ) / CLOCKS_PER_SEC ) >= Interval ) ) {
      LastClock = clock ( ) ;
      if ( *Server && *UserName && *Password ) {
         return ( GetMsgCnt ( Port, Server, UserName, Password, UseAPOP, ForceIP ) ) ;
      } else {
         return ( 0 ) ;
      } /* endif */
   } else {
      return ( Value ) ;
   } /* endif */
} /* endmethod */

void MessageCount::FormatText ( char *Label, char *Text, __int64 Count ) {
   strcpy ( Label, CurrentLabel ) ;
   sprintf ( Text, "%i", Count ) ;
   DdeUpdate ( Text ) ;
} /* endmethod */

void MessageCount::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Count = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Count != Value ) ) {
      BOOL SoundOff =  ( Count > Value ) ;
      char Label[100], Text[100] ;
      FormatText ( Label, Text, Count ) ;
      Paint ( canvas, Rectangle, Label, Text, Count ) ;
      #ifdef __OS2__
      if ( SoundOff && Alert ) {
         if ( WavFile[0] ) {
            HMODULE hModule ; 
            ULONG ( EXPENTRY *mciPlayFile ) ( HWND, PSZ, ULONG, PSZ, HWND ) ;
            if ( DosLoadModule ( 0, 0, "MCIAPI.DLL", &hModule ) || DosQueryProcAddr ( hModule, 0, "mciPlayFile", (PPFN)&mciPlayFile ) ) {
               DosBeep (  32, 100 ) ;
               DosBeep (  64, 100 ) ;
               DosBeep ( 128, 100 ) ;
               DosBeep ( 256, 100 ) ;
               DosBeep ( 512, 100 ) ;
            } else {
               mciPlayFile ( 0, WavFile, 0, 0, 0 ) ;
               DosFreeModule ( hModule ) ;
            } /* endif */
         } else {
            DosBeep (  32, 100 ) ;
            DosBeep (  64, 100 ) ;
            DosBeep ( 128, 100 ) ;
            DosBeep ( 256, 100 ) ;
            DosBeep ( 512, 100 ) ;
         } /* endif */
      } /* endif */
      #endif // __OS2__
   } /* endif */
} /* endmethod */

MemoryFree::MemoryFree ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic, BOOL ut, USHORT sk, USHORT stk, USHORT sm, USHORT sg )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   UseTheseus(ut), ShowK(sk), ShowTrueK(stk), ShowM(sm), ShowG(sg), 
   hTheseus(0), LastClock(0), pT2Initialize0(0), pT2Terminate0(0), pT2GetHandle0(0), pT2ReleaseHandle0(0), pT2FreeMemory(0) {
   LevelSense = -1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   ULONG TotalPhysicalMemory ;
   DosQuerySysInfo ( QSV_TOTPHYSMEM, QSV_TOTPHYSMEM, &TotalPhysicalMemory, sizeof(TotalPhysicalMemory) ) ;
   MaxLevel = TotalPhysicalMemory ;
   DefaultLevels [0] = WarningLevel = TotalPhysicalMemory / 8 ;
   DefaultLevels [1] = ErrorLevel = TotalPhysicalMemory / 16 ;
   if ( UseTheseus && !DosLoadModule ( 0, 0, "THESEUS0.DLL", &hTheseus ) ) {
      DosQueryProcAddr ( hTheseus, 0, "T2Initialize0", (PFN*)&pT2Initialize0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2Terminate0", (PFN*)&pT2Terminate0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2GetHandle0", (PFN*)&pT2GetHandle0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2ReleaseHandle0", (PFN*)&pT2ReleaseHandle0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2FreeMemory", (PFN*)&pT2FreeMemory ) ;
      ULONG ulDDVersion ;
      if ( pT2Initialize0 ( &ulDDVersion, TheseusErrorHandler ) == Theseus2OK ) {
         T2Handle0 = pT2GetHandle0 ( TheseusErrorHandler ) ;
         ULONG FreeRam, IdleRam, DirtyRam, LLockRam, SLockRam, BLockRam ;
         pT2FreeMemory ( T2Handle0, &FreeRam, &IdleRam, &DirtyRam, &LLockRam, &SLockRam, &BLockRam ) ;
         Value = __int64 ( FreeRam * 0x1000 ) ;
      } /* endif */
   } /* endif */
} /* endmethod */

MemoryFree::~MemoryFree ( ) {
   if ( hTheseus ) {
      pT2ReleaseHandle0 ( &T2Handle0 ) ;
      pT2Terminate0 ( ) ;
      DosFreeModule ( hTheseus ) ;
      hTheseus = 0 ;
   } /* endif */
} /* endmethod */


int MemoryFree::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   char Text [100] ;
   int Result = MeasureValue ( Text, QueryCurrentLabel(), ShowK, ShowM, ShowG ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

void MemoryFree::SetUseTheseus ( BOOL flag ) { 
   if ( flag && !UseTheseus ) {
      // Enable use of Theseus/2.
      if ( !DosLoadModule ( 0, 0, "THESEUS0.DLL", &hTheseus ) ) {
         DosQueryProcAddr ( hTheseus, 0, "T2Initialize0", (PFN*)&pT2Initialize0 ) ;
         DosQueryProcAddr ( hTheseus, 0, "T2Terminate0", (PFN*)&pT2Terminate0 ) ;
         DosQueryProcAddr ( hTheseus, 0, "T2GetHandle0", (PFN*)&pT2GetHandle0 ) ;
         DosQueryProcAddr ( hTheseus, 0, "T2ReleaseHandle0", (PFN*)&pT2ReleaseHandle0 ) ;
         DosQueryProcAddr ( hTheseus, 0, "T2FreeMemory", (PFN*)&pT2FreeMemory ) ;
         ULONG ulDDVersion ;
         if ( pT2Initialize0 ( &ulDDVersion, TheseusErrorHandler ) == Theseus2OK ) {
            T2Handle0 = pT2GetHandle0 ( TheseusErrorHandler ) ;
            LastClock = 0 ;
         } /* endif */
      } /* endif */
   } else if ( !flag && UseTheseus ) {
      // Disable use of Theseus/2.
      if ( hTheseus ) {
         pT2ReleaseHandle0 ( &T2Handle0 ) ;
         pT2Terminate0 ( ) ;
         DosFreeModule ( hTheseus ) ;
         hTheseus = 0 ;
      } /* endif */
   } /* endif */
   UseTheseus = flag ;  Value = 0 ; 
} /* endmethod */

extern "C" APIRET16 APIENTRY16 Dos16MemAvail ( PULONG pulAvailMem ) ;

__int64 MemoryFree::NewValue ( void ) {
   ULONG Value ;
   Dos16MemAvail ( &Value ) ;
   if ( ( Value <= 512 * 1024 ) && hTheseus && pT2FreeMemory ) {
      if ( !LastClock || ( clock()-LastClock >= CLOCKS_PER_SEC*10 ) ) {
         LastClock = clock ( ) ;
         ULONG FreeRam, IdleRam, DirtyRam, LLockRam, SLockRam, BLockRam ;
         pT2FreeMemory ( T2Handle0, &FreeRam, &IdleRam, &DirtyRam, &LLockRam, &SLockRam, &BLockRam ) ;
         return ( __int64 ( FreeRam * 0x1000 ) ) ;
      } else {
         return ( MemoryFree::Value ) ;
      } /* endif */
   } else {
      return ( Value ) ;
   } /* endif */
}

void MemoryFree::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   FormatValue ( Value, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   DdeUpdate ( Text ) ;
}

void MemoryFree::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Size = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Size != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Size ) ;
      Paint ( canvas, Rectangle, Label, Text, Size ) ;
   } /* endif */
}


VMFree::VMFree ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic, USHORT sk, USHORT stk, USHORT sm, USHORT sg )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   ShowK(sk), ShowTrueK(stk), ShowM(sm), ShowG(sg) {
   LevelSense = -1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   MaxLevel = 64*1024*1024 ;
   DefaultLevels [0] = WarningLevel = 8*1024*1024 ;
   DefaultLevels [1] = ErrorLevel = 4*1024*1024 ;
} /* endmethod */

int VMFree::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   char Text [100] ;
   int Result = MeasureValue ( Text, QueryCurrentLabel(), ShowK, ShowM, ShowG ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 VMFree::NewValue ( void ) {
   ULONG Space ;
   DosQuerySysInfo ( QSV_TOTAVAILMEM, QSV_TOTAVAILMEM, &Space, sizeof(Space) ) ;
   return ( Space ) ;
}

void VMFree::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   FormatValue ( Value, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   DdeUpdate ( Text ) ;
}

void VMFree::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Size = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Size != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Size ) ;
      Paint ( canvas, Rectangle, Label, Text, Size ) ;
   } /* endif */
}


SwapSize::SwapSize ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic,
   USHORT sk, USHORT stk, USHORT sm, USHORT sg, PSZ swappath, ULONG initialsize )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   ShowK(sk), ShowTrueK(stk), ShowM(sm), ShowG(sg) {
   LevelSense = +1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   MaxLevel = 256 * 1024 * 1024 ;
   SwapPath = new BYTE [ strlen(PCHAR(swappath)) + 1 ] ;
   strcpy ( PCHAR(SwapPath), PCHAR(swappath) ) ;
   DefaultLevels [0] = WarningLevel = initialsize > 0x2000 ? initialsize * 2 * 1024 : 16 * 1024 * 1024 ;
   DefaultLevels [1] = ErrorLevel = initialsize > 0x2000 ? initialsize * 2 * 1024 : 32 * 1024 * 1024 ;
} /* endmethod */

SwapSize::~SwapSize ( void ) {
   delete [] SwapPath ;
} /* endmethod */

int SwapSize::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   char Text [100] ;
   int Result = MeasureValue ( Text, QueryCurrentLabel(), ShowK, ShowM, ShowG ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 SwapSize::NewValue ( void ) {

   char Path [_MAX_PATH+1] ;
   strcpy ( Path, (PCHAR)SwapPath ) ;

   if ( Path[strlen(Path)-1] != '\\' )
      strcat ( Path, "\\" ) ;

   strcat ( Path, "SWAPPER.DAT" ) ;

   ULONG SwapSize = 0 ;
   FILESTATUS3 Status ;
   if ( DosQueryPathInfo ( (PSZ)Path, FIL_STANDARD, &Status, sizeof(Status) ) == 0 )
      SwapSize = Status.cbFile ;

   return ( SwapSize ) ;
}

void SwapSize::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   FormatValue ( Value, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   DdeUpdate ( Text ) ;
}

void SwapSize::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Size = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Size != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Size ) ;
      Paint ( canvas, Rectangle, Label, Text, Size ) ;
   } /* endif */
}


SwapFree::SwapFree ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic,
   USHORT sk, USHORT stk, USHORT sm, USHORT sg, PSZ swappath, ULONG minfree )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   ShowK(sk), ShowTrueK(stk), ShowM(sm), ShowG(sg), MinFree(minfree) {
   LevelSense = -1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   MaxLevel = 64*1024*1024 ;
   SwapPath = new BYTE [ strlen(PCHAR(swappath)) + 1 ] ;
   strcpy ( PCHAR(SwapPath), PCHAR(swappath) ) ;
   DefaultLevels [0] = WarningLevel = 8*1024*1024 ;
   DefaultLevels [1] = ErrorLevel = 4*1024*1024 ;
} /* endmethod */

SwapFree::~SwapFree ( void ) {
   delete [] SwapPath ;
} /* endmethod */

int SwapFree::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   char Text [100] ;
   int Result = MeasureValue ( Text, QueryCurrentLabel(), ShowK, ShowM, ShowG ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 SwapFree::NewValue ( void ) {

   DosError ( FERR_DISABLEHARDERR ) ;
   FSALLOCATE Allocation ;
   DosQueryFSInfo ( SwapPath[0]-'A'+1, FSIL_ALLOC, PBYTE(&Allocation), sizeof(Allocation) ) ;
   DosError ( FERR_ENABLEHARDERR ) ;

   ULONG SwapFree = Allocation.cUnitAvail*Allocation.cSectorUnit*(Allocation.cbSector/256) ;

   SwapFree -= ( MinFree * 1024 ) / 256 ;              // The swap-file will not allocate the space required free.

   SwapFree -= ( SwapFree % ( 1024 * 1024 ) ) / 256 ;  // The swap-file only expands by 1M increments.

   return ( SwapFree ) ;
}

void SwapFree::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   FormatValue ( Value, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG, 256 ) ;
   DdeUpdate ( Text ) ;
}

void SwapFree::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Size = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Size != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Size ) ;
      Paint ( canvas, Rectangle, Label, Text, Size, 256 ) ;
   } /* endif */
}


SpoolSize::SpoolSize ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic,
   USHORT sk, USHORT stk, USHORT sm, USHORT sg, PSZ spoolpath )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   ShowK(sk), ShowTrueK(stk), ShowM(sm), ShowG(sg) {
   LevelSense = +1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   MaxLevel = 64*1024*1024 ;
   SpoolPath = new BYTE [ strlen(PCHAR(spoolpath)) + 1 ] ;
   strcpy ( PCHAR(SpoolPath), PCHAR(spoolpath) ) ;
   DefaultLevels [0] = WarningLevel = 8*1024*1024 ;
   DefaultLevels [1] = ErrorLevel = 16*1024*1024 ;
} /* endmethod */

SpoolSize::~SpoolSize ( void ) {
   delete [] SpoolPath ;
} /* endmethod */

int SpoolSize::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   char Text [100] ;
   int Result = MeasureValue ( Text, QueryCurrentLabel(), ShowK, ShowM, ShowG ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 SpoolSize::NewValue ( void ) {

   ULONG PathSize ;
   DosQuerySysInfo ( QSV_MAX_PATH_LENGTH, QSV_MAX_PATH_LENGTH, &PathSize, sizeof(PathSize) ) ;

   PBYTE Path = PBYTE ( malloc ( size_t(PathSize) ) ) ;
   if ( Path == 0 )
      return ( 0 ) ;

   PFILEFINDBUF3 Found = PFILEFINDBUF3 ( malloc ( size_t( PathSize + sizeof(FILEFINDBUF3) ) ) ) ;
   if ( Found == 0 ) {
      free ( Path ) ;
      return ( 0 ) ;
   } /* endif */

   strcpy ( (PCHAR)Path, (PCHAR)SpoolPath ) ;
   strcat ( (PCHAR)Path, "\\*.*" ) ;

   HDIR hDir = (HDIR) HDIR_CREATE ;
   ULONG Count = 1 ;
   ULONG TotalSize = 0 ;

   if ( !DosFindFirst2 ( Path, &hDir,
      FILE_NORMAL | FILE_READONLY | FILE_DIRECTORY | FILE_ARCHIVED,
      Found, PathSize+sizeof(FILEFINDBUF3), &Count, FIL_STANDARD ) ) {

      do {

         if ( !strcmp ( (PCHAR)Found->achName, "." ) OR !strcmp ( (PCHAR)Found->achName, ".." ) )
            continue ;

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
                  TotalSize += Found->cbFile ;
               } while ( !DosFindNext ( hDir2, Found, PathSize+sizeof(FILEFINDBUF3), &Count2 ) ) ;
               DosFindClose ( hDir2 ) ;
            } /* endif */
         } else {
            TotalSize += Found->cbFile ;
         } /* endif */
      } while ( !DosFindNext ( hDir, Found, PathSize+sizeof(FILEFINDBUF3), &Count ) ) ;

      DosFindClose ( hDir ) ;

   } /* enddo */

   free ( Path ) ;
   free ( Found ) ;

   return ( TotalSize ) ;
}

void SpoolSize::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   FormatValue ( Value, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   DdeUpdate ( Text ) ;
}

void SpoolSize::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Size = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Size != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Size ) ;
      Paint ( canvas, Rectangle, Label, Text, Size ) ;
   } /* endif */
}


AverageLoad::AverageLoad ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic, int interval, CpuLoad *pcpuload )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   Interval(interval), Total(0), NumberOfCounts(0), NextIndex(0), pCpuLoad(pcpuload) {
   LevelSense = +1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   MaxLevel = 100 ;
   DefaultLevels [0] = WarningLevel = 75 ;
   DefaultLevels [1] = ErrorLevel = 90 ;
   memset ( Counts, 0, sizeof(Counts) ) ;
   Interval = min ( Interval, MAX_AVERAGE_COUNT ) ;
} /* endmethod */

void AverageLoad::Reset ( ) {
   Total = 0 ;
   NumberOfCounts = 0 ;
   NextIndex = 0 ;
   memset ( Counts, 0, sizeof(Counts) ) ;
} /* endmethod */

void AverageLoad::StoreCount ( ULONG Count ) {

   // Update the running total.
   Total += Count ;

   // If we already have enough counts in the running total, drop the oldest one.
   if ( NumberOfCounts >= Interval ) {
      int OldIndex = NextIndex - Interval ;
      if ( OldIndex < 0 )
         OldIndex += MAX_AVERAGE_COUNT ;
      Total -= Counts[OldIndex] ;
   } /* endif */

   // Store this latest count.
   Counts[NextIndex++] = Count ;
   if ( NextIndex >= MAX_AVERAGE_COUNT )
      NextIndex = 0 ;

   // Adjust the number of counts stored.
   NumberOfCounts = min ( NumberOfCounts+1, MAX_AVERAGE_COUNT ) ;

} /* endmethod */

void AverageLoad::SetInterval ( int NewInterval ) {
   Interval = NewInterval ;
   Total = 0 ;
   int Index = NextIndex - 1 ;  if ( Index < 0 ) Index += MAX_AVERAGE_COUNT ;
   for ( int i=0; i<min(Interval,NumberOfCounts); i++ ) {
      Total += Counts[Index] ;
      Index-- ;  if ( Index < 0 ) Index += MAX_AVERAGE_COUNT ;
   } /* endfor */
} /* endmethod */

void AverageLoad::UpdateHistory ( int NumberOfSeconds, ULONG Count ) {
   NumberOfSeconds = min ( NumberOfSeconds, 3600 ) ;
   for ( int i=0; i<NumberOfSeconds; i++ )
      StoreCount ( Count ) ;
} /* endmethod */

int AverageLoad::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   int Result ;
   char Text [100] ;
   Result = sprintf ( Text, "%s 100%", QueryCurrentLabel() ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 AverageLoad::NewValue ( void ) {
   if ( pCpuLoad && !pCpuLoad->QueryFlag() ) 
      pCpuLoad->NewValue ( ) ;
   int Divisor = min ( NumberOfCounts, Interval ) ;
   return ( Divisor ? ( Total / Divisor ) : 0 ) ;
}

void AverageLoad::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   sprintf ( Text, "%lu%%", Value ) ;
   DdeUpdate ( Text ) ;
}

void AverageLoad::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Load = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Load != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Load ) ;
      Paint ( canvas, Rectangle, Label, Text, Load ) ;
   } /* endif */
}


#ifndef QSV_NUMPROCESSORS
   #define QSV_NUMPROCESSORS (26)
#endif

CpuLoad::CpuLoad ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic, ULONG maxcount, PULONG idlecount, ResourceString *error, AverageLoad *paverageload )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   MaxCount(maxcount), IdleCount(idlecount), Error(error), CpuCount(1),
   DosPerfSysCall(0), Previous(0), Current(0), LastTime(time(0)), pAverageLoad(paverageload) {
   LevelSense = +1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   MaxLevel = 100 ;
   DefaultLevels [0] = WarningLevel = 75 ;
   DefaultLevels [1] = ErrorLevel = 90 ;
   if ( DosQuerySysInfo ( QSV_NUMPROCESSORS, QSV_NUMPROCESSORS, &CpuCount, sizeof(CpuCount) ) )
      if ( ( CpuCount < 1 ) || ( CpuCount > 64 ) )
         CpuCount = 1 ;
   HMODULE ModuleHandle ;
   Previous = (CPUUTIL*) malloc ( sizeof(CPUUTIL) * CpuCount ) ;
   Current = (CPUUTIL*) malloc ( sizeof(CPUUTIL) * CpuCount ) ;
   if ( UsePerfAPI ( ModuleHandle, (PFN*)&DosPerfSysCall, CpuCount, Previous, Current ) == FALSE ) {
      DosPerfSysCall = 0 ;
      if ( ModuleHandle ) 
         DosFreeModule ( ModuleHandle ) ;
   } /* endif */
} /* endmethod */

CpuLoad::~CpuLoad ( ) {
   if ( Previous )
      free ( Previous ), Previous = 0 ;
   if ( Current )
      free ( Current ), Current = 0 ;
} /* endmethod */

int CpuLoad::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   int Result ;
   char Text [100] ;
   Result = sprintf ( Text, "%s 100%", QueryCurrentLabel() ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 CpuLoad::NewValue ( void ) {
   int NumberOfSeconds = time(0) - LastTime ;
   if ( !NumberOfSeconds )
      return ( Value ) ;
   LastTime += NumberOfSeconds ;
   ULONG Load ( 0 ) ;
   if ( DosPerfSysCall ) {
      DosPerfSysCall ( CMD_KI_RDCNT, (ULONG)Current, 0, 0 ) ;
      __int64 DeltaTime(0), IdleTime(0), BusyTime(0), IntrTime(0) ;
      for ( int i=0; i<CpuCount; i++ ) {
         __int64 CurrentTime = ( (__int64)Current[i].ulTimeHigh << 32 ) + Current[i].ulTimeLow ;
         __int64 PreviousTime = ( (__int64)Previous[i].ulTimeHigh << 32 ) + Previous[i].ulTimeLow ;
         __int64 CurrentIdle = ( (__int64)Current[i].ulIdleHigh << 32 ) + Current[i].ulIdleLow ;
         __int64 PreviousIdle = ( (__int64)Previous[i].ulIdleHigh << 32 ) + Previous[i].ulIdleLow ;
         __int64 CurrentBusy = ( (__int64)Current[i].ulBusyHigh << 32 ) + Current[i].ulBusyLow ;
         __int64 PreviousBusy = ( (__int64)Previous[i].ulBusyHigh << 32 ) + Previous[i].ulBusyLow ;
         __int64 CurrentIntr = ( (__int64)Current[i].ulIntrHigh << 32 ) + Current[i].ulIntrLow ;
         __int64 PreviousIntr = ( (__int64)Previous[i].ulIntrHigh << 32 ) + Previous[i].ulIntrLow ;
         DeltaTime += CurrentTime - PreviousTime ;
         IdleTime += CurrentIdle - PreviousIdle ;
         BusyTime += CurrentBusy - PreviousBusy ;
         IntrTime += CurrentIntr - PreviousIntr ;
      } /* endfor */
      Load = ULONG ( DeltaTime ? ULONG ( ( double(DeltaTime-IdleTime) / DeltaTime ) * 100.0 + 0.49 ) : 0L ) ;
      // Log ( "CPUs: %i  DeltaTime: %12Li  IdleTime: %12Li  BusyTime: %12Li  IntrTime: %12Li  Load: %3i%%", CpuCount, DeltaTime, IdleTime, BusyTime, IntrTime, Load ) ;
      memcpy ( Previous, Current, CpuCount*sizeof(Previous[0]) ) ;
   } else {
      if ( *IdleCount == 0xFFFFFFFF )
         return ( 0xFFFFFFFF ) ;
      MaxCount = (ULONG) max ( MaxCount, *IdleCount ) ;
      Load = ( ( MaxCount - *IdleCount ) * 100 ) / (MaxCount?MaxCount:1) ;
   } /* endif */
   if ( pAverageLoad )
      pAverageLoad->UpdateHistory ( NumberOfSeconds, Load ) ;
   return ( Load ) ;
}

void CpuLoad::FormatText ( char *Label, char *Text, __int64 Value ) {
   if ( Value == 0xFFFFFFFF ) {
      strcpy ( Text, PCHAR(*Error) ) ;
   } else {
      strcpy ( Label, CurrentLabel ) ;
      sprintf ( Text, "%lu%%", (Value>0)?Value:0 ) ;
   } /* endif */
   DdeUpdate ( Text ) ;
}

void CpuLoad::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Load = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Load != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Load ) ;
      Paint ( canvas, Rectangle, Label, Text, Load ) ;
   } /* endif */
}

BOOL CpuLoad::UsePerfAPI ( ) {
   ULONG CpuCount = 1 ;
   if ( DosQuerySysInfo ( QSV_NUMPROCESSORS, QSV_NUMPROCESSORS, &CpuCount, sizeof(CpuCount) ) )
      if ( ( CpuCount < 1 ) || ( CpuCount > 64 ) )
         CpuCount = 1 ;
   CPUUTIL *Previous = (CPUUTIL*) malloc ( sizeof(CPUUTIL) * CpuCount ) ;
   CPUUTIL *Current = (CPUUTIL*) malloc ( sizeof(CPUUTIL) * CpuCount ) ;
   BOOL Result ( FALSE ) ;
   if ( Previous && Current ) {
      HMODULE ModuleHandle ; PFN DosPerfSysCall ;
      Result = UsePerfAPI ( ModuleHandle, (PFN*)&DosPerfSysCall, CpuCount, Previous, Current ) ;
      if ( ModuleHandle ) 
         DosFreeModule ( ModuleHandle ) ;
      free ( Current ) ;
      free ( Previous ) ;
   } /* endif */
   return ( Result ) ;
} /* endmethod */

BOOL CpuLoad::UsePerfAPI ( HMODULE &ModuleHandle, PFN *DosPerfSysCall, ULONG CpuCount, CPUUTIL *Previous, CPUUTIL *Current ) {

   APIRET Status = DosLoadModule ( 0, 0, "DOSCALL1.DLL", &ModuleHandle ) ;
   if ( Status ) {
      Log ( "WARNING: Unable to load DOSCALL1.DLL.  Will use old CPU load technique." ) ;
      return ( FALSE ) ;
   } /* endif */

   Status = DosQueryProcAddr ( ModuleHandle, ORD_DOS32PERFSYSCALL, "", DosPerfSysCall ) ;
   if ( Status ) {
      Log ( "WARNING: Unable to find DosPerfSysCall.  Will use old CPU load technique." ) ;
      return ( FALSE ) ;
   } /* endif */

   APIRET (APIENTRY *pfn) (ULONG, ULONG, ULONG, ULONG) = ( APIRET (APIENTRY *) (ULONG, ULONG, ULONG, ULONG) ) ( *DosPerfSysCall ) ;
   pfn ( CMD_KI_ENABLE, 0, 0, 0 ) ;
   pfn ( CMD_KI_RDCNT, (ULONG)Previous, 0, 0 ) ;
   DosSleep ( 100 ) ;
   pfn ( CMD_KI_RDCNT, (ULONG)Current, 0, 0 ) ;

   __int64 DeltaTime(0), IdleTime(0) ;
   for ( int i=0; i<CpuCount; i++ ) {
      __int64 CurrentTime = ( (__int64)Current[i].ulTimeHigh << 32 ) + Current[i].ulTimeLow ;
      __int64 PreviousTime = ( (__int64)Previous[i].ulTimeHigh << 32 ) + Previous[i].ulTimeLow ;
      __int64 CurrentIdle = ( (__int64)Current[i].ulIdleHigh << 32 ) + Current[i].ulIdleLow ;
      __int64 PreviousIdle = ( (__int64)Previous[i].ulIdleHigh << 32 ) + Previous[i].ulIdleLow ;
      DeltaTime += CurrentTime - PreviousTime ;
      IdleTime += CurrentIdle - PreviousIdle ;
   } /* endfor */

   if ( DeltaTime == IdleTime ) {
      Log ( "WARNING: DosPerfSysCall returning bad results.  Will use old CPU load technique." ) ;
      return ( FALSE ) ;
   } /* endif */

   return ( TRUE ) ;
}


TaskCount::TaskCount ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic, HAB anchor )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   Anchor(anchor) {
   LevelSense = +1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   MaxLevel = 256 ;
   DefaultLevels [0] = WarningLevel = 30 ;
   DefaultLevels [1] = ErrorLevel = 40 ;
} /* endmethod */

int TaskCount::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   int Result ;
   char Text [100] ;
   Result = sprintf ( Text, "%s 999 ", QueryCurrentLabel() ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 TaskCount::NewValue ( void ) {
   return ( WinQuerySwitchList ( Anchor, 0, 0 ) ) ;
}

void TaskCount::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   sprintf ( Text, "%lu ", Value ) ;
   DdeUpdate ( Text ) ;
}

void TaskCount::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Count = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Count != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Count ) ;
      Paint ( canvas, Rectangle, Label, Text, Count ) ;
   } /* endif */
}


DriveFree::DriveFree ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic,
   USHORT sk, USHORT stk, USHORT sm, USHORT sg, USHORT drivenumber, ResourceString *driveerror, BOOL showfilesystemname,
   PSZ filesystem, BOOL showdisklabel, PSZ disklabel )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   ShowK(sk), ShowTrueK(stk), ShowM(sm), ShowG(sg), DriveError(driveerror), DriveNumber(drivenumber),
   ShowFileSystemName(showfilesystemname), ShowDiskLabel(showdisklabel),
   Error(FALSE) {
   LevelSense = -1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   strcpy ( PCHAR(FileSystem), PCHAR(filesystem) ) ;
   strcpy ( PCHAR(DiskLabel), PCHAR(disklabel) ) ;
   DosError ( FERR_DISABLEHARDERR ) ;
   FSALLOCATE Allocation ;
   DosQueryFSInfo ( DriveNumber, FSIL_ALLOC, (PBYTE)&Allocation, sizeof(Allocation) ) ;
   DosError ( FERR_ENABLEHARDERR ) ;
   MaxLevel = Allocation.cUnit*Allocation.cSectorUnit*Allocation.cbSector ;
   DefaultLevels [0] = WarningLevel = MaxLevel / 5 ;
   DefaultLevels [1] = ErrorLevel = MaxLevel / 10 ;
} /* endmethod */

int DriveFree::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   char Label[100] ;
   if ( ShowFileSystemName && *FileSystem ) {
      if ( ShowDiskLabel && *DiskLabel ) {
         sprintf ( Label, "%s (%s,%s)", QueryCurrentLabel(), DiskLabel, FileSystem ) ;
      } else {
         sprintf ( Label, "%s (%s)", QueryCurrentLabel(), FileSystem ) ;
      } /* endif */
   } else {
      if ( ShowDiskLabel && *DiskLabel ) {
         sprintf ( Label, "%s (%s)", QueryCurrentLabel(), DiskLabel ) ;
      } else {
         sprintf ( Label, "%s", QueryCurrentLabel() ) ;
      } /* endif */
   } /* endif */
   char Text[100] ;
   int Result = MeasureValue ( Text, Label, ShowK, ShowM, ShowG ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 DriveFree::NewValue ( void ) {

   if ( Error )
      return ( 0 ) ;

   DosError ( FERR_DISABLEHARDERR ) ;

   FSALLOCATE Allocation ;
   APIRET Status = DosQueryFSInfo ( DriveNumber, FSIL_ALLOC, (PBYTE)&Allocation, sizeof(Allocation) ) ;

   if ( !Status ) {
      FSINFO Info ;
      if ( !DosQueryFSInfo ( DriveNumber, FSIL_VOLSER, PBYTE(&Info), sizeof(Info) ) ) 
         strcpy ( DiskLabel, PCHAR(Info.vol.szVolLabel) ) ;
   } /* endif */

   DosError ( FERR_ENABLEHARDERR ) ;

   if ( Status ) {
      Error = TRUE ;
      return ( 0 ) ;
   } /* endif */

   __int64 Result = Allocation.cUnitAvail ;
   Result *= Allocation.cSectorUnit ;
   Result *= Allocation.cbSector ;
   Result /= 256 ;
   return ( Result ) ;
}

void DriveFree::FormatText ( char *Label, char *Text, __int64 Value ) {

   if ( ShowFileSystemName && *FileSystem ) {
      if ( ShowDiskLabel && *DiskLabel ) {
         sprintf ( Label, "%s (%s,%s)", QueryCurrentLabel(), DiskLabel, FileSystem ) ;
      } else {
         sprintf ( Label, "%s (%s)", QueryCurrentLabel(), FileSystem ) ;
      } /* endif */
   } else {
      if ( ShowDiskLabel && *DiskLabel ) {
         sprintf ( Label, "%s (%s)", QueryCurrentLabel(), DiskLabel ) ;
      } else {
         sprintf ( Label, "%s", QueryCurrentLabel() ) ;
      } /* endif */
   } /* endif */

   if ( Error )
      strcpy ( Text, PCHAR(*DriveError) ) ;
   else
      FormatValue ( Value, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG, 256 ) ;

   DdeUpdate ( Text ) ;
}

void DriveFree::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Size = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Size != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Size ) ;
      Paint ( canvas, Rectangle, Label, Text, Size, 256 ) ;
   } /* endif */
}


TotalFree::TotalFree ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic,
   USHORT sk, USHORT stk, USHORT sm, USHORT sg, ULONG drives )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   ShowK(sk), ShowTrueK(stk), ShowM(sm), ShowG(sg), Drives(drives) {
   LevelSense = -1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   MaxLevel = 64*1024*1024 ;
   DefaultLevels [0] = WarningLevel = 8*1024*1024 ;
   DefaultLevels [1] = ErrorLevel = 4*1024*1024 ;
} /* endmethod */

int TotalFree::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   char Text [100] ;
   int Result = MeasureValue ( Text, QueryCurrentLabel(), ShowK, ShowM, ShowG ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 TotalFree::NewValue ( void ) {

   __int64 Free ( 0 ) ;
   ULONG Mask ( Drives >> 2 ) ;

   for ( int Drive=3; Drive<=26; Drive++ ) {
      if ( Mask & 1 ) {

         DosError ( FERR_DISABLEHARDERR ) ;

         FSALLOCATE Allocation ;
         APIRET Status = DosQueryFSInfo ( Drive, FSIL_ALLOC, (PBYTE)&Allocation, sizeof(Allocation) ) ;

         DosError ( FERR_ENABLEHARDERR ) ;

         if ( Status )
            Drives &= ~ ( 1 << (Drive-1) ) ;
         else {
            __int64 Additional = Allocation.cUnitAvail ;
            Additional *= Allocation.cSectorUnit ;
            Additional *= Allocation.cbSector ;
            Additional /= 256 ;
            Free += Additional ;
         } /* endif */
      } /* endif */
      Mask >>= 1 ;

   } /* endfor */

   return ( Free ) ;
}

void TotalFree::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   FormatValue ( Value, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG, 256 ) ;
   DdeUpdate ( Text ) ;
}

void TotalFree::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Size = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Size != Value ) ) {
      char Label[100], Text [100] ;
      FormatText ( Label, Text, Size ) ;
      Paint ( canvas, Rectangle, Label, Text, Size, 256 ) ;
   } /* endif */
}

SwapSlack::SwapSlack ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic,
   BOOL ut, USHORT sk, USHORT stk, USHORT sm, USHORT sg, VMFree *pvf, SwapFree *psf, MemoryFree *pmf )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   UseTheseus(ut), ShowK(sk), ShowTrueK(stk), ShowM(sm), ShowG(sg), pVMFree(pvf), pSwapFree(psf), pMemFree(pmf), 
   hTheseus(0), LastClock(0), pT2Initialize0(0), pT2Terminate0(0), pT2GetHandle0(0), pT2ReleaseHandle0(0), pT2AnalyzeSwapper(0) {
   LevelSense = 0 ;
   SenseEnabled = FALSE ;
   if ( UseTheseus && !DosLoadModule ( 0, 0, "THESEUS0.DLL", &hTheseus ) ) {
      DosQueryProcAddr ( hTheseus, 0, "T2Initialize0", (PFN*)&pT2Initialize0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2Terminate0", (PFN*)&pT2Terminate0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2GetHandle0", (PFN*)&pT2GetHandle0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2ReleaseHandle0", (PFN*)&pT2ReleaseHandle0 ) ;
      DosQueryProcAddr ( hTheseus, 0, "T2AnalyzeSwapper", (PFN*)&pT2AnalyzeSwapper ) ;
      ULONG ulDDVersion ;
      if ( pT2Initialize0 ( &ulDDVersion, TheseusErrorHandler ) == Theseus2OK ) {
         T2Handle0 = pT2GetHandle0 ( TheseusErrorHandler ) ;
         ULONG SwapUsed, SwapFree ;
         pT2AnalyzeSwapper ( T2Handle0, &SwapUsed, &SwapFree ) ;
         Value = __int64 ( SwapFree * 0x1000 ) ;
      } /* endif */
   } /* endif */
} /* endmethod */

SwapSlack::~SwapSlack ( ) {
   if ( hTheseus ) {
      pT2ReleaseHandle0 ( &T2Handle0 ) ;
      pT2Terminate0 ( ) ;
      DosFreeModule ( hTheseus ) ;
      hTheseus = 0 ;
   } /* endif */
} /* endmethod */

void SwapSlack::SetUseTheseus ( BOOL flag ) { 
   if ( flag && !UseTheseus ) {
      // Enable use of Theseus/2.
      if ( !DosLoadModule ( 0, 0, "THESEUS0.DLL", &hTheseus ) ) {
         DosQueryProcAddr ( hTheseus, 0, "T2Initialize0", (PFN*)&pT2Initialize0 ) ;
         DosQueryProcAddr ( hTheseus, 0, "T2Terminate0", (PFN*)&pT2Terminate0 ) ;
         DosQueryProcAddr ( hTheseus, 0, "T2GetHandle0", (PFN*)&pT2GetHandle0 ) ;
         DosQueryProcAddr ( hTheseus, 0, "T2ReleaseHandle0", (PFN*)&pT2ReleaseHandle0 ) ;
         DosQueryProcAddr ( hTheseus, 0, "T2AnalyzeSwapper", (PFN*)&pT2AnalyzeSwapper ) ;
         ULONG ulDDVersion ;
         if ( pT2Initialize0 ( &ulDDVersion, TheseusErrorHandler ) == Theseus2OK ) {
            T2Handle0 = pT2GetHandle0 ( TheseusErrorHandler ) ;
            LastClock = 0 ;
         } /* endif */
      } /* endif */
   } else if ( !flag && UseTheseus ) {
      // Disable use of Theseus/2.
      if ( hTheseus ) {
         pT2ReleaseHandle0 ( &T2Handle0 ) ;
         pT2Terminate0 ( ) ;
         DosFreeModule ( hTheseus ) ;
         hTheseus = 0 ;
      } /* endif */
   } /* endif */
   UseTheseus = flag ;  Value = 0 ; 
} /* endmethod */

int SwapSlack::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   char Text [100] ;
   int Result = MeasureValue ( Text, QueryCurrentLabel(), ShowK, ShowM, ShowG ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
}

__int64 SwapSlack::NewValue ( void ) {
   if ( hTheseus && pT2AnalyzeSwapper ) {
      if ( !LastClock || ( clock()-LastClock >= CLOCKS_PER_SEC*10 ) ) {
         LastClock = clock ( ) ;
         ULONG SwapUsed, SwapFree ;
         pT2AnalyzeSwapper ( T2Handle0, &SwapUsed, &SwapFree ) ;
         return ( __int64 ( SwapFree * 0x1000 ) ) ;
      } else {
         return ( Value ) ;
      } /* endif */
   } else {
      __int64 Slack = pVMFree->QueryFlag() ? pVMFree->QueryValue() : pVMFree->NewValue() ;
      Slack -= ( pSwapFree->QueryFlag() ? pSwapFree->QueryValue() : pSwapFree->NewValue() ) * 256 ;
      Slack -= pMemFree->QueryFlag() ? pMemFree->QueryValue() : pMemFree->NewValue() ;
      return ( ULONG ( max ( 0, Slack ) ) ) ;
   } /* endif */
}

void SwapSlack::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   FormatValue ( Value, Text, Locale, ShowK, ShowTrueK, ShowM, ShowG ) ;
   DdeUpdate ( Text ) ;
}

void SwapSlack::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Size = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Size != Value ) ) {
      char Label[100], Text[100] ;
      FormatText ( Label, Text, Size ) ;
      Paint ( canvas, Rectangle, Label, Text, Size ) ;
   } /* endif */
} /* endmethod */

#define DQSS_BUFFER_SIZE 0x40000

ProcessCount::ProcessCount ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ) {
   LevelSense = +1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   MaxLevel = 256 ;
   DosAllocMem ( (PPVOID)&DQSS_Buffer, DQSS_BUFFER_SIZE, PAG_READ | PAG_WRITE | PAG_COMMIT ) ;
   DefaultLevels [0] = WarningLevel = 30 ;
   DefaultLevels [1] = ErrorLevel = 40 ;
} /* endmethod */

ProcessCount::~ProcessCount ( ) {
   DosFreeMem ( DQSS_Buffer ) ;
} /* endmethod */

int ProcessCount::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   int Result ;
   char Text [100] ;
   Result = sprintf ( Text, "%s 123 ", QueryCurrentLabel() ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
} /* endmethod */

__int64 ProcessCount::NewValue ( void ) {

   if ( DosQuerySysState ( 0x17, 0, 0, 0, DQSS_Buffer, DQSS_BUFFER_SIZE ) )
      return ( 0 ) ;

   __int64 ProcCount ( 0 ) ;
   QPROCESS *pProcRecord = ((QTOPLEVEL*)DQSS_Buffer)->procdata ;
   while ( pProcRecord->rectype == 1 ) {
      ProcCount ++ ;
      char *p ( (char*) ( pProcRecord->threads+pProcRecord->threadcnt ) ) ;
      pProcRecord = (QPROCESS*) p ;
   } /* endfor */

   return ( ProcCount ) ;
   
} /* endmethod */

void ProcessCount::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   sprintf ( Text, "%lu ", Value ) ;
   DdeUpdate ( Text ) ;
} /* endmethod */

void ProcessCount::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Count = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Count != Value ) ) {
      char Label[100], Text[100] ;
      FormatText ( Label, Text, Count ) ;
      Paint ( canvas, Rectangle, Label, Text, Count ) ;
   } /* endif */
} /* endmethod */


ThreadCount::ThreadCount ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ) {
   LevelSense = +1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   DosAllocMem ( (PPVOID)&DQSS_Buffer, DQSS_BUFFER_SIZE, PAG_READ | PAG_WRITE | PAG_COMMIT ) ;
   PSZ Threads = ScanSystemConfig ( 0, "THREADS" ) ;
   if ( Threads == 0 )
      Threads = (PSZ) "64" ;
   int MaxThreads = atoi ( Threads ) ;
   MaxLevel = MaxThreads ;
   DefaultLevels [0] = WarningLevel = ( MaxThreads * 3 ) / 4 ;
   DefaultLevels [1] = ErrorLevel = ( MaxThreads * 9 ) / 10 ;
} /* endmethod */

ThreadCount::~ThreadCount ( ) {
   DosFreeMem ( DQSS_Buffer ) ;
} /* endmethod */

int ThreadCount::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   int Result ;
   char Text [100] ;
   Result = sprintf ( Text, "%s 123 ", QueryCurrentLabel() ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
} /* endmethod */

__int64 ThreadCount::NewValue ( void ) {

   if ( DosQuerySysState ( 0x17, 0, 0, 0, DQSS_Buffer, DQSS_BUFFER_SIZE ) )
      return ( 0 ) ;

   return ( ((QTOPLEVEL*)DQSS_Buffer)->gbldata->threadcnt ) ;
} /* endmethod */

void ThreadCount::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   sprintf ( Text, "%lu ", Value ) ;
   DdeUpdate ( Text ) ;
} /* endmethod */

void ThreadCount::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Count = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Count != Value ) ) {
      char Label[100], Text[100] ;
      FormatText ( Label, Text, Count ) ;
      Paint ( canvas, Rectangle, Label, Text, Count ) ;
   } /* endif */
} /* endmethod */

Battery::Battery ( USHORT id, char *pName, char *pCurrentLabel, char *pDefaultLabel,
   void *pDdeServer, char *Topic, ResourceString *error, ResourceString *charging, ResourceString *ac )
   : Item ( id, pName, pCurrentLabel, pDefaultLabel, pDdeServer, Topic ),
   Handle(-1), Error(error), Charging(charging), AC(ac) {
   LevelSense = -1 ;
   SenseEnabled = TRUE ;
   MinLevel = 0 ;
   MaxLevel = 100 ;
   DefaultLevels [0] = WarningLevel = 25 ;
   DefaultLevels [1] = ErrorLevel = 10 ;
   ULONG Action = 0;
   APIRET Status = DosOpen ( (PSZ) "APM$", &Handle, &Action, 0, FILE_NORMAL,
      FILE_OPEN, OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYREADWRITE | OPEN_FLAGS_FAIL_ON_ERROR, 0 ) ;
   if ( Status )
      Handle = 0xFFFFFFFF ;
} /* endmethod */

Battery::~Battery ( ) {
   if ( Handle != 0xFFFFFFFF )
      DosClose ( Handle ) ;
} /* endmethod */

int Battery::Measure ( Canvas &canvas, RECTANGLE &Rectangle ) {
   int Result ;
   char Text [100] ;
   Result = sprintf ( Text, "%s %s ", QueryCurrentLabel(), PCHAR(*Charging) ) ;
   Rectangle = canvas.MeasureText ( Text ) ;
   return ( Result ) ;
} /* endmethod */

__int64 Battery::NewValue ( void ) {

   if ( Handle == 0xFFFFFFFF )
      return ( -1 ) ;

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
   APIRET Status = DosDevIOCtl ( Handle, IOCTL_POWER, POWER_GETPOWERSTATUS, &Parm, sizeof(Parm), &ParmLen, &Data, sizeof(Data), &DataLen ) ;
   if ( Status ) {
      DosClose ( Handle ) ;
      Handle = 0xFFFFFFFF ;
      return ( -1 ) ;
   } else {
      if ( Parm.PowerFlags & 1 ) {                       // If APM enabled,
         if ( Parm.BatteryStatus == 3 ) {                //   If battery charging,
            if ( Parm.BatteryLife < 100 )                //     If not fully charged,
               return ( 101 ) ;                          //       Report CHARGING.
            else                                         //     Else
               return ( 100 ) ;                          //       Report 100%.
         } else if ( Parm.ACStatus == 1 ) {              //   Else if A/C,
            return ( 201 ) ;                             //     Report A/C.
         } else if ( Parm.BatteryLife <= 100 ) {         //   Else if not fully charged,
            return ( Parm.BatteryLife ) ;                //     Report percentage full.
         } else {                                        //   Else,
            return ( -1 ) ;                              //     Report error.
         } /* endif */                                   //
      } else {                                           // Else,
         return ( -1 ) ;                                 //   Report error.
      } /* endif */
   } /* endif */

} /* endmethod */

void Battery::FormatText ( char *Label, char *Text, __int64 Value ) {
   strcpy ( Label, CurrentLabel ) ;
   if ( (long)Value < 0 ) {
      strcpy ( Text, PCHAR(*Error) ) ;
      strcat ( Text, " " ) ;
   } else if ( Value > 200 ) {
      strcpy ( Text, PCHAR(*AC) ) ;
      strcat ( Text, " " ) ;
   } else if ( Value > 100 ) {
      strcpy ( Text, PCHAR(*Charging) ) ;
      strcat ( Text, " " ) ;
   } else {
      sprintf ( Text, "%lu%%", Value ) ;
   } /* endif */
   DdeUpdate ( Text ) ;
} /* endmethod */

void Battery::Repaint ( Canvas &canvas, RECTANGLE &Rectangle, BOOL Mandatory ) {
   ULONG Count = ULONG ( NewValue ( ) ) ;
   if ( Mandatory || ( Count != Value ) ) {
      char Label[100], Text[100] ;
      FormatText ( Label, Text, Count ) ;
      Paint ( canvas, Rectangle, Label, Text, Count ) ;
   } /* endif */
} /* endmethod */

int Battery::Discharging ( ) {

   if ( Handle != 0xFFFFFFFF )
      return ( FALSE ) ;

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
   APIRET Status = DosDevIOCtl ( Handle, IOCTL_POWER, POWER_GETPOWERSTATUS, &Parm, sizeof(Parm), &ParmLen, &Data, sizeof(Data), &DataLen ) ;
   if ( Status ) {
      DosClose ( Handle ) ;
      Handle = 0xFFFFFFFF ;
   } else {
      if ( Parm.PowerFlags & 1 ) {
         if ( Parm.BatteryStatus == 3 ) {
            ;
         } else if ( Parm.ACStatus == 1 ) {
            ;
         } else if ( Parm.BatteryLife <= 100 ) {
            return ( TRUE ) ;
         } /* endif */
      } /* endif */
   } /* endif */

   return ( FALSE ) ;
}

#endif // __OS2__
