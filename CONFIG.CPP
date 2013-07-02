/***************************************************************** CONFIG.CPP
 *                                                                          *
 *                           Configuration Dialog                           *
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
#include "DateFmt.h"
#include "Debug.h"
#include "Items.h"
#include "Memsize.h"
#include "Pop.h"
#include "Support.h"
#include "Thread.h"
#include "Window.h"

// #define DEBUG

/****************************************************************************
 *                                                                          *
 *      Class: Checkbox Control                                             *
 *                                                                          *
 ****************************************************************************/

class Checkbox : public Window {

   public:
      Checkbox ( HWND control ) : Window ( control ) {
         #ifdef DEBUG
            DebugMessages = TRUE ;
         #endif // DEBUG
      } /* endmethod */

      virtual ~Checkbox ( ) {
      } /* endmethod */

      void Set ( int Flag = TRUE ) {
         SendMessage ( BM_SETCHECK, MP1FROMSHORT(Flag) ) ;
      } /* endmethod */

      int Query ( ) {
         return ( SHORT1FROMMR ( SendMessage ( BM_QUERYCHECK ) ) ) ;
      } /* endmethod */
} ;


/****************************************************************************
 *                                                                          *
 *      Class: Entryfield Control                                           *
 *                                                                          *
 ****************************************************************************/

class Entryfield : public Window {

   public:
      Entryfield ( HWND control ) : Window ( control ) {
         #ifdef DEBUG
            DebugMessages = TRUE ;
         #endif // DEBUG
      } /* endmethod */

      virtual ~Entryfield ( ) {
      } /* endmethod */

      void SetLimit ( int Limit ) {
         #ifdef __OS2__
            SendMessage ( EM_SETTEXTLIMIT, MPFROM2SHORT(Limit,0) ) ;
         #else
            // ELABORATE
            Limit = 0 ;
         #endif
      } /* endmethod */

      void QuerySelection ( int &First, int &Last ) {
         #ifdef __OS2__
            MRESULT Result = SendMessage ( EM_QUERYSEL ) ;
            First = SHORT1FROMMR ( Result ) ;
            Last = SHORT2FROMMR ( Result ) ;
         #else
            First = Last = 0 ;
         #endif
      } /* endmethod */

      void Insert ( char *Text ) {
         char Temp [200] ; GetText ( Temp, sizeof(Temp) ) ;
         int First, Last ; QuerySelection ( First, Last ) ;
         char Result [200] = { 0 } ;
         if ( First )
            sprintf ( Result, "%0.*s", First, Temp ) ;
         strcat ( Result, Text ) ;
         if ( Last < strlen(Temp) )
            strcat ( Result, Temp+Last ) ;
         SetText ( Result ) ;
      } /* endmethod */
} ;


/****************************************************************************
 *                                                                          *
 *      Class: Static Text Control                                          *
 *                                                                          *
 ****************************************************************************/

class StaticText : public Window {

   public:
      StaticText ( HWND control ) : Window ( control ) {
         #ifdef DEBUG
            DebugMessages = TRUE ;
         #endif // DEBUG
      } /* endmethod */

      virtual ~StaticText ( ) {
      } /* endmethod */
} ;


/****************************************************************************
 *                                                                          *
 *      Class: Spin-button Control                                          *
 *                                                                          *
 ****************************************************************************/

class SpinButton : public Window {

   public:
      SpinButton ( HWND control ) : Window ( control ) {
         #ifdef DEBUG
            DebugMessages = TRUE ;
         #endif // DEBUG
      } /* endmethod */

      void SetLimits ( int LowerLimit, int UpperLimit ) {
         #ifdef __OS2__
            SendMessage ( SPBM_SETLIMITS, MPARAM1(UpperLimit), MPARAM2(LowerLimit) ) ;
         #else
            // ELABORATE
            LowerLimit = 0 ; UpperLimit = 0 ; // Pacify the compiler.
         #endif
      } /* endmethod */

      void Set ( int Value ) {
         #ifdef __OS2__
            SendMessage ( SPBM_SETCURRENTVALUE, MPARAM1(Value) ) ;
         #else
            // ELABORATE
            Value = 0 ;  // Pacify the compiler.
         #endif
      } /* endmethod */

      __int64 Query ( ) {
         __int64 Value ( 0 ) ;
         #ifdef __OS2__
            SendMessage ( SPBM_QUERYVALUE, &Value, MPFROM2SHORT(0,SPBQ_UPDATEIFVALID) ) ;
         #else
            // ELABORATE
         #endif
         return ( Value ) ;
      } /* endmethod */
} ;


/****************************************************************************
 *                                                                          *
 *      Class: Slider Control                                               *
 *                                                                          *
 ****************************************************************************/

class Slider : public Window {

   private:
      Entryfield *EntryField ;
      __int64 MinLevel, MaxLevel ;
      int Divisor ;
      char *Suffix ;

   public:
      Slider ( HWND control, HWND entryfield, __int64 Level, __int64 minlevel, __int64 maxlevel, int divisor, char *suffix ) :
         Window(control), MinLevel(minlevel), MaxLevel(maxlevel), Divisor(divisor), Suffix(suffix) {

         #ifdef DEBUG
            DebugMessages = TRUE ;
         #endif // DEBUG

         EntryField = new Entryfield ( entryfield ) ;

         char Text [20] ;

         #ifdef __OS2__

         // Set left label.
         sprintf ( Text, "%i%s", MinLevel/Divisor, Suffix ) ;
         SendMessage ( SLM_SETSCALETEXT, MPFROMSHORT(0), MP2FROMP(Text) ) ;

         // Set center label.
         sprintf ( Text, "%i%s", (MinLevel+(MaxLevel-MinLevel)/2)/Divisor, Suffix ) ;
         SendMessage ( SLM_SETSCALETEXT, MPFROMSHORT(50), MP2FROMP(Text) ) ;

         // Set right label.
         sprintf ( Text, "%i%s", MaxLevel/Divisor, Suffix ) ;
         SendMessage ( SLM_SETSCALETEXT, MPFROMSHORT(100), MP2FROMP(Text) ) ;

         // Position the slider.
         short Tick = short ( ( double ( Level - MinLevel ) / ( Divisor / 100.0 ) ) / ( double ( MaxLevel - MinLevel ) / Divisor ) ) ;
         SendMessage ( SLM_SETSLIDERINFO, MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE), MPFROMSHORT(Tick) ) ;

         #endif // __OS2__

         // Set the entryfield text.
         sprintf ( Text, "%.1lf%s", double(Level)/Divisor, Suffix ) ;
         EntryField->SetText ( Text ) ;

      } /* endmethod */

      Slider ( HWND control, HWND entryfield ) : Window ( control ) {
         EntryField = new Entryfield ( entryfield ) ;
         Enable ( FALSE ) ;
      } /* endmethod */

      virtual ~Slider ( ) {
         delete EntryField ;
      } /* endmethod */

      void Set ( __int64 Level, BOOL SetText=TRUE ) {

         #ifdef __OS2__

         // Position the slider.
         short Tick = short ( ( double ( Level - MinLevel ) / ( Divisor / 100.0 ) ) / ( double ( MaxLevel - MinLevel ) / Divisor ) ) ;
         SendMessage ( SLM_SETSLIDERINFO, MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE), MPFROMSHORT(Tick) ) ;

         #endif // __OS2__

         // Set the entryfield text.
         if ( SetText ) {
            char Text [20] ;
            sprintf ( Text, "%.1lf%s", double(Level)/Divisor, Suffix ) ;
            EntryField->SetText ( Text ) ;
         } /* endif */

      } /* endmethod */

      __int64 Read ( ) {

         #ifdef __OS2__

         // Read the slider.
         ULONG Tick = LONGFROMMR ( SendMessage ( SLM_QUERYSLIDERINFO, MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE) ) ) ;
         __int64 Level = MinLevel + ( Tick * ( double ( MaxLevel - MinLevel ) / ( Divisor * 100.0 ) ) * Divisor ) ;

         #else

         __int64 Level ( 0 ) ;

         #endif // __OS2__

         // Update the entry field.
         char Text [20] ;
         sprintf ( Text, "%.1lf%s", double(Level)/Divisor, Suffix ) ;
         EntryField->SetText ( Text ) ;

         // Return the result.
         return ( Level ) ;

      } /* endmethod */

      void Enable ( int Flag ) {
         EntryField->Enable ( Flag ) ;
         Window::Enable ( Flag ) ;
      } /* endmethod */
} ;


/****************************************************************************
 *                                                                          *
 *      Class: Listbox Control                                              *
 *                                                                          *
 ****************************************************************************/

class ListBox : public SubclassedWindow {

   public:
      ListBox ( HWND handle ) : SubclassedWindow(handle) {
         #ifdef DEBUG
            DebugMessages = TRUE ;
         #endif // DEBUG
      } /* endmethod */

      virtual ~ListBox ( ) {
      } /* endmethod */

      virtual WINDOWPROC MessageProcessor ;

      void Clear ( ) {
         #ifdef __OS2__
            Sys_SendMessage ( Handle, LM_DELETEALL, 0, 0 ) ;
         #else
            Sys_SendMessage ( Handle, LB_RESETCONTENT, 0, 0 ) ;
         #endif
      } /* endmethod */

      void Insert ( char *String, int Where=-1 ) {
         #ifdef __OS2__
            Sys_SendMessage ( Handle, LM_INSERTITEM, MPFROMSHORT(Where), MP2FROMP(String) ) ;
         #else
            Sys_SendMessage ( Handle, LB_INSERTSTRING, MPARAM1(Where), MPARAM2(String) ) ;
         #endif
      } /* endmethod */

      void Delete ( int Where ) {
         #ifdef __OS2__
            Sys_SendMessage ( Handle, LM_DELETEITEM, MPFROMSHORT(Where), 0 ) ;
         #else
            Sys_SendMessage ( Handle, LB_DELETESTRING, MPARAM1(Where), MPARAM2(0) ) ;
         #endif
      } /* endmethod */

      void SelectItem ( int Index ) {
         #ifdef __OS2__
            Sys_SendMessage ( Handle, LM_SELECTITEM, MPFROMSHORT(Index), MPFROMSHORT(TRUE) ) ;
         #else
            Sys_SendMessage ( Handle, LB_SETCURSEL, MPARAM1(Index), MPARAM2(0) ) ;
         #endif
      } /* endmethod */

      int QuerySelection ( int Previous = -1 ) {
         #ifdef __OS2__
            return ( SHORT1FROMMR ( WinSendMsg ( Handle, LM_QUERYSELECTION, MPFROMSHORT(SHORT(Previous)), 0 ) ) ) ;
         #else
            for ( int i=Previous+1; ; i++ ) {
               int rc = SendMessage ( Handle, LB_GETCURSEL, WPARAM(i), 0 ) ;
               if ( rc > 0 ) {
                  return ( i ) ;
               } else if ( rc < 0 ) {
                  break ;
               } /* endif */
            } /* endfor */
            return ( -1 ) ;
         #endif
      } /* endmethod */

      void SetTopIndex ( int Index ) {
         #ifdef __OS2__
            WinSendMsg ( Handle, LM_SETTOPINDEX, MPFROMSHORT(Index), 0 ) ;
         #else
            SendMessage ( Handle, LB_SETTOPINDEX, WPARAM(Index), 0 ) ;
         #endif
      } /* endmethod */

      int QueryTopIndex ( ) {
         #ifdef __OS2__
            return ( SHORT1FROMMR ( Sys_SendMessage ( Handle, LM_QUERYTOPINDEX, 0, 0 ) ) ) ;
         #else
            return ( SendMessage ( Handle, LB_GETTOPINDEX, 0, 0 ) ) ;
         #endif
      } /* endmethod */

      void QueryItemText ( int Index, char *Text, int TextSize ) {
         #ifdef __OS2__
            WinSendMsg ( Handle, LM_QUERYITEMTEXT, MPFROM2SHORT(Index,TextSize), MP2FROMP(Text) ) ;
         #else
            if ( SendMessage ( Handle, LB_GETTEXTLEN, WPARAM(Index), 0 ) < TextSize ) {
               SendMessage ( Handle, LB_GETTEXT, WPARAM(Index), LPARAM(Text) ) ;
            } else {
               strncpy ( Text, "*****", TextSize-1 ) ;
            } /* endif */
         #endif
      } /* endmethod */

