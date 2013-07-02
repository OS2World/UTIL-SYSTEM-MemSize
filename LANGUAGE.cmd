@Echo Off
If *%1==* Goto Help
If Exist OBJECTS.OS2\%1.dll Goto Install
If Exist OBJECTS.OS2\%1.hlp Goto Install
If Exist OBJECTS.W32\%1.dll Goto Install
If Exist OBJECTS.W32\%1.hlp Goto Install
Goto Help

:Install
If Exist OBJECTS.OS2\%1.dll Copy OBJECTS.OS2\%1.dll MEMSIZE2.dll 1>NUL
If NOT Exist OBJECTS.OS2\%1.dll Echo Language DLL for OS/2 not found!
If Exist OBJECTS.OS2\%1.hlp Copy OBJECTS.OS2\%1.hlp MEMSIZE2.hlp 1>NUL
If NOT Exist OBJECTS.OS2\%1.hlp Echo Language help file for OS/2 not found!
If Exist OBJECTS.W32\%1.dll Copy OBJECTS.W32\%1.dll MEMSIZEW.dll 1>NUL
If NOT Exist OBJECTS.W32\%1.dll Echo Language DLL for Win32 not found!
If Exist OBJECTS.W32\%1.hlp Copy OBJECTS.W32\%1.hlp MEMSIZEW.hlp 1>NUL
If NOT Exist OBJECTS.W32\%1.hlp Echo Language help file for Win32 not found!
Echo Language %1 activated.
Goto Exit

:Help
Echo You forgot to specify a valid language!

:Exit

