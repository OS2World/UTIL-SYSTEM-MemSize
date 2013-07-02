/****************************************************************** MEMSIZE.H
 *                                                                          *
 *         Display memory size statistics, refreshed every second.          *
 *                                                                          *
 ****************************************************************************/

#ifndef MEMSIZE_H
#define MEMSIZE_H

// #define USE_OS20MEMU         // Disable this for now, until the bug is resolved.

#ifdef __OS2__
   #define PROGRAM_NAME "MEMSIZE2"
#else
   #define PROGRAM_NAME "MEMSIZEW"
#endif

#define CLASS_NAME   PROGRAM_NAME
#define LOGFILE      PROGRAM_NAME ".LOG"

extern char HomePath [] ;
extern HMODULE LibraryHandle ;

#ifdef INCLUDE_SYSTEM // WM_USER is considered a system item under Windows.
   enum {
      WM_REFRESH = WM_USER,
      WM_UPDATEPARMS,
      WM_UPDATEDRIVES
   } ;   
#endif // INCLUDE_SYSTEM

#define ID_MAIN                            1

#define IDM_MENU                           10
#define IDM_RESET_DEFAULTS                 11
#define IDM_HIDE_CONTROLS                  12
#define IDM_CONFIGURE                      13
#define   IDD_CONFIGURE                    130
#define     IDD_CONFIG_NOTEBOOK            1300
#define       IDD_CONFIG_PAGE_ITEMS        13000
#define         IDD_CONFIG_ITEMS           13001
#define       IDD_CONFIG_PAGE_OPTIONS1     13100
#define         IDD_CONFIG_HIDECONTROLS    13101
#define         IDD_CONFIG_FLOAT           13102
#define         IDD_CONFIG_ANIMATE         13103
#define         IDD_CONFIG_DIALOGBORDER    13104
#define         IDD_CONFIG_TABLE           13105
#define         IDD_CONFIG_CHIME           13106
#define         IDD_CONFIG_SHOWDRIVES      13107
#define         IDD_CONFIG_SHOWREMOTES     13108
#define         IDD_CONFIG_FSNAME          13109
#define         IDD_CONFIG_DLABEL          13110
#define         IDD_CONFIG_SECONDS         13111
#define         IDD_CONFIG_THESEUS         13112
#define         IDD_CONFIG_CENTER_CLOCK    13113
#define       IDD_CONFIG_PAGE_OPTIONS2     13200
#define         IDD_CONFIG_SHOWK_NEVER     13201
#define         IDD_CONFIG_SHOWK_ABOVE512  13202
#define         IDD_CONFIG_SHOWK_ALWAYS    13203
#define         IDD_CONFIG_SHOW_TRUEK      13204
#define         IDD_CONFIG_SHOW_MEGABYTES  13205
#define         IDD_CONFIG_SHOW_GIGABYTES  13206
#define         IDD_CONFIG_PRIORITY        13207
#define         IDD_CONFIG_TIMER           13208
#define         IDD_CONFIG_AVERAGE         13209
#define       IDD_CONFIG_PAGE_COLORS       13300
#define         IDD_CONFIG_NORMAL          13301
#define         IDD_CONFIG_WARNING         13302
#define         IDD_CONFIG_ERROR           13303
#define         IDD_CONFIG_FONT            13304
#define       IDD_CONFIG_PAGE_ANCHOR       13400
#define         IDD_CONFIG_BL              13401
#define         IDD_CONFIG_BR              13402
#define         IDD_CONFIG_TL              13403
#define         IDD_CONFIG_TR              13404
#define       IDD_CONFIG_PAGE_DATEFMT      13500
#define         IDD_CONFIG_DATEFMT         13501
#define         IDD_CONFIG_DATESMP         13502
#define         IDD_CONFIG_DATECODES       13503
#define       IDD_CONFIG_PAGE_POP31        13600
#define         IDD_CONFIG_POPSERV         13601
#define         IDD_CONFIG_POPUSER         13602
#define         IDD_CONFIG_POPPSWD         13603
#define         IDD_CONFIG_POPPORT         13604
#define         IDD_CONFIG_POPFORC         13605
#define         IDD_CONFIG_POPAPOP         13606
#define         IDD_CONFIG_POPTEST         13607
#define         IDD_CONFIG_POPACPT         13608
#define         IDD_CONFIG_POPREST         13609
#define         IDD_CONFIG_POPSTAT         13600
#define       IDD_CONFIG_PAGE_POP32        13700
#define         IDD_CONFIG_POPPOLL         13701
#define         IDD_CONFIG_POPREADER       13702
#define         IDD_CONFIG_POPALERT        13703
#define         IDD_CONFIG_POPWAVTEXT      13704
#define         IDD_CONFIG_POPWAVPLAY      13705
#define         IDD_CONFIG_POPWAVFILE      13706
#define   IDD_LABEL                        138 
#define     IDD_LABEL_DEFLABEL             1380
#define     IDD_LABEL_NEWLABEL             1381
#define     IDD_LABEL_DEFAULT              1382
#define     IDD_LABEL_ENABLE               1383
#define     IDD_LABEL_WARNING_VALUE        1384
#define     IDD_LABEL_WARNING_SLIDER       1385
#define     IDD_LABEL_ERROR_VALUE          1386
#define     IDD_LABEL_ERROR_SLIDER         1387
#define IDM_RESETLOAD                      14
#define IDM_RESETAVERAGE                   15
#define IDM_RESETDRIVES                    16
#define IDM_LAUNCHEMAILER                  17
#define IDM_COPY                           18
#define IDM_ABOUT                          19
#define   IDD_ABOUT                        190
#define IDM_HELP                           20

