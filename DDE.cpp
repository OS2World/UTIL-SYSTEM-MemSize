/******************************************************************** DDE.CPP
 *                                                                          *
 *                      Dynamic Data Exchange Support                       *
 *                                                                          *
 ****************************************************************************/

#ifdef __OS2__

#include "System.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Debug.h"
#include "MemSize.h"
#include "ReString.h"
#include "Support.h"

#include "Dde.h"

 
/****************************************************************************
 *                                                                          *
 *                        Definitions & Declarations                        *
 *                                                                          *
 ****************************************************************************/

   // Constants

#define DDESERVER_CLASS  "Dde_Server"

   // Type Definitions

typedef struct {
   HWND Client ;
   Dde_Topic *Topic ;
} DDESERVER_PARMS ;


   // Function Prototypes

static FNWP Dde_Processor ;

static PDDESTRUCT MakeDDEObject (
   HWND Destination,                    // -> Destination window
   PSZ Item,                            // -> Item name or NULL.
   USHORT Status,                       // Status flags.
   USHORT Format,                       // Data format
   PVOID Data,                          // -> Data or NULL.
   ULONG DataLen                        // Data length in bytes.
) ;


 
/****************************************************************************
 *                                                                          *
 *                         Dde_Server: Constructor                          *
 *                                                                          *
 ****************************************************************************/

Dde_Server::Dde_Server ( HAB Anchor, HMODULE Library, HWND Owner, char *AppName ) :
   Ready(FALSE), Application(AppName) {

   if ( !WinRegisterClass ( Anchor, PSZ(DDESERVER_CLASS),
      Dde_Processor, 0, sizeof(PVOID) ) ) {

      ERRORID Error = WinGetLastError ( Anchor ) ;
      ResourceString Title ( Library, IDS_TITLE ) ;
      ResourceString Format ( Library, IDS_ERROR_WINREGISTERCLASS ) ;
      CHAR Message [200] ;
      sprintf ( Message, PCHAR(Format), DDESERVER_CLASS, Error ) ;
      Log ( "%s", Message ) ;
      WinMessageBox ( HWND_DESKTOP, Owner, PSZ(Message),
         PSZ(Title), 0, MB_ENTER | MB_ICONEXCLAMATION ) ;
      return ;

   } /* endif */

   Ready = TRUE ;
}
 
/****************************************************************************
 *                                                                          *
 *              Dde_Server: Process WM_DDE_INITIATE request.                *
 *                                                                          *
 ****************************************************************************/

void Dde_Server::Initiate ( HWND Owner, HWND Client, PDDEINIT pDdeInit ) {

  /**************************************************************************
   * Prepare to respond with a WM_DDE_INITIATEACK.                          *
   **************************************************************************/

   HMQ Queue = WinQueryWindowULong ( Owner, QWL_HMQ ) ;
   char Text [10] ;
   ULONG Size = 10 ;
   int Country = 1 ;
   if ( PrfQueryProfileData ( HINI_USERPROFILE, PSZ("PM_National"), PSZ("iCountry"), Text, &Size ) )
      Country = atoi ( Text ) ;

   CONVCONTEXT Conv ;
   Conv.cb = sizeof(Conv) ;
   Conv.fsContext = DDECTXT_CASESENSITIVE ;
   Conv.idCountry = Country ;
   Conv.usCodepage = WinQueryCp ( Queue ) ;
   Conv.usLangID = 0 ;
   Conv.usSubLangID = 0 ;

  /**************************************************************************
   * Respond if appropriate.                                                *
   **************************************************************************/

   Application.Initiate ( Owner, Client, pDdeInit, Conv ) ;
}
 
/****************************************************************************
 *                                                                          *
 *            Dde_Application: Process WM_DDE_INITIATE request.             *
 *                                                                          *
 ****************************************************************************/

