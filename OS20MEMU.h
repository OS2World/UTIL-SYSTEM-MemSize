
/******************************************************************************/
/*                                                                            */
/* Filename: OS20MEMU.H                                                       */
/*                                                                            */
/* Version:  1.0                                                              */
/*                                                                            */
/* Purpose:  Header file and definition of variables returned from the        */
/*           OS20MEMU dynalink - OS20MEMU.DLL                                 */
/*                                                                            */
/* Author: Glenn Brew & Ross Cook                                             */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*              Function prototype declarations                               */
/******************************************************************************/

    /**************************************************************************/
    /* The EP16_GET_GENERAL_INFO function returns a snapshot of memory.       */
    /*                                                                        */
    /* It should be called at any time it is felt the state of system memory  */
    /* has changed such as following the allocation/reallocation of any       */
    /* private data segments.  Also, when programs have started or stopped.   */
    /*                                                                        */
    /* The only parameter to this function is a pointer to a structure of     */
    /* type 'os20memu_general_info'.                                          */
    /*                                                                        */
    /**************************************************************************/
    // USHORT far pascal
    //     EP16_GET_GENERAL_INFO( struct os20memu_general_info far *pgi)


    /**************************************************************************/
    /* The EP16_GET_PROCESS_INFO function returns detailed data for a process.*/
    /*                                                                        */
    /* It may be called at any time to obtain several data items for a        */
    /* particular process.                                                    */
    /*                                                                        */
    /* The input to this function is a process ID.  Typically, this would be  */
    /* a process ID returned on a previous call to EP16_GET_GENERAL_INFO.     */
    /* The process must be active or an error will be returned.               */
    /*                                                                        */
    /* The output from this function is described in the structure of type    */
    /* 'os20memu_process_details'.                                            */
    /*                                                                        */
    /**************************************************************************/
    // USHORT far pascal
    //     EP16_GET_PROCESS_INFO( USHORT pid,
    //                            struct os20memu_process_details far *ppi);

/*******************/
/*   DLL Version   */
/***********************************************************/
/* Note: See "gi_version" below for more information.      */
/***********************************************************/

#define OS20MEMU_VERSION        0x0001  /* Version Number   */


/******************************/
/*   DLL Return code values   */
/******************************/
                                        /*====================================*/
#define ERROR_UNKNOWN_KERNEL    0xffff  /* The OS/2 system presently in use   */
                                        /* is not 'known' by this DLL.        */
                                        /*                                    */
                                        /* An updated DLL is required.        */
                                        /*                                    */
                                        /* This is a terminal error - there   */
                                        /* there is no recovery and no need   */
                                        /* to retry in the calling program.   */
                                        /*                                    */
                                        /*====================================*/

                                        /*====================================*/
#define ERROR_FAULT_OCCURRED    0xfffe  /* A fault occurred while scanning    */
                                        /* the OS/2 system control blocks.    */
                                        /*                                    */
                                        /* The request should be reissued.    */
                                        /*                                    */
                                        /* It may be necessary to increase    */
                                        /* the thread priority of the calling */
                                        /* thread to prevent this error from  */
                                        /* re-occurring.                      */
                                        /*                                    */
                                        /*====================================*/


/*****************************************/
/*   Structure / data area definitions   */
/*****************************************/

/******************************************************************************/
/*   Structure of high level process memory usage data.                       */
/*   An array of these structures is returned at the last entry of the        */
/*   os20memu_general_info structure; see gi_process_data.                    */
/*   That array is variable length and its size should be set = gi_cpids      */
/******************************************************************************/

struct  gi_process_data_s {
   USHORT  pd_pid;              // process id
   USHORT  pd_ppid;             // parent process id
   ULONG   pd_cpriv_im;         // count of private pages in memory
   ULONG   pd_cpriv_od;         // count of private pages on disk
   ULONG   pd_cpriv_nt;         // count of private pages not accessed
   ULONG   pd_cshr_im;          // count of shared pages in memory for process
   ULONG   pd_cshr_od;          // count of shared pages on disk for process
   ULONG   pd_cshr_nt;          // count of shared pages not accessed for proc.
   UCHAR   pd_name[14];         // name of process

};

/******************************************************************************/
/*   Structure of general information.                                        */
/*   A pointer to this structure is provided on a EP16_GET_GENERAL_INFO call  */
/*   and the structure is returned with all data filled in.                   */
/******************************************************************************/

struct os20memu_general_info {
   ULONG   gi_version;          // version

   ULONG   gi_phys_mem;         // amount of physical storage, in pages
   ULONG   gi_free_mem;         // count of free pages

   ULONG   gi_tot_res;          // total resident (fixed) memory
                                //  not including locked

   ULONG   gi_res_sys_mem;      // total number of resident pages contained
                                //  within the system arena

   ULONG   gi_swp_sys_mem;      // total number of swapped out pages that
                                //  reside in the system arena
                                //  NOTE: subset of gi_ondisk_swp

   ULONG   gi_swappable_sys_mem;// total number of swappable pages that are
                                //  currently in memory that are located
                                //  within the system arena
                                //  NOTE: subset of gi_inmem_swp

   ULONG   gi_inmem_swp;        // total number of swappable pages that are
                                //  currently in memory includes both
                                //  system and user pages

