/********************************************************************** DDE.H
 *                                                                          *
 *                      Dynamic Data Exchange Support                       *
 *                                                                          *
 ****************************************************************************/

#ifndef DDE_H
#define DDE_H

#include "System.h"

#include "Debug.h"

#ifdef __OS2__

#define MAX_DDE_LINKS (50)
#define MAX_DDE_CLIENTS (50)

class Dde_Item {

   private:
      char *Name ;
      int Format ;
      PVOID Data ;
      int Size ;
      Dde_Item *Prev, *Next ;
      HWND HotLinks [MAX_DDE_LINKS] [2] ;       // [0] Server, [1] Client.
      HWND WarmLinks [MAX_DDE_LINKS] [2] ;      // [0] Server, [1] Client.

   public:
      Dde_Item ( char *name, int format, PVOID data, int size ) :
         Format(format), Size(size), Prev(0), Next(0) {
         Name = (char*) malloc ( strlen(name)+1 ) ;
         strcpy ( Name, name ) ;
         Data = malloc ( Size ) ;
         memcpy ( Data, data, Size ) ;
         memset ( HotLinks, 0, sizeof(HotLinks) ) ;
         memset ( WarmLinks, 0, sizeof(WarmLinks) ) ;
      }

      ~Dde_Item ( ) {
         free ( Name ) ;
         free ( Data ) ;
      }

      char *QueryName ( ) { return ( Name ) ; }
      void *QueryData ( ) { return ( Data ) ; }
      int QuerySize ( ) { return ( Size ) ; }

      Dde_Item *QueryPrev ( ) { return ( Prev ) ; }
      Dde_Item *QueryNext ( ) { return ( Next ) ; }
      void SetPrev ( Dde_Item *Item ) { Prev = Item ; }
      void SetNext ( Dde_Item *Item ) { Next = Item ; }

      void Update ( int format, PVOID data, int size ) {
         Format = format ;
         if ( ( Size < size ) || ( Size > size*3 ) ) {
            Size = size ;
            free ( Data ) ;
            Data = malloc ( Size ) ;
         } /* endif */
         memcpy ( Data, data, Size ) ;
         BroadcastUpdate ( ) ;
      }

      void Query ( int &format, PVOID &data, int &size ) {
         format = Format ;
         data = Data ;
         size = Size ;
      }

      void Request   ( HWND Server, HWND Client ) ;
      void Poke      ( HWND Server, HWND Client, int Format, PVOID Data, int Size ) ;
      void Advise    ( HWND Server, HWND Client, BOOL Hot ) ;
      void Unadvise  ( HWND Server, HWND Client ) ;
      void Execute   ( HWND Server, HWND Client, int Format, PVOID Data, int Size ) ;
      void Terminate ( HWND Server, HWND Client ) ;

      void BroadcastUpdate ( ) ;
} ;

class Dde_Topic {

   private:
      char *Name ;
      char *ItemListName ;
      Dde_Item *First, *Last ;
      Dde_Topic *Prev, *Next ;
      HWND Servers [MAX_DDE_CLIENTS] ;

   public:
      Dde_Topic ( char *name, char *itemlistname ) : First(0), Last(0), Prev(0), Next(0) {
         Name = (char*) malloc ( strlen(name)+1 ) ;
         strcpy ( Name, name ) ;
         ItemListName = (char*) malloc ( strlen(itemlistname)+1 ) ;
         strcpy ( ItemListName, itemlistname ) ;
         memset ( Servers, 0, sizeof(Servers) ) ;
         AddItem ( ItemListName, DDEFMT_TEXT, "", 1 ) ;
      }

      ~Dde_Topic ( ) {
         free ( Name ) ;
         free ( ItemListName ) ;
         while ( First ) {
            Dde_Item *Item = First ;
            First = First->QueryNext() ;
            delete Item ;
         } /* endwhile */
         for ( int i=0; i<sizeof(Servers)/sizeof(Servers[0]); i++ ) 
            if ( Servers[i] ) 
               WinDestroyWindow ( Servers[i] ) ;
      }

      char *QueryName ( ) { return ( Name ) ; }
      Dde_Topic *QueryPrev ( ) { return ( Prev ) ; }
      Dde_Topic *QueryNext ( ) { return ( Next ) ; }

      void SetPrev ( Dde_Topic *Topic ) { Prev = Topic ; }
      void SetNext ( Dde_Topic *Topic ) { Next = Topic ; }

      void AddItem ( Dde_Item *Item ) {
         if ( First ) {
            Last->SetNext ( Item ) ;
            Item->SetPrev ( Last ) ;
         } else {
            First = Item ;
            Item->SetPrev ( 0 ) ;
         } /* endif */
         Last = Item ;
         Item->SetNext ( 0 ) ;
      }