void Dde_Application::Initiate ( HWND Owner, HWND Client, PDDEINIT pDdeInit, CONVCONTEXT &Conv ) {

   // If not for this application, hold your peace.
   if ( pDdeInit->pszAppName AND *pDdeInit->pszAppName AND strcmpi ( PCHAR(pDdeInit->pszAppName), Name ) )
      return ;

   // Pass the initiate request to each of the topics.
   Dde_Topic *Topic = First ;
   while ( Topic ) {
      Topic->Initiate ( Owner, Client, pDdeInit, Conv, Name ) ;
      Topic = Topic->QueryNext() ;
   } /* endwhile */
}
 
/****************************************************************************
 *                                                                          *
 *               Dde_Topic: Process WM_DDE_INITIATE request.                *
 *                                                                          *
 ****************************************************************************/

void Dde_Topic::Initiate ( HWND Owner, HWND Client, PDDEINIT pDdeInit, CONVCONTEXT &Conv, char *AppName ) {

   // If not for this topic, hold your peace.
   if ( pDdeInit->pszTopic AND *pDdeInit->pszTopic AND strcmpi ( PCHAR(pDdeInit->pszTopic), Name ) )
      return ;

   // Find an empty slot in the server list.
   for ( int i=0; i<sizeof(Servers)/sizeof(Servers[0]); i++ ) 
      if ( Servers[i] == 0 ) 
         break ;
   if ( i >= sizeof(Servers)/sizeof(Servers[0]) ) 
      return ;

   // Create the server window and add it to the list.
   DDESERVER_PARMS *Parms = (DDESERVER_PARMS*) malloc ( sizeof(DDESERVER_PARMS) ) ;
   Parms->Client = Client ;
   Parms->Topic = this ;
   Servers[i] = WinCreateWindow ( HWND_OBJECT, PSZ(DDESERVER_CLASS),
      0, 0, 0, 0, 0, 0, Owner, HWND_TOP, 0, Parms, 0 ) ;

   // Respond to the request.
   WinDdeRespond ( Client, Servers[i], PSZ(AppName), PSZ(Name), &Conv ) ;
}
 
/****************************************************************************
 *                                                                          *
 *               Dde_Topic: Process WM_DDE_REQUEST request.                 *
 *                                                                          *
 ****************************************************************************/

void Dde_Topic::Request ( HWND Server, HWND Client, char *ItemName ) {

   // Search for the item and ask it to reply.
   Dde_Item *Item = First ;
   while ( Item ) {
      if ( !strcmpi ( ItemName, Item->QueryName() ) ) {
         Item->Request ( Server, Client ) ;
         return ;
      } /* endif */
      Item = Item->QueryNext() ;
   } /* endwhile */

   // Reply with an error if the topic was not found.
   PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_NOTPROCESSED, DDEFMT_TEXT, 0, 0 ) ;
   WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                 Dde_Topic: Process WM_DDE_POKE request.                  *
 *                                                                          *
 ****************************************************************************/

void Dde_Topic::Poke ( HWND Server, HWND Client, char *ItemName, int Format, PVOID Data, int Size ) {

   // Search for the item and ask it to reply.
   Dde_Item *Item = First ;
   while ( Item ) {
      if ( !strcmpi ( ItemName, Item->QueryName() ) ) {
         Item->Poke ( Server, Client, Format, Data, Size ) ;
         return ;
      } /* endif */
      Item = Item->QueryNext() ;
   } /* endwhile */

   // Reply with an error if the topic was not found.
   PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_NOTPROCESSED, DDEFMT_TEXT, 0, 0 ) ;
   WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                 Dde_Topic: Process WM_DDE_ADVISE request.                *
 *                                                                          *
 ****************************************************************************/

void Dde_Topic::Advise ( HWND Server, HWND Client, char *ItemName, BOOL Hot ) {

   // Search for the item and ask it to reply.
   Dde_Item *Item = First ;
   while ( Item ) {
      if ( !strcmpi ( ItemName, Item->QueryName() ) ) {
         Item->Advise ( Server, Client, Hot ) ;
         return ;
      } /* endif */
      Item = Item->QueryNext() ;
   } /* endwhile */

   // Reply with an error if the topic was not found.
   PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_NOTPROCESSED, DDEFMT_TEXT, 0, 0 ) ;
   WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                 Dde_Topic: Process WM_DDE_UNADVISE request.              *
 *                                                                          *
 ****************************************************************************/

