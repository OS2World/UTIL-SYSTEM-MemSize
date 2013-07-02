/**************************************************************** PROFILE.CPP
 *                                                                          *
 *                     Profile (INI) File Object Class                      *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#include <io.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "Country.h"
#include "Debug.h"
#include "Dialog.h"
#include "Config.h"
#include "MemSize.h"
#include "Profile.h"
#include "Support.h"

// #define DEBUG
 
/****************************************************************************
 *                                                                          *
 *      Profile Class Constructor - Don't ask if you can't find it.         *
 *                                                                          *
 ****************************************************************************/

Profile::Profile ( char *name, ANCHOR Anchor, char *HomePath, int MustExist ) : Ready ( FALSE ) {

  /**************************************************************************
   * Save the name.                                                         *
   **************************************************************************/

   Name = new char [ strlen(name) + 1 ] ;
   strcpy ( Name, name ) ;

  /**************************************************************************
   * Perform other initializations.                                         *
   **************************************************************************/

   #ifdef __OS2__
      Opened = FALSE ;
      Handle = 0 ;
   #else // __NT__
      ProfileName[0] = 0 ;
   #endif // __OS2__ vs __NT__

  /**************************************************************************
   * Query the system INI for the profile file's path.                      *
   **************************************************************************/

   Profile SystemProfile ( Name ) ;

   char *ProfilePath ( 0 ) ;

   if ( SystemProfile.GetString ( "INIPATH", ProfilePath ) ) {

      // Build the profile file name.
      char FullPath [_MAX_PATH] ;
      strcpy ( FullPath, ProfilePath ) ;
      strcat ( FullPath, "\\" ) ;
      strcat ( FullPath, Name ) ;
      strcat ( FullPath, ".INI" ) ;

      // Clean the name up and expand it to a full path.
      char Path [_MAX_PATH] ;
      _fullpath ( Path, FullPath, sizeof(Path) ) ;

      // Does the file exist?  If not, discard the name.
      if ( access ( Path, 0 ) ) {
         delete [] ProfilePath ;
         ProfilePath = 0 ;
      } /* endif */

   } /* endif */

  /**************************************************************************
   * If the profile file couldn't be found, use the home path.              *
   **************************************************************************/

   if ( ProfilePath == 0 ) {
      ProfilePath = new char [ strlen(HomePath) + 1 ] ;
      strcpy ( ProfilePath, HomePath ) ;
   } /* endif */

  /**************************************************************************
   * If the path could be determined . . .                                  *
   **************************************************************************/

   if ( ProfilePath ) {

     /***********************************************************************
      * Build the full profile file name.                                   *
      ***********************************************************************/

      strcpy ( ProfileName, ProfilePath ) ;
      strcat ( ProfileName, "\\"  ) ;
      strcat ( ProfileName, Name ) ;
      strcat ( ProfileName, ".INI" ) ;

     /***********************************************************************
      * Release the memory previously allocated to store the path.          *
      ***********************************************************************/

      if ( ProfilePath ) 
         delete [] ProfilePath ;

     /***********************************************************************
      * If file must exist previously, test for this now.                   *
      ***********************************************************************/

      if ( MustExist && access ( ProfileName, 0 ) )
         return ;

     /***********************************************************************
      * Open/Create the profile file and return the resultant handle.       *
      ***********************************************************************/

      #ifdef __OS2__

         Handle = PrfOpenProfile ( Anchor, PSZ(ProfileName) ) ;
         if ( Handle == 0 ) {
            char Message [512] ;
            Log ( "Profile::Profile: Could not open INI file.  %s", InterpretWinError(0,Message) ) ;
         } else {
            Opened = TRUE ;
            Ready = TRUE ;
            #ifdef DEBUG
               Log ( "Profile::Profile: Opened profile '%s'.", ProfileName ) ;
            #endif // DEBUG
         } /* endif */

      #else // __NT__

			Ready = !access ( ProfileName, 0 ) ;
         Anchor = 0 ; // Pacify the compiler.

      #endif // __OS2__ vs __NT__

   } /* endif */

} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *      Profile Class Constructor - Wrapper for system profile.             *
 *                                                                          *
 ****************************************************************************/

