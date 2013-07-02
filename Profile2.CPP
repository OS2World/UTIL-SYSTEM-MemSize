/*************************************************************** PROFILE2.CPP
 *                                                                          *
 *                        Profile Object (for GUI)                          *
 *                                                                          *
 ****************************************************************************/

#include "System.h"

#include <io.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "Config.h"
#include "Debug.h"
#include "Dialog.h"
#include "Profile2.h"
#include "MemSize.h"
#include "Support.h"

// #define DEBUG
 
/****************************************************************************
 *                                                                          *
 *                     Definitions & Declarations                           *
 *                                                                          *
 ****************************************************************************/

  // Constants

enum { ENTRY, ERRCODE } ;


/****************************************************************************
 *                                                                          *
 *  Class: Profile Path Dialog                                              *
 *                                                                          *
 ****************************************************************************/

class ProfilePathDialog : public Dialog {

   private:
      int DialogID ;
      HelpWindow *Help ;
      char *Path ;
      int PathSize ;

   public:
      ProfilePathDialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, HelpWindow *Help, char *Path, int PathSize ) ;

      MRESULT InitDialog ( HWND Window ) ;
      MRESULT Command ( MPARAM1 mp1, MPARAM2 mp2 ) ;
      MRESULT MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) ;
} ;

ProfilePathDialog::ProfilePathDialog ( HWND Parent, HWND Owner, HMODULE Resource, int Id, HelpWindow *help, char *path, int pathsize ) :
   Dialog ( Parent, Owner, Resource, Id ), DialogID(Id), Help(help), Path(path), PathSize(pathsize) {
   Load ( ) ;
} /* endmethod */

MRESULT ProfilePathDialog::InitDialog ( HWND Window ) {
   MRESULT Result = Dialog::InitDialog ( Window ) ;
   if ( Result == 0 ) {
      Help->Associate ( Window ) ;
      Sys_SetWindowText ( CHILD(Window,DialogID+ENTRY), Path ) ;
   } /* endif */
   return ( Result ) ;
} /* endmethod */

MRESULT ProfilePathDialog::Command ( MPARAM1 mp1, MPARAM2 ) {
   switch ( SHORT1FROMMP(mp1) ) {
      case DID_OK: {
         char Name [256] ;
         Sys_GetWindowText ( CHILD(Handle,DialogID+ENTRY), Name, sizeof(Name) ) ;
         char FullPath [256] ;
         if ( DosQueryPathInfo ( Name, FIL_QUERYFULLNAME, FullPath, sizeof(FullPath) ) ) {
            Sys_SetWindowText ( CHILD(Handle,DialogID+ERRCODE), "ERROR: Not a valid path." ) ;
            Sys_BeepError ( ) ;
            Sys_SetFocus ( CHILD ( Handle, DialogID+ENTRY ) ) ;
            return ( 0 ) ;
         } /* endif */
         FILESTATUS3 Status ;
         if ( DosQueryPathInfo ( FullPath, FIL_STANDARD, &Status, sizeof(Status) ) ) {
            Sys_SetWindowText ( CHILD(Handle,DialogID+ERRCODE), "ERROR: Path does not exist." ) ;
            Sys_BeepError ( ) ;
            Sys_SetFocus ( CHILD ( Handle, DialogID+ENTRY ) ) ;
            return ( 0 ) ;
         } /* endif */
         if ( ! ( Status.attrFile & FILE_DIRECTORY ) ) {
            Sys_SetWindowText ( CHILD(Handle,DialogID+ERRCODE), "ERROR: Specified path is not a directory." ) ;
            Sys_BeepError ( ) ;
            Sys_SetFocus ( CHILD ( Handle, DialogID+ENTRY ) ) ;
            return ( 0 ) ;
         } /* endif */
         strncpy ( Path, FullPath, PathSize ) ;
         Dismiss ( TRUE ) ;
         break; }
      case DID_CANCEL:
         Dismiss ( FALSE ) ;
         break;
   } /* endswitch */
   return ( 0 ) ;
} /* endmethod */