void Dde_Topic::Unadvise ( HWND Server, HWND Client, char *ItemName ) {

   // Search for the item and ask it to reply.
   Dde_Item *Item = First ;
   while ( Item ) {
      if ( !strcmpi ( ItemName, Item->QueryName() ) ) {
         Item->Unadvise ( Server, Client ) ;
         return ;
      } /* endif */
      Item = Item->QueryNext() ;
   } /* endwhile */

   // Reply with an error if the topic was not found.
   PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_NOTPROCESSED, DDEFMT_TEXT, 0, 0 ) ;
   WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                Dde_Topic: Process WM_DDE_EXECUTE request.                *
 *                                                                          *
 ****************************************************************************/

void Dde_Topic::Execute ( HWND Server, HWND Client, char *ItemName, int Format, PVOID Data, int Size ) {

   // Search for the item and ask it to reply.
   Dde_Item *Item = First ;
   while ( Item ) {
      if ( !strcmpi ( ItemName, Item->QueryName() ) ) {
         Item->Execute ( Server, Client, Format, Data, Size ) ;
         return ;
      } /* endif */
      Item = Item->QueryNext() ;
   } /* endwhile */

   // Reply with an error if the topic was not found.
   PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_NOTPROCESSED, DDEFMT_TEXT, 0, 0 ) ;
   WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
}
 
/****************************************************************************
 *                                                                          *
 *               Dde_Topic: Process WM_DDE_TERMINATE request.               *
 *                                                                          *
 ****************************************************************************/

void Dde_Topic::Terminate ( HWND Server, HWND Client, BOOL DestroyServer ) {

   // Echo the termination command to the client.
   WinDdePostMsg ( Client, Server, WM_DDE_TERMINATE, 0, TRUE ) ;

   // Remove the server from the list.
   for ( int i=0; i<sizeof(Servers)/sizeof(Servers[0]); i++ ) 
      if ( Servers[i] == Server ) 
         Servers[i] = 0 ;

   // Cancel any links.
   Dde_Item *Item = First ;
   while ( Item ) {
      Item->Terminate ( Server, Client ) ;
      Item = Item->QueryNext() ;
   } /* endwhile */

   // If server window not already being destroyed, destroy it now.
   if ( DestroyServer )
      WinDestroyWindow ( Server ) ;
}
 
/****************************************************************************
 *                                                                          *
 *               Dde_Item: Process WM_DDE_REQUEST request.                  *
 *                                                                          *
 ****************************************************************************/