Profile::Profile ( char *name ) : Ready ( TRUE ) {

  /**************************************************************************
   * Save the name.                                                         *
   **************************************************************************/

   Name = new char [ strlen(name) + 1 ] ;
   strcpy ( Name, name ) ;

  /**************************************************************************
   * Set up which profile to use.                                           *
   **************************************************************************/

   #ifdef __OS2__
      Opened = FALSE ;
      Handle = HINI_USERPROFILE ;
      #ifdef DEBUG
         Log ( "Profile::Profile: Opened system user profile." ) ;
      #endif // DEBUG
   #else // __NT__
      ProfileName[0] = 0 ;
   #endif // __OS2__ vs __NT__

} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *      Profile Class Destructor                                            *
 *                                                                          *
 ****************************************************************************/

Profile::~Profile ( ) {

  /**************************************************************************
   * Release allocated memory.                                              *
   **************************************************************************/

   delete [] Name ;

  /**************************************************************************
   * Close the profile.                                                     *
   **************************************************************************/

   #ifdef __OS2__

      if ( Handle && Opened ) {
         if ( !PrfCloseProfile ( Handle ) ) {
            char Message [512] ;
            Log ( "Profile::~Profile: Could not close INI file.  %s", InterpretWinError(0,Message) ) ;
         } /* endif */
      } /* endif */

   #endif // __OS2__

  /**************************************************************************
   * Mark unready, just in case somebody tries to use the object anyway.    *
   **************************************************************************/

   Ready = FALSE ;

} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *      Profile: Get Item                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL Profile::GetItem ( char *ItemName, void *Item, int Size ) {

   if ( !IsReady() ) 
      return ( FALSE ) ;

   #ifdef __OS2__

      ULONG HowBig ;
      if ( !PrfQueryProfileSize ( Handle, PSZ(Name), PSZ(ItemName), &HowBig ) ) 
         return ( FALSE ) ;

      if ( Size != HowBig ) {
         Log ( "Profile::GetItem: Could not get INI item.  App %s, Item %s.  Wrong size.", Name, ItemName ) ;
         return ( FALSE ) ;
      } /* endif */

      if ( !PrfQueryProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, &HowBig ) ) {
         char Message [512] ;
         Log ( "Profile::GetItem: Could not get INI item.  App %s, Item %s, size %i.  %s", Name, ItemName, HowBig, InterpretWinError(0,Message) ) ;
         return ( FALSE ) ;
      } /* endif */

   #else // __NT__

      char *Buffer = new char [Size*2+2] ;

      int Returned ( 0 ) ;
      if ( ProfileName[0] ) {
         Returned = GetPrivateProfileString ( Name, ItemName, "", Buffer, Size*2+2, ProfileName ) ; 
      } else {
         Returned = GetProfileString ( Name, ItemName, "", Buffer, Size*2+2 ) ; 
      } /* endif */

      if ( Returned != Size*2 ) {
         delete [] Buffer ;
         Log ( "Profile::GetItem: Could not get INI item.  App %s, Item %s.  Wrong size.", Name, ItemName ) ;
         return ( FALSE ) ;
      } /* endif */

      for ( int i=0; i<Size; i++ ) {
         ((char*)Item)[i] = ( Buffer[i*2+0] > '9' ? Buffer[i*2+0]-'A'+10 : Buffer[i*2+0]-'0' ) << 4 ;
         ((char*)Item)[i] |= ( Buffer[i*2+1] > '9' ? Buffer[i*2+1]-'A'+10 : Buffer[i*2+1]-'0' ) ;
      } /* endfor */

      delete [] Buffer ;

   #endif // __OS2__ vs __NT__

   return ( TRUE ) ;

} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *      Profile: Set Item                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL Profile::PutItem ( char *ItemName, void *Item, int Size ) {

   if ( !IsReady() ) 
      return ( FALSE ) ;

   #ifdef __OS2__

      if ( !PrfWriteProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, Size ) ) {
         char Message [512] ;
         Log ( "Profile::PutItem: Could not put INI item.  App %s, Item %s, size %i.  %s", Name, ItemName, Size, InterpretWinError(0,Message) ) ;
         return ( FALSE ) ;
      } /* endif */

   #else // __NT__

      char *Buffer = new char [Size*2+1] ;
      if ( Buffer ) {
         for ( int i=0; i<Size; i++ ) 
            sprintf ( &Buffer[i*2], "%02X", ((unsigned char*)Item)[i] ) ;
         if ( ProfileName[0] ) {
            if ( !WritePrivateProfileString ( Name, ItemName, Buffer, ProfileName ) ) {
               char Message [512] ;
               Log ( "Profile::PutString: Could not put INI item.  App %s, Item %s, Value %s.  %s", Name, ItemName, Buffer, InterpretWinError(0,Message) ) ;
               return ( FALSE ) ;
            } /* endif */
         } else {
            if ( !WriteProfileString ( Name, ItemName, Buffer ) ) {
               char Message [512] ;
               Log ( "Profile::PutString: Could not put INI item.  App %s, Item %s, Value %s.  %s", Name, ItemName, Buffer, InterpretWinError(0,Message) ) ;
               return ( FALSE ) ;
            } /* endif */
         } /* endif */
         delete [] Buffer ;
      } else {
         Log ( "Profile::PutItem: Could not put INI item.  App %s, Item %s, size %i.  Insufficient memory.", Name, ItemName, Size ) ;
         return ( FALSE ) ;
      } /* endif */

   #endif // __OS2__ vs __NT__

   return ( TRUE ) ;

} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *      Profile: Get String                                                 *
 *                                                                          *
 ****************************************************************************/

