/*************************************************************** CHKDRIVE.CPP
 *                                                                          *
 *      Check to see if drive should be added to display list.              *
 *                                                                          *
 ****************************************************************************/

#define INCLUDE_SYSTEM
#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ChkDrive.h"
#include "Debug.h"
#include "SVDisk.h"

extern int CheckDrive ( int Drive, char *FileSystem, int cFileSystem, char *DiskLabel, int cDiskLabel ) {

   memset ( FileSystem, 0, cFileSystem ) ;
   memset ( DiskLabel, 0, cDiskLabel ) ;

#ifdef __OS2__

  /**************************************************************************
   * First, check to see if drive is local or remote.  Remote drives are    *
   *   always monitored.                                                    *
   **************************************************************************/

   BYTE Path [3] ;
   Path[0] = (BYTE) ( Drive + 'A' - 1 ) ;
   Path[1] = ':' ;
   Path[2] = 0 ;

   DosError ( FERR_DISABLEHARDERR ) ;

   BYTE Buffer [1024] ;
   ULONG Size = sizeof(Buffer) ;
   ULONG Status = DosQueryFSAttach ( Path, 0, FSAIL_QUERYNAME, (PFSQBUFFER2)Buffer, &Size ) ;
   DosError ( FERR_ENABLEHARDERR ) ;

   if ( Status ) { // If error, we shouldn't monitor.
      #ifdef DEBUG
      Log ( "ERROR: DosQueryFSAttach('%s') returned status %i.", Path, Status ) ;
      #endif // DEBUG
      if ( Status == ERROR_NOT_READY ) // If not ready, must be removable media.
         strcpy ( FileSystem, "REMV" ) ;
      return ( 0 ) ;   // Don't monitor.
   } /* endif */

   USHORT cbName = PFSQBUFFER2(Buffer)->cbName ;
   strcpy ( FileSystem, PCHAR(PFSQBUFFER2(Buffer+cbName)->szFSDName) ) ;

   if ( strcmpi ( FileSystem, "CDFS" ) == 0 ) {
      #ifdef DEBUG
      Log ( "Drive %s has system CDFS and will not be monitored.", Path ) ;
      #endif // DEBUG
      return ( 0 ) ;  // Reject CDs.
   } /* endif */

   if ( strcmpi ( FileSystem, "CDWFS" ) == 0 ) {
      #ifdef DEBUG
      Log ( "Drive %s has system CDWFS and will not be monitored.", Path ) ;
      #endif // DEBUG
      return ( 0 ) ;  // Reject writable CDs.
   } /* endif */

   if ( PFSQBUFFER2(Buffer)->iType == FSAT_REMOTEDRV ) {
      #ifdef DEBUG
      Log ( "Drive %s a remote drive and will be monitored.", Path ) ;
      #endif // DEBUG
      USHORT cbFSDName = PFSQBUFFER2(Buffer)->cbFSDName ;
      strcpy ( DiskLabel, PCHAR(PFSQBUFFER2(Buffer+cbName+cbFSDName)->rgFSAData) ) ;
      return ( -1 ) ;  // Monitor but don't include in the total over all drives.
   } /* endif */

  /**************************************************************************
   * Attempt to open the local drive as an entire device.  If unable to do  *
   *   so, we cannot monitor this drive.                                    *
   **************************************************************************/

   ULONG Action ;
   HFILE Handle ;
   Status = DosOpen ( Path, &Handle, &Action, 0, 0, FILE_OPEN,
      OPEN_ACCESS_READONLY | OPEN_SHARE_DENYNONE |
      OPEN_FLAGS_DASD | OPEN_FLAGS_FAIL_ON_ERROR, 0 ) ;

   if ( Status ) {
      #ifdef DEBUG
      Log ( "ERROR: DosOpen('%s') failed with status %i.", Path, Status ) ;
      #endif // DEBUG
      return ( 0 ) ;   // Don't monitor.
   } /* endif */

  /**************************************************************************
   * Check to see if the drive has removable media.                         *
   *   We cannot monitor true removable media because of how the media      *
   *   check function ties up the system.                                   *
   **************************************************************************/

   BOOL Addit = FALSE ;
   BYTE Command = 0 ;
   BYTE NonRemovable ;

   ULONG LengthIn = sizeof(Command) ;
   ULONG LengthOut = sizeof(NonRemovable);

   if ( NOT DosDevIOCtl ( Handle, 8, 0x20,
      &Command, sizeof(Command), &LengthIn,
      &NonRemovable, sizeof(NonRemovable), &LengthOut ) ) {

      Addit = NonRemovable ;

      if ( !NonRemovable && IsSVDisk((UCHAR)Drive) ) 
         Addit = TRUE ;

   } /* endif */

  /**************************************************************************
   * Close the drive.                                                       *
   **************************************************************************/

   DosClose ( Handle ) ;

  /**************************************************************************
   * Get the drive label.                                                   *
   **************************************************************************/

   FSINFO Info ;
   if ( !DosQueryFSInfo ( Drive, FSIL_VOLSER, PBYTE(&Info), sizeof(Info) ) )
      strcpy ( PCHAR(DiskLabel), PCHAR(Info.vol.szVolLabel) ) ;

  /**************************************************************************
   * Return the final verdict.                                              *
   **************************************************************************/

   #ifdef DEBUG
   Log ( "CheckDrive('%s') returning status %s.", Path, Addit?"ADD":"NO ADD" ) ;
   #endif // DEBUG

   return ( int(Addit) ) ;    // Monitor and include in overall total if not removable.

#else

  /**************************************************************************
   * Determine the overall drive type (local, remote, removable).           *
   **************************************************************************/

   char DriveName [10] ; 
   sprintf ( DriveName, "%c:\\", Drive+'A'-1 ) ;
   switch ( GetDriveType ( DriveName ) ) {

     /***********************************************************************
      * Fixed drives are OK.                                                *
      ***********************************************************************/

      case DRIVE_FIXED: {
         GetVolumeInformation ( DriveName, DiskLabel, cDiskLabel, 0, 0, 0, FileSystem, cFileSystem ) ;
         return ( 1 ) ; }

     /***********************************************************************
      * Remote & virtual disks are OK, sort of.                             *
      ***********************************************************************/

      case DRIVE_REMOTE:
      case DRIVE_RAMDISK:
         GetVolumeInformation ( DriveName, DiskLabel, cDiskLabel, 0, 0, 0, FileSystem, cFileSystem ) ;
         return ( -1 ) ;

     /***********************************************************************
      * Everything else we discard.                                         *
      ***********************************************************************/

      case 0:   // Couldn't tell.
      case 1:   // Doesn't exist.
      case DRIVE_REMOVABLE:
      case DRIVE_CDROM:
      default:  // Something's wrong.
         return ( 0 ) ;

   } /* endswitch */

#endif // __OS2__ vs __NT__
}

