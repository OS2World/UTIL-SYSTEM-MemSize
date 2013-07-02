/******************************************************************* DIALOG.H
 *                                                                          *
 *                       Basic Dialog Definitions                           *
 *                                                                          *
 ****************************************************************************/

#ifndef DIALOG_H
#define DIALOG_H

#include "System.h"

class Dialog {

   protected:
      HWND Parent ;
      HWND Owner ;
      HMODULE Resource ;
      int Id ;
      int Result ;
      HWND Handle ;
      BOOL DebugMessages ;
      int MessageIndent ;

   public:

      Dialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id ) ;
      virtual ~Dialog ( ) ;

      HWND QueryHandle() { return ( Handle ) ; }
      HMODULE QueryResource ( ) { return ( Resource ) ; }

      int Load ( ) ;
      int Process ( HACCEL Accelerator = 0 ) ;
      void Dismiss ( int Status ) ;

      virtual DRESULT InitDialog ( HWND hwnd ) ;
      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;

} ;

class PageDialog : public Dialog {

   protected:
      HWND Notebook ;
      ULONG PageHandle ;

   public:

      PageDialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id ) ;
      virtual ~PageDialog ( ) ;

      void BindToPage ( HWND notebook, ULONG pagehandle ) ;
      void TurnTo ( ) ;

      virtual DRESULT InitDialog ( HWND hwnd ) ;
      virtual DRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      virtual DRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;

} ;

#endif 