BOOL Profile::GetString ( char *ItemName, char *Item, int MaxSize ) {

   if ( !IsReady() ) 
      return ( FALSE ) ;

   #ifdef __OS2__

      ULONG TrueSize ;
      if ( !PrfQueryProfileSize ( Handle, PSZ(Name), PSZ(ItemName), &TrueSize ) ) 
         return ( FALSE ) ;

      ULONG HowBig = min ( TrueSize+1, MaxSize ) ;
      if ( !PrfQueryProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, &HowBig ) ) {
         char Message [512] ;
         Log ( "Profile::GetString: Could not get INI item.  App %s, Item %s, TrueSize %i, MaxSize %i.  %s", Name, ItemName, TrueSize, MaxSize, InterpretWinError(0,Message) ) ;
         return ( FALSE ) ;
      } /* endif */

      Item[HowBig] = 0 ;

   #else // __NT__

      char *Buffer = new char [MaxSize+1] ;
      int Returned ( 0 ) ;
      if ( ProfileName[0] ) {
         Returned = GetPrivateProfileString ( Name, ItemName, "", Buffer, MaxSize+1, ProfileName ) ;
      } else {
         Returned = GetProfileString ( Name, ItemName, "", Buffer, MaxSize+1 ) ;
      } /* endif */

      if ( Returned >= MaxSize ) {
         delete [] Buffer ;
         Log ( "Profile::GetString: Could not get INI item.  App %s, Item %s.  Too large.", Name, ItemName ) ;
         return ( FALSE ) ;
      } /* endif */

      strcpy ( Item, Buffer ) ;

      delete [] Buffer ;

   #endif // __OS2__ vs __NT__

   return ( TRUE ) ;

} /* endmethod */
 
