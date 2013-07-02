#include "memsize.h"
:userdoc.
:title.&apos.System Resources&apos. Help
:body.

:h1 res=ID_MAIN.Introduction
:i1.Introduction
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
This program displays several items related to system resources,
and updates the display once per second, providing it is given
CPU time to do so.  The items displayed are&colon.
:p.
:hp2.Date&slash.Time:ehp2. - The current date and time, in the format called for
in the default country information for your system, as specified in
the &apos.COUNTRY=&apos. entry of your CONFIG.SYS file.
:p.
:hp2.Elapsed Time:ehp2. - The elapsed time since the computer
was last restarted.
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.Physical Memory Free:ehp2. - The amount of system memory
available, according to the :hp1.DosMemAvail:ehp1. function.
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
:hp2.Swap File Size:ehp2. - The current size of the system
virtual memory swap file, SWAPPER.DAT.
To locate the file, the file CONFIG.SYS is scanned for its SWAPPATH entry.
That entry provides the full name of the swap-file&apos.s directory and
indicates the minimum free space that must be left on the swap-file&apos.s
disk drive.
:p.
:hp2.Available Swap Space:ehp2. - The amount of free disk space on the
logical disk drive where the system swap file resides, less the mininum
free space.  This is how much more the swap file could expand, if necessary.
:p.
:hp2.Unused Swap Space:ehp2. - The amount of free space within the currently
allocated system swap file.  When this drops to zero, the system swap file
must be increased in size, normally by one megabyte at a time.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Spool File Size:ehp2. - The amount of disk space consumed by spool files.
:p.
:hp2.CPU Load (%):ehp2. - The approximate percentage of the CPU&apos.s available
horsepower that&apos.s being used at the moment.  It is averaged over the previous
second.
:note.Under versions of OS/2 before release 4.0, this function and PULSE do not get along with each other.  
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
:hp2.Active Task Count:ehp2. - The number of entries in the system switch list,
which is the list displayed when you press CTRL+ESC.
:note.Not all entries in the system switch list are displayed in the
Window List.  Some are marked for non-display.
:p.
:hp2.Process Count:ehp2. - The number of processes currently loaded
on the system.
:p.
:hp2.Thread Count:ehp2. - The number of threads currently running
on the system.  There is at least one thread for each process.
:p.
:hp2.Total Free Disk Space:ehp2. - The amount of free space on all the
local non-removable disks combined.
:p.
:hp2.Drive X Free:ehp2. - The amount of free space on drive X.
:p.
The help facility is active, as you&apos.ve already seen, and those program
commands that exist may be accessed via the window&apos.s system menu.  The
following commands are available&colon.
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.Reset Defaults:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.Hide Controls:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.Configure...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.Reset Load Meter:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.Reset Drives:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Launch E-Mail Reader:elink.
:li.:link reftype=hd res=IDM_COPY.Copy:elink.
:li.:link reftype=hd res=IDM_ABOUT.About:elink.
:li.:link reftype=hd res=IDM_EXIT.Close:elink.
:esl.:p.
In addition to those features already described, this program accepts
commands from the OS/2 2.0 Font and Color Palette programs, and is a
DDE server for all the currently displayed items.  The DDE application
name is MEMSIZE, and the item names are the same as the default display
item names (like Physical Memory Free).

:h1 res=IDM_RESET_DEFAULTS.Reset Defaults (Menu Option)
:i1.Reset Defaults (Menu Option)
Selecting this menu option will reset the program&apos.s font and color
attributes to their default values.

:h1 res=IDM_HIDE_CONTROLS.Hide Controls (Menu Option)
:i1.Hide Controls (Menu Option)
This menu option, when selected, will cause the program&apos.s frame controls
(the system menu, the titlebar and the minimize button) to be hidden.
This option can be toggled with a double-click on either mouse button.
Also, since I saw it as very useful to be able to move the window while
the controls were hidden, the window has been set up so that you can
drag it with either mouse button.
:p.
The key combination Alt+H will perform this function also.

:h1 res=IDM_CONFIGURE.Configure (Menu Option)
:i1.Configure (Menu Option)
This menu option, when selected, will cause the program&apos.s configuration
dialog to be displayed.
For more information, see :link reftype=hd res=IDD_CONFIGURE.Configure (Dialog):elink.
:note.The short-cut key to this option is Alt+C.