   ULONG   gi_inmem_dsc;        // total number of discardable pages that are
                                //  currently in memory includes both
                                //  system and user pages

   ULONG   gi_ondisk_dsc;       // total number of discardable pages that are
                                //  not in memory

   ULONG   gi_ondisk_swp;       // total number of swappable pages that are
                                //  not in memory

   ULONG   gi_notacc_swp;       // total number of swappable pages that were
                                //  allocated (with the commit bit set) but
                                //  have never been accessed

   ULONG   gi_slock_swp;        // short term locks on swappable pages, in pgs
                                //  NOTE: subset of inmem_swp

   ULONG   gi_slock_dsc;        // short term locks on discardable pages, in pgs
                                //  NOTE: subset of inmem_dsc

   ULONG   gi_llock_swp;        // long term locks on swappable pages, in pages
                                //  NOTE: subset of inmem_swp

   ULONG   gi_llock_dsc;        // long term locks on discardable pages, in pgs
                                //  NOTE: subset of inmem_dsc

   // swapper info
   ULONG   gi_swapfile_size;    // size of swapfile in pages
   ULONG   gi_swapfile_inuse;   // number of pages with valid information
   ULONG   gi_swapfile_max_inuse;// maximum inuse page


   // process info
   ULONG   gi_app_sys_mem_res;  // total amount of resident system memory
                                //  used to run all processes (IN BYTES)
                                //  NOTE: subset of res_sys_mem

   ULONG   gi_app_sys_mem_im;   // total amount of in memory system storage
                                //  used to run all processes (IN BYTES)
                                //  NOTE: subset of swappable_sys_mem

   ULONG   gi_app_sys_mem_od;   // total amount of swapped system storage
                                //  used to run all processes (IN BYTES)
                                //  NOTE: subset of swp_sys_mem
   // shared arena info
   ULONG   gi_shr_sys_mem_res;  // total amount of resident system memory
   ULONG   gi_shr_sys_mem_im;   // total number of shared pages in memory
   ULONG   gi_shr_sys_mem_od;   // total number of shared pages in memory

   ULONG   gi_cshr_im;          // total number of shared pages in memory
                                //  NOTE: subset of inmem_swp and inmem_dsc

   ULONG   gi_cshr_od;          // total number of shared pages on disk
                                //  NOTE: subset of ondisk_swp and ondisk_dsc

   ULONG   gi_cshr_nt;          // total number of shared pages not accessed
                                //  NOTE: subset of notacc_swp

   ULONG   gi_cpids;            // count of processes in gi_process_data array

   // per process info
   struct  gi_process_data_s gi_process_data[1]; // array of process records
                                // The number of entries in this array is
                                // variable and its contents is complete only
                                // if its size is => gi_cpids.
};


/******************************************************************************/
/*   Structure of the detail record of a memory object.                       */
/*   An array of these structures is returned at the last entry of the        */
/*   os20memu_process_details; see pi_object_data.                            */
/*   That array is variable length and its size should be set = the sum of    */
/*   pi_cprivate + pi_cshared.                                                */
/******************************************************************************/


struct pi_object_data_s {
   ULONG   od_linaddr;          // linear address of object
   ULONG   od_flags;            // object flags
   ULONG   od_size;             // size of object in pages
   ULONG   od_inmem;            // number of pages in memory
   ULONG   od_ondisk;           // number of pages on  disk
   ULONG   od_notacc;           // number of pages that were not accessed
   UCHAR   od_owner_name[14];   // name of executable that allocated the object
};

struct os20memu_process_details {
   ULONG   pi_sys_memory_used_res;// current amount of resident system
                                //  memory being used to run this process
                                //  (IN BYTES)

   ULONG   pi_sys_memory_used_im;// current amount of in memory swappable
                                //  system storage being used to run
                                //  this process (IN BYTES)

   ULONG   pi_sys_memory_used_od;// current amount of swapped system memory
                                //  being used to run this process
                                //  (IN BYTES)

   USHORT  pi_cprivate;         // On return, this value will reflect the
                                // number of objects in the private arena
                                //
                                // This variable must be set to the number of
                                // entries in the pi_object_data array before
                                // issuing an EP16_GET_PROCESS_INFO request.

   USHORT  pi_cshared;          // number of objects in the shared arena

   struct pi_object_data_s pi_object_data[1]; // array of object data records
                                // The number of entries in this array is
                                // variable and its contents is complete only
                                // if its size is => the sum of
                                // pi_cprivate + pi_cshared.
};

#define OB_DISCARDABLE 0x40000  // Discardable Object
#define OB_SWAPPABLE   0x20000  // Swappable Object
#define OB_RESIDENT    0x10000  // Resident Object

#define OB_LALIAS       0x1000  // Object has aliases
#define OB_SHARED       0x0800  // Object's contents are shared
#define OB_UVIRT        0x0400  // UVirt object
#define OB_ZEROINIT     0x0200  // Object is zero-initialized
#define OB_RES_INIT     0x0100  // Initial allocation was resident
#define OB_LOWMEM       0x0040  // Object is in low memory

#define OB_GUARD        0x0080  // Page attribute/permission flags
#define OB_EXEC         0x0020
#define OB_READ         0x0010
#define OB_USER         0x0008
#define OB_WRITE        0x0004

