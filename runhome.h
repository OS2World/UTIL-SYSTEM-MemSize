#ifndef RUNHOME_H
#define RUNHOME_H

#ifndef OS2_INCLUDED
   #define INCL_BASE
   #include <os2.h>
#endif // OS2_INCLUDED

#include <dos.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>

// Class: Run Home - Run program in the directory where its EXE file came from.

class RunHome {

   private:
      int OriginalDrive ;
      char OriginalDir[_MAX_DIR+1];

   public:

      RunHome ( ) {

         // First, record where we are.  ELABORATE: Save all drive paths.
         OriginalDrive = _getdrive (  ) ;
         getcwd ( OriginalDir, sizeof ( OriginalDir ) );

         // Discover who we are.
         PPIB pPIB ; PTIB pTIB ; 
         DosGetInfoBlocks ( &pTIB, &pPIB ) ;
         char FullPath[_MAX_PATH+1] ;
         DosQueryModuleName ( pPIB->pib_hmte, sizeof(FullPath), FullPath ) ;

         // Move to where the program is being loaded from.
         char Drive [_MAX_DRIVE+1] ; 
         char Dir [_MAX_DIR+1] ; 
         char FName [_MAX_FNAME+1] ; 
         char Ext [_MAX_EXT+1] ;
         _splitpath ( FullPath, Drive, Dir, FName, Ext );
         strupr ( Drive );
         unsigned Total ; _dos_setdrive ( Drive[0] - 'A' + 1, &Total ) ;
         chdir ( "\\" );
         Dir[strlen(Dir)-1] = 0;
         if ( Dir[0] )
            chdir ( Dir );

      } /* endmethod */

      ~RunHome ( ) {

          // Restore the original drive and directory.  ELABORATE: Restore all drive paths.
          unsigned Total ; _dos_setdrive ( OriginalDrive, &Total );
          chdir ( "\\" );
          chdir ( OriginalDir );

      } /* endmethod */

      int QueryOriginalDrive ( ) { return ( OriginalDrive ) ; }
      char *QueryOriginalDir ( ) { return ( OriginalDir ) ; }
} ;

#endif // RUNHOME_H