:h2 res=IDD_CONFIGURE.Configure (Dialog)
This notebook dialog allows you to change all the configurable 
aspects of this program&apos.s operations.  The configuration options
are grouped by category on the following pages&colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.Items:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.Options (Page 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Options (Page 2):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.Colors:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.Anchor:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Date Format:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (Page 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (Page 2):elink.
:esl.:p.
All changes take effect immediately.

:h3 res=IDD_CONFIG_PAGE_ITEMS.Items (Configuration Page)
This page displays a list of displayable items.
You may select any or all of the items in the Display Items list for monitoring
and display.  
Simply click on an item to highlight it, click again to remove the highlight.
Highlighted items are monitored.
To change an item&apos.s default label, or to adjust its warning and/or error 
thresholds, either double-click mouse button one on the item list entry, or single-click
mouse button two on the entry.  In either case, the 
:link reftype=hd res=IDD_LABEL.Item Attribute:elink. dialog will be displayed.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.Options (Page 1) (Configuration Page)
This page shows a number of program options that may be
activated or deactivated.
:p.
:hp2.Hide Controls:ehp2.
When the program window is not iconized, it can be shown with or without the usual
title bar, system menu and minimize/maximize buttons.  If you chose to
hide these controls, the window may still be positioned by dragging its
center area.
:p.
:hp2.Float to Top:ehp2.
This button activates an window&apos.s float-to-top function.
When this button has been check-marked, the clock will float to the top
of the window stack, making itself visible all the time.
:p.
:hp2.Animate:ehp2.
This button activates the window open &amp. close animation feature.
When this is active, the program window will open and close in the
same way as the folder objects do&colon. with sound and motion.
As with folders, animation will not occur unless it has been
enabled at the system level.
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
attached to the system are monitored in the same way as local nonremovable
drives are, except that they are not included in the Total Free Disk Space
item.
:p.
:hp2.Show File System Names:ehp2.
This button causes the individual drive free space entries to show the type
of file system in use with the drive.  Use of this option results in
a wider monitor window.
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
This button causes the program to make use of IBM's utility 
program :link reftype=hd res=60003.Theseus/2:elink.,
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
:hp2.Monitor Priority:ehp2.
This control allows you to set the absolute priority of the monitoring
thread of the program, within the time-critical priority class.
You may vary the priority from 0 to 31.  Priority 31 is the highest
possible priority in the system, short of physical interrupts.
Priority 0 is simply the lowest priority within the time-critical class.
Normally, you should set this to the maximum for more accuracy in
system monitoring.  However, some have complained that their communications
programs occasionally miss data while this program is running.  For that
reason the priority is now variable.
:p.
:hp2.Timer Interval:ehp2.
This control allows you to set the interval between monitor window
updates.  It can be varied from 1.0 seconds to 30.0 seconds, in
steps of 1/10 second.  The number displayed is in tenths of a second.
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

:h1 res=IDM_ABOUT.About (Menu Option)
:i1.About (Menu Option)
This menu option, when selected, will cause the program&apos.s About dialog
to be displayed.
For more information, see :link reftype=hd res=IDD_ABOUT.About (Dialog):elink..

:h2 res=IDD_ABOUT.About (Dialog)
This dialog box displays the program name, icon and copyright information.
To exit the dialog, press the ENTER key, the SPACE bar or the ESCAPE key,
or click on the OK button with the mouse.

:h1 res=IDM_EXIT.Close (Menu Option)
:i1.Close (Menu Option)
This menu option, when selected, will cause the program to terminate.

:h1 res=IDM_KEYS_HELP.Keys Help
:i1.Keys Help
The following function keys have been defined for this program&colon.
:sl compact.
:li.F1 - Help
:li.F3 - :link reftype=hd res=IDM_EXIT.Exit:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.Configure:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.Hide Controls:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.Ctrl+Ins - :link reftype=hd res=IDM_COPY.Copy:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.Set Profile Path (Dialog)
:i1.Set Profile Path (Dialog)
This dialog is displayed when the program cannot find its profile (INI) file,
and asks you where the file is or where it is to be created.
:p.
Only a valid existing directory name will be accepted.
Once you have entered the name, press the ENTER key or click on the OK
button for the program to continue.
:p.
If you wish to abort the program&apos.s initialization sequence, press
the ESC key or click on the escape button.

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