BOOL Profile::GetString ( char *ItemName, char* &Item ) {

   if ( !IsReady() ) {
      Item = 0 ;
      return ( FALSE ) ;
   } /* endif */

   #ifdef __OS2__

      ULONG TrueSize ;
      if ( !PrfQueryProfileSize ( Handle, PSZ(Name), PSZ(ItemName), &TrueSize ) ) {
         Item = 0 ;
         return ( FALSE ) ;
      } /* endif */

      Item = new char [TrueSize+1] ;
      if ( Item == 0 ) {
         Log ( "Profile::GetString: Unable to allocate memory to hold string.  App %s, Item %s, TrueSize %i.", Name, ItemName, TrueSize ) ;
         return ( FALSE ) ;
      } /* endif */

      if ( !PrfQueryProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, &TrueSize ) ) {
         char Message [512] ;
         Log ( "Profile::GetString: Could not get INI item.  App %s, Item %s, TrueSize %i.  %s", Name, ItemName, TrueSize, InterpretWinError(0,Message) ) ;
         delete [] Item ;
         Item = 0 ;
         return ( FALSE ) ;
      } /* endif */

      Item[TrueSize] = 0 ;

   #else // __NT__

      char *Buffer = new char [0x1000] ;
      int Returned ( 0 ) ;
      if ( ProfileName[0] ) {
         Returned = GetPrivateProfileString ( Name, ItemName, "", Buffer, 0x1000, ProfileName ) ;
      } else {
         Returned = GetProfileString ( Name, ItemName, "", Buffer, 0x1000 ) ;
      } /* endif */

      Item = new char [Returned+1] ;
      if ( Item == 0 ) {
         Log ( "Profile::GetString: Unable to allocate memory to hold string.  App %s, Item %s, TrueSize %i.", Name, ItemName, Returned ) ;
         return ( FALSE ) ;
      } /* endif */

      strcpy ( Item, Buffer ) ;

      delete [] Buffer ;

   #endif // __OS2__ vs __NT__

   return ( TRUE ) ;

} /* endmethod */
 
/****************************************************************************
 *                                                                          *
 *      Profile: Set Item                                                   *
 *                                                                          *
 ****************************************************************************/

