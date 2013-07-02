/******************************************************************* CONFIG.H
 *                                                                          *
 *                     Configure Dialog definitions                         *
 *                                                                          *
 ****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include "System.h"

#include "Dialog.h"
#include "Profile2.h"
#include "Thread.h"
#include "Window.h"

enum {
   CORNER_BL,
   CORNER_BR,
   CORNER_TL,
   CORNER_TR
} ;

enum { 
   SHOWK_NEVER, 
   SHOWK_ABOVE512, 
   SHOWK_ALWAYS 
} ;

typedef struct {

  BOOL   HideControls ;
  BOOL   Float ;
  BOOL   Animate ;
  BOOL   DialogBorder ;
  BOOL   TableFormat ;
  BOOL   Chime ;
  BOOL   ShowDiskDrives ;
  BOOL   ShowRemoteDrives ;
  BOOL   ShowFileSystemNames ;
  BOOL   ShowDiskLabels ;
  BOOL   ShowSeconds ;
  BOOL   UseTheseus ;
  BOOL   CenterClock ;
  USHORT ShowK ;
  USHORT ShowTrueK ;
  USHORT ShowM ;
  USHORT ShowG ;
  Priority MonitorPriority ;
  ULONG  TimerInterval ;
  ULONG  AveragingInterval ;
  USHORT AnchorCorner ;
  char   DateFormat [80] ;
  ULONG  NormalBackground, NormalForeground ;
  ULONG  WarningBackground, WarningForeground ;
  ULONG  ErrorBackground, ErrorForeground ;
  char   Font [80] ;
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

  USHORT  ItemCount ;
  BOOL    ItemFlags [ ITEM_BASE_COUNT + MAX_DRIVES ] ;
  char    DefaultLabels [ ITEM_BASE_COUNT + MAX_DRIVES ] [ 80 ] ;
  char    CurrentLabels [ ITEM_BASE_COUNT + MAX_DRIVES ] [ 80 ] ;
  __int64 DefaultLevels [ ITEM_BASE_COUNT + MAX_DRIVES ] [ 2 ] ;
  __int64 WarningLevel [ ITEM_BASE_COUNT + MAX_DRIVES ] ;
  __int64 ErrorLevel [ ITEM_BASE_COUNT + MAX_DRIVES ] ;
  int     LevelSense [ ITEM_BASE_COUNT + MAX_DRIVES ] ;
  int     SenseEnabled [ ITEM_BASE_COUNT + MAX_DRIVES ] ;
  __int64 MinLevel [ ITEM_BASE_COUNT + MAX_DRIVES ] ;
  __int64 MaxLevel [ ITEM_BASE_COUNT + MAX_DRIVES ] ;

  // State information.
  int    Ready ;
  int    MostRecentSelection ;
  HWND   MainWindow ;

} CONFIG_PARMS, *PCONFIG_PARMS ;

class Configure_Dialog : public Dialog {

   private:
      CONFIG_PARMS *Parms ;

   public:
      Configure_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, CONFIG_PARMS &parms ) ;

      virtual DRESULT Control ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT FormatFrame ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT UpdateDrives ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

#endif
