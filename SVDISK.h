// Definitions for accessing Albert Shan's SVDisk device driver.

#ifndef SVDISK_H
#define SVDISK_H

#ifdef __OS2__

#define SVDISKKEY (0x8ACE4007)
#define SVD_QUERYSTATUS (0xFF)

#pragma pack(1)

   typedef struct { 
      USHORT BytesPerSector; 
      UCHAR  SectorsPerCluster; 
      USHORT ReservedSectors; 
      UCHAR  NumberOfFATs; 
      USHORT RootEntries; 
      USHORT TotalSectors; 
      UCHAR  MediaDescriptor; 
      USHORT SectorsPerFAT; 
      USHORT SectorsPerTrack; 
      USHORT Heads; 
      ULONG  HiddenSectors; 
      ULONG  BigTotalSectors; 
      UCHAR  Reserved[6]; 
   } BPB; 
   
   typedef struct { 
      BPB    DeviceBPB; 
      USHORT NumberOfCylinders; 
      UCHAR  DeviceType; 
      USHORT DeviceAttributes; 
   } DEVICEPARM; 

   typedef struct _SVD {
      ULONG Key ;
      USHORT Command ;
      USHORT Version ;
      ULONG MediaSize ;
      ULONG DeviceSize ;
      ULONG LockBase ;
      ULONG LockSize ;
      USHORT Error ;
      UCHAR Dummy ;
      DEVICEPARM DeviceParm ;
   } SVD, *PSVD ;

#pragma pack()

#ifdef __cplusplus

   inline BOOL IsSVDisk ( UCHAR Unit ) {
      UCHAR Parms [2] = { 0, (UCHAR)(Unit-1) } ;
      SVD Disk ;
      Disk.Command = SVD_QUERYSTATUS ;
      Disk.Version = 0xFFFF ;
      Disk.Key = SVDISKKEY ;
      ULONG ParmLen = sizeof(Parms), DataLen = sizeof(Disk) ;
      APIRET Result = DosDevIOCtl ( -1, IOCTL_DISK, DSK_GETDEVICEPARAMS, 
         (PVOID)Parms, ParmLen, &ParmLen, (PVOID)&Disk, DataLen, &DataLen ) ;
      if ( Result ) 
         return ( FALSE ) ;
      if ( Disk.Key != SVDISKKEY ) 
         return ( FALSE ) ;
      return ( TRUE ) ;
   } /* endfunction */

#endif // If C++

#endif // If OS/2

#endif // If not already included.
