#include "memsize.h"                                
:userdoc.
:title.&apos.Ajuda de Recursos del Sistema OS/2&apos.
:body.

:h1 res=ID_MAIN.Introducci¢
:i1.Introducci¢
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
Aquest programa mostra per pantalla diferents informacions sobre
els recursos del sistema, i les actualiza una vegada per segon,
si es que el processador tÇ temps per a fer-ho. Les informacions
mostrades s¢n&colon.
:p.
:hp2.Data/Hora:ehp2. - La data i hora actual, en el format normal
de cada pa°s, depenent de com estigui configurada la l°nia &apos.COUNTRY=&apos. 
al fitxer CONFIG.SYS
:p.
:hp2.Temps transcorregut:ehp2. - El temps que ha passat des de la darrera
ocasi¢ en que s&apos.ha arrencat l&apos.ordinador.
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.Physical Memory Free:ehp2. - La quantitat de memïria lliure del
sistema, d&apos.acord amb allï que la funci¢ :hp1.DosMemAvail:ehp1. indica.
:note.This function is part of the old 16-bit system, and only reports
physical memory below 16Mb, and was also altered just before
OS/2 2.0 was released to never report less than 512K.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Virtual Memory Free:ehp2. - The amount of virtual memory
available, according to the :hp1.DosQuerySysInfo:ehp1. function.
This is the amount of free physical memory, plus the amount of
free space within the current swapping file, plus the amount of
space by which the swapping file could yet grow, less the amount
of free space reserved on the swapping drive.
:p.
:hp2.Tamany del fitxer d&apos.intercanvi:ehp2. - El tamany que ocupa al disc el
fitxer de memïria virtual d&apos.intercanvi (swap), SWAPPER.DAT. Per
localitzar aquest fitxer, busqueu al fitxer CONFIG.SYS la l°nia que
comenáa amb SWAPPATH, on trobareu indicat el nom complet (inclïs el
directori) del fitxer d&apos.intercanvi, aix° com l&apos.espai que cal reservar a
la unitat on es troba situat aquest fitxer.
:p.
:hp2.Espai disponible pel fitxer d&apos.intercanvi:ehp2. - La quantitat total
d&apos.espai a la unitat de disc on es troba el fitxer d&apos.intercanvi, menys la
quantitat d&apos.espai que cal reservar. Aquest valor Çs el tamany mÖxim que
pot utilitzar el fitxer d&apos.intercanvi.
:p.
:hp2.Unused Swap Space:ehp2. - The amount of free space within the currently
allocated system swap file.  When this drops to zero, the system swap file
must be increased in size, normally by one megabyte at a time.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Tamany del fitxer d&apos.spool:ehp2. - La quantitat total de disc consumida
pels fitxers enviats a l&apos.spooler d&apos.impressora.
:p.
:hp2.Percentatge d&apos.£s de la CPU:ehp2. - El percentatge aproximat d&apos.£s
del processador. El valor que es mostra Çs el percentatge corresponent
al promig d&apos.utilitzaci¢ del darrer segon.
:note.Under versions of OS/2 before release 4.0, aquesta funci¢ i el PULSE que s&apos.inclou amb l&apos.OS/2 2.0, no s¢n gaire compatibles.
:link reftype=hd res=60001.Why?:elink.
:note.Under versions of OS/2 before release 4.0, if you have the driver :link reftype=hd res=60000.HRTIMER.SYS:elink.  
installed, this number will be more accurate.
:link reftype=hd res=60002.Why?:elink.
:p.
:hp2.Average Load (%):ehp2. - The approximate percentage of the CPU&apos.s available
power that has been used in the recent past.  The number of seconds to be 
averaged may be configured by the user 
(see the :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Configuration:elink. dialog).
:p.
:hp2.Battery:ehp2. - The current battery capacity remaining,
or its status (Charging, AC) is displayed.
:p.
:hp2.N£mero de tasques actives:ehp2. -
El n£mero d&apos.entrades a lliista de finestres, que Çs la llista de veieu quan
premeu CTRL-ESC.
:note.No totes les tasques del sistema s¢n mostrades a la Llista de
Finestres, donat que algunes tasques no s¢n visibles.
:p.
:hp2.Process Count:ehp2. - The number of processes currently loaded
on the system.
:p.
:hp2.Thread Count:ehp2. - The number of threads currently running
on the system.  There is at least one thread for each process.
:p.
:hp2.Espai lliure total:ehp2. - La suma de l&apos.espai lliure a totes les
unitats locales de disc no removibles.
:p.
:hp2.Espai lliure a la unitat X:ehp2. - La quanitat total d&apos.espai lliure a
la unitat de disc X.
:p.
Aquesta ajuda Çs sensible al contexte, com ja se n&apos.haureu adonat.
L&apos.accÇs a les segÅents funcions es fa a travÇs del men£ de sistema de
la finestra&colon.
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.Restaurar els valors per defecte:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.Amagar els controls:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.Configuraci¢...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.Reset Load Meter:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.Reset Drives:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Launch E-Mail Reader:elink.
:li.:link reftype=hd res=IDM_COPY.Copy:elink.
:li.:link reftype=hd res=IDM_ABOUT.Informaci¢ del programa:elink.
:li.:link reftype=hd res=IDM_EXIT.Close:elink.
:esl.:p.
A mÇs d&apos.aquestes funcions descrites, el programa accepta ordres dels
controladors de fonts i de la paleta de colors de l&apos.OS/2 2.0, 
and is a DDE server for all the currently displayed items.  
The DDE application name is MEMSIZE, and the item names are the same 
as the default display item names (like Physical Memory Free).