BOOL Profile::PutString ( char *ItemName, char *Item ) {

   if ( !IsReady() ) 
      return ( FALSE ) ;

#ifdef __OS2__

   if ( !PrfWriteProfileData ( Handle, PSZ(Name), PSZ(ItemName), Item, strlen(Item)+1 ) ) {
      char Message [512] ;
      Log ( "Profile::PutString: Could not put INI item.  App %s, Item %s, Value %s.  %s", Name, ItemName, Item, InterpretWinError(0,Message) ) ;
      return ( FALSE ) ;
   } /* endif */

#else // __NT__

   if ( ProfileName[0] ) {
      if ( !WritePrivateProfileString ( Name, ItemName, Item, ProfileName ) ) {
         char Message [512] ;
         Log ( "Profile::PutString: Could not put INI item.  App %s, Item %s, Value %s.  %s", Name, ItemName, Item, InterpretWinError(0,Message) ) ;
         return ( FALSE ) ;
      } /* endif */
   } else {
      if ( !WriteProfileString ( Name, ItemName, Item ) ) {
         char Message [512] ;
         Log ( "Profile::PutString: Could not put INI item.  App %s, Item %s, Value %s.  %s", Name, ItemName, Item, InterpretWinError(0,Message) ) ;
         return ( FALSE ) ;
      } /* endif */
   } /* endif */

#endif // __OS2__ vs __NT__

   return ( TRUE ) ;

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *      Profile: Resets                                                     *
 *                                                                          *
 ****************************************************************************/

void Profile::Reset ( char *ItemName ) { 
   #ifdef __OS2__
      PrfWriteProfileData ( Handle, PSZ(Name), PSZ(ItemName), 0, 0 ) ; 
   #else // __NT__
      if ( ProfileName[0] ) {
         WritePrivateProfileString ( Name, ItemName, 0, ProfileName ) ;
      } else {
         WriteProfileString ( Name, ItemName, 0 ) ;
      } /* endif */
   #endif // __OS2__ vs __NT__
} /* endmethod */

void Profile::Reset ( ) { 
   #ifdef __OS2__
      PrfWriteProfileData ( Handle, PSZ(Name), 0, 0, 0 ) ; 
   #else // __NT__
      if ( ProfileName[0] ) {
         WritePrivateProfileString ( Name, 0, 0, ProfileName ) ;
      } else {
         WriteProfileString ( Name, 0, 0 ) ;
      } /* endif */
   #endif // __OS2__ vs __NT__
} /* endmethod */

/****************************************************************************
 *                                                                          *
 *      Profile: Copy All Items                                             *
 *                                                                          *
 ****************************************************************************/

void Profile::Copy ( Profile &Source ) {

   #ifdef __OS2__

      // If either profile isn't really open, return w/o doing anything at all.
      if ( !Handle || !Source.IsReady() ) {
         Log ( "Profile::Copy: Either the source or the target handle is invalid." ) ;
         return ;
      } /* endif */

      // Find out how much memory is needed to get the entire list of keys for this application.
      ULONG KeyListSize ;
      if ( !PrfQueryProfileSize ( Source.Handle, Source.QueryName(), 0, &KeyListSize ) ) {
         ERRORID Error = Sys_GetLastError ( 0 ) ;
         Log ( "Profile::Copy: Unable to query for the list of defined keys.  Status %08X.", Error ) ;
         return ;
      } /* endif */

      // Allocate memory to hold the key list.
      char *KeyList = new char [ ++KeyListSize ] ;
      if ( !KeyList ) {
         Log ( "Profile::Copy: Unable to allocate memory for the key list." ) ;
         return ;
      } /* endif */

      // Fetch the list of keys.
      if ( !PrfQueryProfileData ( Source.Handle, Source.QueryName(), 0, KeyList, &KeyListSize ) ) {
         ERRORID Error = Sys_GetLastError ( 0 ) ;
         Log ( "Profile::Copy: Unable to query for the key list.  Status %08X.", Error ) ;
         delete [] KeyList ;
         return ;
      } /* endif */

      // Scan the key list, copying the data to the new profile.
      char *Key = KeyList ;
      while ( *Key ) {
         ULONG ItemSize ;
         if ( !PrfQueryProfileSize ( Source.Handle, Source.QueryName(), Key, &ItemSize ) ) 
            break ;
         char *Item = new char [ ItemSize ] ;
         if ( !Item ) 
            break ;
         if ( !PrfQueryProfileData ( Source.Handle, Source.QueryName(), Key, Item, &ItemSize ) )  {
            delete [] Item ;
            break ;
         } /* endif */
         if ( !PrfWriteProfileData ( Handle, Name, Key, Item, ItemSize ) ) {
            delete [] Item ;
            break ;
         } /* endif */
         Key += strlen(Key) + 1 ;
      } /* endwhile */
  
      // Release the memory for the key list.
      delete [] KeyList ;

   #else // __NT__

      // We're not going to implement this one for now, as the only reason for it was
      //   to convert from MemSize 3.31's INI file to MemSize 4.00's file, and the Win32
      //   version had no version before 4.00.
      Source.QueryName ( ) ;

   #endif // __OS2__ vs __NT__

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *      Profile: Phase Out (make obsolete)                                  *
 *                                                                          *
 ****************************************************************************/

void Profile::PhaseOut ( ) {

   #ifdef __OS2__

      if ( Handle && Opened ) {
         Log ( "Profile::PhaseOut: Phasing out '%s'.", ProfileName ) ;
         if ( !PrfCloseProfile ( Handle ) ) {
            char Message [512] ;
            Log ( "Profile::PhaseOut: Could not close INI file.  %s", InterpretWinError(0,Message) ) ;
         } /* endif */
         Handle = 0 ;
         Opened = FALSE ;
         char Drive[_MAX_DRIVE+1]; char Dir[_MAX_DIR+1]; char FName[_MAX_FNAME+1]; char Ext[_MAX_EXT+1] ;
         _splitpath ( ProfileName, Drive, Dir, FName, Ext ) ;
         char NewName [_MAX_PATH] ;
         sprintf ( NewName, "%s%s%s.BAK", Drive, Dir, FName ) ;
         if ( !access ( NewName, 0 ) ) 
            if ( remove ( NewName ) )
               Log ( "Profile::PhaseOut: Could not remove '%s'.", NewName ) ;
         if ( rename ( ProfileName, NewName ) ) 
            Log ( "Profile::PhaseOut: Could not rename '%s' to '%s'.", ProfileName, NewName ) ;
      } /* endif */

   #else // __NT__

      // We're not going to implement this one for now, as the only reason for it was
      //   to convert from MemSize 3.31's INI file to MemSize 4.00's file, and the Win32
      //   version had no version before 4.00.

   #endif // __OS2__ vs __NT__

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *                        Get Basic INI Information                         *
 *                                                                          *
 ****************************************************************************/

#define OFF(type, var)    offsetof(type,var)                    /* ((int) &((*(type *) 0).var)) */ 
#define SIZE(type, var)   sizeof((*(type *) 0).var)

extern PROFILEITEM ProfileItems [] = {
   { "AnchorCorner",        OFF(INIDATA,AnchorCorner),          SIZE(INIDATA,AnchorCorner)          },
   { "fAnchorCorner",       OFF(INIDATA,AnchorCorner),          SIZE(INIDATA,fAnchorCorner)         },
   { "Animate",             OFF(INIDATA,Animate),               SIZE(INIDATA,Animate)               },
   { "fAnimate",            OFF(INIDATA,fAnimate),              SIZE(INIDATA,fAnimate)              },
   { "BackgroundColor",     OFF(INIDATA,BackColor),             SIZE(INIDATA,BackColor)             },
   { "fBackgroundColor",    OFF(INIDATA,fBackColor),            SIZE(INIDATA,fBackColor)            },
   { "CenterClock",         OFF(INIDATA,CenterClock),           SIZE(INIDATA,CenterClock)           },
   { "fCenterClock",        OFF(INIDATA,fCenterClock),          SIZE(INIDATA,fCenterClock)          },
   { "Chime",               OFF(INIDATA,Chime),                 SIZE(INIDATA,Chime)                 },
   { "fChime",              OFF(INIDATA,fChime),                SIZE(INIDATA,fChime)                },
   { "DateFormat",          OFF(INIDATA,DateFormat),            SIZE(INIDATA,DateFormat)            },
   { "DialogBorder",        OFF(INIDATA,DialogBorder),          SIZE(INIDATA,DialogBorder)          },
   { "fDialogBorder",       OFF(INIDATA,fDialogBorder),         SIZE(INIDATA,fDialogBorder)         },
   { "ErrorBackground",     OFF(INIDATA,ErrorBackground),       SIZE(INIDATA,ErrorBackground)       },
   { "fErrorBackground",    OFF(INIDATA,fErrorBackground),      SIZE(INIDATA,fErrorBackground)      },
   { "ErrorForeground",     OFF(INIDATA,ErrorForeground),       SIZE(INIDATA,ErrorForeground)       },
   { "fErrorForeground",    OFF(INIDATA,fErrorForeground),      SIZE(INIDATA,fErrorForeground)      },
   { "Float",               OFF(INIDATA,Float),                 SIZE(INIDATA,Float)                 },
   { "fFloat",              OFF(INIDATA,fFloat),                SIZE(INIDATA,fFloat)                },
   { "FontNameSize",        OFF(INIDATA,FontNameSize),          SIZE(INIDATA,FontNameSize)          },
   { "fFontNameSize",       OFF(INIDATA,fFontNameSize),         SIZE(INIDATA,fFontNameSize)         },
   { "ForegroundColor",     OFF(INIDATA,TextColor),             SIZE(INIDATA,TextColor)             },
   { "fForegroundColor",    OFF(INIDATA,fTextColor),            SIZE(INIDATA,fTextColor)            },
   { "HideControls",        OFF(INIDATA,HideControls),          SIZE(INIDATA,HideControls)          },
   { "fHideControls",       OFF(INIDATA,fHideControls),         SIZE(INIDATA,fHideControls)         },
   { "MonitorPriority",     OFF(INIDATA,MonitorPriority),       SIZE(INIDATA,MonitorPriority)       },
   { "fMonitorPriority",    OFF(INIDATA,fMonitorPriority),      SIZE(INIDATA,fMonitorPriority)      },
   { "PopServer",           OFF(INIDATA,PopServer),             SIZE(INIDATA,PopServer)             },
   { "PopUserName",         OFF(INIDATA,PopUserName),           SIZE(INIDATA,PopUserName)           },
   { "PopPassword",         OFF(INIDATA,PopPassword),           SIZE(INIDATA,PopPassword)           },
   { "PopPort",             OFF(INIDATA,PopPort),               SIZE(INIDATA,PopPort)               },
   { "PopForceIP",          OFF(INIDATA,PopForceIP),            SIZE(INIDATA,PopForceIP)            },
   { "PopUseAPOP",          OFF(INIDATA,PopUseAPOP),            SIZE(INIDATA,PopUseAPOP)            },
   { "PopInterval",         OFF(INIDATA,PopInterval),           SIZE(INIDATA,PopInterval)           },
   { "PopReader",           OFF(INIDATA,PopReader),             SIZE(INIDATA,PopReader)             },
   { "PopAlert",            OFF(INIDATA,PopAlert),              SIZE(INIDATA,PopAlert)              },
   { "PopWavFile",          OFF(INIDATA,PopWavFile),            SIZE(INIDATA,PopWavFile)            },
   { "ShowDiskDrives",      OFF(INIDATA,ShowDiskDrives),        SIZE(INIDATA,ShowDiskDrives)        },
   { "fShowDiskDrives",     OFF(INIDATA,fShowDiskDrives),       SIZE(INIDATA,fShowDiskDrives)       },
   { "ShowDiskLabels",      OFF(INIDATA,ShowDiskLabels),        SIZE(INIDATA,ShowDiskLabels),       },
   { "fShowDiskLabels",     OFF(INIDATA,fShowDiskLabels),       SIZE(INIDATA,fShowDiskLabels),      },
   { "ShowFileSystemNames", OFF(INIDATA,ShowFileSystemNames),   SIZE(INIDATA,ShowFileSystemNames)   },
   { "fShowFileSystemNames",OFF(INIDATA,fShowFileSystemNames),  SIZE(INIDATA,fShowFileSystemNames)  },
   { "ShowRemoteDrives",    OFF(INIDATA,ShowRemoteDrives),      SIZE(INIDATA,ShowRemoteDrives)      },
   { "fShowRemoteDrives",   OFF(INIDATA,fShowRemoteDrives),     SIZE(INIDATA,fShowRemoteDrives)     },
   { "ShowSeconds",         OFF(INIDATA,ShowSeconds),           SIZE(INIDATA,ShowSeconds)           },
   { "fShowSeconds",        OFF(INIDATA,fShowSeconds),          SIZE(INIDATA,fShowSeconds)          },
   { "UseTheseus",          OFF(INIDATA,UseTheseus),            SIZE(INIDATA,UseTheseus)            },
   { "fUseTheseus",         OFF(INIDATA,fUseTheseus),           SIZE(INIDATA,fUseTheseus)           },
   { "ShowG",               OFF(INIDATA,ShowG),                 SIZE(INIDATA,ShowG)                 },
   { "fShowG",              OFF(INIDATA,fShowG),                SIZE(INIDATA,fShowG)                },
   { "ShowK",               OFF(INIDATA,ShowK),                 SIZE(INIDATA,ShowK)                 },
   { "fShowK",              OFF(INIDATA,fShowK),                SIZE(INIDATA,fShowK)                },
   { "ShowM",               OFF(INIDATA,ShowM),                 SIZE(INIDATA,ShowM)                 },
   { "fShowM",              OFF(INIDATA,fShowM),                SIZE(INIDATA,fShowM)                },
   { "ShowTrueK",           OFF(INIDATA,ShowTrueK),             SIZE(INIDATA,ShowTrueK)             },
   { "fShowTrueK",          OFF(INIDATA,fShowTrueK),            SIZE(INIDATA,fShowTrueK)            },
   { "TableFormat",         OFF(INIDATA,TableFormat),           SIZE(INIDATA,TableFormat)           },
   { "fTableFormat",        OFF(INIDATA,fTableFormat),          SIZE(INIDATA,fTableFormat)          },
   { "TimerInterval",       OFF(INIDATA,TimerInterval),         SIZE(INIDATA,TimerInterval)         },
   { "fTimerInterval",      OFF(INIDATA,fTimerInterval),        SIZE(INIDATA,fTimerInterval)        },
   { "AveragingInterval",   OFF(INIDATA,AveragingInterval),     SIZE(INIDATA,AveragingInterval)     },
   { "fAveragingInterval",  OFF(INIDATA,fAveragingInterval),    SIZE(INIDATA,fAveragingInterval)    },
   { "WarningBackground",   OFF(INIDATA,WarningBackground),     SIZE(INIDATA,WarningBackground)     },
   { "fWarningBackground",  OFF(INIDATA,fWarningBackground),    SIZE(INIDATA,fWarningBackground)    },
   { "WarningForeground",   OFF(INIDATA,WarningForeground),     SIZE(INIDATA,WarningForeground)     },
   { "fWarningForeground",  OFF(INIDATA,fWarningForeground),    SIZE(INIDATA,fWarningForeground)    },
} ;

extern int ProfileItemCount ( sizeof(ProfileItems) / sizeof(ProfileItems[0]) ) ;

int Profile::GetIniData ( PINIDATA IniData ) {

 /***************************************************************************
  * Get the window's current size and position.                             *
  ***************************************************************************/

  memset ( &IniData->Position, 0, sizeof(IniData->Position) ) ;
  IniData->fPosition = FALSE ;
  if ( GetItem ( "Position", &IniData->Position, sizeof(IniData->Position) ) ) 
     IniData->fPosition = TRUE ;

  if ( !IniData->fPosition ) {
     Reset ( "Position" ) ;
     Reset ( "AnchorCorner" ) ;
     Reset ( "fAnchorCorner" ) ;
  } /* endif */
  
 /***************************************************************************
  * If unable to get position, and profile is not the preferred one, then   *
  *   return TRUE, indicating profile fetch error.                          *
  ***************************************************************************/

  if ( NOT IniData->fPosition ) {
     if ( ( ProfileName[0] == 0 ) || strcmp ( Name, PROGRAM_NAME ) ) {
        #ifdef DEBUG
           Log ( "Profile::GetIniData: No position obtained with non-default INI handle/name." ) ;
        #endif // DEBUG
        return ( 1 ) ;
     } /* endif */
     #ifdef DEBUG
        Log ( "Profile::GetIniData: No position obtained with handle %08X.", Handle ) ;
     #endif // DEBUG
  } /* endif */

 /***************************************************************************
  * Set the default profile.                                                *
  ***************************************************************************/

  IniData->Reset ( ) ;

 /***************************************************************************
  * Go get the saved values for the above, if they are to be found.         *
  ***************************************************************************/

  for ( int i=0; i<sizeof(ProfileItems)/sizeof(ProfileItems[0]); i++ ) 
     GetItem ( ProfileItems[i].Name, PCHAR(IniData)+ProfileItems[i].Offset, ProfileItems[i].Size ) ;

  if ( IniData->FontNameSize[0] )
     IniData->fFontNameSize = TRUE ;

  return ( 0 ) ;
}

