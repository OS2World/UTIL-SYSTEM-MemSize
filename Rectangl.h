/***************************************************************** RECTANGL.H
 *                                                                          *
 *                        Portable Rectangle Object                         *
 *                                                                          *
 ****************************************************************************/

#ifndef RECTANGLE_H
#define RECTANGLE_H

struct RECTANGLE {

   long xLeft ; 
   long yBottom ;
   long xRight ; 
   long yTop ;

   RECTANGLE ( int l, int b, int r, int t ) : xLeft(l), yBottom(b), xRight(r), yTop(t) { } 
   RECTANGLE ( HWND hwnd ) ;
   RECTANGLE ( ) : xLeft(0), yBottom(0), xRight(0), yTop(0) { } 

   #ifdef __OS2__
      RECTANGLE ( RECTL &Rect ) ;
      RECTL Get ( ) ;
   #else
      RECTANGLE ( HWND hwnd, RECT &Rect ) ;
      RECT Get ( HWND hwnd ) ;
   #endif
} ;

#endif