      void Enable ( int Flag ) {
         Sys_EnableWindow ( Handle, Flag ) ;
      } /* endmethod */
} ;

MRESULT ListBox::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   #ifdef DEBUG
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "ListBox", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   MRESULT Result = SubclassedWindow::MessageProcessor ( hwnd, msg, mp1, mp2 ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      LogMsgDone ( Indent, "ListBox", msg, Result ) ;
   #endif

   return ( Result ) ;

} /* endmethod */


/****************************************************************************
 *                                                                          *
 *      Item Label dialog processor.                                        *
 *                                                                          *
 ****************************************************************************/

inline void GetDivisor ( __int64 MaxLevel, USHORT ShowTrueK, int &Divisor, char* &Suffix ) {

   Divisor = 1 ;
   Suffix = "" ;

   if ( ShowTrueK ) {
      if ( MaxLevel > 1024*1024 ) {
         Divisor = 1024*1024 ;
         Suffix = "M" ;
      } else if ( MaxLevel > 1024 ) {
         Divisor = 1024 ;
         Suffix = "K" ;
      } /* endif */

   } else {
      if ( MaxLevel > 1000*1000 ) {
         Divisor = 1000*1000 ;
         Suffix = "m" ;
      } else if ( MaxLevel > 1000 ) {
         Divisor = 1000 ;
         Suffix = "k" ;
      } /* endif */

   } /* endif */
}

inline char *TrimRight ( char *Text ) {
   while ( strlen(Text) && Text[strlen(Text)-1] == ' ' ) Text[strlen(Text)-1] = 0 ;
   return ( Text ) ;
}

typedef struct {
   char    DefaultLabel [80] ;
   char    CurrentLabel [80] ;
   __int64 DefaultLevels [2] ;
   __int64 WarningLevel ;
   __int64 ErrorLevel ;
   int     LevelSense ;
   int     SenseEnabled ;
   __int64 MinLevel ;
   __int64 MaxLevel ;
   USHORT  ShowTrueK ;
   Slider *Warning ;
   Slider *Error ;
} LABEL_PARMS, *PLABEL_PARMS ;

class Attribute_Dialog : public Dialog {

   private:
      LABEL_PARMS *Parms ;

   public:
      Attribute_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, LABEL_PARMS &parms ) ;

      virtual DRESULT Destroy ( ) ;
      virtual DRESULT Control ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

Attribute_Dialog::Attribute_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, LABEL_PARMS &parms ) :
   Dialog ( Parent, Owner, Resource, IDD_LABEL ), Parms(&parms) {
   #ifdef DEBUG
      DebugMessages = TRUE ;
   #endif // DEBUG
   Load ( ) ;
   HelpWindow TheHelpWindow ( Handle, OWNER(Handle) ) ;
   TheHelpWindow.Associate ( Handle ) ;
   Entryfield _DefaultLabel ( CHILD(Handle,IDD_LABEL_DEFLABEL) ) ; _DefaultLabel.SetText ( Parms->DefaultLabel ) ;
   Entryfield _CurrentLabel ( CHILD(Handle,IDD_LABEL_NEWLABEL) ) ; _CurrentLabel.SetText ( Parms->CurrentLabel ) ;
   _CurrentLabel.SetLimit ( sizeof(Parms->CurrentLabel) ) ;
   Checkbox _Enabled ( CHILD(Handle,IDD_LABEL_ENABLE) ) ;
   _Enabled.Set ( Parms->SenseEnabled ) ;
   if ( Parms->LevelSense ) {
      int Divisor ;  char *Suffix ;
      GetDivisor ( Parms->MaxLevel, Parms->ShowTrueK, Divisor, Suffix ) ;
      Parms->Warning = new Slider ( CHILD(Handle,IDD_LABEL_WARNING_SLIDER), CHILD(Handle,IDD_LABEL_WARNING_VALUE),
         Parms->WarningLevel, Parms->MinLevel, Parms->MaxLevel, Divisor, Suffix ) ;
      Parms->Error = new Slider ( CHILD(Handle,IDD_LABEL_ERROR_SLIDER), CHILD(Handle,IDD_LABEL_ERROR_VALUE),
         Parms->ErrorLevel, Parms->MinLevel, Parms->MaxLevel, Divisor, Suffix ) ;
   } else {
      _Enabled.Enable ( FALSE ) ;
      Parms->Warning = new Slider ( CHILD(Handle,IDD_LABEL_WARNING_SLIDER), CHILD(Handle,IDD_LABEL_WARNING_VALUE) ) ;
      Parms->Error = new Slider ( CHILD(Handle,IDD_LABEL_ERROR_SLIDER), CHILD(Handle,IDD_LABEL_ERROR_VALUE) ) ;
   } /* endif */
   Parms->Warning->Enable ( Parms->LevelSense && Parms->SenseEnabled ) ;
   Parms->Error->Enable ( Parms->LevelSense && Parms->SenseEnabled ) ;
} /* endmethod */

DRESULT Attribute_Dialog::Destroy ( ) {
   if ( Parms->Warning ) delete Parms->Warning, Parms->Warning = 0 ;
   if ( Parms->Error ) delete Parms->Error, Parms->Error = 0 ;
   return ( 0 ) ;
} /* endmethod */