      void RemoveItem ( Dde_Item *Item ) {
         if ( Item->QueryPrev() ) 
            Item->QueryPrev()->SetNext ( Item->QueryNext() ) ;
         else
            First = Item->QueryNext() ;
         if ( Item->QueryNext() )
            Item->QueryNext()->SetPrev ( Item->QueryPrev() ) ;
         else
            Last = Item->QueryPrev() ;
         delete Item ;
      }

      Dde_Item *FindItem ( char *ItemName ) {
         Dde_Item *Item = First ;
         while ( Item ) {
            if ( !strcmpi ( ItemName, Item->QueryName() ) ) 
               return ( Item ) ;
            Item = Item->QueryNext() ;
         } /* endwhile */
         return ( 0 ) ;
      }

      BOOL AddItem ( char *ItemName, int Format, PVOID Data, int Size ) {
         Dde_Item *Item = FindItem ( ItemName ) ;
         if ( Item ) return ( FALSE ) ;
         Item = new Dde_Item ( ItemName, Format, Data, Size ) ;
         AddItem ( Item ) ;
         UpdateItemList ( ) ;
         return ( TRUE ) ;
      }

      BOOL UpdateItem ( char *ItemName, int Format, PVOID Data, int Size ) {
         Dde_Item *Item = FindItem ( ItemName ) ;
         if ( Item == 0 ) return ( FALSE ) ;
         Item->Update ( Format, Data, Size ) ;
         return ( TRUE ) ;
      }

      BOOL RemoveItem ( char *ItemName ) {
         Dde_Item *Item = FindItem ( ItemName ) ;
         if ( Item == 0 ) return ( FALSE ) ;
         RemoveItem ( Item ) ;
         UpdateItemList ( ) ;
         return ( TRUE ) ;
      }

      BOOL QueryItem ( char *ItemName, int &Format, PVOID &Data, int &Size ) {
         Dde_Item *Item = FindItem ( ItemName ) ;
         if ( Item == 0 ) return ( FALSE ) ;
         Item->Query ( Format, Data, Size ) ;
         return ( TRUE ) ;
      }

      void UpdateItemList ( ) {
         // Determine how large the item list is.
         int ListSize = 0 ;
         Dde_Item *Item = First ;
         while ( Item ) {
            ListSize += strlen(Item->QueryName()) + 1 ;
            Item = Item->QueryNext() ;
         } /* endwhile */
         // Allocate memory for the list.
         char *ItemList = (char*) malloc ( ListSize ) ;
         // Fill the item list.
         ItemList[0] = 0 ;
         Item = First ;
         while ( Item ) {
            strcat ( ItemList, Item->QueryName() ) ;
            if ( Item->QueryNext() ) 
               strcat ( ItemList, "\t" ) ;
            Item = Item->QueryNext() ;
         } /* endwhile */
         // Update the item list item.
         Item = First ;
         while ( Item ) {
            if ( !strcmpi ( ItemListName, Item->QueryName() ) ) {
               Item->Update ( DDEFMT_TEXT, ItemList, strlen(ItemList)+1 ) ;
               break ;
            } /* endif */
            Item = Item->QueryNext() ;
         } /* endwhile */
      }

      void Initiate  ( HWND Owner,  HWND Client, PDDEINIT pDdeInit, CONVCONTEXT &Conv, char *AppName ) ;
      void Request   ( HWND Server, HWND Client, char *ItemName ) ;
      void Poke      ( HWND Server, HWND Client, char *ItemName, int Format, PVOID Data, int Size ) ;
      void Advise    ( HWND Server, HWND Client, char *ItemName, BOOL Hot ) ;
      void Unadvise  ( HWND Server, HWND Client, char *ItemName ) ;
      void Execute   ( HWND Server, HWND Client, char *ItemName, int Format, PVOID Data, int Size ) ;
      void Terminate ( HWND Server, HWND Client, BOOL DestroyServer ) ;
} ;

class Dde_Application {

   private:
      char *Name ;
      Dde_Topic *First, *Last ;

   public:
      Dde_Application ( char *name ) : First(0), Last(0) {
         Name = (char*) malloc ( strlen(name)+1 ) ;
         strcpy ( Name, name ) ;
      }

      ~Dde_Application ( ) {
         free ( Name ) ;
         while ( First ) {
            Dde_Topic *Topic = First ;
            First = First->QueryNext() ;
            delete Topic ;
         } /* endwhile */
      }

      char *QueryName ( ) { return ( Name ) ; }

      void AddTopic ( Dde_Topic *Topic ) {
         if ( First ) {
            Last->SetNext ( Topic ) ;
            Topic->SetPrev ( Last ) ;
         } else {
            First = Topic ;
            Topic->SetPrev ( 0 ) ;
         } /* endif */
         Last = Topic ;
         Topic->SetNext ( 0 ) ;
      }