void Dde_Item::Request ( HWND Server, HWND Client ) {

   // Reply with the item data.
   PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), 0, Format, Data, Size ) ;
   WinDdePostMsg ( Client, Server, WM_DDE_DATA, Response, DDEPM_RETRY ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                 Dde_Item: Process WM_DDE_POKE request.                   *
 *                                                                          *
 ****************************************************************************/

void Dde_Item::Poke ( HWND Server, HWND Client, int, PVOID, int ) {

   // Reply with an error, as we don't support this function.
   PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_NOTPROCESSED, DDEFMT_TEXT, 0, 0 ) ;
   WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                 Dde_Item: Process WM_DDE_ADVISE request.                 *
 *                                                                          *
 ****************************************************************************/

void Dde_Item::Advise ( HWND Server, HWND Client, BOOL Hot ) {

   if ( Hot ) {
      // Search for a matching entry in the hot link list.  Return if found.
      for ( int i=0; i<sizeof(HotLinks)/sizeof(HotLinks[0]); i++ ) {
         if ( ( HotLinks[i][0] == Server ) AND ( HotLinks[i][1] == Client ) ) {
            Log ( "WARNING: Link already established." ) ;
            PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_FACK, DDEFMT_TEXT, 0, 0 ) ;
            WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
            return ;
         } /* endif */
      } /* endfor */
      // Search for an empty entry in the hot link list.  Use if not found.
      for ( i=0; i<sizeof(HotLinks)/sizeof(HotLinks[0]); i++ ) {
         if ( ( HotLinks[i][0] == 0 ) AND ( HotLinks[i][1] == 0 ) ) {
            HotLinks[i][0] = Server ;
            HotLinks[i][1] = Client ;
            PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_FACK, DDEFMT_TEXT, 0, 0 ) ;
            WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
            return ;
         } /* endif */
      } /* endfor */

      Log ( "  WARNING: Hot link table full." ) ;

   } else {
      // Search for a matching entry in the warm link list.  Return if found.
      for ( int i=0; i<sizeof(WarmLinks)/sizeof(WarmLinks[0]); i++ ) {
         if ( ( WarmLinks[i][0] == Server ) AND ( WarmLinks[i][1] == Client ) ) {
            Log ( "WARNING: Link already established." ) ;
            PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_FACK, DDEFMT_TEXT, 0, 0 ) ;
            WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
            return ;
         } /* endif */
      } /* endfor */
      // Search for an empty entry in the warm link list.  Use if not found.
      for ( i=0; i<sizeof(WarmLinks)/sizeof(WarmLinks[0]); i++ ) {
         if ( ( WarmLinks[i][0] == 0 ) AND ( WarmLinks[i][1] == 0 ) ) {
            WarmLinks[i][0] = Server ;
            WarmLinks[i][1] = Client ;
            PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_FACK, DDEFMT_TEXT, 0, 0 ) ;
            WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
            return ;
         } /* endif */
      } /* endfor */

      Log ( "  WARNING: Warm link table full." ) ;

   } /* endif */

   // Reply with an error.
   PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_NOTPROCESSED, DDEFMT_TEXT, 0, 0 ) ;
   WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                 Dde_Item: Process WM_DDE_UNADVISE request.               *
 *                                                                          *
 ****************************************************************************/

void Dde_Item::Unadvise ( HWND Server, HWND Client ) {

   // Cancel any links on this item for this Server/Client pair.
   Terminate ( Server, Client ) ;

   // Reply with an ACK.
   PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_FACK, DDEFMT_TEXT, 0, 0 ) ;
   WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                Dde_Item: Process WM_DDE_EXECUTE request.                 *
 *                                                                          *
 ****************************************************************************/

void Dde_Item::Execute ( HWND Server, HWND Client, int, PVOID, int ) {

   // Reply with an error, as we don't support this function.
   PDDESTRUCT Response = MakeDDEObject ( Client, PSZ(Name), DDE_NOTPROCESSED, DDEFMT_TEXT, 0, 0 ) ;
   WinDdePostMsg ( Client, Server, WM_DDE_ACK, Response, DDEPM_RETRY ) ;
}
 
/****************************************************************************
 *                                                                          *
 *               Dde_Item: Process WM_DDE_TERMINATE request.                *
 *                                                                          *
 ****************************************************************************/

void Dde_Item::Terminate ( HWND Server, HWND Client ) {

   // Search for and remove all hot links for this client/server pair.
   for ( int i=0; i<sizeof(HotLinks)/sizeof(HotLinks[0]); i++ ) 
      if ( ( HotLinks[i][0] == Server ) AND ( HotLinks[i][1] == Client ) ) 
         HotLinks[i][0] = HotLinks[i][1] = 0 ;

   // Search for and remove all warm links for this client/server pair.
   for ( i=0; i<sizeof(WarmLinks)/sizeof(WarmLinks[0]); i++ ) 
      if ( ( WarmLinks[i][0] == Server ) AND ( WarmLinks[i][1] == Client ) ) 
         WarmLinks[i][0] = WarmLinks[i][1] = 0 ;
}
 
/****************************************************************************
 *                                                                          *
 *                       Dde_Item: Broadcast Update                         *
 *                                                                          *
 ****************************************************************************/

