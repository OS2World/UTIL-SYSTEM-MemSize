#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_PM
#include <os2.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

extern void Log ( char *Message, ... ) ;

extern "C" {
   #include <types.h>
   #include <sys\socket.h>
   #include <netdb.h>
   #include <utils.h>
   #include <netinet\in.h>
   #include <sys\ioctl.h>
   #include "md5.h"
}

static BOOL PopInitialized = FALSE ;

static int (_System *p_connect) ( int, struct sockaddr *, int ) = 0 ;
static int (_System *p_ioctl) ( int, int, char *, int ) = 0 ;
static int (_System *p_recv) ( int, char *, int, int ) = 0 ;
static int (_System *p_select) ( int *, int, int, int, long ) = 0 ;
static int (_System *p_send) ( int, char *, int, int ) = 0 ;
static int (_System *p_socket) ( int, int, int ) = 0 ;
static int (_System *p_soclose) ( int ) = 0 ;

static unsigned long (_System *p_inet_addr) ( char * ) = 0 ;

static struct hostent * (_System *p_gethostbyname) ( char * ) = 0 ;
static struct hostent * (_System *p_gethostbyaddr) ( char *, int, int ) = 0 ;
static struct servent * (_System *p_getservbyname) ( char *, char * ) = 0 ;

static unsigned short (_System *p_bswap) ( unsigned short ) = 0 ;
#define p_htons(x) (p_bswap(x))

extern BOOL InitPop3 ( ) {

   if ( PopInitialized ) 
      return ( TRUE ) ;

   HMODULE Handle ;
   if ( DosLoadModule ( 0, 0, PSZ("SO32DLL.DLL"), &Handle ) ) 
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("CONNECT"), (PFN*)&p_connect ) )
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("IOCTL"),   (PFN*)&p_ioctl   ) )
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("RECV"),    (PFN*)&p_recv    ) )
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("SELECT"),  (PFN*)&p_select  ) )
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("SEND"),    (PFN*)&p_send    ) )
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("SOCKET"),  (PFN*)&p_socket  ) )
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("SOCLOSE"), (PFN*)&p_soclose ) )
      return ( FALSE ) ;

   if ( DosLoadModule ( 0, 0, PSZ("TCP32DLL.DLL"), &Handle ) ) 
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("INET_ADDR"), (PFN*)&p_inet_addr ) ) 
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("GETHOSTBYNAME"), (PFN*)&p_gethostbyname ) ) 
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("GETHOSTBYADDR"), (PFN*)&p_gethostbyaddr ) ) 
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("GETSERVBYNAME"), (PFN*)&p_getservbyname ) ) 
      return ( FALSE ) ;
   if ( DosQueryProcAddr ( Handle, 0, PSZ("BSWAP"), (PFN*)&p_bswap ) ) 
      return ( FALSE ) ;

   PopInitialized = TRUE ;

   return ( TRUE ) ;
}

static int
Transact( int      sock,
          char   * obuf,
          char   * ibuf,
          int      ilen )
/***********************/
{
   static char   buffer[ 4096 ];
   static char * bufptr = buffer;
   static char * bufend = buffer;
   int           bytes;
   int           socks[ 1 ];

   if (( obuf != NULL ) && *obuf )
   {
      int olen = strlen( obuf );

      if (( p_ioctl( sock, FIONREAD, ( caddr_t ) &bytes, sizeof( bytes )) >= 0 ) && bytes )
      {
         for ( ; ; )
         {
            socks[ 0 ] = sock;

            if ( p_select( socks, 1, 0, 0, 500 ) <= 0 )
            {
               break;
            }

            if ( p_recv( sock, buffer, sizeof( buffer ), 0 ) <= 0 )
            {
               break;
            }
         }
      }

      while ( olen > 0 )
      {
         bytes = p_send( sock, obuf, olen, 0 );
         if ( bytes <= 0 )
         {
            return ( bytes );
         }

         olen -= bytes;
         obuf += bytes;
      }
   }

   if (( ibuf != NULL ) && ( ilen > 0 ))
   {
      int    c;
      int    lastc = '\0';
      char * iptr  = ibuf;

      bufptr = bufend = buffer;

      *iptr = '\0';

      while ( --ilen > 0 )
      {
         if ( bufptr == bufend )
         {
            bytes = p_recv( sock, buffer, sizeof( buffer ), 0 );
            if ( bytes <= 0 )
            {
               *iptr = '\0';
               return ( bytes );
            }

            bufptr = buffer;
            bufend = buffer + bytes;
         }

         c = *bufptr++;

         if (( c == '\r' ) || ( c == '\0' ))
         {
            lastc = c;
            ++ilen;
            continue;
         }

         if (( c == '\n' ) && ( lastc == '\r' ))
         {
            lastc = '\n';
            break;
         }

         lastc = c;
         *iptr++ = c;
      }

      *iptr = '\0';

      if ( lastc != '\n' )
      {
         for ( ; ; )
         {
            socks[ 0 ] = sock;

            if ( p_select( socks, 1, 0, 0, 500 ) <= 0 )
            {
               break;
            }

            if ( p_recv( sock, buffer, sizeof( buffer ), 0 ) <= 0 )
            {
               break;
            }
         }
      }

      return ( *ibuf );
   }

   return ( '+' );
}