MRESULT ProfilePathDialog::MessageProcessor ( HWND hwnd, MESG msg, MPARAM1 mp1, MPARAM2 mp2 ) {
   MRESULT Result ;
   switch ( msg ) {
      case WM_INITDLG:  Result = InitDialog ( hwnd ) ; break ;
      case WM_COMMAND:  Result = Command ( mp1, mp2 ) ; break ;
      default:          Result = Dialog::MessageProcessor ( hwnd, msg, mp1, mp2 ) ;
   } /* endswitch */
   return ( Result ) ;
} /* endmethod */


/****************************************************************************
 *                                                                          *
 *      Profile Class Constructor                                           *
 *                                                                          *
 ****************************************************************************/

Profile2::Profile2 ( char *name, HAB Anchor, HMODULE Library, int DialogID, HelpWindow *Help, BOOL ResetFlag ) : Profile(name) {

  /**************************************************************************
   * Despite what just happened in the superclass constructor, we are       *
   *   -not- using the system profile area here.                            *
   **************************************************************************/

   Ready = FALSE ;
   #ifdef __OS2__
      Opened = FALSE ;
      Handle = 0 ;
   #else // __NT__
      ProfileName[0] = 0 ;
   #endif // __OS2__ vs __NT__

  /**************************************************************************
   * If resetting the profile, clear the system profile information now.    *
   **************************************************************************/

   Profile SystemProfile ( Name ) ;
   if ( ResetFlag ) 
      SystemProfile.Reset ( ) ;

  /**************************************************************************
   * Query the system INI for the profile file's path.                      *
   **************************************************************************/

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
   * If the profile file couldn't be found, ask the user for a path.        *
   **************************************************************************/

   if ( ProfilePath == 0 ) {

      // Set the default path.
      char Path [256] ;
      DosQueryPathInfo ( PSZ("."), FIL_QUERYFULLNAME, PSZ(Path), sizeof(Path) ) ;

      // Call up the entry dialog.
      ProfilePathDialog ThisDialog ( HWND_DESKTOP, HWND_DESKTOP, Library, DialogID, Help, Path, sizeof(Path) ) ;
      if ( ThisDialog.Process ( ) ) {

         // Save the approved path in the system profile.
         ProfilePath = new char [ strlen(Path) + 1 ] ;
         strcpy ( ProfilePath, Path ) ;
         Profile SystemProfile ( Name ) ;
         SystemProfile.PutString ( "INIPATH", ProfilePath ) ;

      } /* endif */

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
      * Open/Create the profile file and return the resultant handle.       *
      ***********************************************************************/

      Handle = PrfOpenProfile ( Anchor, PSZ(ProfileName) ) ;
      if ( Handle == NULLHANDLE ) {
         char Message [512] ;
         Log ( "Profile2::Profile2: Could not open INI file.  %s", InterpretWinError(0,Message) ) ;
      } else {
         Opened = TRUE ;
         Ready = TRUE ;
         #ifdef DEBUG
            Log ( "Profile2::Profile2: Opened profile '%s'.", ProfileName ) ;
         #endif // DEBUG
      } /* endif */

     /***********************************************************************
      * If resetting, then clean this profile out.                          *
      ***********************************************************************/

      if ( Opened && ResetFlag )
         Reset ( ) ;

   } /* endif */

} /* endmethod */

/****************************************************************************
 *                                                                          *
 *                           Save INI Information                           *
 *                                                                          *
 ****************************************************************************/

void Profile2::PutIniData ( PINIDATA IniData ) {

  /**************************************************************************
   * Save the window's current size and position.                           *
   **************************************************************************/

   PutItem ( "Position", &IniData->Position, sizeof(IniData->Position) ) ;

  /**************************************************************************
   * Save the new profile.                                                  *
   **************************************************************************/

   for ( int i=0; i<ProfileItemCount; i++ ) 
      PutItem ( ProfileItems[i].Name, ((PBYTE)IniData)+ProfileItems[i].Offset, ProfileItems[i].Size ) ;

  /**************************************************************************
   * Save the item options.                                                 *
   **************************************************************************/

   for ( i=0; i<IniData->ItemCount; i++ ) 
      IniData->Items[i]->PutProfile ( *this ) ;

}