void Dde_Item::BroadcastUpdate ( ) {

   // Send data complete to all hot links.
   for ( int i=0; i<sizeof(HotLinks)/sizeof(HotLinks[0]); i++ ) {
      if ( HotLinks[i][0] ) {
         PDDESTRUCT Message = MakeDDEObject ( HotLinks[i][1], PSZ(Name), 0, Format, Data, Size ) ;
         WinDdePostMsg ( HotLinks[i][1], HotLinks[i][0], WM_DDE_DATA, Message, DDEPM_RETRY ) ;
      } /* endif */
   } /* endfor */

   // Send advisory to all warm links.
   for ( i=0; i<sizeof(WarmLinks)/sizeof(WarmLinks[0]); i++ ) {
      if ( WarmLinks[i][0] ) {
         PDDESTRUCT Message = MakeDDEObject ( WarmLinks[i][1], PSZ(Name), DDE_FNODATA, Format, 0, 0 ) ;
         WinDdePostMsg ( WarmLinks[i][1], WarmLinks[i][0], WM_DDE_DATA, Message, DDEPM_RETRY ) ;
      } /* endif */
   } /* endfor */
}
 
/****************************************************************************
 *                                                                          *
 *                    Dde Server Object Window Class                        *
 *                                                                          *
 ****************************************************************************/

extern MRESULT APIENTRY Dde_Processor ( HWND Server, ULONG Msg, MPARAM mp1, MPARAM mp2 ) {

   switch ( Msg ) {

      case WM_CREATE: {
         DDESERVER_PARMS *Parms = (DDESERVER_PARMS*) mp1 ;
         WinSetWindowPtr ( Server, QWL_USER, Parms ) ;
         return ( 0 ) ; }

      case WM_DESTROY: {
         DDESERVER_PARMS *Parms = (DDESERVER_PARMS*) WinQueryWindowPtr ( Server, QWL_USER ) ;
         Parms->Topic->Terminate ( Server, Parms->Client, FALSE ) ;
         return ( 0 ) ; }

      case WM_DDE_REQUEST: {
         DDESERVER_PARMS *Parms = (DDESERVER_PARMS*) WinQueryWindowPtr ( Server, QWL_USER ) ;
         HWND Client = HWNDFROMMP ( mp1 ) ;
         PDDESTRUCT Request = PDDESTRUCT ( mp2 ) ;
         PCHAR Item = PCHAR ( DDES_PSZITEMNAME ( Request ) ) ;
         Parms->Topic->Request ( Server, Client, Item ) ;
         DosFreeMem ( Request ) ;
         return ( 0 ) ; }

      case WM_DDE_POKE: {
         DDESERVER_PARMS *Parms = (DDESERVER_PARMS*) WinQueryWindowPtr ( Server, QWL_USER ) ;
         HWND Client = HWNDFROMMP ( mp1 ) ;
         PDDESTRUCT Request = PDDESTRUCT ( mp2 ) ;
         PCHAR Item = PCHAR ( DDES_PSZITEMNAME ( Request ) ) ;
         int Format = int ( Request->usFormat ) ;
         PVOID Data = PVOID ( DDES_PABDATA ( Request ) ) ;
         int Size = int ( Request->cbData ) ;
         Parms->Topic->Poke ( Server, Client, Item, Format, Data, Size ) ;
         DosFreeMem ( Request ) ;
         return ( 0 ) ; }

      case WM_DDE_ADVISE: {
         DDESERVER_PARMS *Parms = (DDESERVER_PARMS*) WinQueryWindowPtr ( Server, QWL_USER ) ;
         HWND Client = HWNDFROMMP ( mp1 ) ;
         PDDESTRUCT Request = PDDESTRUCT ( mp2 ) ;
         PCHAR Item = PCHAR ( DDES_PSZITEMNAME ( Request ) ) ;
         Parms->Topic->Advise ( Server, Client, Item, ! ( Request->fsStatus & DDE_FNODATA ) ) ;
         DosFreeMem ( Request ) ;
         return ( 0 ) ; }

      case WM_DDE_UNADVISE: {
         DDESERVER_PARMS *Parms = (DDESERVER_PARMS*) WinQueryWindowPtr ( Server, QWL_USER ) ;
         HWND Client = HWNDFROMMP ( mp1 ) ;
         PDDESTRUCT Request = PDDESTRUCT ( mp2 ) ;
         PCHAR Item = PCHAR ( DDES_PSZITEMNAME ( Request ) ) ;
         Parms->Topic->Unadvise ( Server, Client, Item ) ;
         DosFreeMem ( Request ) ;
         return ( 0 ) ; }

      case WM_DDE_EXECUTE: {
         DDESERVER_PARMS *Parms = (DDESERVER_PARMS*) WinQueryWindowPtr ( Server, QWL_USER ) ;
         HWND Client = HWNDFROMMP ( mp1 ) ;
         PDDESTRUCT Request = PDDESTRUCT ( mp2 ) ;
         PCHAR Item = PCHAR ( DDES_PSZITEMNAME ( Request ) ) ;
         int Format = int ( Request->usFormat ) ;
         PVOID Data = PVOID ( DDES_PABDATA ( Request ) ) ;
         int Size = int ( Request->cbData ) ;
         Parms->Topic->Execute ( Server, Client, Item, Format, Data, Size ) ;
         DosFreeMem ( Request ) ;
         return ( 0 ) ; }

      case WM_DDE_TERMINATE: {
         DDESERVER_PARMS *Parms = (DDESERVER_PARMS*) WinQueryWindowPtr ( Server, QWL_USER ) ;
         Parms->Topic->Terminate ( Server, Parms->Client, TRUE ) ;
         return ( 0 ) ; }

   } /* endswitch */

   return ( WinDefWindowProc ( Server, Msg, mp1, mp2 ) ) ;
}
 
