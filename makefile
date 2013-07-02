# Makefile for use with WATCOM C/C++

.SILENT
.ERASE
.EXTENSIONS:

ALL : OS2 W32 .SYMBOLIC
    %NULL

OS2 : .SYMBOLIC
    Cls
    Echo Building version for OS/2 . . .
    -MD OBJECTS.OS2 2>NUL
    CD OBJECTS.OS2
    SET CPP_OPTIONS=/D__OS2__
    SET INCLUDE=$(%WATCOM)\H\OS2;$(%WATCOM)\H;$(%WATCOM)\SOM\INCLUDE;$(%TOOLKIT)\SOM\INCLUDE;$(%TOOLKIT)\H;$(%TOOLKIT)\INC;$(%TCPTK)\INCLUDE;$(%THESEUS);
    SET LIB=$(%TOOLKIT)\LIB;$(%TOOLKIT)\SOM\LIB;$(%WATCOM)\SOM\LIB;$(%TCPTK)\LIB;
    SET RC=rc
    SET RCFLAGS=-d __OS2__ -i ..
    SET SYSTEM=os2
    wmake $(__MAKEOPTS__) -h -f ..\makefile.os2
    CD ..

W32 : .SYMBOLIC
    Cls
    Echo Building version for Win32 . . .
    -MD OBJECTS.W32 2>NUL
    CD OBJECTS.W32
    SET CPP_OPTIONS=/D__NT__
    SET INCLUDE=$(%WATCOM)\H\NT;$(%WATCOM)\H;
    SET LIB=
    SET RC=wrc
    SET RCFLAGS=-q -ad -d__NT__ -i=.. -bt=nt
    SET SYSTEM=nt
    wmake $(__MAKEOPTS__) -h -f ..\makefile.w32
    CD ..