#define IDM_EXIT                           98
#define IDM_KEYS_HELP                      99

#define IDD_PROFILE_PATH                   9900
#define   IDD_PROF_ENTRY                   9900
#define   IDD_PROF_ERR                     9901

#define IDS_HELPTITLE                      0x0000
#define IDS_TITLE                          0x0001

#define IDS_RESET_DEFAULTS                 0x0002
#define IDS_HIDE_CONTROLS                  0x0003
#define IDS_CONFIGURE                      0x0004
#define IDS_RESETLOAD                      0x0005
#define IDS_RESETAVERAGE                   0x0006
#define IDS_RESETDRIVES                    0x0007
#define IDS_LAUNCHEMAILER                  0x0008
#define IDS_COPY                           0x0009
#define IDS_ABOUT                          0x000A
#define IDS_HELP                           0x000B

#define IDS_CLOCK                          0x000C
#define IDS_ELAPSED                        0x000D
#define IDS_MESSAGECOUNT                   0x000E
#define IDS_MEMORY                         0x000F
#define IDS_VIRTUAL                        0x0010
#define IDS_SWAPSIZE                       0x0011
#define IDS_SWAPFREE                       0x0012
#define IDS_SWAPSLACK                      0x0013
#define IDS_SPOOLSIZE                      0x0014
#define IDS_CPULOAD                        0x0015
#define IDS_AVERAGELOAD                    0x0016
#define IDS_BATTERY                        0x0017
#define IDS_TASKCOUNT                      0x0018
#define IDS_PROCESSCOUNT                   0x0019
#define IDS_THREADCOUNT                    0x001A
#define IDS_TOTALFREE                      0x001B
#define IDS_DRIVE_FREE                     0x001C
                                                 
#define IDS_DRIVEERROR                     0x001D

#define IDS_PARAMETERLIST                  0x001E
#define IDS_PARMS_RESET                    0x001F
#define IDS_PARMS_EXCLUDE                  0x0020
                                                 
#define IDS_ERROR_PRFOPENPROFILE           0x0021
#define IDS_ERROR_CREATEHELP               0x0022
#define IDS_ERROR_CREATEFRAME              0x0023
#define IDS_ERROR_WINREGISTERCLASS         0x0024
#define IDS_ERROR_CREATECLIENT             0x0025
#define IDS_ERROR_DOSGETCTRYINFO           0x0026
#define IDS_ERROR_BADCODEPAGE              0x0027
                                                 
#define IDS_DAY                            0x0028
#define IDS_DAYS                           0x0029

#define IDS_DAYSOFWEEK                     0x002A
#define IDS_DATEFMT_DAY0                   0x002B
#define IDS_DATEFMT_DAY1                   0x002C
#define IDS_DATEFMT_DAY2                   0x002D
#define IDS_DATEFMT_DAY3                   0x002E
#define IDS_DATEFMT_DAY4                   0x002F
#define IDS_DATEFMT_DAY5                   0x0030
#define IDS_DATEFMT_DAY6                   0x0031
                                                 
#define IDS_DATEFMT_MONS                   0x0032
#define IDS_DATEFMT_MONTH1                 0x0033
#define IDS_DATEFMT_MONTH2                 0x0034
#define IDS_DATEFMT_MONTH3                 0x0035
#define IDS_DATEFMT_MONTH4                 0x0036
#define IDS_DATEFMT_MONTH5                 0x0037
#define IDS_DATEFMT_MONTH6                 0x0038
#define IDS_DATEFMT_MONTH7                 0x0039
#define IDS_DATEFMT_MONTH8                 0x003A
#define IDS_DATEFMT_MONTH9                 0x003B
#define IDS_DATEFMT_MONTH10                0x003C
#define IDS_DATEFMT_MONTH11                0x003D
#define IDS_DATEFMT_MONTH12                0x003E
                                                 
#define IDS_APM_ERROR                      0x003F
#define IDS_APM_AC                         0x0040
#define IDS_APM_CHARGING                   0x0041
                                                 
#define IDS_CONFIG_PAGE_ITEMS              0x0042
#define IDS_CONFIG_PAGE_OPTIONS            0x0043
#define IDS_CONFIG_PAGE_COLORS             0x0044
#define IDS_CONFIG_PAGE_ANCHOR             0x0045
#define IDS_CONFIG_PAGE_DATEFMT            0x0046
#define IDS_CONFIG_PAGE_POP3               0x0047
                                                 