static BOOL
Logon( PSZ pszSrvr, int sock, BOOL bUseAPOP, char *szUser, char *szPassword )  
/**********************/
{
   CHAR szBuf[ 4096 ];

   if ( bUseAPOP )
   {
      char * p;

      p = strchr( pszSrvr, '<' );
      if ( p != NULL )
      {
         strcpy( szBuf, p );

         p = strchr( szBuf, '>' );
         if ( p != NULL )
         {
            MD5_CTX   context;
            int       n;

            *( p + 1 ) = '\0';

            strcat( szBuf, szPassword );

            MD5Init( &context );
            MD5Update( &context, (unsigned char*)szBuf, strlen( szBuf ));
            MD5Final( &context );

            p = szBuf;

            p += sprintf( p, "APOP %s ", szUser );

            for ( n = 0; n < 16; n++ )
            {
               p += sprintf( p, "%02.2x", context.digest[ n ] );
            }

            sprintf( p, "\r\n" );

            if ( Transact( sock, szBuf, szBuf, sizeof( szBuf )) == '+' )
            {
               return ( TRUE );
            }
         }
      }
   }

   sprintf( szBuf, "USER %s\r\n", szUser );

   if ( Transact( sock, szBuf, szBuf, sizeof( szBuf )) == '+' )
   {
      sprintf( szBuf, "PASS %s\r\n", szPassword );
   
      if ( Transact( sock, szBuf, szBuf, sizeof( szBuf )) == '+' )
      {
         return ( TRUE );
      }
   }

   return ( FALSE );
}

extern long
GetMsgCnt( USHORT usPort, char *szServer, char *szUser, char *szPassword, BOOL bUseAPOP, BOOL bForceIP )
/***********************/
{
   if ( !PopInitialized ) 
      return ( -1 ) ;

   struct sockaddr_in   sa  = { 0 };
   struct hostent     * he;
   CHAR                 szBuf[ 512 ];
   long                 lMsgCnt = 0;
   int                  noblock;
   u_long               ip = 0;

   sa.sin_family = AF_INET;

   if ( usPort != 0 )
   {
      sa.sin_port   = p_htons( usPort );
   }
   else
   {
      struct servent * pse = p_getservbyname( "pop3", "tcp" );
      if ( pse == NULL )
      {
         sa.sin_port = p_htons( 110 );
      }
      else
      {
         sa.sin_port = pse->s_port;
      }
   }
   
   he = p_gethostbyname( szServer );
   if ( he == NULL )
   {
      ip = p_inet_addr( szServer );

      he = p_gethostbyaddr(( char * ) &ip, sizeof( ip ), AF_INET );
      if ( he == NULL )
      {
         if  (( ip == 0 ) || ( ip == 0xffffffff ) || ( ! bForceIP ))
         {
            // Log ( "%s(%i) ERROR: Cannot resolve address of '%s'.", __FILE__, __LINE__, szServer ) ;
            return ( -5 ); /* can't resolve address */
         }
      }
   }

   if ( he != NULL )
   {
      sa.sin_addr.s_addr = *(( u_long * ) he->h_addr );
   }
   else
   {
      /* 
       * Use raw IP even though DNS lookup failed.
       * Only happens if bForceIP is TRUE.
       */
      sa.sin_addr.s_addr = ip;
   }

   int sock = p_socket( PF_INET, SOCK_STREAM, 0 );
   if ( sock == ( -1 ))
   {
      // Log ( "%s(%i) ERROR: Unable to create socket.", __FILE__, __LINE__ ) ;
      return ( -2 ); /* no socket */
   }

   noblock = 0;
   p_ioctl( sock, FIONBIO, ( caddr_t ) &noblock, sizeof( noblock ));

   if ( p_connect( sock, ( struct sockaddr * ) &sa, sizeof( sa )) >= 0 )
   {
      if ( Transact( sock, NULL, szBuf, sizeof( szBuf )) == '+' )
      {
         if ( Logon( szBuf, sock, bUseAPOP, szUser, szPassword ))
         {
            if ( Transact( sock, "STAT\r\n", szBuf, sizeof( szBuf )) == '+' )
            {
               // Log ( "%s(%i) STAT results: '%s'.", __FILE__, __LINE__, szBuf ) ;
               char *p = strchr( szBuf, ' ' );
               if ( p != NULL )
               {
                  lMsgCnt = strtol( p, NULL, 10 );
               }

               Transact( sock, "QUIT\r\n", szBuf, sizeof( szBuf ));
            }
            else
            {
               // Log ( "%s(%i) ERROR: STAT command failed.", __FILE__, __LINE__ ) ;
               lMsgCnt = ( -7 ); /* STAT failed */
            }
         }
         else
         {
            // Log ( "%s(%i) ERROR: Unable to log into post office.", __FILE__, __LINE__ ) ;
            lMsgCnt = ( -6 );
         }
      }
      else
      {
         // Log ( "%s(%i) ERROR: Server won't talk with us.", __FILE__, __LINE__ ) ;
         lMsgCnt = ( -4 ); /* server won't talk to us */
      }
   }
   else
   {
      // Log ( "%s(%i) ERROR: Unable to connect socket.", __FILE__, __LINE__ ) ;
      lMsgCnt = ( -3 ); /* no connect */
   }

   p_soclose( sock );
   sock = ( -1 );

   // Log ( "%s(%i) Result %i.", __FILE__, __LINE__, lMsgCnt ) ;
   return ( lMsgCnt );
}

