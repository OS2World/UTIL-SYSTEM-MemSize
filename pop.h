#ifndef POP_H
#define POP_H

extern BOOL InitPop3 ( ) ;

extern long GetMsgCnt ( USHORT usPort, char *szServer, char *szUser, char *szPassword, BOOL bUseAPOP, BOOL bForceIP ) ;

#endif // POP_H
