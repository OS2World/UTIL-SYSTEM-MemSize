/******************************************************************* Canvas.H
 *                                                                          *
 *                   Graphical Workspace (Canvas) Object                    *
 *                                                                          *
 ****************************************************************************/

#ifndef CANVAS_H
#define CANVAS_H

#include "Mutex.h"
#include "Rectangle.h"

typedef struct {        // All the information it takes to duplicate a font.
   #ifdef __OS2__
      FONTMETRICS Metrics ;
      LONG        Mode ;   
      SIZEF       Box ;    
      USHORT      CodePage ;   
   #else
      TEXTMETRIC  Metrics ;
   #endif
} FONTINFO ;

#ifndef __OS2__
   enum { TXTBOX_TOPLEFT, TXTBOX_BOTTOMLEFT, TXTBOX_TOPRIGHT, TXTBOX_BOTTOMRIGHT, TXTBOX_CONCAT, TXTBOX_COUNT } ;
#endif

enum WorkspaceType { WRKSPC_GETPS, WRKSPC_PAINT, WRKSPC_GPI } ;

class Canvas {

   private:
      char *Tag ;
      HWND Window ;
      WorkspaceType Type ;
      #ifdef __OS2__
         ANCHOR Anchor ;
         HDC hDC ;
         HPS Handle ;
      #else
         PAINTSTRUCT PaintInfo ;
         HDC Handle ;
         HGDIOBJ PreviousObject ;
      #endif
      POINTL LastPosition ;
      HBITMAP CurrentBitmap ;
      int CodePage ;
      Mutex Lock ;

   public:
      // Constructors
      Canvas ( char *Tag, HWND Window ) ;
      Canvas ( char *Tag, HWND Window, RECTANGLE &Clip ) ;
      Canvas ( char *Tag="" ) ; // Default.

      // Destructor
      ~Canvas ( ) ;     

      // Access functions.
      #ifdef __OS2__
         HPS QueryHandle ( ) { return ( Handle ) ; }
      #else
         HDC QueryHandle ( ) { return ( Handle ) ; }
      #endif

      // Set CodePage.
      int SetCodePage ( int CodePage ) ;

      // Set Color.
      void SetColor ( long Color ) ;
      void SetBackColor ( long Color ) ;

      // Move to point
      void Move ( POINTL &Point ) ;
      void Move ( int x, int y ) ;

      // Draw line to point
      void Line ( POINTL &Point ) ;
      void Line ( int x, int y ) ;

      // Draw box to point.
      void Box ( RECTANGLE &Rectangle ) ;
      void Box ( POINTL &Point ) ;
      void Box ( int x, int y ) ;
      void BoxFill ( RECTANGLE &Rectangle ) ;
      void BoxFill ( POINTL &Point ) ;
      void BoxFill ( int x, int y ) ;

      // Method: Measure Text Rectangle
      RECTANGLE MeasureText ( char *Text ) ;

      // Method: Query Text Box (more elaborate than MeasureText).
      void QueryTextBox ( char *Text, POINTL TextBox[] ) ;

      // Method: Draw Text in Rectangle
      static int VP_BOTTOM, VP_CENTER, VP_TOP ;
      static int HP_LEFT, HP_CENTER, HP_RIGHT ;
      void DrawText ( char *Text, RECTANGLE &Rectangle, int TextColor=0, int BackColor=0, int HPos=HP_LEFT, int VPos=VP_BOTTOM, int Clear=FALSE ) ;

      // Method: Create Bitmap compatible with this canvas.
      HBITMAP CreateBitmap ( int cx, int cy ) ;

      // Methods: Set/Reset Bitmap to this canvas.
      void SetBitmap ( HBITMAP Bitmap ) ;
      void ResetBitmap ( ) ;

      // Method: Gather information about the current font.
      void QueryFontInfo ( FONTINFO &Info ) ;

      // Method: Set Font
      void SetFont ( FONTINFO &Info ) ;
} ;

#endif // End if not already included.