/****************************************************************************
 *                                                                          *
 *                        Allocate DDE Structure                            *
 *                                                                          *
 *  This function is straight from the "OS/2 2.0 Presentation Manager       *
 *    Programming Guide", Copyright 1992, IBM.  Adapted to C++ from C.      *
 *                                                                          *
 ****************************************************************************/

static PDDESTRUCT MakeDDEObject (
   HWND Destination,                    // -> Destination window
   PSZ Item,                            // -> Item name or NULL.
   USHORT Status,                       // Status flags.
   USHORT Format,                       // Data format
   PVOID Data,                          // -> Data or NULL.
   ULONG DataLen                        // Data length in bytes.
) {

   ULONG ItemLen = Item ? strlen(PCHAR(Item))+1 : 1 ;

   PDDESTRUCT pBlock ;
   if ( DosAllocSharedMem ( PPVOID(&pBlock), 0, sizeof(DDESTRUCT) + ItemLen + DataLen,
      PAG_COMMIT | PAG_READ | PAG_WRITE | OBJ_GIVEABLE ) )
      return ( 0 ) ;

   pBlock->cbData = DataLen ;
   pBlock->fsStatus = Status ;
   pBlock->usFormat = Format ;
   pBlock->offszItemName = sizeof(DDESTRUCT) ;

   if ( DataLen AND Data )
      pBlock->offabData = USHORT ( sizeof(DDESTRUCT) + ItemLen ) ;
   else
      pBlock->offabData = 0 ;

   if ( Item )
      strcpy ( PCHAR(DDES_PSZITEMNAME(pBlock)), PCHAR(Item) ) ;
   else
      strcpy ( PCHAR(DDES_PSZITEMNAME(pBlock)), "" ) ;

   if ( Data )
      memcpy ( DDES_PABDATA(pBlock), Data, size_t(DataLen) ) ;

   PID pid ; TID tid ;
   WinQueryWindowProcess ( Destination, &pid, &tid ) ;
   DosGiveSharedMem( (PVOID)pBlock, pid, PAG_READ | PAG_WRITE) ;

   return ( pBlock ) ;
}

#endif // Only for OS/2.