      void RemoveTopic ( Dde_Topic *Topic ) {
         if ( Topic->QueryPrev() ) 
            Topic->QueryPrev()->SetNext ( Topic->QueryNext() ) ;
         else
            First = Topic->QueryNext() ;
         if ( Topic->QueryNext() )
            Topic->QueryNext()->SetPrev ( Topic->QueryPrev() ) ;
         else
            Last = Topic->QueryPrev() ;
         delete Topic ;
      }

      Dde_Topic *FindTopic ( char *TopicName ) {
         Dde_Topic *Topic = First ;
         while ( Topic ) {
            if ( !strcmpi ( TopicName, Topic->QueryName() ) ) 
               return ( Topic ) ;
            Topic = Topic->QueryNext() ;
         } /* endwhile */
         return ( 0 ) ;
      }

      BOOL AddTopic ( char *TopicName, char *ItemListName ) {
         Dde_Topic *Topic = FindTopic ( TopicName ) ;
         if ( Topic ) return ( FALSE ) ;
         Topic = new Dde_Topic ( TopicName, ItemListName ) ;
         AddTopic ( Topic ) ;
         return ( TRUE ) ;
      }

      BOOL RemoveTopic ( char *TopicName ) {
         Dde_Topic *Topic = FindTopic ( TopicName ) ;
         if ( Topic == 0 ) return ( FALSE ) ;
         RemoveTopic ( Topic ) ;
         return ( TRUE ) ;
      }

      Dde_Item *FindItem ( char *TopicName, char *ItemName ) {
         Dde_Topic *Topic = FindTopic ( TopicName ) ;
         return ( Topic ? Topic->FindItem ( ItemName ) : 0 ) ;
      }

      BOOL AddItem ( char *TopicName, char *ItemName, int Format, PVOID Data, int Size ) {
         Dde_Topic *Topic = FindTopic ( TopicName ) ;
         return ( Topic ? Topic->AddItem ( ItemName, Format, Data, Size ) : FALSE ) ;
      }

      BOOL UpdateItem ( char *TopicName, char *ItemName, int Format, PVOID Data, int Size ) {
         Dde_Topic *Topic = FindTopic ( TopicName ) ;
         return ( Topic ? Topic->UpdateItem ( ItemName, Format, Data, Size ) : FALSE ) ;
      }

      BOOL RemoveItem ( char *TopicName, char *ItemName ) {
         Dde_Topic *Topic = FindTopic ( TopicName ) ;
         return ( Topic ? Topic->RemoveItem ( ItemName ) : FALSE ) ;
      }

      BOOL QueryItem ( char *TopicName, char *ItemName, int &Format, PVOID &Data, int &Size ) {
         Dde_Topic *Topic = FindTopic ( TopicName ) ;
         return ( Topic ? Topic->QueryItem ( ItemName, Format, Data, Size ) : FALSE ) ;
      }

      void Initiate ( HWND Owner, HWND Client, PDDEINIT pDdeInit, CONVCONTEXT &Conv ) ;
} ;

#endif // __OS2__

class Dde_Server {

   #ifdef __OS2__

   private:
      BOOL Ready ;
      Dde_Application Application ;

   public:
      Dde_Server ( ANCHOR Anchor, HMODULE Library, HWND Owner, char *AppName ) ;

      ~Dde_Server ( ) { }

      void Initiate ( HWND Owner, HWND Client, PDDEINIT pDdeInit ) ;

      BOOL IsReady ( ) { return ( Ready ) ; }

      BOOL AddTopic ( char *TopicName, char *ItemListName ) {
         return ( Application.AddTopic ( TopicName, ItemListName ) ) ;
      }

      BOOL RemoveTopic ( char *TopicName ) {
         return ( Application.RemoveTopic ( TopicName ) ) ;
      }

      BOOL AddItem ( char *TopicName, char *ItemName, int Format, PVOID Data, int Size ) {
         return ( Application.AddItem ( TopicName, ItemName, Format, Data, Size ) ) ;
      }

      BOOL UpdateItem ( char *TopicName, char *ItemName, int Format, PVOID Data, int Size ) {
         return ( Application.UpdateItem ( TopicName, ItemName, Format, Data, Size ) ) ;
      }

      BOOL RemoveItem ( char *TopicName, char *ItemName ) {
         return ( Application.RemoveItem ( TopicName, ItemName ) ) ;
      }

      BOOL QueryItem ( char *TopicName, char *ItemName, int &Format, PVOID &Data, int &Size ) {
         return ( Application.QueryItem ( TopicName, ItemName, Format, Data, Size ) ) ;
      }

      Dde_Topic *FindTopic ( char *TopicName ) {
         return ( Application.FindTopic ( TopicName ) ) ;
      }

      Dde_Item *FindItem ( char *TopicName, char *ItemName ) {
         return ( Application.FindItem ( TopicName, ItemName ) ) ;
      }

   #endif // __OS2__

} ;

#endif
