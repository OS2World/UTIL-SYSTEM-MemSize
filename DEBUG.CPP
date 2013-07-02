/****************************************************************** DEBUG.CPP
 *                                                                          *
 *  Debugging Aids                                                          *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys\stat.h>

#include "Debug.h"
#include "MemSize.h"
#include "Mutex.h"

// #define DEBUG
 
/****************************************************************************
 *                                                                          *
 *                       Display Debug Message                              *
 *                                                                          *
 ****************************************************************************/

extern void Debug ( HWND hwnd, char *Message, ... ) {

  /**************************************************************************
   * Format the debug message.                                              *
   **************************************************************************/

   va_list Marker ;
   va_start ( Marker, Message ) ;

   char Text [500] ;
   vsprintf ( Text, Message, Marker ) ;

   va_end ( Marker ) ;

  /**************************************************************************
   * Display the log message and wait for the user to press ENTER.          *
   **************************************************************************/

   Sys_MessageBox ( HWND_DESKTOP, hwnd, Text, "Debug", 0, MB_OK ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                         Log Debug Message                                *
 *                                                                          *
 ****************************************************************************/

Mutex LogSemaphore ( 0 ) ;      // Special MUTEX: Don't try to log errors . . . ever.

extern void Log ( char *Message, ... ) {

  /**************************************************************************
   * Serialize this function by requesting its semaphore.                   *
   **************************************************************************/

   LogSemaphore.Request ( ) ;

  /**************************************************************************
   * Determine the log file location.                                       *
   **************************************************************************/

   char LogFile [_MAX_PATH] ;
   if ( HomePath[0] ) {
      strcpy ( LogFile, HomePath ) ;
      strcat ( LogFile, "\\" LOGFILE ) ;
   } else {
      strcpy ( LogFile, LOGFILE ) ;
   } /* endif */

  /**************************************************************************
   * If the log file has grown too large, back it up and start a new one.   *
   **************************************************************************/

   struct stat FileStatus ;
   if ( stat ( LogFile, &FileStatus ) == 0 ) {
      if ( FileStatus.st_size > 100000 ) { // If file over one hundred thousand bytes . . .
         char BackupFile [_MAX_PATH] ;
         strcpy ( BackupFile, HomePath ) ;
         strcat ( BackupFile, "\\LOGFILE.BAK" ) ;
         remove ( BackupFile ) ;
         rename ( LogFile, BackupFile ) ;
      } /* endif */
   } /* endif */

  /**************************************************************************
   * Open the log file.                                                     *
   **************************************************************************/

   FILE *File = fopen ( LogFile, "a" ) ;

  /**************************************************************************
   * If the file got opened, write the message to the log file and close it.*
   **************************************************************************/

   if ( File ) {
      char Time [9], Date [9] ;
      fprintf ( File, "%s %s ", _strtime(Time), _strdate(Date) ) ;
      va_list Marker ;
      va_start ( Marker, Message ) ;
      vfprintf ( File, Message, Marker ) ;
      va_end ( Marker ) ;
      fprintf ( File, "\n" ) ;
      fclose ( File ) ;
   } /* endif */

  /**************************************************************************
   * Release the function semaphore and return.                             *
   **************************************************************************/

   LogSemaphore.Release ( ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                          Dump Memory to Log                              *
 *                                                                          *
 ****************************************************************************/

extern void DumpMemory ( char *Note, void *pMemory, int Count ) {
   Log ( "Dump of memory at %08lX for %i bytes (%s)", pMemory, Count, Note ) ;
   for ( int i=0; i<Count; i+=16 ) {
      char Buffer [100] ;
      sprintf ( Buffer, "%08lX: ", PCHAR(pMemory)+i ) ;
      for ( int j=i; j<i+16 && j<Count; j++ ) {
         sprintf ( Buffer+strlen(Buffer), "%02X ", *(PUCHAR(pMemory)+j) ) ;
      } /* endfor */
      while ( j<i+16 ) {
         strcat ( Buffer, "   " ) ;
         j ++ ;
      } /* endwhile */
      strcat ( Buffer, "|" ) ;
      for ( j=i; j<i+16 && j<Count; j++ ) {
         sprintf ( Buffer+strlen(Buffer), "%c", isprint(*(PUCHAR(pMemory)+j)) ? *(PUCHAR(pMemory)+j) : ' ' ) ;
      } /* endfor */
      while ( j<i+16 ) {
         strcat ( Buffer, " " ) ;
         j ++ ;
      } /* endwhile */
      strcat ( Buffer, "|" ) ;
      Log ( "  %s", Buffer ) ;
   } /* endfor */
}
 
/****************************************************************************
 *                                                                          *
 *                          Open Timer for Use                              *
 *                                                                          *
 ****************************************************************************/

#ifdef __OS2__

extern HFILE Timer ( 0 ) ;

extern BOOL OpenTimer ( void ) {

   if ( Timer )
      DosClose ( Timer ) ;

   ULONG Action ;
   if ( DosOpen ( (PSZ)"TIMER$", &Timer, &Action, 0, FILE_NORMAL, FILE_OPEN, OPEN_SHARE_DENYNONE, 0 ) ) 
      return ( FALSE ) ;

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                              Close Timer                                 *
 *                                                                          *
 ****************************************************************************/

extern void CloseTimer ( void ) {
   DosClose ( Timer ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                       Read Time from HRTIMER.SYS                         *
 *                                                                          *
 ****************************************************************************/

extern BOOL GetTime ( PTIMESTAMP pts ) {

   ULONG ByteCount ;

   if ( DosRead ( Timer, pts, sizeof(*pts), &ByteCount ) )
      return ( FALSE ) ;

   return ( TRUE ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                         Calculate Elapsed Time                           *
 *                                                                          *
 ****************************************************************************/

extern ULONG ComputeElapsedTime ( PTIMESTAMP ptsStart, PTIMESTAMP ptsStop, PULONG pulNs ) {

   TIMESTAMP tsStart = *ptsStart ;                      // De-reference timestamp
                                                        //     structures for speed
   TIMESTAMP tsStop  = *ptsStop ;

   ULONG ulMsecs = tsStop.ulMs - tsStart.ulMs ;         // Elapsed milliseconds

   ULONG ulNsecs ;
   if( tsStart.ulNs > tsStop.ulNs ) {                   // If nanosecond overflow ...
      ulNsecs = (1000000 + tsStop.ulNs) - tsStart.ulNs; // Adjust nanoseconds
      ulMsecs--;                                        // Adjust milliseconds
   } else
      ulNsecs = tsStop.ulNs - tsStart.ulNs ;            // No overflow..Elapsed nanos

   *pulNs = ulNsecs ;

   return ( ulMsecs ) ;
}

#endif // __OS2__
 
/****************************************************************************
 *                                                                          *
 *  Allocate Memory                                                         *
 *                                                                          *
 ****************************************************************************/

//#define ALLOCATE_THROUGH_DOS

extern void *AllocateMemory ( size_t ByteCount ) {

   #if defined(ALLOCATE_THROUGH_DOS) && defined(__OS2__)

      void *Memory ;
      DosAllocMem ( &Memory, ByteCount, PAG_READ | PAG_WRITE | PAG_COMMIT ) ;
      return ( Memory ) ;

   #else

      return ( malloc ( ByteCount ) ) ;

   #endif
}
 
/****************************************************************************
 *                                                                          *
 *  Free Memory                                                             *
 *                                                                          *
 ****************************************************************************/

extern void FreeMemory ( void *Memory ) {

   #if defined(ALLOCATE_THROUGH_DOS) && defined(__OS2__)

      DosFreeMem ( Memory ) ;

   #else

      free ( Memory ) ;

   #endif
}

/****************************************************************************
 *                                                                          *
 *  Interpret WIN/GPI errors                                                *
 *                                                                          *
 ****************************************************************************/

extern char *InterpretWinError ( ANCHOR Anchor, char *Message ) {

#ifdef __OS2__

   PERRINFO pErrorInfo = WinGetErrorInfo ( Anchor ) ;
   if ( pErrorInfo ) {
      strcpy ( Message, "" ) ;
      long *Offsets = (long*) ( ((char*)pErrorInfo) + pErrorInfo->offaoffszMsg ) ;
      void *BinaryData = ( ((char*)pErrorInfo) + pErrorInfo->offBinaryData ) ;
      for ( int i=0; i<pErrorInfo->cDetailLevel; i++ ) {
         char *FormatString = (char*)pErrorInfo + Offsets[i] ;
         if ( FormatString[strlen(FormatString)-1] == '\n' ) 
            FormatString[strlen(FormatString)-1] = 0 ;
         if ( FormatString[strlen(FormatString)-1] == '\r' )
            FormatString[strlen(FormatString)-1] = 0 ;
         vsprintf ( Message+strlen(Message), FormatString, (char**)BinaryData ) ;
         if ( i < pErrorInfo->cDetailLevel-1 ) 
            strcat ( Message, ", " ) ;
         else
            strcat ( Message, "." ) ;
      } /* endfor */
      WinFreeErrorInfo ( pErrorInfo ) ;
      return ( Message ) ;
   } else {
      return ( 0 ) ;
   } /* endif */

#else // __NT__

   sprintf ( Message, "%08X", GetLastError() ) ;
   Anchor = 0 ; // Pacify the compiler.
   return ( Message ) ;

#endif // __OS2__ vs __NT__

}

/****************************************************************************
 *                                                                          *
 *  Log Received Message                                                    *
 *                                                                          *
 ****************************************************************************/

static struct MessageTable {
   MESG Code ;
   char *Name ;
} KnownMessages [] = {
#ifdef DEBUG
 #ifdef __OS2__
   { WM_NULL,                       "WM_NULL"                       },  // 0000
   { WM_CREATE,                     "WM_CREATE"                     },  // 0001
   { WM_DESTROY,                    "WM_DESTROY"                    },  // 0002
   { WM_ENABLE,                     "WM_ENABLE"                     },  // 0004
   { WM_SHOW,                       "WM_SHOW"                       },  // 0005
   { WM_MOVE,                       "WM_MOVE"                       },  // 0006
   { WM_SIZE,                       "WM_SIZE"                       },  // 0007
   { WM_ADJUSTWINDOWPOS,            "WM_ADJUSTWINDOWPOS"            },  // 0008
   { WM_CALCVALIDRECTS,             "WM_CALCVALIDRECTS"             },  // 0009
   { WM_SETWINDOWPARAMS,            "WM_SETWINDOWPARAMS"            },  // 000A
   { WM_QUERYWINDOWPARAMS,          "WM_QUERYWINDOWPARAMS"          },  // 000B
   { WM_HITTEST,                    "WM_HITTEST"                    },  // 000C
   { WM_ACTIVATE,                   "WM_ACTIVATE"                   },  // 000D
   { WM_SETFOCUS,                   "WM_SETFOCUS"                   },  // 000F +
   { WM_SETSELECTION,               "WM_SETSELECTION"               },  // 0010
   { WM_PPAINT,                     "WM_PPAINT"                     },  // 0011
   { WM_PSETFOCUS,                  "WM_PSETFOCUS"                  },  // 0012
   { WM_PSYSCOLORCHANGE,            "WM_PSYSCOLORCHANGE"            },  // 0013
   { WM_PSIZE,                      "WM_PSIZE"                      },  // 0014
   { WM_PACTIVATE,                  "WM_PACTIVATE"                  },  // 0015
   { WM_PCONTROL,                   "WM_PCONTROL"                   },  // 0016
   { WM_COMMAND,                    "WM_COMMAND"                    },  // 0020 +
   { WM_SYSCOMMAND,                 "WM_SYSCOMMAND"                 },  // 0021
   { WM_HELP,                       "WM_HELP"                       },  // 0022
   { WM_PAINT,                      "WM_PAINT"                      },  // 0023
   { WM_TIMER,                      "WM_TIMER"                      },  // 0024
   { WM_SEM1,                       "WM_SEM1"                       },  // 0025
   { WM_SEM2,                       "WM_SEM2"                       },  // 0026
   { WM_SEM3,                       "WM_SEM3"                       },  // 0027
   { WM_SEM4,                       "WM_SEM4"                       },  // 0028
   { WM_CLOSE,                      "WM_CLOSE"                      },  // 0029
   { WM_QUIT,                       "WM_QUIT"                       },  // 002A
   { WM_SYSCOLORCHANGE,             "WM_SYSCOLORCHANGE"             },  // 002B
   { WM_SYSVALUECHANGED,            "WM_SYSVALUECHANGED"            },  // 002D +
   { WM_APPTERMINATENOTIFY,         "WM_APPTERMINATENOTIFY"         },  // 002E
   { WM_PRESPARAMCHANGED,           "WM_PRESPARAMCHANGED"           },  // 002F
   { WM_CONTROL,                    "WM_CONTROL"                    },  // 0030
   { WM_VSCROLL,                    "WM_VSCROLL"                    },  // 0031
   { WM_HSCROLL,                    "WM_HSCROLL"                    },  // 0032
   { WM_INITMENU,                   "WM_INITMENU"                   },  // 0033
   { WM_MENUSELECT,                 "WM_MENUSELECT"                 },  // 0034
   { WM_MENUEND,                    "WM_MENUEND"                    },  // 0035
   { WM_DRAWITEM,                   "WM_DRAWITEM"                   },  // 0036
   { WM_MEASUREITEM,                "WM_MEASUREITEM"                },  // 0037
   { WM_CONTROLPOINTER,             "WM_CONTROLPOINTER"             },  // 0038
   { WM_QUERYDLGCODE,               "WM_QUERYDLGCODE"               },  // 003A
   { WM_INITDLG,                    "WM_INITDLG"                    },  // 003B
   { WM_SUBSTITUTESTRING,           "WM_SUBSTITUTESTRING"           },  // 003C
   { WM_MATCHMNEMONIC,              "WM_MATCHMNEMONIC"              },  // 003D
   { WM_SAVEAPPLICATION,            "WM_SAVEAPPLICATION"            },  // 003E
   { WM_FLASHWINDOW,                "WM_FLASHWINDOW"                },  // 0040 +
   { WM_FORMATFRAME,                "WM_FORMATFRAME"                },  // 0041
   { WM_UPDATEFRAME,                "WM_UPDATEFRAME"                },  // 0042
   { WM_FOCUSCHANGE,                "WM_FOCUSCHANGE"                },  // 0043
   { WM_SETBORDERSIZE,              "WM_SETBORDERSIZE"              },  // 0044
   { WM_TRACKFRAME,                 "WM_TRACKFRAME"                 },  // 0045
   { WM_MINMAXFRAME,                "WM_MINMAXFRAME"                },  // 0046
   { WM_SETICON,                    "WM_SETICON"                    },  // 0047
   { WM_QUERYICON,                  "WM_QUERYICON"                  },  // 0048
   { WM_SETACCELTABLE,              "WM_SETACCELTABLE"              },  // 0049
   { WM_QUERYACCELTABLE,            "WM_QUERYACCELTABLE"            },  // 004A
   { WM_TRANSLATEACCEL,             "WM_TRANSLATEACCEL"             },  // 004B
   { WM_QUERYTRACKINFO,             "WM_QUERYTRACKINFO"             },  // 004C
   { WM_QUERYBORDERSIZE,            "WM_QUERYBORDERSIZE"            },  // 004D
   { WM_NEXTMENU,                   "WM_NEXTMENU"                   },  // 004E
   { WM_ERASEBACKGROUND,            "WM_ERASEBACKGROUND"            },  // 004F
   { WM_QUERYFRAMEINFO,             "WM_QUERYFRAMEINFO"             },  // 0050
   { WM_QUERYFOCUSCHAIN,            "WM_QUERYFOCUSCHAIN"            },  // 0051
   { WM_OWNERPOSCHANGE,             "WM_OWNERPOSCHANGE"             },  // 0052
   { WM_CALCFRAMERECT,              "WM_CALCFRAMERECT"              },  // 0053
   { WM_WINDOWPOSCHANGED,           "WM_WINDOWPOSCHANGED"           },  // 0055 +
   { WM_ADJUSTFRAMEPOS,             "WM_ADJUSTFRAMEPOS"             },  // 0056
   { WM_QUERYFRAMECTLCOUNT,         "WM_QUERYFRAMECTLCOUNT"         },  // 0059 +
   { WM_QUERYHELPINFO,              "WM_QUERYHELPINFO"              },  // 005B +
   { WM_SETHELPINFO,                "WM_SETHELPINFO"                },  // 005C
   { WM_ERROR,                      "WM_ERROR"                      },  // 005D
   { WM_REALIZEPALETTE,             "WM_REALIZEPALETTE"             },  // 005E
   { WM_RENDERFMT,                  "WM_RENDERFMT"                  },  // 0060 +
   { WM_RENDERALLFMTS,              "WM_RENDERALLFMTS"              },  // 0061
   { WM_DESTROYCLIPBOARD,           "WM_DESTROYCLIPBOARD"           },  // 0062
   { WM_PAINTCLIPBOARD,             "WM_PAINTCLIPBOARD"             },  // 0063
   { WM_SIZECLIPBOARD,              "WM_SIZECLIPBOARD"              },  // 0064
   { WM_HSCROLLCLIPBOARD,           "WM_HSCROLLCLIPBOARD"           },  // 0065
   { WM_VSCROLLCLIPBOARD,           "WM_VSCROLLCLIPBOARD"           },  // 0066
   { WM_DRAWCLIPBOARD,              "WM_DRAWCLIPBOARD"              },  // 0067
   { WM_MOUSEMOVE,                  "WM_MOUSEMOVE"                  },  // 0070 +
   { WM_BUTTON1DOWN,                "WM_BUTTON1DOWN"                },  // 0071
   { WM_BUTTON1UP,                  "WM_BUTTON1UP"                  },  // 0072
   { WM_BUTTON1DBLCLK,              "WM_BUTTON1DBLCLK"              },  // 0073
   { WM_BUTTON2DOWN,                "WM_BUTTON2DOWN"                },  // 0074
   { WM_BUTTON2UP,                  "WM_BUTTON2UP"                  },  // 0075
   { WM_BUTTON2DBLCLK,              "WM_BUTTON2DBLCLK"              },  // 0076
   { WM_BUTTON3DOWN,                "WM_BUTTON3DOWN"                },  // 0077
   { WM_BUTTON3UP,                  "WM_BUTTON3UP"                  },  // 0078
   { WM_BUTTON3DBLCLK,              "WM_BUTTON3DBLCLK"              },  // 0079
   { WM_CHAR,                       "WM_CHAR"                       },  // 007A
   { WM_VIOCHAR,                    "WM_VIOCHAR"                    },  // 007B
   { WM_JOURNALNOTIFY,              "WM_JOURNALNOTIFY"              },  // 007C
   { WM_MOUSEMAP,                   "WM_MOUSEMAP"                   },  // 007D
   { WM_VRNDISABLED,                "WM_VRNDISABLED"                },  // 007E
   { WM_VRNENABLED,                 "WM_VRNENABLED"                 },  // 007F
   { WM_DDE_INITIATE,               "WM_DDE_INITIATE"               },  // 00A0 +
   { WM_DDE_REQUEST,                "WM_DDE_REQUEST"                },  // 00A1
   { WM_DDE_ACK,                    "WM_DDE_ACK"                    },  // 00A2
   { WM_DDE_DATA,                   "WM_DDE_DATA"                   },  // 00A3
   { WM_DDE_ADVISE,                 "WM_DDE_ADVISE"                 },  // 00A4
   { WM_DDE_UNADVISE,               "WM_DDE_UNADVISE"               },  // 00A5
   { WM_DDE_POKE,                   "WM_DDE_POKE"                   },  // 00A6
   { WM_DDE_EXECUTE,                "WM_DDE_EXECUTE"                },  // 00A7
   { WM_DDE_TERMINATE,              "WM_DDE_TERMINATE"              },  // 00A8
   { WM_DDE_INITIATEACK,            "WM_DDE_INITIATEACK"            },  // 00A9
   { WM_QUERYCONVERTPOS,            "WM_QUERYCONVERTPOS"            },  // 00B0 +
   { SM_SETHANDLE,                  "SM_SETHANDLE"                  },  // 0100 +
   { SM_QUERYHANDLE,                "SM_QUERYHANDLE"                },  // 0101
   { WM_MSGBOXINIT,                 "WM_MSGBOXINIT"                 },  // 010E +
   { WM_MSGBOXDISMISS,              "WM_MSGBOXDISMISS"              },  // 010F
   { BM_CLICK,                      "BM_CLICK"                      },  // 0120 +
   { BM_QUERYCHECKINDEX,            "BM_QUERYCHECKINDEX"            },  // 0121
   { BM_QUERYHILITE,                "BM_QUERYHILITE"                },  // 0122
   { BM_SETHILITE,                  "BM_SETHILITE"                  },  // 0123
   { BM_QUERYCHECK,                 "BM_QUERYCHECK"                 },  // 0124
   { BM_SETCHECK,                   "BM_SETCHECK"                   },  // 0125
   { BM_SETDEFAULT,                 "BM_SETDEFAULT"                 },  // 0126
   { BM_AUTOSIZE,                   "BM_AUTOSIZE"                   },  // 0128 +
   { WM_CTLCOLORCHANGE,             "WM_CTLCOLORCHANGE"             },  // 0129
   { WM_QUERYCTLTYPE,               "WM_QUERYCTLTYPE"               },  // 0130 +
   { EM_QUERYCHANGED,               "EM_QUERYCHANGED"               },  // 0140 +
   { EM_QUERYSEL,                   "EM_QUERYSEL"                   },  // 0141
   { EM_SETSEL,                     "EM_SETSEL"                     },  // 0142
   { EM_SETTEXTLIMIT,               "EM_SETTEXTLIMIT"               },  // 0143
   { EM_CUT,                        "EM_CUT"                        },  // 0144
   { EM_COPY,                       "EM_COPY"                       },  // 0145
   { EM_CLEAR,                      "EM_CLEAR"                      },  // 0146
   { EM_PASTE,                      "EM_PASTE"                      },  // 0147
   { EM_QUERYFIRSTCHAR,             "EM_QUERYFIRSTCHAR"             },  // 0148
   { EM_SETFIRSTCHAR,               "EM_SETFIRSTCHAR"               },  // 0149
   { EM_QUERYREADONLY,              "EM_QUERYREADONLY"              },  // 014A
   { EM_SETREADONLY,                "EM_SETREADONLY"                },  // 014B
   { EM_SETINSERTMODE,              "EM_SETINSERTMODE"              },  // 014C
   { LM_QUERYITEMCOUNT,             "LM_QUERYITEMCOUNT"             },  // 0160 +
   { LM_INSERTITEM,                 "LM_INSERTITEM"                 },  // 0161
   { LM_SETTOPINDEX,                "LM_SETTOPINDEX"                },  // 0162
   { LM_DELETEITEM,                 "LM_DELETEITEM"                 },  // 0163
   { LM_SELECTITEM,                 "LM_SELECTITEM"                 },  // 0164
   { LM_QUERYSELECTION,             "LM_QUERYSELECTION"             },  // 0165
   { LM_SETITEMTEXT,                "LM_SETITEMTEXT"                },  // 0166
   { LM_QUERYITEMTEXTLENGTH,        "LM_QUERYITEMTEXTLENGTH"        },  // 0167
   { LM_QUERYITEMTEXT,              "LM_QUERYITEMTEXT"              },  // 0168
   { LM_SETITEMHANDLE,              "LM_SETITEMHANDLE"              },  // 0169
   { LM_QUERYITEMHANDLE,            "LM_QUERYITEMHANDLE"            },  // 016A
   { LM_SEARCHSTRING,               "LM_SEARCHSTRING"               },  // 016B
   { LM_SETITEMHEIGHT,              "LM_SETITEMHEIGHT"              },  // 016C
   { LM_QUERYTOPINDEX,              "LM_QUERYTOPINDEX"              },  // 016D
   { LM_DELETEALL,                  "LM_DELETEALL"                  },  // 016E
   { LM_INSERTMULTITEMS,            "LM_INSERTMULTITEMS"            },  // 016F
   { CBM_SHOWLIST,                  "CBM_SHOWLIST"                  },  // 0170
   { CBM_HILITE,                    "CBM_HILITE"                    },  // 0171
   { CBM_ISLISTSHOWING,             "CBM_ISLISTSHOWING"             },  // 0172
   { MM_INSERTITEM,                 "MM_INSERTITEM"                 },  // 0180 +
   { MM_DELETEITEM,                 "MM_DELETEITEM"                 },  // 0181
   { MM_QUERYITEM,                  "MM_QUERYITEM"                  },  // 0182
   { MM_SETITEM,                    "MM_SETITEM"                    },  // 0183
   { MM_QUERYITEMCOUNT,             "MM_QUERYITEMCOUNT"             },  // 0184
   { MM_STARTMENUMODE,              "MM_STARTMENUMODE"              },  // 0185
   { MM_ENDMENUMODE,                "MM_ENDMENUMODE"                },  // 0186
   { MM_REMOVEITEM,                 "MM_REMOVEITEM"                 },  // 0188 +
   { MM_SELECTITEM,                 "MM_SELECTITEM"                 },  // 0189
   { MM_QUERYSELITEMID,             "MM_QUERYSELITEMID"             },  // 018A
   { MM_QUERYITEMTEXT,              "MM_QUERYITEMTEXT"              },  // 018B
   { MM_QUERYITEMTEXTLENGTH,        "MM_QUERYITEMTEXTLENGTH"        },  // 018C
   { MM_SETITEMHANDLE,              "MM_SETITEMHANDLE"              },  // 018D
   { MM_SETITEMTEXT,                "MM_SETITEMTEXT"                },  // 018E
   { MM_ITEMPOSITIONFROMID,         "MM_ITEMPOSITIONFROMID"         },  // 018F
   { MM_ITEMIDFROMPOSITION,         "MM_ITEMIDFROMPOSITION"         },  // 0190
   { MM_QUERYITEMATTR,              "MM_QUERYITEMATTR"              },  // 0191
   { MM_SETITEMATTR,                "MM_SETITEMATTR"                },  // 0192
   { MM_ISITEMVALID,                "MM_ISITEMVALID"                },  // 0193
   { MM_QUERYITEMRECT,              "MM_QUERYITEMRECT"              },  // 0194
   { SBM_SETSCROLLBAR,              "SBM_SETSCROLLBAR"              },  // 01A0 +
   { SBM_SETPOS,                    "SBM_SETPOS"                    },  // 01A1
   { SBM_QUERYPOS,                  "SBM_QUERYPOS"                  },  // 01A2
   { SBM_QUERYRANGE,                "SBM_QUERYRANGE"                },  // 01A3
   { SBM_SETTHUMBSIZE,              "SBM_SETTHUMBSIZE"              },  // 01A6 +
   { TBM_SETHILITE,                 "TBM_SETHILITE"                 },  // 01E3 +
   { TBM_QUERYHILITE,               "TBM_QUERYHILITE"               },  // 01E4
   { HM_DISMISS_WINDOW,             "HM_DISMISS_WINDOW"             },  // 0221 +
   { HM_DISPLAY_HELP,               "HM_DISPLAY_HELP"               },  // 0222
   { HM_EXT_HELP,                   "HM_EXT_HELP"                   },  // 0223
   { HM_SET_ACTIVE_WINDOW,          "HM_SET_ACTIVE_WINDOW"          },  // 0224
   { HM_LOAD_HELP_TABLE,            "HM_LOAD_HELP_TABLE"            },  // 0225
   { HM_CREATE_HELP_TABLE,          "HM_CREATE_HELP_TABLE"          },  // 0226
   { HM_SET_HELP_WINDOW_TITLE,      "HM_SET_HELP_WINDOW_TITLE"      },  // 0227
   { HM_SET_SHOW_PANEL_ID,          "HM_SET_SHOW_PANEL_ID"          },  // 0228
   { HM_REPLACE_HELP_FOR_HELP,      "HM_REPLACE_HELP_FOR_HELP"      },  // 0229
   { HM_HELP_INDEX,                 "HM_HELP_INDEX"                 },  // 022A
   { HM_HELP_CONTENTS,              "HM_HELP_CONTENTS"              },  // 022B
   { HM_KEYS_HELP,                  "HM_KEYS_HELP"                  },  // 022C
   { HM_SET_HELP_LIBRARY_NAME,      "HM_SET_HELP_LIBRARY_NAME"      },  // 022D
   { HM_ERROR,                      "HM_ERROR"                      },  // 022E
   { HM_HELPSUBITEM_NOT_FOUND,      "HM_HELPSUBITEM_NOT_FOUND"      },  // 022F
   { HM_QUERY_KEYS_HELP,            "HM_QUERY_KEYS_HELP"            },  // 0230
   { HM_TUTORIAL,                   "HM_TUTORIAL"                   },  // 0231
   { HM_EXT_HELP_UNDEFINED,         "HM_EXT_HELP_UNDEFINED"         },  // 0232
   { HM_ACTIONBAR_COMMAND,          "HM_ACTIONBAR_COMMAND"          },  // 0233
   { HM_INFORM,                     "HM_INFORM"                     },  // 0234
   { HM_SET_OBJCOM_WINDOW,          "HM_SET_OBJCOM_WINDOW"          },  // 0238 +
   { HM_UPDATE_OBJCOM_WINDOW_CHAIN, "HM_UPDATE_OBJCOM_WINDOW_CHAIN" },  // 0239
   { HM_QUERY_DDF_DATA,             "HM_QUERY_DDF_DATA"             },  // 023A
   { HM_INVALIDATE_DDF_DATA,        "HM_INVALIDATE_DDF_DATA"        },  // 023B
   { HM_QUERY,                      "HM_QUERY"                      },  // 023C
   { HM_SET_COVERPAGE_SIZE,         "HM_SET_COVERPAGE_SIZE"         },  // 023D
   { HM_NOTIFY,                     "HM_NOTIFY"                     },  // 0242 +
   { HM_SET_USERDATA,               "HM_SET_USERDATA"               },  // 0243
   { HM_CONTROL,                    "HM_CONTROL"                    },  // 0244
   { DM_DROPNOTIFY,                 "DM_DROPNOTIFY"                 },  // 031E +
   { DM_DISCARDOBJECT,              "DM_DISCARDOBJECT"              },  // 031F
   { DM_PRINTOBJECT,                "DM_PRINTOBJECT"                },  // 0320
   { DM_DRAGOVERNOTIFY,             "DM_DRAGOVERNOTIFY"             },  // 0321
   { DM_RENDERFILE,                 "DM_RENDERFILE"                 },  // 0322
   { DM_FILERENDERED,               "DM_FILERENDERED"               },  // 0323
   { DM_DRAGERROR,                  "DM_DRAGERROR"                  },  // 0324
   { DM_EMPHASIZETARGET,            "DM_EMPHASIZETARGET"            },  // 0325
   { DM_DRAGFILECOMPLETE,           "DM_DRAGFILECOMPLETE"           },  // 0326
   { DM_RENDERPREPARE,              "DM_RENDERPREPARE"              },  // 0327
   { DM_RENDERCOMPLETE,             "DM_RENDERCOMPLETE"             },  // 0328
   { DM_RENDER,                     "DM_RENDER"                     },  // 0329
   { DM_PRINT,                      "DM_PRINT"                      },  // 032A
   { DM_ENDCONVERSATION,            "DM_ENDCONVERSATION"            },  // 032B
   { DM_DROPHELP,                   "DM_DROPHELP"                   },  // 032C
   { DM_DRAGLEAVE,                  "DM_DRAGLEAVE"                  },  // 032D
   { DM_DRAGOVER,                   "DM_DRAGOVER"                   },  // 032E
   { DM_DROP,                       "DM_DROP"                       },  // 032F
   { CM_ALLOCDETAILFIELDINFO,       "CM_ALLOCDETAILFIELDINFO"       },  // 0330
   { CM_ALLOCRECORD,                "CM_ALLOCRECORD"                },  // 0331
   { CM_ARRANGE,                    "CM_ARRANGE"                    },  // 0332
   { CM_ERASERECORD,                "CM_ERASERECORD"                },  // 0333
   { CM_FILTER,                     "CM_FILTER"                     },  // 0334
   { CM_FREEDETAILFIELDINFO,        "CM_FREEDETAILFIELDINFO"        },  // 0335
   { CM_FREERECORD,                 "CM_FREERECORD"                 },  // 0336
   { CM_HORZSCROLLSPLITWINDOW,      "CM_HORZSCROLLSPLITWINDOW"      },  // 0337
   { CM_INSERTDETAILFIELDINFO,      "CM_INSERTDETAILFIELDINFO"      },  // 0338
   { CM_INSERTRECORD,               "CM_INSERTRECORD"               },  // 0339
   { CM_INVALIDATEDETAILFIELDINFO,  "CM_INVALIDATEDETAILFIELDINFO"  },  // 033A
   { CM_INVALIDATERECORD,           "CM_INVALIDATERECORD"           },  // 033B
   { CM_PAINTBACKGROUND,            "CM_PAINTBACKGROUND"            },  // 033C
   { CM_QUERYCNRINFO,               "CM_QUERYCNRINFO"               },  // 033D
   { CM_QUERYDETAILFIELDINFO,       "CM_QUERYDETAILFIELDINFO"       },  // 033E
   { CM_QUERYDRAGIMAGE,             "CM_QUERYDRAGIMAGE"             },  // 033F
   { CM_QUERYRECORD,                "CM_QUERYRECORD"                },  // 0340
   { CM_QUERYRECORDEMPHASIS,        "CM_QUERYRECORDEMPHASIS"        },  // 0341
   { CM_QUERYRECORDFROMRECT,        "CM_QUERYRECORDFROMRECT"        },  // 0342
   { CM_QUERYRECORDRECT,            "CM_QUERYRECORDRECT"            },  // 0343
   { CM_QUERYVIEWPORTRECT,          "CM_QUERYVIEWPORTRECT"          },  // 0344
   { CM_REMOVEDETAILFIELDINFO,      "CM_REMOVEDETAILFIELDINFO"      },  // 0345
   { CM_REMOVERECORD,               "CM_REMOVERECORD"               },  // 0346
   { CM_SCROLLWINDOW,               "CM_SCROLLWINDOW"               },  // 0347
   { CM_SEARCHSTRING,               "CM_SEARCHSTRING"               },  // 0348
   { CM_SETCNRINFO,                 "CM_SETCNRINFO"                 },  // 0349
   { CM_SETRECORDEMPHASIS,          "CM_SETRECORDEMPHASIS"          },  // 034A
   { CM_SORTRECORD,                 "CM_SORTRECORD"                 },  // 034B
   { CM_OPENEDIT,                   "CM_OPENEDIT"                   },  // 034C
   { CM_CLOSEEDIT,                  "CM_CLOSEEDIT"                  },  // 034D
   { CM_COLLAPSETREE,               "CM_COLLAPSETREE"               },  // 034E
   { CM_EXPANDTREE,                 "CM_EXPANDTREE"                 },  // 034F
   { CM_QUERYRECORDINFO,            "CM_QUERYRECORDINFO"            },  // 0350
   { CM_INSERTRECORDARRAY,          "CM_INSERTRECORDARRAY"          },  // 0351
   { CM_MOVETREE,                   "CM_MOVETREE"                   },  // 0352
   { CM_SETTEXTVISIBILITY,          "CM_SETTEXTVISIBILITY"          },  // 0353
   { CM_SETGRIDINFO,                "CM_SETGRIDINFO"                },  // 0354
   { CM_QUERYGRIDINFO,              "CM_QUERYGRIDINFO"              },  // 0355
   { CM_SNAPTOGRID,                 "CM_SNAPTOGRID"                 },  // 0356
   { SLM_ADDDETENT,                 "SLM_ADDDETENT"                 },  // 0369 +
   { SLM_QUERYDETENTPOS,            "SLM_QUERYDETENTPOS"            },  // 036A
   { SLM_QUERYSCALETEXT,            "SLM_QUERYSCALETEXT"            },  // 036B
   { SLM_QUERYSLIDERINFO,           "SLM_QUERYSLIDERINFO"           },  // 036C
   { SLM_QUERYTICKPOS,              "SLM_QUERYTICKPOS"              },  // 036D
   { SLM_QUERYTICKSIZE,             "SLM_QUERYTICKSIZE"             },  // 036E
   { SLM_REMOVEDETENT,              "SLM_REMOVEDETENT"              },  // 036F
   { SLM_SETSCALETEXT,              "SLM_SETSCALETEXT"              },  // 0370
   { SLM_SETSLIDERINFO,             "SLM_SETSLIDERINFO"             },  // 0371
   { SLM_SETTICKSIZE,               "SLM_SETTICKSIZE"               },  // 0372
   { VM_QUERYITEM,                  "VM_QUERYITEM"                  },  // 0375 +
   { VM_QUERYITEMATTR,              "VM_QUERYITEMATTR"              },  // 0376
   { VM_QUERYMETRICS,               "VM_QUERYMETRICS"               },  // 0377
   { VM_QUERYSELECTEDITEM,          "VM_QUERYSELECTEDITEM"          },  // 0378
   { VM_SELECTITEM,                 "VM_SELECTITEM"                 },  // 0379
   { VM_SETITEM,                    "VM_SETITEM"                    },  // 037A
   { VM_SETITEMATTR,                "VM_SETITEMATTR"                },  // 037B
   { VM_SETMETRICS,                 "VM_SETMETRICS"                 },  // 037C
   { CM_SETITEMBIDIATTR,            "CM_SETITEMBIDIATTR"            },  // 0390 +
   { CM_SETFIELDBIDIATTR,           "CM_SETFIELDBIDIATTR"           },  // 0391
   { CM_QUERYITEMBIDIATTR,          "CM_QUERYITEMBIDIATTR"          },  // 0392
   { CM_QUERYFIELDBIDIATTR,         "CM_QUERYFIELDBIDIATTR"         },  // 0393
   { BKM_SETSTATUSLINEBIDIATTR,     "BKM_SETSTATUSLINEBIDIATTR"     },  // 0394
   { BKM_QUERYSTATUSLINEBIDIATTR,   "BKM_QUERYSTATUSLINEBIDIATTR"   },  // 0395
   { BKM_SETTABTEXTBIDIATTR,        "BKM_SETTABTEXTBIDIATTR"        },  // 0396
   { BKM_QUERYTABTEXTBIDIATTR,      "BKM_QUERYTABTEXTBIDIATTR"      },  // 0397
   { VM_SETITEMBIDIATTR,            "VM_SETITEMBIDIATTR"            },  // 0398
   { VM_QUERYITEMBIDIATTR,          "VM_QUERYITEMBIDIATTR"          },  // 0399
   { WM_CHORD,                      "WM_CHORD"                      },  // 0410 +
   { WM_BUTTON1MOTIONSTART,         "WM_BUTTON1MOTIONSTART"         },  // 0411
   { WM_BUTTON1MOTIONEND,           "WM_BUTTON1MOTIONEND"           },  // 0412
   { WM_BUTTON1CLICK,               "WM_BUTTON1CLICK"               },  // 0413
   { WM_BUTTON2MOTIONSTART,         "WM_BUTTON2MOTIONSTART"         },  // 0414
   { WM_BUTTON2MOTIONEND,           "WM_BUTTON2MOTIONEND"           },  // 0415
   { WM_BUTTON2CLICK,               "WM_BUTTON2CLICK"               },  // 0416
   { WM_BUTTON3MOTIONSTART,         "WM_BUTTON3MOTIONSTART"         },  // 0417
   { WM_BUTTON3MOTIONEND,           "WM_BUTTON3MOTIONEND"           },  // 0418
   { WM_BUTTON3CLICK,               "WM_BUTTON3CLICK"               },  // 0419
   { WM_BEGINDRAG,                  "WM_BEGINDRAG"                  },  // 0420 + 
   { WM_ENDDRAG,                    "WM_ENDDRAG"                    },  // 0421
   { WM_SINGLESELECT,               "WM_SINGLESELECT"               },  // 0422
   { WM_OPEN,                       "WM_OPEN"                       },  // 0423
   { WM_CONTEXTMENU,                "WM_CONTEXTMENU"                },  // 0424
   { WM_CONTEXTHELP,                "WM_CONTEXTHELP"                },  // 0425
   { WM_TEXTEDIT,                   "WM_TEXTEDIT"                   },  // 0426
   { WM_BEGINSELECT,                "WM_BEGINSELECT"                },  // 0427
   { WM_ENDSELECT,                  "WM_ENDSELECT"                  },  // 0428
   { WM_PICKUP,                     "WM_PICKUP"                     },  // 0429
   { MM_QUERYDEFAULTITEMID,         "MM_QUERYDEFAULTITEMID"         },  // 0431 +
   { MM_SETDEFAULTITEMID,           "MM_SETDEFAULTITEMID"           },  // 0432
   { WM_SEMANTICEVENT,              "WM_SEMANTICEVENT"              },  // 0490
   { 0x04A0,                        "WM_SetVideoMode"               },  // 04A0: GAMESRVR extension
   { 0x04A1,                        "WM_NotifyVideoModeChange"      },  // 04A1: GAMESRVR extension
   { 0x04A2,                        "WM_GetVideoModeTable"          },  // 04A2: GAMESRVR extension
   { CSM_QUERYRANGE,                "CSM_QUERYRANGE"                },  // 053D      
   { CSM_SETRANGE,                  "CSM_SETRANGE"                  },  // 053E      
   { CSM_QUERYVALUE,                "CSM_QUERYVALUE"                },  // 053F      
   { CSM_SETVALUE,                  "CSM_SETVALUE"                  },  // 0540      
   { CSM_QUERYRADIUS,               "CSM_QUERYRADIUS"               },  // 0541      
   { CSM_SETINCREMENT,              "CSM_SETINCREMENT"              },  // 0542      
   { CSM_QUERYINCREMENT,            "CSM_QUERYINCREMENT"            },  // 0543      
   { CSM_SETBITMAPDATA,             "CSM_SETBITMAPDATA"             },  // 0544      
   { CSN_SETFOCUS,                  "CSN_SETFOCUS"                  },  // 0548      
   { CSN_CHANGED,                   "CSN_CHANGED"                   },  // 0549      
   { CSN_TRACKING,                  "CSN_TRACKING"                  },  // 054A      
   { CSN_QUERYBACKGROUNDCOLOR,      "CSN_QUERYBACKGROUNDCOLOR"      },  // 054B 
   { LM_SETITEMWIDTH,               "LM_SETITEMWIDTH"               },  // 0660 +
   { WM_SETBIDIATTR,                "WM_SETBIDIATTR"                },  // 0BD0 +
   { WM_QUERYBIDIATTR,              "WM_QUERYBIDIATTR"              },  // 0BD1
   { WM_SETBIDISTAT,                "WM_SETBIDISTAT"                },  // 0BD2
   { WM_QUERYBIDISTAT,              "WM_QUERYBIDISTAT"              },  // 0BD3
   { WM_KBDLAYERCHANGED,            "WM_KBDLAYERCHANGED"            },  // 0BD4
   { WM_LANGVIEWINFOCHANGED,        "WM_LANGVIEWINFOCHANGED"        },  // 0BE0
   { WM_LANG_OPTIONS_DIALOG,        "WM_LANG_OPTIONS_DIALOG"        },  // 0BE2 +
   { FDM_FILTER,                    "FDM_FILTER"                    },  // 1028 + (WM_USER+40)
   { FDM_VALIDATE,                  "FDM_VALIDATE"                  },  // 1029
   { FDM_ERROR,                     "FDM_ERROR"                     },  // 102A
   { FNTM_FACENAMECHANGED,          "FNTM_FACENAMECHANGED"          },  // 1032 + (WM_USER+50)
   { FNTM_POINTSIZECHANGED,         "FNTM_POINTSIZECHANGED"         },  // 1033
   { FNTM_STYLECHANGED,             "FNTM_STYLECHANGED"             },  // 1034
   { FNTM_COLORCHANGED,             "FNTM_COLORCHANGED"             },  // 1035
   { FNTM_UPDATEPREVIEW,            "FNTM_UPDATEPREVIEW"            },  // 1036
   { FNTM_FILTERLIST,               "FNTM_FILTERLIST"               },  // 1037

   { BKM_CALCPAGERECT,              "BKM_CALCPAGERECT"              },  // 0353 + NOTE: These collide with the CM/VM messages above!
   { BKM_DELETEPAGE,                "BKM_DELETEPAGE"                },  // 0354
   { BKM_INSERTPAGE,                "BKM_INSERTPAGE"                },  // 0355
   { BKM_INVALIDATETABS,            "BKM_INVALIDATETABS"            },  // 0356
   { BKM_TURNTOPAGE,                "BKM_TURNTOPAGE"                },  // 0357
   { BKM_QUERYPAGECOUNT,            "BKM_QUERYPAGECOUNT"            },  // 0358
   { BKM_QUERYPAGEID,               "BKM_QUERYPAGEID"               },  // 0359
   { BKM_QUERYPAGEDATA,             "BKM_QUERYPAGEDATA"             },  // 035A
   { BKM_QUERYPAGEWINDOWHWND,       "BKM_QUERYPAGEWINDOWHWND"       },  // 035B
   { BKM_QUERYTABBITMAP,            "BKM_QUERYTABBITMAP"            },  // 035C
   { BKM_QUERYTABTEXT,              "BKM_QUERYTABTEXT"              },  // 035D
   { BKM_SETDIMENSIONS,             "BKM_SETDIMENSIONS"             },  // 035E
   { BKM_SETPAGEDATA,               "BKM_SETPAGEDATA"               },  // 035F
   { BKM_SETPAGEWINDOWHWND,         "BKM_SETPAGEWINDOWHWND"         },  // 0360
   { BKM_SETSTATUSLINETEXT,         "BKM_SETSTATUSLINETEXT"         },  // 0361
   { BKM_SETTABBITMAP,              "BKM_SETTABBITMAP"              },  // 0362
   { BKM_SETTABTEXT,                "BKM_SETTABTEXT"                },  // 0363
   { BKM_SETNOTEBOOKCOLORS,         "BKM_SETNOTEBOOKCOLORS"         },  // 0364
   { BKM_QUERYPAGESTYLE,            "BKM_QUERYPAGESTYLE"            },  // 0365
   { BKM_QUERYSTATUSLINETEXT,       "BKM_QUERYSTATUSLINETEXT"       },  // 0366
   { BKM_SETPAGEINFO,               "BKM_SETPAGEINFO"               },  // 0367
   { BKM_QUERYPAGEINFO,             "BKM_QUERYPAGEINFO"             },  // 0368
   { BKM_SETTABCOLOR,               "BKM_SETTABCOLOR"               },  // 0374
   { BKM_SETNOTEBOOKBUTTONS,        "BKM_SETNOTEBOOKBUTTONS"        },  // 0375

 #else

   { WM_NULL,                       "WM_NULL"                       },
   { WM_CREATE,                     "WM_CREATE"                     },
   { WM_DESTROY,                    "WM_DESTROY"                    },
   { WM_MOVE,                       "WM_MOVE"                       },
   { WM_SIZE,                       "WM_SIZE"                       },
   { WM_ACTIVATE,                   "WM_ACTIVATE"                   },
   { WM_SETFOCUS,                   "WM_SETFOCUS"                   },
   { WM_KILLFOCUS,                  "WM_KILLFOCUS"                  },
   { WM_ENABLE,                     "WM_ENABLE"                     },
   { WM_SETREDRAW,                  "WM_SETREDRAW"                  },
   { WM_SETTEXT,                    "WM_SETTEXT"                    },
   { WM_GETTEXT,                    "WM_GETTEXT"                    },
   { WM_GETTEXTLENGTH,              "WM_GETTEXTLENGTH"              },
   { WM_PAINT,                      "WM_PAINT"                      },
   { WM_CLOSE,                      "WM_CLOSE"                      },
   { WM_QUERYENDSESSION,            "WM_QUERYENDSESSION"            },
   { WM_QUIT,                       "WM_QUIT"                       },
   { WM_QUERYOPEN,                  "WM_QUERYOPEN"                  },
   { WM_ERASEBKGND,                 "WM_ERASEBKGND"                 },
   { WM_SYSCOLORCHANGE,             "WM_SYSCOLORCHANGE"             },
   { WM_ENDSESSION,                 "WM_ENDSESSION"                 },
   { WM_SHOWWINDOW,                 "WM_SHOWWINDOW"                 },
   { WM_WININICHANGE,               "WM_WININICHANGE"               },
   { WM_DEVMODECHANGE,              "WM_DEVMODECHANGE"              },
   { WM_ACTIVATEAPP,                "WM_ACTIVATEAPP"                },
   { WM_FONTCHANGE,                 "WM_FONTCHANGE"                 },
   { WM_TIMECHANGE,                 "WM_TIMECHANGE"                 },
   { WM_CANCELMODE,                 "WM_CANCELMODE"                 },
   { WM_SETCURSOR,                  "WM_SETCURSOR"                  },
   { WM_MOUSEACTIVATE,              "WM_MOUSEACTIVATE"              },
   { WM_CHILDACTIVATE,              "WM_CHILDACTIVATE"              },
   { WM_QUEUESYNC,                  "WM_QUEUESYNC"                  },
   { WM_GETMINMAXINFO,              "WM_GETMINMAXINFO"              },
   { WM_PAINTICON,                  "WM_PAINTICON"                  },
   { WM_ICONERASEBKGND,             "WM_ICONERASEBKGND"             },
   { WM_NEXTDLGCTL,                 "WM_NEXTDLGCTL"                 },
   { WM_SPOOLERSTATUS,              "WM_SPOOLERSTATUS"              },
   { WM_DRAWITEM,                   "WM_DRAWITEM"                   },
   { WM_MEASUREITEM,                "WM_MEASUREITEM"                },
   { WM_DELETEITEM,                 "WM_DELETEITEM"                 },
   { WM_VKEYTOITEM,                 "WM_VKEYTOITEM"                 },
   { WM_CHARTOITEM,                 "WM_CHARTOITEM"                 },
   { WM_SETFONT,                    "WM_SETFONT"                    },
   { WM_GETFONT,                    "WM_GETFONT"                    },
   { WM_SETHOTKEY,                  "WM_SETHOTKEY"                  },
   { WM_GETHOTKEY,                  "WM_GETHOTKEY"                  },
   { WM_QUERYDRAGICON,              "WM_QUERYDRAGICON"              },
   { WM_COMPAREITEM,                "WM_COMPAREITEM"                },
   { WM_COMPACTING,                 "WM_COMPACTING"                 },
   { WM_COMMNOTIFY,                 "WM_COMMNOTIFY"                 },
   { WM_WINDOWPOSCHANGING,          "WM_WINDOWPOSCHANGING"          },
   { WM_WINDOWPOSCHANGED,           "WM_WINDOWPOSCHANGED"           },
   { WM_POWER,                      "WM_POWER"                      },
   { WM_NCCREATE,                   "WM_NCCREATE"                   },
   { WM_NCDESTROY,                  "WM_NCDESTROY"                  },
   { WM_NCCALCSIZE,                 "WM_NCCALCSIZE"                 },
   { WM_NCHITTEST,                  "WM_NCHITTEST"                  },
   { WM_NCPAINT,                    "WM_NCPAINT"                    },
   { WM_NCACTIVATE,                 "WM_NCACTIVATE"                 },
   { WM_GETDLGCODE,                 "WM_GETDLGCODE"                 },
   { WM_NCMOUSEMOVE,                "WM_NCMOUSEMOVE"                },
   { WM_NCLBUTTONDOWN,              "WM_NCLBUTTONDOWN"              },
   { WM_NCLBUTTONUP,                "WM_NCLBUTTONUP"                },
   { WM_NCLBUTTONDBLCLK,            "WM_NCLBUTTONDBLCLK"            },
   { WM_NCRBUTTONDOWN,              "WM_NCRBUTTONDOWN"              },
   { WM_NCRBUTTONUP,                "WM_NCRBUTTONUP"                },
   { WM_NCRBUTTONDBLCLK,            "WM_NCRBUTTONDBLCLK"            },
   { WM_NCMBUTTONDOWN,              "WM_NCMBUTTONDOWN"              },
   { WM_NCMBUTTONUP,                "WM_NCMBUTTONUP"                },
   { WM_NCMBUTTONDBLCLK,            "WM_NCMBUTTONDBLCLK"            },
   { WM_KEYFIRST,                   "WM_KEYFIRST"                   },
   { WM_KEYDOWN,                    "WM_KEYDOWN"                    },
   { WM_KEYUP,                      "WM_KEYUP"                      },
   { WM_CHAR,                       "WM_CHAR"                       },
   { WM_DEADCHAR,                   "WM_DEADCHAR"                   },
   { WM_SYSKEYDOWN,                 "WM_SYSKEYDOWN"                 },
   { WM_SYSKEYUP,                   "WM_SYSKEYUP"                   },
   { WM_SYSCHAR,                    "WM_SYSCHAR"                    },
   { WM_SYSDEADCHAR,                "WM_SYSDEADCHAR"                },
   { WM_KEYLAST,                    "WM_KEYLAST"                    },
   { WM_INITDIALOG,                 "WM_INITDIALOG"                 },
   { WM_COMMAND,                    "WM_COMMAND"                    },
   { WM_SYSCOMMAND,                 "WM_SYSCOMMAND"                 },
   { WM_TIMER,                      "WM_TIMER"                      },
   { WM_HSCROLL,                    "WM_HSCROLL"                    },
   { WM_VSCROLL,                    "WM_VSCROLL"                    },
   { WM_INITMENU,                   "WM_INITMENU"                   },
   { WM_INITMENUPOPUP,              "WM_INITMENUPOPUP"              },
   { WM_MENUSELECT,                 "WM_MENUSELECT"                 },
   { WM_MENUCHAR,                   "WM_MENUCHAR"                   },
   { WM_ENTERIDLE,                  "WM_ENTERIDLE"                  },
   { WM_CTLCOLORMSGBOX,             "WM_CTLCOLORMSGBOX"             },
   { WM_CTLCOLOREDIT,               "WM_CTLCOLOREDIT"               },
   { WM_CTLCOLORLISTBOX,            "WM_CTLCOLORLISTBOX"            },
   { WM_CTLCOLORBTN,                "WM_CTLCOLORBTN"                },
   { WM_CTLCOLORDLG,                "WM_CTLCOLORDLG"                },
   { WM_CTLCOLORSCROLLBAR,          "WM_CTLCOLORSCROLLBAR"          },
   { WM_CTLCOLORSTATIC,             "WM_CTLCOLORSTATIC"             },
   { WM_MOUSEFIRST,                 "WM_MOUSEFIRST"                 },
   { WM_MOUSEMOVE,                  "WM_MOUSEMOVE"                  },
   { WM_LBUTTONDOWN,                "WM_LBUTTONDOWN"                },
   { WM_LBUTTONUP,                  "WM_LBUTTONUP"                  },
   { WM_LBUTTONDBLCLK,              "WM_LBUTTONDBLCLK"              },
   { WM_RBUTTONDOWN,                "WM_RBUTTONDOWN"                },
   { WM_RBUTTONUP,                  "WM_RBUTTONUP"                  },
   { WM_RBUTTONDBLCLK,              "WM_RBUTTONDBLCLK"              },
   { WM_MBUTTONDOWN,                "WM_MBUTTONDOWN"                },
   { WM_MBUTTONUP,                  "WM_MBUTTONUP"                  },
   { WM_MBUTTONDBLCLK,              "WM_MBUTTONDBLCLK"              },
   { WM_MOUSELAST,                  "WM_MOUSELAST"                  },
   { WM_PARENTNOTIFY,               "WM_PARENTNOTIFY"               },
   { WM_ENTERMENULOOP,              "WM_ENTERMENULOOP"              },
   { WM_EXITMENULOOP,               "WM_EXITMENULOOP"               },
   { WM_MDICREATE,                  "WM_MDICREATE"                  },
   { WM_MDIDESTROY,                 "WM_MDIDESTROY"                 },
   { WM_MDIACTIVATE,                "WM_MDIACTIVATE"                },
   { WM_MDIRESTORE,                 "WM_MDIRESTORE"                 },
   { WM_MDINEXT,                    "WM_MDINEXT"                    },
   { WM_MDIMAXIMIZE,                "WM_MDIMAXIMIZE"                },
   { WM_MDITILE,                    "WM_MDITILE"                    },
   { WM_MDICASCADE,                 "WM_MDICASCADE"                 },
   { WM_MDIICONARRANGE,             "WM_MDIICONARRANGE"             },
   { WM_MDIGETACTIVE,               "WM_MDIGETACTIVE"               },
   { WM_MDISETMENU,                 "WM_MDISETMENU"                 },
   { WM_DROPFILES,                  "WM_DROPFILES"                  },
   { WM_MDIREFRESHMENU,             "WM_MDIREFRESHMENU"             },
   { WM_CUT,                        "WM_CUT"                        },
   { WM_COPY,                       "WM_COPY"                       },
   { WM_PASTE,                      "WM_PASTE"                      },
   { WM_CLEAR,                      "WM_CLEAR"                      },
   { WM_UNDO,                       "WM_UNDO"                       },
   { WM_RENDERFORMAT,               "WM_RENDERFORMAT"               },
   { WM_RENDERALLFORMATS,           "WM_RENDERALLFORMATS"           },
   { WM_DESTROYCLIPBOARD,           "WM_DESTROYCLIPBOARD"           },
   { WM_DRAWCLIPBOARD,              "WM_DRAWCLIPBOARD"              },
   { WM_PAINTCLIPBOARD,             "WM_PAINTCLIPBOARD"             },
   { WM_VSCROLLCLIPBOARD,           "WM_VSCROLLCLIPBOARD"           },
   { WM_SIZECLIPBOARD,              "WM_SIZECLIPBOARD"              },
   { WM_ASKCBFORMATNAME,            "WM_ASKCBFORMATNAME"            },
   { WM_CHANGECBCHAIN,              "WM_CHANGECBCHAIN"              },
   { WM_HSCROLLCLIPBOARD,           "WM_HSCROLLCLIPBOARD"           },
   { WM_QUERYNEWPALETTE,            "WM_QUERYNEWPALETTE"            },
   { WM_PALETTEISCHANGING,          "WM_PALETTEISCHANGING"          },
   { WM_PALETTECHANGED,             "WM_PALETTECHANGED"             },
   { WM_HOTKEY,                     "WM_HOTKEY"                     },

 #endif
#endif // DEBUG

   { 0,                             0                               }
} ;

extern char *InterpretMsg ( MESG msg, char *Buffer ) {

   struct MessageTable *Entry = KnownMessages ;
   while ( Entry->Name ) {
      if ( Entry->Code == msg ) 
         break ;
      Entry ++ ;
   } /* endwhile */

   if ( Entry->Name ) 
      return ( Entry->Name ) ;

   sprintf ( Buffer, "%08X", msg ) ;
   return ( Buffer ) ;
}

extern void LogMsgRcvd ( int Indent, char *Prefix, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {
   char Name [80] ;
   Log ( "%*s%s: Message %s(%08X,%08X) received.", Indent, "", Prefix, InterpretMsg(msg,Name), mp1, mp2 ) ;
   if ( Indent > 100 ) {
      Log ( "%*s%s: Aborting (excessive recursion).", Indent, "", Prefix ) ;
      exit ( 1 ) ;
   } /* endif */
}

extern void LogMsgDone ( int Indent, char *Prefix, MESG msg, MRESULT Result ) {
   char Name [80] ;
   Log ( "%*s%s: Message %s done.  Result %08X.", Indent, "", Prefix, InterpretMsg(msg,Name), Result ) ;
}
