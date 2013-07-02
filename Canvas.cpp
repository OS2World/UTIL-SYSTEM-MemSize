/***************************************************************** Canvas.CPP
 *                                                                          *
 *                   Graphical Workspace (Canvas) Object                    *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#include <string.h>

#include "Canvas.h"
#include "Debug.h"

/****************************************************************************
 *                                                                          *
 *  Canvas: Initialize the static constants.                                *
 *                                                                          *
 ****************************************************************************/

int Canvas::VP_BOTTOM = 0 ;
int Canvas::VP_CENTER = 1 ;
int Canvas::VP_TOP    = 2 ;
int Canvas::HP_LEFT   = 0 ;
int Canvas::HP_CENTER = 1 ;
int Canvas::HP_RIGHT  = 2 ;

/****************************************************************************
 *                                                                          *
 *  Canvas: Simple unscaled workspace associated with a window.             *
 *                                                                          *
 ****************************************************************************/

Canvas::Canvas ( char *tag, HWND window ) : 
   Tag(tag), Window(window), Type(WRKSPC_GETPS), CurrentBitmap(0), CodePage(0) {
   Lock.Request ( ) ;
   #ifdef __OS2__
      Anchor = 0 ;
      Handle = 0 ;
      if ( Window ) {
         Anchor = WinQueryAnchorBlock ( Window ) ;
         Handle = WinGetPS ( Window ) ;
         if ( Handle ) {
            GpiCreateLogColorTable ( Handle, LCOL_RESET, LCOLF_RGB, 0, 0, 0 ) ;
         } else {
            char Message [512] ;
            Log ( "Canvas(%08X:'%s')::Canvas() Unable to get presentation space: %s", this, Tag, InterpretWinError(Anchor,Message) ) ;
         } /* endif */
      } else {
         Log ( "Canvas(%08X:'%s')::Canvas() ERROR: Attempt to create presentation space when window isn't defined yet.", this, Tag ) ;
      } /* endif */
   #else
      PreviousObject = 0 ;
      Handle = GetDC ( Window ) ;
   #endif 
   Lock.Release ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Canvas: Simple unscaled workspace associated with a window, but to be   *
 *    used in satisfying a current Paint request.                           *
 *                                                                          *
 ****************************************************************************/

Canvas::Canvas ( char *tag, HWND window, RECTANGLE &Clip ) :
   Tag(tag), Window(window), Type(WRKSPC_PAINT), CurrentBitmap(0), CodePage(0) {
   Lock.Request ( ) ;
   #ifdef __OS2__
      RECTL Rect ;
      Anchor = WinQueryAnchorBlock ( Window ) ;
      Handle = WinBeginPaint ( Window, 0, &Rect ) ;
      Clip = RECTANGLE ( Rect ) ;
      GpiCreateLogColorTable ( Handle, LCOL_RESET, LCOLF_RGB, 0, 0, 0 ) ;
   #else
      PreviousObject = 0 ;
      Handle = BeginPaint ( Window, &PaintInfo ) ;
      Clip = RECTANGLE ( Window, PaintInfo.rcPaint ) ;
   #endif 
   Lock.Release ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Canvas: Simple unscaled workspace associated with memory.               *
 *                                                                          *
 ****************************************************************************/

Canvas::Canvas ( char *tag ) : 
   Tag(tag), Window(0), Type(WRKSPC_GPI), CurrentBitmap(0), CodePage(0) {
   Lock.Request ( ) ;
   #ifdef __OS2__
      Anchor = 0 ;
      static PSZ pszData [2] = { 0, PSZ("Display") } ;
      hDC = DevOpenDC ( Anchor, OD_MEMORY, PSZ("*"), 2, PDEVOPENDATA(pszData), 0 ) ;
      SIZEL PageSize = { 0, 0 } ;
      Handle = GpiCreatePS ( Anchor, hDC, &PageSize, PU_PELS | GPIA_ASSOC | GPIT_MICRO ) ;
      GpiCreateLogColorTable ( Handle, LCOL_RESET, LCOLF_RGB, 0, 0, 0 ) ;
   #else
      PreviousObject = 0 ;
      Handle = CreateCompatibleDC ( 0 ) ;
   #endif 
   Lock.Release ( ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Canvas: Destructor                                                      *
 *                                                                          *
 ****************************************************************************/

Canvas::~Canvas ( ) {

   Lock.Request ( ) ;

   if ( CurrentBitmap ) 
      ResetBitmap ( ) ;

   switch ( Type ) {

      case WRKSPC_GETPS:
         if ( Handle ) { 
            #ifdef __OS2__
               if ( !WinReleasePS ( Handle ) ) 
                  Log ( "Canvas(%08X:'%s')::~Canvas() Unable to release presentation space: Status %i.", this, Tag, WinGetLastError(Anchor) ) ;
            #else
               if ( !ReleaseDC ( Window, Handle ) ) 
                  Log ( "Canvas(%08X:'%s')::~Canvas() Unable to release device context.  Status %i", this, Tag, GetLastError() ) ;
            #endif
            Handle = 0 ;
         } /* endif */
         break;

      case WRKSPC_PAINT:
         if ( Handle ) { 
            #ifdef __OS2__
               if ( !WinEndPaint ( Handle ) ) {
                  char Message [512] ;
                  Log ( "Canvas(%08X:'%s')::~Canvas() Unable to end paint: %s", this, Tag, InterpretWinError(Anchor,Message) ) ;
               } /* endif */
            #else
               if ( !EndPaint ( Window, &PaintInfo ) ) 
                  Log ( "Canvas(%08X:'%s')::~Canvas() Unable to end paint.  Status %i", this, Tag, GetLastError() ) ;
            #endif
            Handle = 0 ;
         } /* endif */
         break;

      case WRKSPC_GPI:
         if ( Handle ) { 
            #ifdef __OS2__
               if ( !GpiDestroyPS ( Handle ) ) {
                  char Message [512] ;
                  Log ( "Canvas(%08X:'%s')::~Canvas() Unable to destroy presentation space: %s", this, Tag, InterpretWinError(Anchor,Message) ) ;
               } /* endif */
               DevCloseDC ( hDC ) ;
            #else
               if ( !DeleteDC ( Handle ) ) 
                  Log ( "Canvas(%08X:'%s')::~Canvas() Unable to destroy device context: %i", this, Tag, GetLastError() ) ;
            #endif
            Handle = 0 ;
         } /* endif */
         break;

      default: {
         Log ( "Canvas(%08X:'%s')::~Canvas() Undefined type!", this, Tag ) ;
         break; }

   } /* endswitch */

   Lock.Release ( ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Set CodePage.                                                   *
 *                                                                          *
 ****************************************************************************/

int Canvas::SetCodePage ( int codepage ) {

   CodePage = codepage ;

   #ifdef __OS2__

      return ( GpiSetCp ( Handle, CodePage ) ) ;

   #else

      // ELABORATE
      return ( 0 ) ;

   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Set Color                                                       *
 *                                                                          *
 ****************************************************************************/

void Canvas::SetColor ( long Color ) {

   #ifdef __OS2__

      GpiSetColor ( Handle, Color ) ;

   #else

      HBRUSH Brush = CreateSolidBrush ( Color ) ;
      SelectObject ( Handle, Brush ) ;

   #endif

} /* endmethod */

void Canvas::SetBackColor ( long Color ) {

   #ifdef __OS2__

      GpiSetBackColor ( Handle, Color ) ;

   #else

      SetBkColor ( Handle, Color ) ;

   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Move To Point                                                   *
 *                                                                          *
 ****************************************************************************/

void Canvas::Move ( POINTL &Point ) {

   #ifdef __OS2__

      GpiMove ( Handle, &Point ) ;

   #else

      MoveToEx ( Handle, Point.x, Point.y, 0 ) ;

   #endif

   LastPosition = Point ;

} /* endmethod */

void Canvas::Move ( int x, int y ) {
   POINTL Point = { x, y } ;
   Move ( Point ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Draw Line To Point                                              *
 *                                                                          *
 ****************************************************************************/

void Canvas::Line ( POINTL &Point ) {

   #ifdef __OS2__

      GpiLine ( Handle, &Point ) ;

   #else

      LineTo ( Handle, Point.x, Point.y ) ;

   #endif

   LastPosition = Point ;

} /* endmethod */

void Canvas::Line ( int x, int y ) {
   POINTL Point = { x, y } ;
   Line ( Point ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Draw Box To Point                                               *
 *                                                                          *
 ****************************************************************************/

void Canvas::Box ( RECTANGLE &Rectangle ) {
   Move ( Rectangle.xLeft, Rectangle.yBottom ) ;
   Box ( Rectangle.xRight, Rectangle.yTop ) ;
} /* endmethod */

void Canvas::Box ( POINTL &Point ) {

   #ifdef __OS2__

      GpiBox ( Handle, DRO_OUTLINE, &Point, 0, 0 ) ;

   #else

      Line ( LastPosition.x, Point.y ) ;
      Line ( Point.x, Point.y ) ;
      Line ( Point.x, LastPosition.y ) ;
      Line ( LastPosition.x, LastPosition.y ) ;

   #endif

} /* endmethod */

void Canvas::Box ( int x, int y ) {
   POINTL Point = { x, y } ;
   Box ( Point ) ;
} /* endmethod */

void Canvas::BoxFill ( RECTANGLE &Rectangle ) {
   Move ( Rectangle.xLeft, Rectangle.yBottom ) ;
   BoxFill ( Rectangle.xRight, Rectangle.yTop ) ;
} /* endmethod */

void Canvas::BoxFill ( POINTL &Point ) {

   #ifdef __OS2__

      GpiBox ( Handle, DRO_FILL, &Point, 0, 0 ) ;

   #else

      RECTANGLE Rectangle ( 
         min ( Point.x, LastPosition.x ),
         max ( Point.y, LastPosition.y ),
         max ( Point.x, LastPosition.x ),
         min ( Point.y, LastPosition.y ) ) ;
      RECT Rect = Rectangle.Get ( Window ) ;
      ::Rectangle ( Handle, Rect.left, Rect.top, Rect.right, Rect.bottom ) ;

   #endif

} /* endmethod */

void Canvas::BoxFill ( int x, int y ) {
   POINTL Point = { x, y } ;
   BoxFill ( Point ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Measure Text Box                                                *
 *                                                                          *
 ****************************************************************************/

RECTANGLE Canvas::MeasureText ( char *Text ) {

   #ifdef __OS2__

      RECTL Rect ;
      WinQueryWindowRect ( HWND_DESKTOP, &Rect ) ;
      WinDrawText ( Handle, strlen(Text), PSZ(Text), &Rect, 0, 0, DT_LEFT | DT_BOTTOM | DT_QUERYEXTENT ) ;
      RECTANGLE Rectangle ( Rect ) ;

   #else

      // ELABORATE
      Text = 0 ;
      RECTANGLE Rectangle ;

   #endif

   return ( Rectangle ) ;
} /* endmethod */

void Canvas::QueryTextBox ( char *Text, POINTL TextBox [] ) {

   #ifdef __OS2__

      GpiQueryTextBox ( Handle, strlen(Text), PCH(Text), TXTBOX_COUNT, TextBox ) ;

   #else

      SIZE Size ;
      GetTextExtentPoint ( Handle, Text, strlen(Text), &Size ) ;

      TEXTMETRIC TextMetrics ;      
      GetTextMetrics ( Handle, &TextMetrics ) ;

      TextBox[TXTBOX_TOPLEFT].x = 0 ;   
      TextBox[TXTBOX_TOPLEFT].y = Size.cy - TextMetrics.tmDescent ;

      TextBox[TXTBOX_BOTTOMLEFT].x = 0 ;
      TextBox[TXTBOX_BOTTOMLEFT].y = -TextMetrics.tmDescent ;

      TextBox[TXTBOX_TOPRIGHT].x = Size.cx ;
      TextBox[TXTBOX_TOPRIGHT].y = Size.cy - TextMetrics.tmDescent ;

      TextBox[TXTBOX_BOTTOMRIGHT].x = Size.cx ;
      TextBox[TXTBOX_BOTTOMRIGHT].y = -TextMetrics.tmDescent ;

      TextBox[TXTBOX_CONCAT].x = Size.cx ;
      TextBox[TXTBOX_CONCAT].y = 0 ;    

   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Draw Text                                                       *
 *                                                                          *
 ****************************************************************************/

void Canvas::DrawText ( char *Text, RECTANGLE &Rectangle, int TextColor, int BackColor, int HPos, int VPos, int Clear ) {

   #ifdef __OS2__

      RECTL Rect = Rectangle.Get ( ) ;
      WinDrawText ( Handle, strlen(Text), PSZ(Text), &Rect, TextColor, BackColor, 
         ((HPos==HP_LEFT)?DT_LEFT:(HPos==HP_CENTER?DT_CENTER:DT_RIGHT)) |
         ((VPos==VP_BOTTOM)?DT_BOTTOM:(VPos==VP_CENTER?DT_VCENTER:DT_TOP)) |
         ( Clear ? DT_ERASERECT : 0 ) ) ;

   #else

      RECT Rect = Rectangle.Get ( Window ) ;

      if ( Clear ) { 
         SetColor ( BackColor ) ;
         ::Rectangle ( Handle, Rect.left, Rect.top, Rect.right, Rect.bottom ) ;
      } /* endif */

      SetColor ( TextColor ) ;
      ::DrawText ( Handle, Text, strlen(Text), &Rect, 
         ((HPos==HP_LEFT)?DT_LEFT:(HPos==HP_CENTER?DT_CENTER:DT_RIGHT)) |
         ((VPos==VP_BOTTOM)?DT_BOTTOM:(VPos==VP_CENTER?DT_VCENTER:DT_TOP)) ) ;

   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Create Bitmap (compatible with this canvas)                     *
 *                                                                          *
 ****************************************************************************/

HBITMAP Canvas::CreateBitmap ( int cx, int cy ) {

   #ifdef __OS2__

      // Determine this canvas's format.
      long alData [2] ;
      GpiQueryDeviceBitmapFormats ( Handle, 2, alData ) ;

      // Set up the bitmap header.
      BITMAPINFOHEADER2 BitmapHeader ;
      memset ( &BitmapHeader, 0, sizeof(BitmapHeader) ) ;
      BitmapHeader.cbFix = 16 ;
      BitmapHeader.cx = cx ;
      BitmapHeader.cy = cy ;
      BitmapHeader.cPlanes = USHORT ( alData[0] ) ;
      BitmapHeader.cBitCount = USHORT ( alData[1] ) ;

      // Create the bitmap.
      HBITMAP Bitmap = GpiCreateBitmap ( Handle, &BitmapHeader, 0, 0, 0 ) ;

   #else

      HBITMAP Bitmap = CreateCompatibleBitmap ( Handle, cx, cy ) ;

   #endif

   // Done.
   return ( Bitmap ) ;
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Set Bitmap to Canvas.                                           *
 *                                                                          *
 ****************************************************************************/

void Canvas::SetBitmap ( HBITMAP Bitmap ) {
   CurrentBitmap = Bitmap ;

   #ifdef __OS2__

      GpiSetBitmap ( Handle, Bitmap ) ;

   #else

      PreviousObject = SelectObject ( Handle, (HGDIOBJ)Bitmap ) ;

   #endif

} /* endmethod */

void Canvas::ResetBitmap ( ) {

   #ifdef __OS2__

      GpiSetBitmap ( Handle, 0 ) ;

   #else

      SelectObject ( Handle, PreviousObject ) ;

   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Gather Font Information                                         *
 *                                                                          *
 ****************************************************************************/

void Canvas::QueryFontInfo ( FONTINFO &Info ) {

   #ifdef __OS2__

      GpiQueryFontMetrics ( Handle, sizeof(Info.Metrics), &Info.Metrics ) ;
      Info.Mode = GpiQueryCharMode ( Handle ) ;
      GpiQueryCharBox ( Handle, &Info.Box ) ;
      Info.CodePage = CodePage ;

   #else

      GetTextMetrics ( Handle, &Info.Metrics ) ;

   #endif

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *  Method: Set Font, given information previously gathered.                *
 *                                                                          *
 ****************************************************************************/

void Canvas::SetFont ( FONTINFO &Info ) {

   #ifdef __OS2__

      FATTRS FontAttributes = { 0 } ;
      strcpy ( FontAttributes.szFacename, Info.Metrics.szFacename ) ;
      FontAttributes.usRecordLength  = sizeof(FontAttributes) ;
      FontAttributes.usCodePage      = Info.Metrics.usCodePage ;
      FontAttributes.lMaxBaselineExt = Info.Metrics.lMaxBaselineExt ;
      FontAttributes.lAveCharWidth   = Info.Metrics.lAveCharWidth ;
      GpiCreateLogFont ( Handle, 0, 1, &FontAttributes ) ;
      GpiSetCharSet ( Handle, 1 ) ;
      GpiSetCharMode ( Handle, Info.Mode ) ;
      if ( Info.Mode == CM_MODE3 )
         GpiSetCharBox ( Handle, &Info.Box ) ;

   #else

      // ELABORATE
      Info.Metrics.tmHeight = 0 ;

   #endif

} /* endmethod */