:h1 res=IDM_RESET_DEFAULTS.Restaurar els valors per defecte (Opci¢ del men£)
:i1.Restaurar els valors per defecte (Opci¢ del men£)
Seleccionant aquesta opci¢ del men£, es restaura el tipus de lletra i
els atributs de color als seus valors per defecte.

:h1 res=IDM_HIDE_CONTROLS.Amagar els controls (Opci¢ del men£)
:i1.Amagar els controls (Opci¢ del men£)
Aquesta opci¢ del men£, quan estÖ seleccionada, fa que els controls de
la finestra del programa (el men£ de sistema, la barra del t°tol i el bot¢
de minimitzaci¢) s&apos.amagin. ês possible variar el valor d&apos.aquesta funci¢
si premeu dues vegades el bot¢ del mouse dintre de la finestra.
:p.
La tecla acceleradora assignada a aquesta funci¢ Çs ALT+H.

:h1 res=IDM_CONFIGURE.Configurar (Opci¢ del men£)
:i1.Configurar (Opci¢ del men£)
Quan seleccioneu aquesta opci¢ del men£ es visualitzar† la pantalla
de configuraci¢ del programa.
For more information, see :link reftype=hd res=IDD_CONFIGURE.Configure (Dialog):elink..
:note.La tecla acceleradora assignada a aquesta funci¢ Çs Alt+C.