#define IDS_PAGE                           0x0048
#define IDS_OF                             0x0049
#define IDS_CONFIG_FONT_TEXT               0x004A

#define IDS_POP_NOSERVER                   0x004B
#define IDS_POP_NOUSERNAME                 0x004C
#define IDS_POP_NOPASSWORD                 0x004D
#define IDS_POP_HASMESSAGES                0x004E
#define IDS_POP_HASONEMESSAGE              0x004F
#define IDS_POP_HASNOMESSAGES              0x0050
#define IDS_POP_ERROR1                     0x0051
#define IDS_POP_ERROR2                     0x0052
#define IDS_POP_ERROR3                     0x0053
#define IDS_POP_ERROR4                     0x0054
#define IDS_POP_ERROR5                     0x0055
#define IDS_POP_ERROR6                     0x0056
#define IDS_POP_ERROR7                     0x0057
#define IDS_POP_ERRORX                     0x0058
#define IDS_POP_TITLE                      0x0059
#define IDS_POP_OVERRIDE                   0x005A

#define IDS_EXCEPTION                      0x005B

#define IDS_DATECODES_00                   0x1000
#define IDS_DATECODES_01                   0x1001
#define IDS_DATECODES_02                   0x1002
#define IDS_DATECODES_03                   0x1003
#define IDS_DATECODES_04                   0x1004
#define IDS_DATECODES_05                   0x1005
#define IDS_DATECODES_06                   0x1006
#define IDS_DATECODES_07                   0x1007
#define IDS_DATECODES_08                   0x1008
#define IDS_DATECODES_09                   0x1009
#define IDS_DATECODES_10                   0x100A
#define IDS_DATECODES_11                   0x100B
#define IDS_DATECODES_12                   0x100C
#define IDS_DATECODES_13                   0x100D
#define IDS_DATECODES_14                   0x100E 
#define IDS_DATECODES_15                   0x100F
#define IDS_DATECODES_16                   0x1010
#define IDS_DATECODES_17                   0x1011
#define IDS_DATECODES_18                   0x1012
#define IDS_DATECODES_19                   0x1013
#define IDS_DATECODES_20                   0x1014
#define IDS_DATECODES_21                   0x1015
#define IDS_DATECODES_22                   0x1016 
#define IDS_DATECODES_23                   0x1017
#define IDS_DATECODES_24                   0x1018
#define IDS_DATECODES_25                   0x1019
#define IDS_DATECODES_COUNT                ( IDS_DATECODES_25 - IDS_DATECODES_00 + 1 )

#define IDS_HMERR                          0x8000
#define IDS_HMERR_EXTHELPUNDEFINED         0x8001
#define IDS_HMERR_NO_FRAME_WND_IN_CHAIN    0x8002
#define IDS_HMERR_INVALID_ASSOC_APP_WND    0x8003
#define IDS_HMERR_INVALID_ASSOC_HELP_IN    0x8004
#define IDS_HMERR_INVALID_DESTROY_HELP_    0x8005
#define IDS_HMERR_NO_HELP_INST_IN_CHAIN    0x8006
#define IDS_HMERR_INVALID_HELP_INSTANCE    0x8007
#define IDS_HMERR_INVALID_QUERY_APP_WND    0x8008
#define IDS_HMERR_HELP_INST_CALLED_INVA    0x8009
#define IDS_HMERR_HELPTABLE_UNDEFINE       0x800A
#define IDS_HMERR_HELP_INSTANCE_UNDEFIN    0x800B
#define IDS_HMERR_HELPITEM_NOT_FOUND       0x800C
#define IDS_HMERR_INVALID_HELPSUBITEM_S    0x800D
#define IDS_HMERR_HELPSUBITEM_NOT_FOUND    0x800E
#define IDS_HMERR_INDEX_NOT_FOUND          0x800F
#define IDS_HMERR_CONTENT_NOT_FOUND        0x8010
#define IDS_HMERR_OPEN_LIB_FILE            0x8011
#define IDS_HMERR_READ_LIB_FILE            0x8012
#define IDS_HMERR_CLOSE_LIB_FILE           0x8013
#define IDS_HMERR_INVALID_LIB_FILE         0x8014
#define IDS_HMERR_NO_MEMORY                0x8015
#define IDS_HMERR_ALLOCATE_SEGMENT         0x8016
#define IDS_HMERR_FREE_MEMORY              0x8017
#define IDS_HMERR_PANEL_NOT_FOUND          0x8018
#define IDS_HMERR_DATABASE_NOT_OPEN        0x8019
#define IDS_HMERR_UNKNOWN                  0x801A
#define IDS_HELPMODE_FRAME                 0x801B
#define IDS_HELPMODE_MENU                  0x801C
#define IDS_HELPMODE_WINDOW                0x801D
#define IDS_HELPMODE_UNKNOWN               0x801E
#define IDS_HELPSUBITEMNOTFOUND            0x801F

#endif
