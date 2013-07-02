/*************************************************************** RECTANGL.CPP
 *                                                                          *
 *                        Portable Rectangle Object                         *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include "Rectangl.h"

/****************************************************************************
 *                                                                          *
 *  RECTANGLE: Constructor from window rectangle.                           *
 *                                                                          *
 ****************************************************************************/

RECTANGLE::RECTANGLE ( HWND hwnd ) {
   #ifdef __OS2__
      RECTL WindowRect ; WinQueryWindowRect ( hwnd, &WindowRect ) ;
      xLeft = WindowRect.xLeft ;
      xRight = WindowRect.xRight ;
      yBottom = WindowRect.yBottom ;
      yTop = WindowRect.yTop ;
   #else
      RECT WindowRect ;  GetWindowRect ( hwnd, &WindowRect ) ;
      xLeft = 0 ;
      xRight = WindowRect.right - WindowRect.left ;
      yBottom = 0 ;
      yTop = WindowRect.bottom - WindowRect.top ;
   #endif
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  RECTANGLE: Conversion from native formats.                              *
 *                                                                          *
 ****************************************************************************/

#ifdef __OS2__

   RECTANGLE::RECTANGLE ( RECTL &Rect ) : xLeft(Rect.xLeft), yBottom(Rect.yBottom), xRight(Rect.xRight), yTop(Rect.yTop) {
   } /* endmethod */

#else

   RECTANGLE::RECTANGLE ( HWND hwnd, RECT &Rect ) : xLeft(Rect.left), xRight(Rect.right) {
      RECT WindowRect ;  GetWindowRect ( hwnd, &WindowRect ) ;
      yBottom = WindowRect.bottom - Rect.bottom ;
      yTop = WindowRect.bottom - Rect.top ;
   } /* endmethod */

#endif 

/****************************************************************************
 *                                                                          *
 *  RECTANGLE: Conversion to native formats.                                *
 *                                                                          *
 ****************************************************************************/

#ifdef __OS2__

   RECTL RECTANGLE::Get ( ) {
      RECTL Rect = { xLeft, yBottom, xRight, yTop } ;
      return ( Rect ) ;
   } /* endmethod */

#else

   RECT RECTANGLE::Get ( HWND hwnd ) {
      RECT Rect = { xLeft, 0, xRight, 0 } ;
      RECT WindowRect ;  GetWindowRect ( hwnd, &WindowRect ) ;
      Rect.bottom = WindowRect.bottom - yBottom ;
      Rect.top = WindowRect.bottom - yTop ;
      return ( Rect ) ;
   } /* endmethod */

#endif 

