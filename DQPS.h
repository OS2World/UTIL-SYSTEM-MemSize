/********************************************************************* DQPS.H
 *                                                                          *
 *                        DosQProcStatus Definitions                        *
 *                                                                          *
 ****************************************************************************/

#ifndef DQPS_H
#define DQPS_H

#ifdef __OS2__

extern "C" APIRET16 APIENTRY16 DosQProcStatus ( PULONG buffer, USHORT buffer_size );

typedef struct qsGrec_s {
   ULONG cThrds;                /* number of threads in use */
   ULONG Reserved1;
   ULONG Reserved2;
} qsGrec_t;

typedef struct qsTrec_s {
   ULONG RecType;               /* 00: Record Type */
                                /*     Thread rectype = 0x00000100 */
   USHORT tid;                  /* 04: thread ID */
   USHORT slot;                 /* 06: "unique" thread slot number */
   ULONG sleepid;               /* 08: sleep id thread is sleeping on */
   ULONG priority;              /* 0C: thread priority */
   ULONG systime;               /* 10: thread system time */
   ULONG usertime;              /* 14: thread user time */
   UCHAR state;                 /* 18: thread state */
   UCHAR padchar ;              /* 19: */
   USHORT padshort ;            /* 1A: */
} qsTrec_t;                     /* Total size: 28 bytes */

typedef struct qsPrec_s {
   ULONG RecType;               /* 00: type of record being processed */
                                /*     process rectype = 0x00000001   */
   qsTrec_t * _Seg16 pThrdRec;  /* 04: ptr to 1st thread rec for this prc */
   USHORT pid;                  /* 08: process ID */
   USHORT ppid;                 /* 0A: parent process ID */
   ULONG type;                  /* 0C: process type */
   ULONG stat;                  /* 10: process status */
   ULONG sgid;                  /* 14: process screen group */
   USHORT hMte;                 /* 18: program module handle for process */
   USHORT cTCB;                 /* 1A: # of TCBs in use in process */
   ULONG Reserved1 ;
   ULONG Reserved2 ;
   USHORT c16Sem;               /* 24: # of 16 bit system sems in use by proc */
   USHORT cLib;                 /* 26: number of runtime linked libraries */
   USHORT cShrMem;              /* 28: number of shared memory handles */
   USHORT Reserved3 ;
   USHORT * _Seg16 p16SemRec;   /* 2C: ptr to head of 16 bit sem inf for proc */
   USHORT * _Seg16 pLibRec;     /* 30: ptr to list of runtime lib in use by process */
   USHORT * _Seg16 pShrMemRec;  /* 34: ptr to list of shared mem handles in use by process */
   ULONG Reserved4 ;            /* 38: */
} qsPrec_t;                     /* Total size: 60 bytes */

typedef struct qsS16Headrec_s { 
   ULONG SRecType;              /* semaphore rectype = 3 */
   ULONG Reserved1;             /* overlays NextRec of 1st qsS16rec_t*/
   ULONG Reserved2;
   ULONG S16TblOff;             /* index of first semaphore,SEE PSTAT OUTPUT*/
                                /* System Semaphore Information Section     */
} qsS16Headrec_t;
 
typedef struct qsS16rec_s { 
   ULONG NextRec;               /* offset to next record in buffer*/
   UINT  s_SysSemOwner ;        /* thread owning this semaphore   */
   UCHAR s_SysSemFlag ;         /* system semaphore flag bit field*/
   UCHAR s_SysSemRefCnt ;       /* number of references to this   */
                                /*   system semaphore             */
   UCHAR s_SysSemProcCnt ;      /*number of requests by sem owner*/
   UCHAR Reserved1;
   ULONG Reserved2;
   UINT  Reserved3;
   CHAR  SemName;               /* start of semaphore name string */
} qsS16rec_t;
 
 
typedef struct qsMrec_s { 
   ULONG  MemNextRec;           /* offset to next record in buffer */
   USHORT hmem;                 /* handle for shared memory */
   USHORT sel;                  /* shared memory selector */
   USHORT refcnt;               /* reference count */
   CHAR   Memname;              /* start of shared memory name string */
} qsMrec_t;
 
typedef struct qsLrec_s { 
   void * _Seg16 pNextRec;      /* pointer to next record in buffer */
   USHORT hmte;                 /* handle for this mte */
   USHORT Reserved1;            /* Reserved */
   ULONG  ctImpMod;             /* # of imported modules in table */
   ULONG  Reserved2;            /* Reserved */
   ULONG  Reserved3;            /* Reserved */
   UCHAR * _Seg16 pName;        /* ptr to name string following stru*/
} qsLrec_t;
 
typedef struct qsPtrRec_s {
   qsGrec_t *pGlobalRec;        /* ptr to the global data section */
   qsPrec_t *pProcRec;          /* ptr to process record section  */
   qsS16Headrec_t *p16SemRec;   /* ptr to 16 bit sem section      */
   qsMrec_t *pShrMemRec;        /* ptr to shared mem section      */
   qsLrec_t *pLibRec;           /* ptr to exe module record section */
} qsPtrRec_t;

#endif

#endif