DRESULT Attribute_Dialog::Control ( MPARAM1 mp1, MPARAM2 ) {
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;
   int Divisor ;  char *Suffix ;
   GetDivisor ( Parms->MaxLevel, Parms->ShowTrueK, Divisor, Suffix ) ;
   switch ( Id ) {
      case IDD_LABEL_ENABLE: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Checkbox _Enabled ( CHILD(Handle,IDD_LABEL_ENABLE) ) ;
            Parms->SenseEnabled = _Enabled.Query ( ) ;
            Parms->Warning->Enable ( Parms->SenseEnabled ) ;
            Parms->Error->Enable ( Parms->SenseEnabled ) ;
         } /* endif */
         break; }
      case IDD_LABEL_WARNING_SLIDER: {
         #ifdef __OS2__
         if ( Sys_GetFocus() == CHILD(Handle,IDD_LABEL_WARNING_SLIDER) )
            if ( ( Message == SLN_CHANGE ) || ( Message == SLN_SLIDERTRACK ) )
               if ( Parms->Warning )
                  Parms->WarningLevel = Parms->Warning->Read ( ) ;
         #endif // __OS2__
         break; }
      case IDD_LABEL_ERROR_SLIDER: {
         #ifdef __OS2__
         if ( Sys_GetFocus() == CHILD(Handle,IDD_LABEL_ERROR_SLIDER) )
            if ( ( Message == SLN_CHANGE ) || ( Message == SLN_SLIDERTRACK ) )
               if ( Parms->Error )
                  Parms->ErrorLevel = Parms->Error->Read ( ) ;
         #endif // __OS2__
         break; }
      case IDD_LABEL_WARNING_VALUE: {
         if ( Sys_GetFocus() == CHILD(Handle,IDD_LABEL_WARNING_VALUE) ) {
            if ( Message == EN_CHANGE ) {
               Entryfield _WarningValue ( CHILD(Handle,IDD_LABEL_WARNING_VALUE) ) ;
               char Text [20] ; _WarningValue.GetText ( Text, sizeof(Text) ) ;
               TrimRight ( Text ) ;
               if ( Text[strlen(Text)-1] == 'M' ) {
                  Parms->WarningLevel = atof ( Text ) * 1024 * 1024 ;
               } else if ( Text[strlen(Text)-1] == 'K' ) {
                  Parms->WarningLevel = atof ( Text ) * 1024 ;
               } else if ( Text[strlen(Text)-1] == 'm' ) {
                  Parms->WarningLevel = atof ( Text ) * 1000 * 1000 ;
               } else if ( Text[strlen(Text)-1] == 'k' ) {
                  Parms->WarningLevel = atof ( Text ) * 1000 ;
               } else {
                  Parms->WarningLevel = atol ( Text ) ;
               } /* endif */
               if ( Parms->Warning )
                  Parms->Warning->Set ( Parms->WarningLevel, FALSE ) ;
            } /* endif */
         } /* endif */
         break; }
      case IDD_LABEL_ERROR_VALUE: {
         if ( Sys_GetFocus() == CHILD(Handle,IDD_LABEL_ERROR_VALUE) ) {
            if ( Message == EN_CHANGE ) {
               Entryfield _ErrorValue ( CHILD(Handle,IDD_LABEL_ERROR_VALUE) ) ;
               char Text [20] ; _ErrorValue.GetText ( Text, sizeof(Text) ) ;
               TrimRight ( Text ) ;
               if ( Text[strlen(Text)-1] == 'M' ) {
                  Parms->ErrorLevel = atof ( Text ) * 1024 * 1024 ;
               } else if ( Text[strlen(Text)-1] == 'K' ) {
                  Parms->ErrorLevel = atof ( Text ) * 1024 ;
               } else if ( Text[strlen(Text)-1] == 'm' ) {
                  Parms->ErrorLevel = atof ( Text ) * 1000 * 1000 ;
               } else if ( Text[strlen(Text)-1] == 'k' ) {
                  Parms->ErrorLevel = atof ( Text ) * 1000 ;
               } else {
                  Parms->ErrorLevel = atol ( Text ) ;
               } /* endif */
               if ( Parms->Error )
                  Parms->Error->Set ( Parms->ErrorLevel, FALSE ) ;
            } /* endif */
         } /* endif */
         break; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

DRESULT Attribute_Dialog::Command ( MPARAM1 mp1, MPARAM2 mp2 ) {
   #ifdef __NT__
      if ( SHORT2FROMMP(mp1) > 1 )
         return ( Control ( mp1, mp2 ) ) ;
   #else
      mp2 = 0 ; // Pacify the compiler.
   #endif
   switch ( SHORT1FROMMP(mp1) ) {
      case DID_OK: {
         Entryfield _NewLabel ( CHILD(Handle,IDD_LABEL_NEWLABEL) ) ;
         _NewLabel.GetText ( Parms->CurrentLabel, sizeof(Parms->CurrentLabel) ) ;
         if ( Parms->CurrentLabel[0] == 0 ) {
            Sys_BeepNote ( ) ;
            _NewLabel.SetFocus ( ) ;
            break ;
         } /* endif */
         Checkbox _Enabled ( CHILD(Handle,IDD_LABEL_ENABLE) ) ;
         Parms->SenseEnabled = _Enabled.Query ( ) ;
         Entryfield _WarningValue ( CHILD(Handle,IDD_LABEL_WARNING_VALUE) ) ;
         char Text [20] ; _WarningValue.GetText ( Text, sizeof(Text) ) ;
         TrimRight ( Text ) ;
         if ( Text[strlen(Text)-1] == 'M' ) {
            Parms->WarningLevel = atof ( Text ) * 1024 * 1024 ;
         } else if ( Text[strlen(Text)-1] == 'K' ) {
            Parms->WarningLevel = atof ( Text ) * 1024 ;
         } else if ( Text[strlen(Text)-1] == 'm' ) {
            Parms->WarningLevel = atof ( Text ) * 1000 * 1000 ;
         } else if ( Text[strlen(Text)-1] == 'k' ) {
            Parms->WarningLevel = atof ( Text ) * 1000 ;
         } else {
            Parms->WarningLevel = atol ( Text ) ;
         } /* endif */
         Entryfield _ErrorValue ( CHILD(Handle,IDD_LABEL_ERROR_VALUE) ) ;
         _ErrorValue.GetText ( Text, sizeof(Text) ) ;
         TrimRight ( Text ) ;
         if ( Text[strlen(Text)-1] == 'M' ) {
            Parms->ErrorLevel = atof ( Text ) * 1024 * 1024 ;
         } else if ( Text[strlen(Text)-1] == 'K' ) {
            Parms->ErrorLevel = atof ( Text ) * 1024 ;
         } else if ( Text[strlen(Text)-1] == 'm' ) {
            Parms->ErrorLevel = atof ( Text ) * 1000 * 1000 ;
         } else if ( Text[strlen(Text)-1] == 'k' ) {
            Parms->ErrorLevel = atof ( Text ) * 1000 ;
         } else {
            Parms->ErrorLevel = atol ( Text ) ;
         } /* endif */
         Sys_EndDialog ( Handle, TRUE ) ;
         break ; }
      case DID_CANCEL:
         Sys_EndDialog ( Handle, FALSE ) ;
         break ;
      case IDD_LABEL_DEFAULT: {
         strcpy ( Parms->CurrentLabel, Parms->DefaultLabel ) ;
         Entryfield _NewLabel ( CHILD(Handle,IDD_LABEL_NEWLABEL) ) ;
         _NewLabel.SetText ( Parms->CurrentLabel ) ;
         Parms->WarningLevel = Parms->DefaultLevels [0] ;
         Parms->ErrorLevel = Parms->DefaultLevels [1] ;
         if ( Parms->LevelSense ) {
            Parms->SenseEnabled = TRUE ;
            int Divisor ;  char *Suffix ;
            GetDivisor ( Parms->MaxLevel, Parms->ShowTrueK, Divisor, Suffix ) ;
            if ( Parms->Warning )
               Parms->Warning->Set ( Parms->WarningLevel ) ;
            if ( Parms->Error )
               Parms->Error->Set ( Parms->ErrorLevel ) ;
         } else {
            Parms->SenseEnabled = FALSE ;
         } /* endif */
         _NewLabel.SetFocus ( ) ;
         break ; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

DRESULT Attribute_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   #ifdef DEBUG
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "Attribute_Dialog", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   DRESULT Result ;
   switch ( msg ) {
      case WM_DESTROY:      Result = Destroy ( ) ; break ;
      #ifdef __OS2__ // Under Windows, WM_CONTROL and WM_COMMAND are the same message.
      case WM_CONTROL:      Result = Control ( mp1, mp2 ) ; break ;
      #endif // __OS2__
      case WM_COMMAND:      Result = Command ( mp1, mp2 ) ; break ;
      default:              Result = Dialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break;
   } /* endswitch */

   #ifdef DEBUG
      Indent -= 2 ;
      LogMsgDone ( Indent, "Attribute_Dialog", msg, Result ) ;
   #endif

   return ( Result ) ;

} /* endmethod */


/****************************************************************************
 *                                                                          *
 *      Class: Item Listbox Control                                         *
 *                                                                          *
 ****************************************************************************/

class ItemListBox : public ListBox {

   private:
      PCONFIG_PARMS Parms ;

   public:
      ItemListBox ( HWND handle, PCONFIG_PARMS parms ) : ListBox(handle), Parms(parms) {
         #ifdef DEBUG
            DebugMessages = TRUE ;
         #endif // DEBUG
         Load ( ) ;
      } /* endmethod */

      virtual ~ItemListBox ( ) {
      } /* endmethod */

      virtual WINDOWPROC MessageProcessor ;

      void Load ( ) {
         for ( int i=0; i<Parms->ItemCount; i++ ) {
            char Label [164] ;
            strcpy ( Label, Parms->CurrentLabels[i] ) ;
            if ( strcmp ( Parms->DefaultLabels[i], Parms->CurrentLabels[i] ) ) {
               strcat ( Label, " (" ) ;
               strcat ( Label, Parms->DefaultLabels[i] ) ;
               strcat ( Label, ")" ) ;
            } /* endif */
            Insert ( Label ) ;
            if ( Parms->ItemFlags[i] )
               SelectItem ( i ) ;
         } /* endfor */
      } /* endmethod */
} ;

MRESULT ItemListBox::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   #ifdef DEBUG
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "ItemListBox", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   switch ( msg ) {

      case WM_BUTTON2UP: {
         POINTL Point = { SHORT(SHORT1FROMMP(mp1)), SHORT(SHORT2FROMMP(mp1)) } ;
         Canvas canvas ( "ItemListBox::WM_BUTTON2UP:canvas", hwnd ) ;
         POINTL TextBox [TXTBOX_COUNT] ; canvas.QueryTextBox ( "0123456789", TextBox ) ;
         Window ThisWindow ( hwnd ) ; RECTANGLE Rectangle ( ThisWindow.QueryHandle() ) ;
         Parms->MostRecentSelection = ( Rectangle.yTop - Point.y ) / ( TextBox[TXTBOX_TOPLEFT].y - TextBox[TXTBOX_BOTTOMLEFT].y ) ;
         Parms->MostRecentSelection += QueryTopIndex() ;
         break; }

      case WM_CONTEXTMENU: {
         if ( Parms->MostRecentSelection == -1 )
            break ;
         LABEL_PARMS LabelParms ;
         strcpy ( LabelParms.DefaultLabel, Parms->DefaultLabels[Parms->MostRecentSelection] ) ;
         strcpy ( LabelParms.CurrentLabel, Parms->CurrentLabels[Parms->MostRecentSelection] ) ;
         LabelParms.DefaultLevels [0] = Parms->DefaultLevels[Parms->MostRecentSelection] [0] ;
         LabelParms.DefaultLevels [1] = Parms->DefaultLevels[Parms->MostRecentSelection] [1] ;
         LabelParms.WarningLevel = Parms->WarningLevel[Parms->MostRecentSelection] ;
         LabelParms.ErrorLevel = Parms->ErrorLevel[Parms->MostRecentSelection] ;
         LabelParms.LevelSense = Parms->LevelSense[Parms->MostRecentSelection] ;
         LabelParms.SenseEnabled = Parms->SenseEnabled[Parms->MostRecentSelection] ;
         LabelParms.MinLevel = Parms->MinLevel[Parms->MostRecentSelection] ;
         LabelParms.MaxLevel = Parms->MaxLevel[Parms->MostRecentSelection] ;
         LabelParms.ShowTrueK = Parms->ShowTrueK ;
         LabelParms.Warning = 0 ;
         LabelParms.Error = 0 ;
         Attribute_Dialog ThisDialog ( HWND_DESKTOP, hwnd, LibraryHandle, LabelParms ) ;
         if ( ThisDialog.QueryHandle() && ThisDialog.Process ( ) ) {
            strcpy ( Parms->CurrentLabels[Parms->MostRecentSelection], LabelParms.CurrentLabel ) ;
            char Label [164] ;
            strcpy ( Label, Parms->CurrentLabels[Parms->MostRecentSelection] ) ;
            if ( strcmp ( Parms->DefaultLabels[Parms->MostRecentSelection], Parms->CurrentLabels[Parms->MostRecentSelection] ) ) {
               strcat ( Label, " (" ) ;
               strcat ( Label, Parms->DefaultLabels[Parms->MostRecentSelection] ) ;
               strcat ( Label, ")" ) ;
            } /* endif */
            Parms->SenseEnabled[Parms->MostRecentSelection] = LabelParms.SenseEnabled ;
            Parms->WarningLevel[Parms->MostRecentSelection] = LabelParms.WarningLevel ;
            Parms->ErrorLevel[Parms->MostRecentSelection] = LabelParms.ErrorLevel ;
            Delete ( Parms->MostRecentSelection ) ;
            Insert ( Label, Parms->MostRecentSelection ) ;
            SelectItem ( Parms->MostRecentSelection ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }

      default:
         break ;

   } /* endswitch */

   MRESULT Result = ListBox::MessageProcessor ( hwnd, msg, mp1, mp2 ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      LogMsgDone ( Indent, "ItemListBox", msg, Result ) ;
   #endif

   return ( Result ) ;

} /* endmethod */


/****************************************************************************
 *                                                                          *
 *      Subclasses STATIC control message processor                         *
 *                                                                          *
 ****************************************************************************/

class ColorSample : public SubclassedWindow {
   private:
      #ifdef __OS2__
         HMODULE hPMWP ;
         HOBJECT ( APIENTRY *pWinQueryObject ) ( PSZ pszObjectID ) ;
         BOOL ( APIENTRY *pWinOpenObject ) ( HOBJECT hObject, ULONG ulView, BOOL Flag ) ;
      #else
         ULONG Foreground ;
         ULONG Background ;
      #endif // __OS2__
   public:
      ColorSample ( HWND Handle, ULONG Foreground, ULONG Background ) ;
      virtual ~ColorSample ( ) ;
      virtual WINDOWPROC MessageProcessor ;
      void QueryColors ( ULONG &Foreground, ULONG &Background ) ;
} ;

ColorSample::ColorSample ( HWND hwnd, ULONG foreground, ULONG background ) : SubclassedWindow(hwnd) {
   #ifdef DEBUG
      DebugMessages = TRUE ;
   #endif // DEBUG
   #ifdef __OS2__
      hPMWP = 0 ;
      pWinQueryObject = 0 ;
      pWinOpenObject = 0 ;
      if ( DosLoadModule ( 0, 0, "PMWP", &hPMWP ) ) {
         hPMWP = 0;
         return ;
      } /* endif */
      if ( DosQueryProcAddr ( hPMWP, 252, 0, (PFN*) & pWinQueryObject ) ) {
         DosFreeModule ( hPMWP ) ;
         hPMWP = 0;
         return ;
      } /* endif */
      if ( DosQueryProcAddr ( hPMWP, 286, 0, (PFN*) & pWinOpenObject ) ) {
         DosFreeModule ( hPMWP ) ;
         pWinQueryObject = 0 ;
         hPMWP = 0;
         return ;
      } /* endif */
      WinSetPresParam ( hwnd, PP_BACKGROUNDCOLOR, sizeof(background), MPFROMP(&background) ) ;
      WinSetPresParam ( hwnd, PP_FOREGROUNDCOLOR, sizeof(foreground), MPFROMP(&foreground) ) ;
   #else // __NT__
      // ELABORATE
      Foreground = foreground ;
      Background = background ;
   #endif // __OS2__ vs __NT__
} /* endmethod */

ColorSample::~ColorSample ( ) {
   #ifdef __OS2__
      if ( hPMWP )
         DosFreeModule ( hPMWP ), hPMWP = 0 ;
   #else // __NT__
      // ELABORATE
   #endif // __OS2__ vs __NT__
} /* endmethod */

MRESULT ColorSample::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   #ifdef DEBUG
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "ColorSample", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   switch ( msg ) {

      #ifdef __OS2__

      case WM_SINGLESELECT:
      case WM_CONTEXTMENU: {
         if ( pWinQueryObject && pWinOpenObject ) {
            HOBJECT hObject = pWinQueryObject ( "<WP_LORESCLRPAL>" ) ;
            if ( hObject ) {
               if ( !pWinOpenObject ( hObject, 0, TRUE ) ) {
                  pWinOpenObject ( hObject, 0, FALSE ) ;
               } /* endif */
            } /* endif */
         } /* endif */
         break ; }

      case WM_PRESPARAMCHANGED: {
         USHORT Id = WinQueryWindowUShort ( hwnd, QWS_ID ) ;
         switch ( LONGFROMMP(mp1) ) {
            case PP_BACKGROUNDCOLOR: {
               Sys_SendMessage ( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,0), 0 ) ;
               break; }
            case PP_FOREGROUNDCOLOR: {
               Sys_SendMessage ( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,0), 0 ) ;
               break; }
         } /* endswitch */
         break ; }

      #endif // __OS2__

      default:
         break;

   } /* endswitch */

   MRESULT Result = SubclassedWindow::MessageProcessor ( hwnd, msg, mp1, mp2 ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      LogMsgDone ( Indent, "ColorSample", msg, Result ) ;
   #endif

   return ( Result ) ;

} /* endmethod */

void ColorSample::QueryColors ( ULONG &foreground, ULONG &background ) {
   #ifdef __OS2__
      WinQueryPresParam ( Handle, PP_FOREGROUNDCOLOR, 0, 0, sizeof(foreground), &foreground, 0 ) ;
      WinQueryPresParam ( Handle, PP_BACKGROUNDCOLOR, 0, 0, sizeof(background), &background, 0 ) ;
   #else
      // ELABORATE
      foreground = Foreground ;
      background = Background ;
   #endif
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *      Subclasses STATIC control message processor                         *
 *                                                                          *
 ****************************************************************************/

class FontSample : public SubclassedWindow {
   private:
      char Font [80] ;
      #ifdef __OS2__
         HMODULE hPMWP ;
         HOBJECT ( APIENTRY *pWinQueryObject ) ( PSZ pszObjectID ) ;
         BOOL ( APIENTRY *pWinOpenObject ) ( HOBJECT hObject, ULONG ulView, BOOL Flag ) ;
      #endif // __OS2__
   public:
      FontSample ( HWND Handle, char *font ) ;
      virtual ~FontSample ( ) ;
      virtual WINDOWPROC MessageProcessor ;
      void QueryFont ( char *font, int fontsize ) ;
} ;

FontSample::FontSample ( HWND hwnd, char *font ) : SubclassedWindow(hwnd) {
   #ifdef DEBUG
      DebugMessages = TRUE ;
   #endif // DEBUG
   strcpy ( Font, font ) ;
   #ifdef __OS2__
      hPMWP = 0 ;
      pWinQueryObject = 0 ;
      pWinOpenObject = 0 ;
      if ( DosLoadModule ( 0, 0, "PMWP", &hPMWP ) ) {
         hPMWP = 0;
         return ;
      } /* endif */
      if ( DosQueryProcAddr ( hPMWP, 252, 0, (PFN*) & pWinQueryObject ) ) {
         DosFreeModule ( hPMWP ) ;
         hPMWP = 0;
         return ;
      } /* endif */
      if ( DosQueryProcAddr ( hPMWP, 286, 0, (PFN*) & pWinOpenObject ) ) {
         DosFreeModule ( hPMWP ) ;
         pWinQueryObject = 0 ;
         hPMWP = 0;
         return ;
      } /* endif */
      WinSetPresParam ( hwnd, PP_FONTNAMESIZE, strlen(Font)+1, MPFROMP(Font) ) ;
   #else // __NT__
      // ELABORATE
   #endif // __OS2__ vs __NT__
   ResourceString Format ( LibraryHandle, IDS_CONFIG_FONT_TEXT ) ;
   char Text [80] ;
   sprintf ( Text, PCHAR(Format), Font ) ;
   Sys_SetWindowText ( hwnd, Text ) ;
} /* endmethod */

FontSample::~FontSample ( ) {
   #ifdef __OS2__
      if ( hPMWP )
         DosFreeModule ( hPMWP ), hPMWP = 0 ;
   #else // __NT__
      // ELABORATE
   #endif // __OS2__ vs __NT__
} /* endmethod */

MRESULT FontSample::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   #ifdef DEBUG
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "FontSample", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   switch ( msg ) {

      #ifdef __OS2__

      case WM_SINGLESELECT:
      case WM_CONTEXTMENU: {
         if ( pWinQueryObject && pWinOpenObject ) {
            HOBJECT hObject = pWinQueryObject ( "<WP_FNTPAL>" ) ;
            if ( hObject ) {
               if ( !pWinOpenObject ( hObject, 0, TRUE ) ) {
                  pWinOpenObject ( hObject, 0, FALSE ) ;
               } /* endif */
            } /* endif */
         } /* endif */
         break ; }

      case WM_PRESPARAMCHANGED: {
         USHORT Id = WinQueryWindowUShort ( hwnd, QWS_ID ) ;
         switch ( LONGFROMMP(mp1) ) {
            case PP_FONTNAMESIZE: {
               WinQueryPresParam ( Handle, PP_FONTNAMESIZE, 0, 0, sizeof(Font), Font, 0 ) ;
               ResourceString Format ( LibraryHandle, IDS_CONFIG_FONT_TEXT ) ;
               char Text [80] ;
               sprintf ( Text, PCHAR(Format), Font ) ;
               Sys_SetWindowText ( hwnd, Text ) ;
               Sys_SendMessage ( OWNER(hwnd), WM_CONTROL, MPFROM2SHORT(Id,0), 0 ) ;
               break; }
         } /* endswitch */
         break ; }

      #endif // __OS2__

      default:
         break;

   } /* endswitch */

   MRESULT Result = SubclassedWindow::MessageProcessor ( hwnd, msg, mp1, mp2 ) ;

   #ifdef DEBUG
      Indent -= 2 ;
      LogMsgDone ( Indent, "FontSample", msg, Result ) ;
   #endif

   return ( Result ) ;

} /* endmethod */

void FontSample::QueryFont ( char *font, int fontsize ) {
   #ifdef __OS2__
      WinQueryPresParam ( Handle, PP_FONTNAMESIZE, 0, 0, fontsize, font, 0 ) ;
   #else
      // ELABORATE
      strncpy ( font, Font, fontsize ) ;
   #endif
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *  Configuration Items Page message processor                              *
 *                                                                          *
 ****************************************************************************/

class Configure_Items_Dialog : public PageDialog {

   private:
      PCONFIG_PARMS Parms ;
      ItemListBox _ItemListBox ;

   public:
      Configure_Items_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) ;
      virtual ~Configure_Items_Dialog ( ) { }

      virtual DRESULT Control ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT UpdateDrives ( PINIDATA IniData ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

Configure_Items_Dialog::Configure_Items_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) :
   PageDialog ( Parent, Owner, Resource, Id ), Parms(parms), _ItemListBox(CHILD(Handle,IDD_CONFIG_ITEMS),parms) {
   HelpWindow TheHelpWindow ( Handle, Parms->MainWindow ) ;
   TheHelpWindow.Associate ( Handle ) ;
} /* endmethod */

DRESULT Configure_Items_Dialog::Control ( MPARAM1 mp1, MPARAM2 ) {
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;
   switch ( Id ) {
      case IDD_CONFIG_ITEMS: {
         switch ( Message ) {
            #ifdef __OS2__
            case LN_SELECT: {
               if ( NOT Parms->Ready )
                  break ;
               SHORT Selection = -1 ;
               SHORT Last = -1 ;
               do {
                  Selection = _ItemListBox.QuerySelection ( Selection ) ;
                  if ( Selection != -1 ) {
                     for ( Last++; Last<Selection; Last++ ) {
                        if ( Parms->ItemFlags[Last] ) {
                           Parms->ItemFlags[Last] = FALSE ;
                           Parms->MostRecentSelection = Last ;
                        } /* endif */
                     } /* endfor */
                     if ( Parms->ItemFlags[Selection] == FALSE ) {
                        Parms->ItemFlags[Selection] = TRUE ;
                        Parms->MostRecentSelection = Selection ;
                     } /* endif */
                  } else {
                     for ( Last++; Last<Parms->ItemCount; Last++ ) {
                        if ( Parms->ItemFlags[Last] ) {
                           Parms->ItemFlags[Last] = FALSE ;
                           Parms->MostRecentSelection = Last ;
                        } /* endif */
                     } /* endfor */
                  } /* endif */
               } while ( Selection != -1 ) ;
               Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
               break ;
            } /* endcase */
            case LN_ENTER: {
               if ( Parms->MostRecentSelection == -1 )
                  break ;
               LABEL_PARMS LabelParms ;
               strcpy ( LabelParms.DefaultLabel, Parms->DefaultLabels[Parms->MostRecentSelection] ) ;
               strcpy ( LabelParms.CurrentLabel, Parms->CurrentLabels[Parms->MostRecentSelection] ) ;
               LabelParms.DefaultLevels [0] = Parms->DefaultLevels[Parms->MostRecentSelection] [0] ;
               LabelParms.DefaultLevels [1] = Parms->DefaultLevels[Parms->MostRecentSelection] [1] ;
               LabelParms.WarningLevel = Parms->WarningLevel[Parms->MostRecentSelection] ;
               LabelParms.ErrorLevel = Parms->ErrorLevel[Parms->MostRecentSelection] ;
               LabelParms.LevelSense = Parms->LevelSense[Parms->MostRecentSelection] ;
               LabelParms.SenseEnabled = Parms->SenseEnabled[Parms->MostRecentSelection] ;
               LabelParms.MinLevel = Parms->MinLevel[Parms->MostRecentSelection] ;
               LabelParms.MaxLevel = Parms->MaxLevel[Parms->MostRecentSelection] ;
               LabelParms.ShowTrueK = Parms->ShowTrueK ;
               LabelParms.Warning = 0 ;
               LabelParms.Error = 0 ;
               Attribute_Dialog ThisDialog ( HWND_DESKTOP, Handle, LibraryHandle, LabelParms ) ;
               if ( ThisDialog.QueryHandle() && ThisDialog.Process ( ) ) {
                  strcpy ( Parms->CurrentLabels[Parms->MostRecentSelection], LabelParms.CurrentLabel ) ;
                  char Label [164] ;
                  strcpy ( Label, Parms->CurrentLabels[Parms->MostRecentSelection] ) ;
                  if ( strcmp ( Parms->DefaultLabels[Parms->MostRecentSelection], Parms->CurrentLabels[Parms->MostRecentSelection] ) ) {
                     strcat ( Label, " (" ) ;
                     strcat ( Label, Parms->DefaultLabels[Parms->MostRecentSelection] ) ;
                     strcat ( Label, ")" ) ;
                  } /* endif */
                  Parms->SenseEnabled[Parms->MostRecentSelection] = LabelParms.SenseEnabled ;
                  Parms->WarningLevel[Parms->MostRecentSelection] = LabelParms.WarningLevel ;
                  Parms->ErrorLevel[Parms->MostRecentSelection] = LabelParms.ErrorLevel ;
                  _ItemListBox.Delete ( Parms->MostRecentSelection ) ;
                  _ItemListBox.Insert ( Label, Parms->MostRecentSelection ) ;
                  _ItemListBox.SelectItem ( Parms->MostRecentSelection ) ;
                  Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
               } /* endif */
               break ;
            } /* endcase */
            #endif // __OS2__
            default: 
               break ;
         } /* endswitch */
         break; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

DRESULT Configure_Items_Dialog::Command ( MPARAM1 mp1, MPARAM2 mp2 ) {
   #ifdef __NT__
      if ( SHORT2FROMMP(mp1) > 1 )
         return ( Control ( mp1, mp2 ) ) ;
   #else
      mp2 = 0 ; // Pacify the compiler.
   #endif
   return ( PageDialog::Command ( mp1, mp2 ) ) ;
} /* endmethod */

DRESULT Configure_Items_Dialog::UpdateDrives ( PINIDATA IniData ) {
   Parms->Ready = FALSE ;
   Parms->ItemCount = IniData->ItemCount ;
   for ( int i=0; i<IniData->ItemCount; i++ ) {
      Item *pItem = IniData->Items[i] ;
      Parms->ItemFlags[i] = pItem->QueryFlag () ;
      strcpy ( Parms->CurrentLabels[i], PCHAR(pItem->QueryCurrentLabel()) ) ;
      strcpy ( Parms->DefaultLabels[i], PCHAR(pItem->QueryDefaultLabel()) ) ;
      pItem->QueryDefaultLevels ( Parms->DefaultLevels[i][0], Parms->DefaultLevels[i][1] ) ;
      Parms->WarningLevel[i] = pItem->QueryWarningLevel() ;
      Parms->ErrorLevel[i] = pItem->QueryErrorLevel() ;
      Parms->LevelSense[i] = pItem->QueryLevelSense() ;
      Parms->SenseEnabled[i] = pItem->QuerySenseEnabled() ;
      Parms->MinLevel[i] = pItem->QueryMinLevel() ;
      Parms->MaxLevel[i] = pItem->QueryMaxLevel() ;
   } /* endfor */
   _ItemListBox.Clear ( ) ;
   _ItemListBox.Load ( ) ;
   Parms->Ready = TRUE ;
   return ( 0 ) ;
} /* endmethod */

DRESULT Configure_Items_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Configure_Items_Dialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   DRESULT Result ;

   switch ( msg ) {
      #ifdef __OS2__
      case WM_CONTROL:      Result = Control ( mp1, mp2 ) ; break ;
      #endif // __OS2__
      case WM_COMMAND:      Result = Command ( mp1, mp2 ) ; break ;
      case WM_UPDATEDRIVES: Result = UpdateDrives ( PINIDATA ( PVOIDFROMMP ( mp1 ) ) ) ; break ;
      default:              Result = PageDialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Configure_Items_Dialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *  Configuration Options Page 1 message processor                          *
 *                                                                          *
 ****************************************************************************/

class Configure_Options1_Dialog : public PageDialog {

   private:
      PCONFIG_PARMS Parms ;
      Checkbox _HideControls, _Float, _Animate, _DialogBorder, _TableFormat ;
      Checkbox _Chime, _ShowDiskDrives, _ShowRemoteDrives, _ShowFileSystemNames, _ShowDiskLabels, _ShowSeconds, _UseTheseus, _CenterClock ;

   public:
      Configure_Options1_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) ;
      virtual ~Configure_Options1_Dialog ( ) { }

      virtual DRESULT Control ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

Configure_Options1_Dialog::Configure_Options1_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) :
   PageDialog ( Parent, Owner, Resource, Id ), Parms(parms),
   _HideControls(CHILD(Handle,IDD_CONFIG_HIDECONTROLS)),
   _Float(CHILD(Handle,IDD_CONFIG_FLOAT)),
   _Animate(CHILD(Handle,IDD_CONFIG_ANIMATE)),
   _DialogBorder(CHILD(Handle,IDD_CONFIG_DIALOGBORDER)),
   _TableFormat(CHILD(Handle,IDD_CONFIG_TABLE)),
   _Chime(CHILD(Handle,IDD_CONFIG_CHIME)),
   _ShowDiskDrives(CHILD(Handle,IDD_CONFIG_SHOWDRIVES)),
   _ShowRemoteDrives(CHILD(Handle,IDD_CONFIG_SHOWREMOTES)),
   _ShowFileSystemNames(CHILD(Handle,IDD_CONFIG_FSNAME)),
   _ShowDiskLabels(CHILD(Handle,IDD_CONFIG_DLABEL)),
   _ShowSeconds(CHILD(Handle,IDD_CONFIG_SECONDS)), 
   _UseTheseus(CHILD(Handle,IDD_CONFIG_THESEUS)), 
   _CenterClock(CHILD(Handle,IDD_CONFIG_CENTER_CLOCK)) {
   HelpWindow TheHelpWindow ( Handle, Parms->MainWindow ) ;
   TheHelpWindow.Associate ( Handle ) ;
   _HideControls.Set ( Parms->HideControls ) ;
   #ifdef __NT__
      _HideControls.Disable ( ) ;
   #endif // __NT__
   _Float.Set ( Parms->Float ) ;
   _Animate.Set ( Parms->Animate ) ;
   _DialogBorder.Set ( Parms->DialogBorder ) ;
   _TableFormat.Set ( Parms->TableFormat ) ;
   _Chime.Set ( Parms->Chime ) ;
   _ShowDiskDrives.Set ( Parms->ShowDiskDrives ) ;
   _ShowRemoteDrives.Set ( Parms->ShowRemoteDrives ) ;
   _ShowRemoteDrives.Enable ( Parms->ShowDiskDrives ) ;
   _ShowFileSystemNames.Set ( Parms->ShowFileSystemNames ) ;
   _ShowDiskLabels.Set ( Parms->ShowDiskLabels ) ;
   _ShowSeconds.Set ( Parms->ShowSeconds ) ;
   _CenterClock.Set ( Parms->CenterClock ) ;
   #ifdef __OS2__
      HMODULE hTheseus ;  
      if ( !DosLoadModule ( 0, 0, "THESEUS0.DLL", &hTheseus ) ) {
         PFN pfn ;
         if ( !DosQueryProcAddr ( hTheseus, 0, "T2Initialize0", &pfn ) && !DosQueryProcAddr ( hTheseus, 0, "T2Terminate0", &pfn ) 
            && !DosQueryProcAddr ( hTheseus, 0, "T2GetHandle0", &pfn ) && !DosQueryProcAddr ( hTheseus, 0, "T2ReleaseHandle0", &pfn ) ) {
            ;
         } else {
            Parms->UseTheseus = FALSE ;
            _UseTheseus.Disable ( ) ;
         } /* endif */
         DosFreeModule ( hTheseus ) ;
      } else {
         Parms->UseTheseus = FALSE ;
         _UseTheseus.Disable ( ) ;
      } /* endif */
      _UseTheseus.Set ( Parms->UseTheseus ) ;
   #else
      _UseTheseus.Set ( FALSE ) ;
      _UseTheseus.Disable ( ) ;
   #endif
} /* endmethod */

DRESULT Configure_Options1_Dialog::Control ( MPARAM1 mp1, MPARAM2 ) {
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;
   switch ( Id ) {
      case IDD_CONFIG_HIDECONTROLS: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->HideControls = _HideControls.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_FLOAT: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->Float = _Float.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_ANIMATE: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->Animate = _Animate.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_DIALOGBORDER: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->DialogBorder = _DialogBorder.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_TABLE: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->TableFormat = _TableFormat.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_CHIME: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->Chime = _Chime.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_SHOWDRIVES: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowDiskDrives = _ShowDiskDrives.Query ( ) ;
            _ShowRemoteDrives.Enable ( Parms->ShowDiskDrives ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_SHOWREMOTES: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowRemoteDrives = _ShowRemoteDrives.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_FSNAME: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowFileSystemNames = _ShowFileSystemNames.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_DLABEL: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowDiskLabels = _ShowDiskLabels.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_SECONDS: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowSeconds = _ShowSeconds.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_THESEUS: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->UseTheseus = _UseTheseus.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_CENTER_CLOCK: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->CenterClock = _CenterClock.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

DRESULT Configure_Options1_Dialog::Command ( MPARAM1 mp1, MPARAM2 mp2 ) {
   #ifdef __NT__
      if ( SHORT2FROMMP(mp1) > 1 )
         return ( Control ( mp1, mp2 ) ) ;
   #else
      mp2 = 0 ; // Pacify the compiler.
   #endif
   return ( PageDialog::Command ( mp1, mp2 ) ) ;
} /* endmethod */

DRESULT Configure_Options1_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Configure_Options1_Dialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   DRESULT Result ;

   switch ( msg ) {
      #ifdef __OS2__
      case WM_CONTROL:      Result = Control ( mp1, mp2 ) ; break ;
      #endif // __OS2__
      case WM_COMMAND:      Result = Command ( mp1, mp2 ) ; break ;
      default:              Result = PageDialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Configure_Options1_Dialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *  Configuration Options Page 2 message processor                          *
 *                                                                          *
 ****************************************************************************/

class Configure_Options2_Dialog : public PageDialog {

   private:
      PCONFIG_PARMS Parms ;
      Checkbox _ShowKNever, _ShowKAlways, _ShowKAbove512K, _ShowTrueK, _ShowM, _ShowG ;
      SpinButton _Priority, _Timer, _Average ;

   public:
      Configure_Options2_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) ;
      virtual ~Configure_Options2_Dialog ( ) { }

      virtual DRESULT Control ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

Configure_Options2_Dialog::Configure_Options2_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) :
   PageDialog ( Parent, Owner, Resource, Id ), Parms(parms),
   _ShowKNever(CHILD(Handle,IDD_CONFIG_SHOWK_NEVER)),
   _ShowKAlways(CHILD(Handle,IDD_CONFIG_SHOWK_ALWAYS)),
   _ShowKAbove512K(CHILD(Handle,IDD_CONFIG_SHOWK_ABOVE512)),
   _ShowTrueK(CHILD(Handle,IDD_CONFIG_SHOW_TRUEK)),
   _ShowM(CHILD(Handle,IDD_CONFIG_SHOW_MEGABYTES)),
   _ShowG(CHILD(Handle,IDD_CONFIG_SHOW_GIGABYTES)),
   _Priority(CHILD(Handle,IDD_CONFIG_PRIORITY)),
   _Timer(CHILD(Handle,IDD_CONFIG_TIMER)),
   _Average(CHILD(Handle,IDD_CONFIG_AVERAGE)) {
   HelpWindow TheHelpWindow ( Handle, Parms->MainWindow ) ;
   TheHelpWindow.Associate ( Handle ) ;
   switch ( Parms->ShowK ) {
      case SHOWK_NEVER:
         _ShowKNever.Set ( ) ;
         break;
      case SHOWK_ALWAYS:
         _ShowKAlways.Set ( ) ;
         break;
      case SHOWK_ABOVE512:
      default:
         _ShowKAbove512K.Set ( ) ;
   } /* endswitch */
   _ShowTrueK.Set ( Parms->ShowTrueK ) ;
   _ShowM.Set ( Parms->ShowM ) ;
   _ShowG.Set ( Parms->ShowG ) ;
   _Priority.SetLimits ( PRIORITY_ABOVE_NORMAL, PRIORITY_TIME_CRITICAL ) ;
   _Priority.Set ( Parms->MonitorPriority ) ;
   _Timer.SetLimits ( 10, 300 ) ;
   _Timer.Set ( Parms->TimerInterval / 100 ) ;
   _Average.SetLimits ( 10, MAX_AVERAGE_COUNT ) ;
   _Average.Set ( Parms->AveragingInterval ) ;
} /* endmethod */

DRESULT Configure_Options2_Dialog::Control ( MPARAM1 mp1, MPARAM2 ) {
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;
   switch ( Id ) {
      case IDD_CONFIG_SHOWK_NEVER: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowK = SHOWK_NEVER ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_SHOWK_ABOVE512: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowK = SHOWK_ABOVE512 ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_SHOWK_ALWAYS: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowK = SHOWK_ALWAYS ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_SHOW_TRUEK: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowTrueK = _ShowTrueK.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_SHOW_MEGABYTES: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowM = _ShowM.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_SHOW_GIGABYTES: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->ShowG = _ShowG.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_PRIORITY: {
         #ifdef __OS2__
         if ( Message == SPBN_ENDSPIN ) {
            Parms->MonitorPriority = Priority ( _Priority.Query ( ) ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         #endif // __OS2__
         break; }
      case IDD_CONFIG_TIMER: {
         #ifdef __OS2__
         if ( Message == SPBN_ENDSPIN ) {
            Parms->TimerInterval = ULONG ( _Timer.Query ( ) * 100 ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         #endif // __OS2__
         break; }
      case IDD_CONFIG_AVERAGE: {
         #ifdef __OS2__
         if ( Message == SPBN_ENDSPIN ) {
            Parms->AveragingInterval = ULONG ( _Average.Query ( ) ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         #endif // __OS2__
         break; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

DRESULT Configure_Options2_Dialog::Command ( MPARAM1 mp1, MPARAM2 mp2 ) {
   #ifdef __NT__
      if ( SHORT2FROMMP(mp1) > 1 )
         return ( Control ( mp1, mp2 ) ) ;
   #else
      mp2 = 0 ; // Pacify the compiler.
   #endif
   return ( PageDialog::Command ( mp1, mp2 ) ) ;
} /* endmethod */

DRESULT Configure_Options2_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Configure_Options2_Dialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   DRESULT Result ;

   switch ( msg ) {
      #ifdef __OS2__
      case WM_CONTROL:      Result = Control ( mp1, mp2 ) ; break ;
      #endif // __OS2__
      case WM_COMMAND:      Result = Command ( mp1, mp2 ) ; break ;
      default:              Result = PageDialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Configure_Options2_Dialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *  Configuration Colors Page message processor                             *
 *                                                                          *
 ****************************************************************************/

class Configure_Colors_Dialog : public PageDialog {

   private:
      PCONFIG_PARMS Parms ;
      ColorSample *ColorSamples[3] ;
      FontSample *FontSampleBox ;

   public:
      Configure_Colors_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) ;
      virtual ~Configure_Colors_Dialog ( ) ;

      virtual DRESULT Control ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

Configure_Colors_Dialog::Configure_Colors_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) :
   PageDialog ( Parent, Owner, Resource, Id ), Parms(parms), FontSampleBox(0) {
   HelpWindow TheHelpWindow ( Handle, Parms->MainWindow ) ;
   TheHelpWindow.Associate ( Handle ) ;
   memset ( ColorSamples, 0, sizeof(ColorSamples) ) ;
   ColorSamples[0] = new ColorSample ( CHILD(Handle,IDD_CONFIG_NORMAL), Parms->NormalForeground, Parms->NormalBackground ) ;
   ColorSamples[1] = new ColorSample ( CHILD(Handle,IDD_CONFIG_WARNING), Parms->WarningForeground, Parms->WarningBackground ) ;
   ColorSamples[2] = new ColorSample ( CHILD(Handle,IDD_CONFIG_ERROR), Parms->ErrorForeground, Parms->ErrorBackground ) ;
   FontSampleBox = new FontSample ( CHILD(Handle,IDD_CONFIG_FONT), Parms->Font ) ;
} /* endmethod */

Configure_Colors_Dialog::~Configure_Colors_Dialog ( ) {
   delete ColorSamples [0] ;
   delete ColorSamples [1] ;
   delete ColorSamples [2] ;
   delete FontSampleBox ;
} /* endif */

DRESULT Configure_Colors_Dialog::Control ( MPARAM1 mp1, MPARAM2 ) {
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;
   switch ( Id ) {
      case IDD_CONFIG_NORMAL: {
         if ( ColorSamples[0] ) 
            ColorSamples[0]->QueryColors ( Parms->NormalForeground, Parms->NormalBackground ) ;
         Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         break; }
      case IDD_CONFIG_WARNING: {
         if ( ColorSamples[1] ) 
            ColorSamples[1]->QueryColors ( Parms->WarningForeground, Parms->WarningBackground ) ;
         Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         break; }
      case IDD_CONFIG_ERROR: {
         if ( ColorSamples[2] ) 
            ColorSamples[2]->QueryColors ( Parms->ErrorForeground, Parms->ErrorBackground ) ;
         Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         break; }
      case IDD_CONFIG_FONT: {
         if ( FontSampleBox ) 
            FontSampleBox->QueryFont ( Parms->Font, sizeof(Parms->Font) ) ;
         Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         break; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

DRESULT Configure_Colors_Dialog::Command ( MPARAM1 mp1, MPARAM2 mp2 ) {
   #ifdef __NT__
      if ( SHORT2FROMMP(mp1) > 1 )
         return ( Control ( mp1, mp2 ) ) ;
   #else
      mp2 = 0 ; // Pacify the compiler.
   #endif
   return ( PageDialog::Command ( mp1, mp2 ) ) ;
} /* endmethod */

DRESULT Configure_Colors_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Configure_Colors_Dialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   DRESULT Result ;

   switch ( msg ) {
      #ifdef __OS2__
      case WM_CONTROL:      Result = Control ( mp1, mp2 ) ; break ;
      #endif // __OS2__
      case WM_COMMAND:      Result = Command ( mp1, mp2 ) ; break ;
      default:              Result = PageDialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Configure_Colors_Dialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *  Configuration Anchor Page message processor                             *
 *                                                                          *
 ****************************************************************************/

class Configure_Anchor_Dialog : public PageDialog {

   private:
      PCONFIG_PARMS Parms ;
      Checkbox _CornerBL, _CornerBR, _CornerTL, _CornerTR ;

   public:
      Configure_Anchor_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) ;

      virtual DRESULT Control ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

Configure_Anchor_Dialog::Configure_Anchor_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) :
   PageDialog ( Parent, Owner, Resource, Id ), Parms(parms),
   _CornerBL(CHILD(Handle,IDD_CONFIG_BL)), _CornerBR(CHILD(Handle,IDD_CONFIG_BR)),
   _CornerTL(CHILD(Handle,IDD_CONFIG_TL)), _CornerTR(CHILD(Handle,IDD_CONFIG_TR)) {
   HelpWindow TheHelpWindow ( Handle, Parms->MainWindow ) ;
   TheHelpWindow.Associate ( Handle ) ;
   switch ( Parms->AnchorCorner ) {
      case CORNER_BL:
      default:
         _CornerBL.Set ( ) ;
         break;
      case CORNER_BR:
         _CornerBR.Set ( ) ;
         break;
      case CORNER_TL:
         _CornerTL.Set ( ) ;
         break;
      case CORNER_TR:
         _CornerTR.Set ( ) ;
         break;
   } /* endswitch */
} /* endmethod */

DRESULT Configure_Anchor_Dialog::Control ( MPARAM1 mp1, MPARAM2 ) {
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;
   switch ( Id ) {
      case IDD_CONFIG_BL: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->AnchorCorner = CORNER_BL ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break ; } /* endcase */
      case IDD_CONFIG_BR: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->AnchorCorner = CORNER_BR ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break ; } /* endcase */
      case IDD_CONFIG_TL: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->AnchorCorner = CORNER_TL ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break ; } /* endcase */
      case IDD_CONFIG_TR: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->AnchorCorner = CORNER_TR ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         break ; } /* endcase */
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

DRESULT Configure_Anchor_Dialog::Command ( MPARAM1 mp1, MPARAM2 mp2 ) {
   #ifdef __NT__
      if ( SHORT2FROMMP(mp1) > 1 )
         return ( Control ( mp1, mp2 ) ) ;
   #else
      mp2 = 0 ; // Pacify the compiler.
   #endif
   return ( PageDialog::Command ( mp1, mp2 ) ) ;
} /* endmethod */

DRESULT Configure_Anchor_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Configure_Anchor_Dialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   DRESULT Result ;

   switch ( msg ) {
      #ifdef __OS2__
      case WM_CONTROL:      Result = Control ( mp1, mp2 ) ; break ;
      #endif // __OS2__
      case WM_COMMAND:      Result = Command ( mp1, mp2 ) ; break ;
      default:              Result = PageDialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Configure_Anchor_Dialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *  Configuration Date Format Page message processor                        *
 *                                                                          *
 ****************************************************************************/

class Configure_DateFmt_Dialog : public PageDialog {

   private:
      PCONFIG_PARMS Parms ;
      Entryfield _Format ;
      StaticText _Sample ;
      ListBox _DateCodes ;

   public:
      Configure_DateFmt_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) ;

      virtual DRESULT Control ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

Configure_DateFmt_Dialog::Configure_DateFmt_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) :
   PageDialog ( Parent, Owner, Resource, Id ), Parms(parms),
   _Format(CHILD(Handle,IDD_CONFIG_DATEFMT)), _Sample(CHILD(Handle,IDD_CONFIG_DATESMP)),
   _DateCodes(CHILD(Handle,IDD_CONFIG_DATECODES)) {
   HelpWindow TheHelpWindow ( Handle, Parms->MainWindow ) ;
   TheHelpWindow.Associate ( Handle ) ;
   _Format.SetLimit ( 80 ) ;
   _DateCodes.Clear ( ) ;
   for ( int i=0; i<IDS_DATECODES_COUNT; i++ ) {
      ResourceString Text ( LibraryHandle, IDS_DATECODES_00+i ) ;
      _DateCodes.Insert ( PSZ(Text) ) ;
   } /* endfor */
   _Format.SetText ( Parms->DateFormat ) ;
} /* endmethod */

DRESULT Configure_DateFmt_Dialog::Control ( MPARAM1 mp1, MPARAM2 ) {
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;
   switch ( Id ) {
      case IDD_CONFIG_DATEFMT: {
         switch ( Message ) {
            case EN_CHANGE: {
               char Text [80], Result [80] ;
               _Format.GetText ( Text, sizeof(Text) ) ;
               FormatDate ( Text, Result, sizeof(Result) ) ;
               _Sample.SetText ( Result ) ;
               strncpy ( Parms->DateFormat, Text, sizeof(Parms->DateFormat) ) ;
               Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
               break; }
         } /* endswitch */
         break; }
      case IDD_CONFIG_DATECODES: {
         switch ( Message ) {
            #ifdef __OS2__
            case LN_ENTER: {
               int Selection = _DateCodes.QuerySelection() ;
               if ( ( Selection >= 0 ) && ( Selection < IDS_DATECODES_COUNT ) ) {
                  char Text [80], Result [80] ;
                  _DateCodes.QueryItemText ( Selection, Text, sizeof(Text) ) ;
                  char *p = strchr ( Text, ' ' ) ; if ( p ) *p = 0 ;
                  _Format.Insert ( Text ) ;
                  _Format.GetText ( Text, sizeof(Text) ) ;
                  FormatDate ( Text, Result, sizeof(Result) ) ;
                  _Sample.SetText ( Result ) ;
                  strcpy ( Parms->DateFormat, Text ) ;
                  Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
               } /* endif */
               break; }
            #endif // __OS2__
            default:
               break;
         } /* endswitch */
         break; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

DRESULT Configure_DateFmt_Dialog::Command ( MPARAM1 mp1, MPARAM2 mp2 ) {
   #ifdef __NT__
      if ( SHORT2FROMMP(mp1) > 1 )
         return ( Control ( mp1, mp2 ) ) ;
   #else
      mp2 = 0 ; // Pacify the compiler.
   #endif
   return ( PageDialog::Command ( mp1, mp2 ) ) ;
} /* endmethod */

DRESULT Configure_DateFmt_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Configure_DateFmt_Dialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   DRESULT Result ;

   switch ( msg ) {
      #ifdef __OS2__
      case WM_CONTROL:      Result = Control ( mp1, mp2 ) ; break ;
      #endif // __OS2__
      case WM_COMMAND:      Result = Command ( mp1, mp2 ) ; break ;
      default:              Result = PageDialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Configure_DateFmt_Dialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *  Configuration POP3 Page message processor (Page 1)                      *
 *                                                                          *
 ****************************************************************************/

class Configure_Pop31_Dialog : public PageDialog {

   private:
      PCONFIG_PARMS Parms ;
      Entryfield _ServerName ;
      Entryfield _UserName ;
      Entryfield _Password ;
      Entryfield _PortNumber ;
      Checkbox _ForceIP ;
      Checkbox _UseAPOP ;
      Window _PopTest ;
      StaticText _PopStatus ;

   public:
      Configure_Pop31_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) ;

      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

Configure_Pop31_Dialog::Configure_Pop31_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) :
   PageDialog ( Parent, Owner, Resource, Id ), Parms(parms),
   _ServerName(CHILD(Handle,IDD_CONFIG_POPSERV)), 
   _UserName(CHILD(Handle,IDD_CONFIG_POPUSER)), 
   _Password(CHILD(Handle,IDD_CONFIG_POPPSWD)), 
   _PortNumber(CHILD(Handle,IDD_CONFIG_POPPORT)), 
   _ForceIP(CHILD(Handle,IDD_CONFIG_POPFORC)), 
   _UseAPOP(CHILD(Handle,IDD_CONFIG_POPAPOP)), 
   _PopTest(CHILD(Handle,IDD_CONFIG_POPTEST)), 
   _PopStatus(CHILD(Handle,IDD_CONFIG_POPSTAT)) {
   HelpWindow TheHelpWindow ( Handle, Parms->MainWindow ) ;
   TheHelpWindow.Associate ( Handle ) ;
   _ServerName.SetText ( Parms->PopServer ) ;
   _UserName.SetText ( Parms->PopUserName ) ;
   _Password.SetText ( Parms->PopPassword ) ;
   char Buffer [10] ; sprintf ( Buffer, "%i", Parms->PopPort ) ; _PortNumber.SetText ( Buffer ) ;
   _ForceIP.Set ( Parms->PopForceIP ) ;
   _UseAPOP.Set ( Parms->PopUseAPOP ) ;
   _PopStatus.Set_TextColor ( RGB_RED ) ;
   _PopStatus.SetText ( "" ) ;
} /* endmethod */

DRESULT Configure_Pop31_Dialog::Command ( MPARAM1 mp1, MPARAM2 mp2 ) {
   #ifdef __OS2__
      switch ( SHORT1FROMMP(mp1) ) {
         case IDD_CONFIG_POPACPT:
         case IDD_CONFIG_POPTEST: {
            char Server [80] ; _ServerName.GetText ( Server, sizeof(Server) ) ;
            if ( !Server[0] ) {
               _PopStatus.Set_TextColor ( RGB_RED ) ;
               ResourceString Format ( LibraryHandle, IDS_POP_NOSERVER ) ;
               _PopStatus.SetText ( (char*)Format ) ;
               _ServerName.SetFocus ( ) ;
               break ;
            } /* endif */
            char UserName [80] ; _UserName.GetText ( UserName, sizeof(UserName) ) ;
            if ( !UserName[0] ) {
               _PopStatus.Set_TextColor ( RGB_RED ) ;
               ResourceString Format ( LibraryHandle, IDS_POP_NOUSERNAME ) ;
               _PopStatus.SetText ( (char*)Format ) ;
               _UserName.SetFocus ( ) ;
               break ;
            } /* endif */
            char Password [80] ; _Password.GetText ( Password, sizeof(Password) ) ;
            if ( !Password[0] ) {
               _PopStatus.Set_TextColor ( RGB_RED ) ;
               ResourceString Format ( LibraryHandle, IDS_POP_NOPASSWORD ) ;
               _PopStatus.SetText ( (char*)Format ) ;
               _Password.SetFocus ( ) ;
               break ;
            } /* endif */
            char PortNumber [80] ; _PortNumber.GetText ( PortNumber, sizeof(PortNumber) ) ; 
            int Port = atoi(PortNumber) ;
            int UseAPOP = _UseAPOP.Query ( ) ;
            int ForceIP = _ForceIP.Query ( ) ;
            int MessageCount = GetMsgCnt ( Port, Server, UserName, Password, UseAPOP, ForceIP ) ;
            if ( MessageCount > 1 ) {
               ResourceString Format ( LibraryHandle, IDS_POP_HASMESSAGES ) ;
               char Text [80] ;  sprintf ( Text, (char*)Format, MessageCount ) ;
               _PopStatus.Set_TextColor ( WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOWSTATICTEXT, 0 ) ) ;
               _PopStatus.SetText ( Text ) ;
            } else if ( MessageCount == 1 ) {
               _PopStatus.Set_TextColor ( WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOWSTATICTEXT, 0 ) ) ;
               ResourceString Format ( LibraryHandle, IDS_POP_HASONEMESSAGE ) ;
               _PopStatus.SetText ( (char*)Format ) ;
            } else if ( MessageCount == 0 ) {
               _PopStatus.Set_TextColor ( WinQuerySysColor ( HWND_DESKTOP, SYSCLR_WINDOWSTATICTEXT, 0 ) ) ;
               ResourceString Format ( LibraryHandle, IDS_POP_HASNOMESSAGES ) ;
               _PopStatus.SetText ( (char*)Format ) ;
            } else {
               int ID = IDS_POP_ERROR1 - ( MessageCount + 1 ) ;
               ID = min ( ID, IDS_POP_ERRORX ) ;
               ResourceString Message ( LibraryHandle, ID ) ;
               _PopStatus.Set_TextColor ( RGB_RED ) ;
               _PopStatus.SetText ( (char*)Message ) ;
               if ( SHORT1FROMMP(mp1) == IDD_CONFIG_POPACPT ) {
                  ResourceString Title ( LibraryHandle, IDS_POP_TITLE ) ;
                  ResourceString Prompt ( LibraryHandle, IDS_POP_OVERRIDE ) ;
                  ULONG Button = Sys_MessageBox ( HWND_DESKTOP, Parms->MainWindow, Prompt, Title, 0, MB_YESNO | MB_WARNING ) ;
                  if ( Button == MBID_YES ) 
                     MessageCount = 0 ;
               } /* endif */
            } /* endif */
            if ( ( MessageCount >= 0 ) && ( SHORT1FROMMP(mp1) == IDD_CONFIG_POPACPT ) ) {
               char Text [80] ; 
               _ServerName.GetText ( Text, sizeof(Text) ) ; strncpy ( Parms->PopServer, Text, sizeof(Parms->PopServer) ) ;
               _UserName.GetText ( Text, sizeof(Text) ) ; strncpy ( Parms->PopUserName, Text, sizeof(Parms->PopUserName) ) ;
               _Password.GetText ( Text, sizeof(Text) ) ; strncpy ( Parms->PopPassword, Text, sizeof(Parms->PopPassword) ) ;
               _PortNumber.GetText ( Text, sizeof(Text) ) ; Parms->PopPort = atoi ( Text ) ;
               Parms->PopForceIP = _ForceIP.Query ( ) ;
               Parms->PopUseAPOP = _UseAPOP.Query ( ) ;
               Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
            } /* endif */
            break; } 
         case IDD_CONFIG_POPREST: {
            _ServerName.SetText ( Parms->PopServer ) ;
            _UserName.SetText ( Parms->PopUserName ) ;
            _Password.SetText ( Parms->PopPassword ) ;
            char Buffer [10] ; sprintf ( Buffer, "%i", Parms->PopPort ) ; _PortNumber.SetText ( Buffer ) ;
            _ForceIP.Set ( Parms->PopForceIP ) ;
            _UseAPOP.Set ( Parms->PopUseAPOP ) ;
            _PopStatus.SetText ( "" ) ;
            break; } 
      } /* endswitch */
   #endif
   return ( PageDialog::Command ( mp1, mp2 ) ) ;
} /* endmethod */

DRESULT Configure_Pop31_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Configure_Pop31_Dialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   DRESULT Result ;

   switch ( msg ) {
      case WM_COMMAND:      Result = Command ( mp1, mp2 ) ; break ;
      default:              Result = PageDialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Configure_Pop31_Dialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *  Configuration POP3 Page message processor (Page 2)                      *
 *                                                                          *
 ****************************************************************************/

class Configure_Pop32_Dialog : public PageDialog {

   private:
      PCONFIG_PARMS Parms ;
      SpinButton _Interval ;
      Entryfield _Reader ;
      Checkbox _Alert ;
      StaticText _WavText ;
      Window _WavPlay ;
      Entryfield _WavFile ;

   public:
      Configure_Pop32_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) ;

      virtual DRESULT Control ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

Configure_Pop32_Dialog::Configure_Pop32_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, PCONFIG_PARMS parms ) :
   PageDialog ( Parent, Owner, Resource, Id ), Parms(parms),
   _Interval(CHILD(Handle,IDD_CONFIG_POPPOLL)), 
   _Reader(CHILD(Handle,IDD_CONFIG_POPREADER)),
   _Alert(CHILD(Handle,IDD_CONFIG_POPALERT)),
   _WavText(CHILD(Handle,IDD_CONFIG_POPWAVTEXT)),
   _WavPlay(CHILD(Handle,IDD_CONFIG_POPWAVPLAY)),
   _WavFile(CHILD(Handle,IDD_CONFIG_POPWAVFILE)) {
   HelpWindow TheHelpWindow ( Handle, Parms->MainWindow ) ;
   TheHelpWindow.Associate ( Handle ) ;
   _Interval.SetLimits ( 10, 600 ) ;
   _Interval.Set ( Parms->PopInterval ) ;
   _Reader.SetText ( Parms->PopReader ) ;
   _Reader.SetLimit ( 250 ) ;
   _Alert.Set ( Parms->PopAlert ) ;
   _WavText.Enable ( Parms->PopAlert ) ;
   _WavFile.SetText ( Parms->PopWavFile ) ;
   _WavFile.SetLimit ( 250 ) ;
   _WavPlay.Enable ( Parms->PopAlert ) ;
   _WavFile.Enable ( Parms->PopAlert ) ;
} /* endmethod */

DRESULT Configure_Pop32_Dialog::Control ( MPARAM1 mp1, MPARAM2 ) {
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;
   switch ( Id ) {
      case IDD_CONFIG_POPPOLL: {
         #ifdef __OS2__
         if ( Message == SPBN_ENDSPIN ) {
            Parms->PopInterval = int ( _Interval.Query ( ) ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
         } /* endif */
         #endif // __OS2__
         break; }
      case IDD_CONFIG_POPREADER: {
         switch ( Message ) {
            case EN_CHANGE: {
               char Text [260] ; 
               _Reader.GetText ( Text, sizeof(Text) ) ;
               strncpy ( Parms->PopReader, Text, sizeof(Parms->PopReader) ) ;
               Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
               break; }
         } /* endswitch */
         break; }
      case IDD_CONFIG_POPALERT: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            Parms->PopAlert = _Alert.Query ( ) ;
            Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
            _WavText.Enable ( Parms->PopAlert ) ;
            _WavPlay.Enable ( Parms->PopAlert ) ;
            _WavFile.Enable ( Parms->PopAlert ) ;
         } /* endif */
         break; }
      case IDD_CONFIG_POPWAVPLAY: {
         if ( ( Message == BN_CLICKED ) || ( Message == BN_DBLCLICKED ) ) {
            HMODULE hModule ; 
            ULONG ( EXPENTRY *mciPlayFile ) ( HWND, PSZ, ULONG, PSZ, HWND ) ;
            if ( DosLoadModule ( 0, 0, "MCIAPI.DLL", &hModule ) || DosQueryProcAddr ( hModule, 0, "mciPlayFile", (PPFN)&mciPlayFile ) ) {
               DosBeep (  32, 100 ) ;
               DosBeep (  64, 100 ) ;
               DosBeep ( 128, 100 ) ;
               DosBeep ( 256, 100 ) ;
               DosBeep ( 512, 100 ) ;
            } else {
               char Text [80] ; 
               _WavFile.GetText ( Text, sizeof(Text) ) ;
               mciPlayFile ( 0, Text, 0, 0, 0 ) ;
               DosFreeModule ( hModule ) ;
            } /* endif */
         } /* endif */
         break; }
      case IDD_CONFIG_POPWAVFILE: {
         switch ( Message ) {
            case EN_CHANGE: {
               char Text [260] ; 
               _WavFile.GetText ( Text, sizeof(Text) ) ;
               strncpy ( Parms->PopWavFile, Text, sizeof(Parms->PopWavFile) ) ;
               Sys_SendMessage ( Parms->MainWindow, WM_UPDATEPARMS, MP1FROMP(Parms), 0 ) ;
               break; }
         } /* endswitch */
         break; }
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

DRESULT Configure_Pop32_Dialog::Command ( MPARAM1 mp1, MPARAM2 mp2 ) {
   #ifdef __NT__
      if ( SHORT2FROMMP(mp1) > 1 )
         return ( Control ( mp1, mp2 ) ) ;
   #else
      mp2 = 0 ; // Pacify the compiler.
   #endif
   switch ( SHORT1FROMMP(mp1) ) {
      case IDD_CONFIG_POPWAVPLAY: {
         HMODULE hModule ; 
         ULONG ( EXPENTRY *mciPlayFile ) ( HWND, PSZ, ULONG, PSZ, HWND ) ;
         if ( DosLoadModule ( 0, 0, "MCIAPI.DLL", &hModule ) || DosQueryProcAddr ( hModule, 0, "mciPlayFile", (PPFN)&mciPlayFile ) ) {
            DosBeep (  32, 100 ) ;
            DosBeep (  64, 100 ) ;
            DosBeep ( 128, 100 ) ;
            DosBeep ( 256, 100 ) ;
            DosBeep ( 512, 100 ) ;
         } else {
            char Text [80] ; 
            _WavFile.GetText ( Text, sizeof(Text) ) ;
            mciPlayFile ( 0, Text, 0, 0, 0 ) ;
            DosFreeModule ( hModule ) ;
         } /* endif */
         return ( MRFROMLONG ( TRUE ) ) ; }
      default:
         return ( PageDialog::Command ( mp1, mp2 ) ) ;
   } /* endswitch */
} /* endmethod */

DRESULT Configure_Pop32_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   if ( DebugMessages ) {
      LogMsgRcvd ( MessageIndent, "Configure_Pop31_Dialog::MessageProcessor", msg, mp1, mp2 ) ;
      MessageIndent += 2 ;
   } /* endif */

   DRESULT Result ;

   switch ( msg ) {
      #ifdef __OS2__
      case WM_CONTROL:      Result = Control ( mp1, mp2 ) ; break ;
      #endif // __OS2__
      case WM_COMMAND:      Result = Command ( mp1, mp2 ) ; break ;
      default:              Result = PageDialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break ;
   } /* endswitch */

   if ( DebugMessages ) {
      MessageIndent -= 2 ;
      LogMsgDone ( MessageIndent, "Configure_Pop31_Dialog::MessageProcessor", msg, Result ) ;
   } /* endif */

   return ( Result ) ;
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *      Class: Configuration Dialog                                         *
 *                                                                          *
 ****************************************************************************/

Configure_Dialog::Configure_Dialog ( HWND Parent, HWND Owner, HMODULE Resource, CONFIG_PARMS &parms ) :
   Dialog ( Parent, Owner, Resource, IDD_CONFIGURE ), Parms(&parms) {
   Load ( ) ;

   // Mark the dialog not ready so that Control messages are ignored.
   Parms->Ready = FALSE ;

   // Create the notebook pages.
   ResourceString   ItemsPageName ( Resource, IDS_CONFIG_PAGE_ITEMS ) ;
   ResourceString OptionsPageName ( Resource, IDS_CONFIG_PAGE_OPTIONS ) ;
   ResourceString  ColorsPageName ( Resource, IDS_CONFIG_PAGE_COLORS ) ;
   ResourceString  AnchorPageName ( Resource, IDS_CONFIG_PAGE_ANCHOR ) ;
   ResourceString DateFmtPageName ( Resource, IDS_CONFIG_PAGE_DATEFMT ) ;
   ResourceString     PopPageName ( Resource, IDS_CONFIG_PAGE_POP3    ) ;

#ifdef __OS2__
   HWND hwndNotebook = CHILD ( Handle, IDD_CONFIG_NOTEBOOK ) ;
   Sys_SendMessage ( hwndNotebook, BKM_SETNOTEBOOKCOLORS,
      MPFROMLONG(SYSCLR_FIELDBACKGROUND), MPFROMSHORT(BKA_BACKGROUNDPAGECOLORINDEX) ) ;

   struct {
      char *Name ;              PageDialog *pDialog ;         
   } Pages [] = {
      { PCHAR(  ItemsPageName), 0 },
      { PCHAR(OptionsPageName), 0 },
      { 0,                      0 },
      { PCHAR( ColorsPageName), 0 },
      { PCHAR( AnchorPageName), 0 },
      { PCHAR(DateFmtPageName), 0 },
      { PCHAR(    PopPageName), 0 },
      { 0,                      0 },
   } ;

   Pages[0].pDialog = new Configure_Items_Dialog    ( hwndNotebook, Handle, Resource, IDD_CONFIG_PAGE_ITEMS,    Parms ) ;
   Pages[1].pDialog = new Configure_Options1_Dialog ( hwndNotebook, Handle, Resource, IDD_CONFIG_PAGE_OPTIONS1, Parms ) ;
   Pages[2].pDialog = new Configure_Options2_Dialog ( hwndNotebook, Handle, Resource, IDD_CONFIG_PAGE_OPTIONS2, Parms ) ;
   Pages[3].pDialog = new Configure_Colors_Dialog   ( hwndNotebook, Handle, Resource, IDD_CONFIG_PAGE_COLORS,   Parms ) ;
   Pages[4].pDialog = new Configure_Anchor_Dialog   ( hwndNotebook, Handle, Resource, IDD_CONFIG_PAGE_ANCHOR,   Parms ) ;
   Pages[5].pDialog = new Configure_DateFmt_Dialog  ( hwndNotebook, Handle, Resource, IDD_CONFIG_PAGE_DATEFMT,  Parms ) ;
   Pages[6].pDialog = new Configure_Pop31_Dialog    ( hwndNotebook, Handle, Resource, IDD_CONFIG_PAGE_POP31,    Parms ) ;
   Pages[7].pDialog = new Configure_Pop32_Dialog    ( hwndNotebook, Handle, Resource, IDD_CONFIG_PAGE_POP32,    Parms ) ;

   Canvas canvas ( "Config::InitDlg::canvas", Handle ) ;
   char *Characters = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789" ;
   RECTANGLE Rectangle = canvas.MeasureText ( Characters ) ;
   long AvgWidth  = ( Rectangle.xRight - Rectangle.xLeft ) / strlen(Characters) ;
   long AvgHeight = Rectangle.yTop - Rectangle.yBottom ;
   long MajorTabWidth = 0 ;
   for ( int i=0; i<sizeof(Pages)/sizeof(Pages[0]); i++ ) {
      BOOL MajorTab ( ( i == 0 ) || Pages[i].Name ) ;
      int PageNumber(1), PageCount(1) ;  char *Name(0);
      for ( int j=i; j>=0; j-- )
         if ( Pages[j].Name )
            break ;
      Name = Pages[j].Name ;
      for ( j=j+1; j<sizeof(Pages)/sizeof(Pages[0]) && !Pages[j].Name; j++ ) {
         PageCount ++ ;
         if ( j <= i )
            PageNumber ++ ;
      } /* endfor */
      ULONG PageHandle = ULONG ( Sys_SendMessage ( hwndNotebook, BKM_INSERTPAGE,
         MPFROMLONG(0), MPFROM2SHORT( (MajorTab?BKA_MAJOR:0) | BKA_STATUSTEXTON, BKA_LAST ) ) ) ;
      char StatusText [80] ;
      if ( PageCount == 1 ) {
         strcpy ( StatusText, Pages[i].Name ) ;
      } else {
         ResourceString Page ( Resource, IDS_PAGE ) ;
         ResourceString Of ( Resource, IDS_OF ) ;
         sprintf ( StatusText, "%s, %s %i %s %i", Name, PCHAR(Page), PageNumber, PCHAR(Of), PageCount ) ;
      } /* endif */
      Sys_SendMessage ( hwndNotebook, BKM_SETSTATUSLINETEXT, MPFROMLONG(PageHandle), MP2FROMP(StatusText) ) ;
      if ( MajorTab ) {
         Sys_SendMessage ( hwndNotebook, BKM_SETTABTEXT, MPFROMLONG(PageHandle), MP2FROMP(Pages[i].Name) ) ;
         POINTL TextBox [TXTBOX_COUNT] ;
         canvas.QueryTextBox ( Pages[i].Name, TextBox ) ;
         MajorTabWidth = max ( MajorTabWidth, TextBox[TXTBOX_CONCAT].x ) ;
      } /* endif */
      Pages[i].pDialog->BindToPage ( hwndNotebook, PageHandle ) ;
      if ( i == 0 ) {
         HWND Dialog = Pages[i].pDialog->QueryHandle() ;
         Sys_SendMessage ( WinQueryHelpInstance(Handle), HM_SET_ACTIVE_WINDOW, MPFROMHWND(Dialog), MPFROMHWND(Dialog) ) ;
      } /* endif */
   } /* endfor */
   Sys_SendMessage ( hwndNotebook, BKM_SETDIMENSIONS,
      MPFROM2SHORT( SHORT(MajorTabWidth+AvgWidth*2), SHORT((AvgHeight*3)/2) ),
      MPFROMSHORT( BKA_MAJORTAB ) ) ;

#endif // __OS2__

   // Initialize state.
   Parms->Ready = TRUE ;
   Parms->MostRecentSelection = -1 ;

} /* endmethod */

DRESULT Configure_Dialog::UpdateDrives ( MPARAM1 mp1, MPARAM2 mp2 ) {

   // Update the item list.
   HWND Notebook = CHILD ( Handle, IDD_CONFIG_NOTEBOOK ) ;
   #ifdef __OS2__
      ULONG ItemsID = LONGFROMMR ( Sys_SendMessage ( Notebook, BKM_QUERYPAGEID, 0, MPFROM2SHORT(BKA_FIRST,BKA_MAJOR) ) ) ;
      HWND ItemsPage = HWND ( Sys_SendMessage ( Notebook, BKM_QUERYPAGEWINDOWHWND, MPFROMLONG(ItemsID), 0 ) ) ;
      Sys_SendMessage ( ItemsPage, WM_UPDATEDRIVES, mp1, mp2 ) ;
   #endif // __OS2__

   // Return success.
   mp1 = 0 ; mp2 = 0 ; // Pacify the compiler.
   return ( 0 ) ;
} /* endmethod */

#ifdef __OS2__

DRESULT Configure_Dialog::Control ( MPARAM1 mp1, MPARAM2 mp2 ) {

   // Decode the message.  Find out what control sent it, and what the
   //   control had to say.
   SHORT Id = SHORT1FROMMP ( mp1 ) ;
   SHORT Message = SHORT2FROMMP ( mp1 ) ;

   // Process notification according to which control sent it.
   switch ( Id ) {

      case IDD_CONFIG_NOTEBOOK: {
         switch ( Message ) {
            case BKN_PAGESELECTED: {
               PAGESELECTNOTIFY *pInfo = (PAGESELECTNOTIFY*) PVOIDFROMMP(mp2) ;
               HWND Dialog = HWND ( Sys_SendMessage ( pInfo->hwndBook, BKM_QUERYPAGEWINDOWHWND, MPFROMLONG(pInfo->ulPageIdNew), 0 ) ) ;
               Sys_SendMessage ( WinQueryHelpInstance(Handle), HM_SET_ACTIVE_WINDOW, MPFROMHWND(Dialog), MPFROMHWND(Dialog) ) ;
               break ; } /* endcase */
         } /* endswitch */
         break ; } /* endcase */

   } /* endswitch */

   // Return no error.
   return ( MRFROMSHORT ( FALSE ) ) ;

} /* endmethod */

DRESULT Configure_Dialog::FormatFrame ( MPARAM1 mp1, MPARAM2 mp2 ) {

   // First let the default dialog processor do its stuff.
   DRESULT Result = Dialog::MessageProcessor ( Handle, WM_FORMATFRAME, mp1, mp2 ) ;

   // Set the new notebook size, preserving the space for the controls below.
   SWP DialogPosition ;
   WinQueryWindowPos ( Handle, &DialogPosition ) ;

   SWP NotebookPosition ;
   WinQueryWindowPos ( CHILD(Handle,IDD_CONFIG_NOTEBOOK), &NotebookPosition ) ;

   LONG cyBorder = WinQuerySysValue ( HWND_DESKTOP, SV_CYSIZEBORDER ) ;
   LONG cyTitle = WinQuerySysValue ( HWND_DESKTOP, SV_CYTITLEBAR ) ;

   NotebookPosition.cx = DialogPosition.cx - NotebookPosition.x*2 ;
   NotebookPosition.cy = DialogPosition.cy - NotebookPosition.y - cyBorder*2 - cyTitle ;

   WinSetWindowPos ( CHILD(Handle,IDD_CONFIG_NOTEBOOK),
      0, 0, 0, NotebookPosition.cx, NotebookPosition.cy, SWP_SIZE ) ;

   // Return whatever the default said.
   return ( Result ) ;

} /* endmethod */

#endif // __OS2__

DRESULT Configure_Dialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {

   #ifdef DEBUG
      static int Indent = 0 ;
      LogMsgRcvd ( Indent, "Configure_Dialog", msg, mp1, mp2 ) ;
      Indent += 2 ;
   #endif

   DRESULT Result ;
   switch ( msg ) {
      case WM_UPDATEDRIVES: Result = UpdateDrives ( mp1, mp2 ) ; break ;
      #ifdef __OS2__
      case WM_FORMATFRAME:  Result = FormatFrame ( mp1, mp2 ) ; break ;
      case WM_CONTROL:      Result = Control ( mp1, mp2 ) ; break ;
      #endif // __OS2__
      default:              Result = Dialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ; break;
   } /* endswitch */

   #ifdef DEBUG
      Indent -= 2 ;
      LogMsgDone ( Indent, "Configure_Dialog", msg, Result ) ;
   #endif

   return ( Result ) ;

} /* endmethod */