:h2 res=IDD_CONFIGURE.Configure (Dialog)
This notebook dialog allows you to change all the configurable 
aspects of this program&apos.s operations.  The configuration options
are grouped by category on the following pages&colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.Elements:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.Options (Page 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Options (Page 2):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.Colors:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.Anchor:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Date Format:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (Page 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (Page 2):elink.
:esl.:p.
All changes take effect immediately.

:h3 res=IDD_CONFIG_PAGE_ITEMS.Elements (Configuration Page)
This page displays a list of displayable items.
Podeu seleccionar qualsevol, o tots, els elements que voleu que
s&apos.incloguin a la finestra del programa. NomÇs heu de marcar els elements
desitjats fent un click al seu damunt per marcar-lo, o bÇ desactivar-los
amb un segon click. Tots els elements marcats s¢n els que es visualitzaran.
To change an item&apos.s default label, or to adjust its warning and/or error 
thresholds, either double-click mouse button one on the item list entry, or single-click
mouse button two on the entry.  In either case, the 
:link reftype=hd res=IDD_LABEL.Item Attribute:elink. dialog will be displayed.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.Options (Page 1) (Configuration Page)
This page shows a number of program options that may be
activated or deactivated.
:p.
:hp2.Amagar els controls:ehp2.
Quan el programa no estÖ en forma d&apos.icona, el podeu veure amb o sense els
elements normals de les finestres&colon. barra de t°tol, men£ de sistema i els
botons de maximitzar i minimitzar. If you chose to hide these controls,
the window may still be positioned by dragging its center area.
:p.
:hp2.Restar en primer pla:ehp2.
Aquest bot¢ activa la funci¢ que fa restar la finestra del programa
sempre en primer pla. Quan aquesta funci¢ estÖ seleccionada, la finestra
del programa estarÖ sempre al primer pla de visualitzaci¢, de forma que
sigui sempre visible.
:p.
:hp2.Animaci¢:ehp2.
Aquest bot¢ activa les funci¢ d&apos.animaci¢ a l&apos.obrir i tancar la finestra.
Quan estÖ activa, la finestra del programa s&apos.obrirÖ de la mateixa forma
que ho fan les carpetes&colon. amb so i moviment. De la mateixa forma que amb
les carpetes, no hi haurÖ cap animaci¢ si ha estat deshabilitada a
nivell de sistema.
:p.
:hp2.Dialog Border:ehp2.
This button determines whether the main program window will have 
a simple line border, or a more elaborate dialog border.  The line
border requires less space on the screen, but the dialog border can
look better, especially on OS/2 Warp 4.0 or later, which gives this
border style a three-dimensional appearance.
:p.
:hp2.Table Format:ehp2.
This button, when checked, causes the window to arrange itself in 
table format, with the display items arranged in rows and columns.
It will optimize itself to use as little vertical space as possible.
When this button is not checked, the window will arrange itself
with a single column of all the displayed items.
:p.
:hp2.Hourly Chime:ehp2.
This button activates an hourly chime feature.  When active, the
program will beep twice at the top of every hour.
:p.
:hp2.Show Disk Drives:ehp2.
When this button is checked, disk drive free space is monitored, subject
to the restrictions placed on this monitoring by other options.  
This control mainly exists as a way to disable disk monitoring.
:p.
:hp2.Show Remote Drives:ehp2.
When this button is checked, remote (network) drives that are currently
attached to the system are monitored in the same was as local nonremovable
drives are, except that they are not included in the Total Free Disk Space
item.
:p.
:hp2.Mostrar el nom del sistema de fitxers:ehp2.
Si aquest bot¢ estÖ marcat s&apos.indicarÖ, a mÇs d&apos.indicar l&apos.espai
lliure a la unitat el nom del sistema de fitxers que utilitza la
mateixa. L&apos.£s d&apos.aquesta opci¢ comporta un tamany de finestra mÇs
ampla.
:p.
:hp2.Show Disk Labels:ehp2.
This button causes the individual drive free space entries to show their
disk labels.  Use of this option results in a wider monitor window.
:p.
:hp2.Show Seconds:ehp2.
This button causes the clock and elapsed time displays to include the
seconds of the minute.
:p.
:hp2.Use Theseus/2:ehp2.
This button causes the program to make use of IBM's utility program Theseus/2,
if it is present, for such things where it provides more accurate information
than can be found otherwise.  Due to the large overhead of using it, however,
Theseus/2 never polled more than once every ten seconds.  Currently, this 
feature provides better measurement of free physical memory and unused swap
space.
:p.
:hp2.Center Clock:ehp2.
This button causes the clock display to be centered within its allocated
cell, rather than left-justified.

:h3 res=IDD_CONFIG_PAGE_OPTIONS2.Options (Page 2) (Configuration Page)
This page shows additional program options that are more complex
than simply on/off.
:p.
:hp2.Show &apos.K&apos.ilobytes:ehp2.
This group of radiobuttons allows you to select whether large sizes
are displayed in kilobyte notation always, never, or only when their
values exceed 512K.  
The default is to show all sizes over 512K in this way.  
.br
The &apos.K=1024&apos. button, if checked, causes the numbers to be displayed
in true K (1024).  If not checked, the numbers are displayed in
decimal K (1000).
.br
The &apos.M&apos.egabytes button, if checked, will cause the numbers to be
displayed in megabytes once their values exceed eight megabytes.
.br
The &apos.G&apos.igabytes button, if checked, will cause the numbers to be
displayed in gigabytes once their values exceed four gigabytes.
:p.
:hp2.Prioritat de monitoritzaci¢:ehp2.
Aquesta opci¢ us permetrÖ establir el valor de prioritat del thread
de monitoritzaci¢, dins la categoria de temps-cr°tic. Podeu variar
la prioritat entre 0 i 31. La prioritat 31 Çs el valor mÇs elevat al
sistema per sota de les interrupcions f°siques. La prioritat 0 Çs el
valor mÇs baix dins la categoria de temps-cr°tic. Normalment, aquest
valor ha d&apos.estar configurat al mÖxim per a obtenir un valor mÇs
acurat. No obstant aixï, alguns usuaris han informat que,
ocasionalment, perden algunes dades a les comunicaciones que es fan
mentres el programa estÖ en funcionament. ês per aquesta ra¢ que la
prioritat Çs ara configurable.
:p.
:hp2.Interval de temps:ehp2.
Aquesta opci¢ us permetrÖ establir l&apos.interval d&apos.actualitzaci¢ de la
finestra. Pot variar entre 1.0 segon i 30.0 segons, en increments
d&apos.una däcima de segon. El n£mero que es mostra estÖ en däcimes de
segon.
:p.
:hp2.Averaging Interval:ehp2.
This control allows you to set the interval over which averages are
computed.  It can be varied from 10 seconds to 3600 seconds (one hour).

:h3 res=IDD_CONFIG_PAGE_COLORS.Colors (Configuration Page)
This page shows the three sets of colors associated with the Normal, Warning
and Error levels of those display items which support them.  You can set these
colors by dragging colors over from one of the system color palettes.
:note.Under OS/2 Warp 3.0 or beyond, 
if you click on the color samples with either mouse button, the 
system&apos.s Solid Color Palette window will be displayed for 
your convenience.  The Mixed Color Palette can also be used,
but must be requested from the System Setup folder.

:h3 res=IDD_CONFIG_PAGE_ANCHOR.Anchor (Configuration Page)
This page contains a control with which you can set the corner
of the monitor window which remains fixed in position when the 
window size changes.  The monitor window can change in size when 
items are added or removed from the active list, and when the 
display font is changed from the Font Palette.

:h3 res=IDD_CONFIG_PAGE_DATEFMT.Date Format (Configuration Page)
This page allows you to specify the format with which the date &amp. time
item will be displayed.  Any text you would like may be entered, and
special function codes can be included.  The entire list of valid 
function codes is provided in a list at the bottom of the page, and
by double-clicking on any item in that list the item is immediately
inserted into the current format text.  A sample result text, with 
the function codes expanded, is given right below the format text
entry field.

:h3 res=IDD_CONFIG_PAGE_POP31.E-Mail (Configuration Page)
This page allows you to specify the parameters by which the Message Count
item obtains its information.  You must specify a POP3 server name, the
user name and the password for the user.  You can optionally specify
a POP3 port number if your server uses a different one from the customary.
You can also force the usage of a specific IP address.
:p.The option to use APOP allows you to use an enhanced password validation
protocol which reduces the exposure your ID and password get on the open
network.
:p.Once you have entered the server name or address, the user name and the
password, you can click on the <Validate> button to test.  You will be told
whether or not the e-mail account could be checked, and either how many
messages are waiting, or what went wrong.  To save the new parameters,
click on the <Accept> button.  To reset any changes made since the last
touch of the <Accept> button, click on <Reset>.
:p.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.Next Page...:elink.

:h3 res=IDD_CONFIG_PAGE_POP32.E-Mail (Configuration Page)
This page allows you to specify the parameters by which the Message Count
item reacts to its information.
:p.The Polling Interval field allows you to set the number of seconds 
to delay between each check of the post-office for new mail.
:p.The E-Mail Reader field is for specifying the command line to be used
to launch your preferred mailer program.
:p.The Alert When Mail Arrives checkbox determines whether or not the
program sounds an audible alert when new mail arrives.
:p.The Mail Alert Sound field is for specifying a .WAV file to be played
upon the arrival of new mail.  If no WAV file is specified, and sound
is requested, then an ascending sequence of tones will be generated
from the speaker.

:h3 res=IDD_LABEL.Item Attributes (Dialog)
This dialog box displays the current and default labels for a display item,
along with the current Warning and Error levels, if that feature is enabled
for the current item.  To change the label, simply type the new label over 
the current one.  To change the error or warning levels, either enter a new
level in the entryfield, or move the slider associated with the level.
:p.To restore all values to their defaults, click on the Default button.

:h1 res=IDM_RESETLOAD.Reset Load Meter (Menu Option)
:i1.Reset Load Meter (Menu Option)
This menu option allows the user to reset the CPU Load meter,
recalibrating it, causing it to forget previous CPU activity in its
calculations.

:note.This menu option is not available when on Warp 4.0 or better,
nor on certain higher fixpack levels of Warp 3.0, as the newer load
measuring technique makes this command irrelevant.

:h1 res=IDM_RESETAVERAGE.Reset Average Load (Menu Option)
:i1.Reset Average Load (Menu Option)
This menu option allows the user to reset the Average Load meter,
causing it to forget previous CPU activity in its calculations.
:p.
The short-cut key for this command is Alt+R.

:h1 res=IDM_RESETDRIVES.Reset Drives (Menu Option)
:i1.Reset Drives (Menu Option)
This menu option allows the user to reset any drive errors
that might be present.  This will happen automatically over
time, but the command allows the user to reset the errors without delay.

:h1 res=IDM_LAUNCHEMAILER.Launch E-Mail Reader (Menu Option)
:i1.Launch E-Mail Reader (Menu Option)
This option will launch the e-mail reader configured by the user.
See :link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail:elink. for more
information.

:h1 res=IDM_COPY.Copy (Menu Option)
:i1.Copy (Menu Option)
This menu option copies the current window display to the system
clipboard in two formats&colon. Text and Bitmap.  The text version
shows all the currently displayed items in a form suitable for
pasting into a text editor.  The bitmap version shows an exact
copy of the current window, and can be used by graphic editors
and viewers.
:p.
The short-cut key for this command is Ctrl+Ins.

:h1 res=IDM_ABOUT.Informaci¢ del programa (Opci¢ del men£)
:i1.Informaci¢ del programa (Opci¢ del men£)
Aquesta opci¢ ofereix, quan es selecciona, informaci¢ sobre el programa.
For more information, see :link reftype=hd res=IDD_ABOUT.About (Dialog):elink..

:h2 res=IDD_ABOUT.Informaci¢ del programa (Capsa de diÖleg)
Aquesta capsa de diÖleg mostra el nom del programa, la icona i
informaci¢ sobre els drets de cïpia i l&apos.autor.

Per sortir d&apos.aquesta capsa de diÖleg, podeu prÇmer qualsevol d&apos.aquestes
tecles: ENTER, la barra d&apos.espais o bÇ ESCAPE. TambÇ es possible sortir
seleccionat el but¢ OK del mouse.

:h1 res=IDM_EXIT.Close (Menu Option)
:i1.Close (Menu Option)
This menu option, when selected, will cause the program to terminate.

:h1 res=IDM_KEYS_HELP.Ajuda de les tecles
:i1.Ajuda de les tecles
Les segÅents tecles de funci¢ s&apos.han definit per aquest programa&colon.
:sl compact.
:li.F1 - Ajuda
:li.F3 - :link reftype=hd res=IDM_EXIT.Finalitzar:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.Configurar:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.Amagar els controls:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.Ctrl+Ins - :link reftype=hd res=IDM_COPY.Copy:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.Indicar el directori del fitxer .INI (Capsa de diÖleg)
:i1.Indicar el directori del fitxer .INI (Capsa de diÖleg)
Aquesta capsa de diÖleg es mostra quan el programa no pot trobar el
seu fitxer .INI, i pregunta quin Çs el nom d&apos.aquest fitxer i a quin
directori es troba.
:p.
NomÇs s&apos.accepten noms vÖlids de directoris ja
existents. Una vegada que hagiu escrit el nom, prement la tecla
INTRO (RETURN) o bÇ fent un click al bot¢ "OK" fa que el programa
continuã la seva seqÅäncia d&apos.inicialitzaci¢.
:p.
Si, per contra, voleu tallar la inicialitzaci¢ del programa, premeu
la tecla ESC o bÇ feu un click al bot¢ d&apos.anul˙laci¢.

:h1 res=60000.HRTIMER - High Resolution Timer driver
The high resolution timer device driver, HRTIMER.SYS, enables a program
to access a normally unused timer chip found on IBM PCs.  It has a resolution
as high as 840 nanoseconds, 64K times better than the normal timer resolution 
of 55 milliseconds.
:p.
The only places I know of right now that has this program available are 
:hp4.CompuServe:ehp4., 
where the program can be found in the 
:hp4.Device Driver Development:ehp4.
library of the 
:hp4.OS/2 Developer&apos.s Forum I (OS2DF1):ehp4., 
under the name 
:hp4.TIMER.ZIP:ehp4.,
and on the anonymous ftp sites
:hp4.ftp.cdrom.com:ehp4. 
and 
:hp4.hobbes.nmsu.edu:ehp4..
The file can be found in the directory 
:hp4./pub/os2/dev16:ehp4.,
as 
:hp4.hrtimer.zip:ehp4..
:p.The source code for this device driver was obtained from the Fall 1991 issue of
IBM Personal Systems Developer magazine.  It is freeware, but the original
copyright belongs to IBM.

:h1 res=60001.What is the conflict with PULSE?
This program, as well as PULSE, CPULOAD, CPUMETER and similar programs,
all attempt to measure how busy the computer is by trying to see how much
work they can accomplish at the lowest possible priority.  Presumably, if
one of these programs can do anything at that priority, it must be using
idle time.  If there is no idle time, the computer is 100% busy.  The more
this program can do at system idle priority, the less busy the computer
is assumed to be.
:p.The problem with running more than one of these programs at the same
time is that they begin to compete for that idle time.  If you ran two
copies of the same program, and nothing else, they should both show
about 50% CPU load.  If you run two disimilar programs, like this one
and PULSE, they would both be affected, but usually to differing degrees.

:h1 res=60002.How does HRTIMER.SYS help?
With HRTIMER.SYS installed, system overhead is included in the CPU Load.
:p.
The CPU Load is determined by comparing how much work could be done at
idle priority with the most the program has been able to do up to the
present moment.  Without more information, normal system background 
activity would appear as zero load.  If HRTIMER.SYS is installed on your
system, then this program executes a calibration loop at startup, in which
it determines just how much work can be accomplished at the highest 
possible priority.  This amount is then used for comparison.

:h1 res=60003.What is Theseus/2?
Theseus/2 is a system memory analysis tool developed by IBM as part of
a product they once sold, called :hp5.System Performance Monitor/2:ehp5..
Eventually, they stopped selling the tool, and somebody in IBM packaged
Theseus/2 for freeware distribution early in 1997.  It can be found at&colon.
:p.:lm margin=6.
ftp&colon.//hobbes.nmsu.edu/pub/os2/util/system/t2spm.zip
:lm margin=1.:p.
A newer version (Theseus/3) can be found at&colon.
:p.:lm margin=6.
ftp&colon.//ftp.boulder.ibm.com/ps/products/os2/fixes/v4warp/theseus3/thes3000.exe
:lm margin=1.:p.
Theseus/2 has two things my program will probably never have&colon.
a deep internal knowledge of the structure of the OS/2 kernel, and
a special device driver which allows it to inspect the kernel directly.
These advantages have a price, however&colon. the program takes a
relatively long time to performs its analysis.  For that reason, 
MemSize does not use it more than once every ten seconds, so as to 
not overburden the system.

:euserdoc.
