#include "memsize.h"
:userdoc.
:title.&apos.System ressourcer&apos. Hj‘lp
:body.

:h1 res=ID_MAIN.Introduktion
:i1.Introduktion
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
Dette program viser detaljeret information om dit systems ressourcer.
Informationen bliver opdateret en gang i sekundet, forudsat at det f†r
CPU adgang. De viste informationer er&colon.
:p.
:hp2.Dato/Kl.:ehp2. - Systemets dato og klokkesl‘t, i det standardformat
der er defineret for landet i &apos.COUNTRY=&apos. linjen i CONFIG.SYS.
:p.
:hp2.Forl›bet tid:ehp2. - Den tid der er g†et siden computeren sidst
blev startet.
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.Fysisk fri hukommelse:ehp2. - M‘ngden af tilg‘ngelig systemhukommelse
if›lge :hp1.DosMemAvail:ehp1. funktionen.
:note.Denne funktion er en del af det gamle 16-bit system, og rapporterer
kun fysisk hukommelse under 16 MB. Den blev ogs† rettet lige f›r OS/2 2.0
blev frigivet, til aldrig at rapportere mindre end 512 K.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Virtuel fri hukommelse:ehp2. - M‘ngden af virtuel fri hukommelse
if›lge :hp1.DosQuerySysInfo:ehp1. funktionen. Dette er lig med den
fysiske fri hukommelse, plus m‘ngden af fri plads i den eksisterende
swapfil, plus den m‘ngde fri plads p† harddisken, som swapfilen kan vokse
med minus den reserverede plads p† swapdrevet.
:p.
:hp2.Swapfil st›rrelse:ehp2. - Den nuv‘rende st›rrelse af systemets swapfil
SWAPPER.DAT. For at finde placeringen, s›g i CONFIG.SYS efter &odq.SWAPPATH&cdq..
Denne linje beskriver den eksakte placering af SWAPPER.DAT, samt den minimale
plads der skal v‘re tilbage p† drevet hvor swapfilen ligger.
:p.
:hp2.Tilg‘ngelig swapplads:ehp2. - M‘ngden af fri diskplads p† det logiske
drev, hvor swapfilen er placeret, minus den minimale frie plads p† dette drev.
Dette er lig med hvor meget swapfilen kan vokse med, hvis n›dvendigt.
:p.
:hp2.Ubrugt swapplads:ehp2. - M‘ngden af fri plads inden i den nuv‘rende
swapfil. N†r denne v‘rdi n†r nul, vil swapfilens st›rrelse ›ges, normalt
med 1 megabyte af gangen.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Spool fil st›rrelse:ehp2. - M‘ngden af harddiskplads optaget af
spoolfiler.
:p.
:hp2.CPU belastning (%):ehp2. - En tiln‘rmet v‘rdi af CPU&apos.ens tilg‘ngelige
regnekraft, der benyttes. Det beregnes som et gennemsnit over det sidste
sekund.
:note.Under versions of OS/2 before release 4.0, denne funktion fungerer ikke ordentligt, n†r &odq.Puls&cdq. er aktiv.
:link reftype=hd res=60001.Hvorfor?:elink.
:note.Under versions of OS/2 before release 4.0, hvis du har driveren :link reftype=hd res=60000.HRTIMER.SYS:elink.
installeret, vil denne v‘rdi v‘re mere pr‘cis.
:link reftype=hd res=60002.Hvorfor?:elink.
:p.
:hp2.Average Load (%):ehp2. - The approximate percentage of the CPU&apos.s available
power that has been used in the recent past.  The number of seconds to be 
averaged may be configured by the user 
(see the :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Configuration:elink. dialog).
:p.
:hp2.Batteri:ehp2. - Den eksisterende batterikapacitet tilbage,
eller dets status (Oplader, AC) fremvises.
:p.
:hp2.Aktive Jobs:ehp2. - M‘ngden af jobs i vinduesoversigten.
Vinduesoversigten f†s frem ved at taste CTRL + ESC eller klikke med venstre
og h›jre museknap samtidig.
:note.Ikke alle jobs vises i vinduesoversigten. Nogle jobs er skjult.
:p.
:hp2.Antal processer:ehp2. - Antallet af processer der p† det givne 
tidspunkt er aktive p† systemet. 
:p.
:hp2.Antal tr†de:ehp2. - Antallet af tr†de der p† det givne tidspunkt er 
aktive p† systemet. Der er mindst ‚n tr†d for hver process.
:p.
:hp2.Totalt fri diskplads:ehp2. - M‘ngden af fri diskplads p† alle ikke-
flytbare harddiske tilsammen.
:p.
:hp2.Drev X Fri:ehp2. - M‘ngden af fri plads p† drev X.
:p.
Som du har set er hj‘lpefuktionen aktiv, og de programkommandoer
der eksisterer er tilg‘ngelige vha. vinduets systemmenu. F›lgende
kommandoer er tilg‘ngelige&colon.
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.Nulstil:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.Gem kontrollinje:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.Konfigurer...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.Nulstil belastningsm†ler:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.Nulstil drev:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Launch E-Mail Reader:elink.
:li.:link reftype=hd res=IDM_COPY.Kopi‚r:elink.
:li.:link reftype=hd res=IDM_ABOUT.Programinformation:elink.
:li.:link reftype=hd res=IDM_EXIT.Luk:elink.
:esl.:p.
Udover de n‘vnte egenskaber, accepterer dette program indstillinger fra OS/2
2.0 font- og farvepaletterne,
og er en DDE server for alle de nuv‘rende fremviste emner.
DDE applikationsnavnet er MEMSIZE, og emnenavnene er de samme som de
standard viste emnenavne (f.eks. Fysisk fri hukommelse).

:h1 res=IDM_RESET_DEFAULTS.Nulstil (Menu valgmulighed)
:i1.Nulstil (Menu valgmulighed)
Ved at v‘lge denne mulighed genindstilles programmet&apos.s font og farve
til deres standardv‘rdier.

:h1 res=IDM_HIDE_CONTROLS.Gem kontrollinje (Menu valgmulighed)
:i1.Gem kontrollinje (Menu valgmulighed)
Denne menufunktion vil, n†r valgt, medf›re at programmets vindueskontrol
(system menu‚n, titellinjen on minim‚r knappen) skjules.
Denne mulighed kan skiftes frem og tilbage ved at dobbeltklikke med enten
venstre eller h›jre museknap.
Jeg fandt det ogs† meget nyttigt at kunne flytte vinduet imens kontrollerne
var skjult, og derfor kan man tr‘kke vinduet med begge museknapper.
:p.
Tastekombinationen Alt+H vil ogs† udf›re denne funktion.

:h1 res=IDM_CONFIGURE.Konfigur‚r (Menu valgmulighed)
:i1.Konfigur‚r (Menu valgmulighed)
Denne menu valgmulighed vil, n†r den v‘lges, f›re til at programmets
konfigurationsmenu vises.
For yderligere information se :link reftype=hd res=IDD_CONFIGURE.Konfigur‚r (Dialog):elink..
:note.Genvejstasten til denne mulighed er Alt+C.

:h2 res=IDD_CONFIGURE.Konfigur‚r (Dialog)
Denne indstillingsbog tillader brugeren at ‘ndre alle de konfigur‚rbare
aspekter af dette programs operationer.  Konfugurationsmulighederne
er grupperet efter kategori p† de f›lgende sider&colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.Emner:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.Muligheder (Side 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Muligheder (Side 2):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.Farver:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.Vinduesanker:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Date Format:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (Side 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (Side 2):elink.
:esl.:p.
Alle ‘ndringer aktiveres ›jeblikkeligt.

:h3 res=IDD_CONFIG_PAGE_ITEMS.Emner (Konfigurationsside)
Denne side viser en liste af fremviselige emner.
Du kan v‘lge enkelte eller alle af punkterne i Vis punkter listen til overv†gning og
fremvisning. Klik p† et punkt for at fremh‘ve det, og klik igen for at fjerne
fremh‘velsen. De fremh‘vede punkter vil blive overv†get.
To change an item&apos.s default label, or to adjust its warning and/or error 
thresholds, either double-click mouse button one on the item list entry, or single-click
mouse button two on the entry.  In either case, the 
:link reftype=hd res=IDD_LABEL.Item Attribute:elink. dialog will be displayed.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.Muligheder (Side 1) (Konfigurationsside)
Denne side viser en r‘kke af programmuligheder der kan blive
aktiveret eller deaktiveret.
:p.
:hp2.Altid ›verst:ehp2.
Denne knap aktiverer et vindues Altid ›verst funktion. N†r dette v‘lges vil
programmet altid ligge oven p† andre vinduer, og dermed v‘re synlig hele
tiden.
:p.
:hp2.Anim‚r:ehp2.
Denne knap aktiverer vinduets †bn og luk animations egenskaber.
N†r dette er aktivt, vil programvinduet †bne og lukke p† samme m†de
som folderobjekter g›r - med lyd og bev‘gelse.
Ligesom med foldere, vil animering ikke forekomme medmindre det er
blevet aktiveret i systemops‘tningen.
:p.
:hp2.Dialog Border:ehp2.
This button determines whether the main program window will have 
a simple line border, or a more elaborate dialog border.  The line
border requires less space on the screen, but the dialog border can
look better, especially on OS/2 Warp 4.0 or later, which gives this
border style a three-dimensional appearance.
:p.
:hp2.Tabel format:ehp2.
Denne knap vil, n†r afkrydset, medf›re at vinduet arrangeres i en tabel, 
med de fremviste emner i r‘kker og kolonner. Tabellen vil bruge s† lidt 
vertikalt plads som muligt. N†r denne knap ikke er afkrydset, vil vinduet 
arrangere sig selv med en enkelt kolonne med alle de valgte emner.
:p.
:hp2.Kimen:ehp2.
Denne knap aktiverer en timenlig kimen.  N†r aktiv, vil
programmet bippe to gange ved hver hele time.
:p.
:hp2.Show Disk Drives:ehp2.
When this button is checked, disk drive free space is monitored, subject
to the restrictions placed on this monitoring by other options.  
This control mainly exists as a way to disable disk monitoring.
:p.
:hp2.Vis netv‘rks drev:ehp2.
N†r denne knap er afkrydset, vil tilsluttede netv‘rksdrev bliver vist 
som andre drev, med den undtagelse at de ikke inkluderes i den samlede 
fri diskplads.
:p.
:hp2.Vis filsystemnavne:ehp2.
Denne knap medf›rer at hver enkelt fri disk plads felt vil vise hvilken
type filsystem, som drevet er formateret med. Denne mulighed vil medf›re
et bredere programvindue.
:p.
:hp2.Vis disknavne:ehp2.
Denne knap medf›rer at hver enkelt Fri disk plads felt vil fremvise deres
disklabel. Brugen af denne funktion medf›rer et bredere programvindue.
:p.
:hp2.Vis sekunder:ehp2.
Denne knap medfr at uret og forlt tid vises med sekundangivelse.
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

:h3 res=IDD_CONFIG_PAGE_OPTIONS2.Muligheder (Side 2) (Konfigurationsside)
Denne side viser flere program muligheder, der er mere komplekse end
bare aktiv/deaktiv.
:p.
:hp2.Vis &apos.K&apos.ilobytes:ehp2.
Denne gruppe af afm‘rkningsfelter giver brugeren mulighed for at v‘lge
om hukommelse skal vises med kilobyte notation altid, aldrig, eller n†r
v‘rdien overskrider 512 K.
Standard er at vise alle st›rrelser over 512 K p† denne m†de.
.br
&apos.K=1024&apos. knappen vil, hvis markeret, medf›re at numrene fremvises
i ‘gte K (1024).  Hvis den ikke er markeret, vil numrene fremvises i
decimal K (1000).
.br
&apos.M&apos.egabytes knappen vil, hvis markeret, medf›re at numrene
fremvises i megabytes n†r deres v‘rdi overstiger 8 megabytes.
.br
The &apos.G&apos.igabytes button, if checked, will cause the numbers to be
displayed in gigabytes once their values exceed four gigabytes.
:p.
:hp2.Overv†gningsprioritet:ehp2.
Denne kontrol g›r det muligt at s‘tte den absolutte prioritet for
overv†gningstr†den, inden for klassen af tidskritiske prioriteter.
Man kan s‘tte prioriteten mellem 0 og 31. Prioritet 31 er den h›jest
mulige prioritet i systemet, bortset fra fysiske afbrydelser. Prioritet
0 er simpelthen den laveste prioritet i klassen af tidskritiske prioriteter.
Man b›r normalt s‘tte v‘rdien til maksimum (31) for at f† st›rre
n›jagtighed i m†lingerne af systemaktiviteten. Da nogle brugere har klaget
over at deres kommunikationsprogrammer somme tider taber data, n†r
dette program er aktivt, er prioriteten nu variabel.
:p.
:hp2.Tidsinterval:ehp2.
Denne kontrol g›r det muligt at s‘tte intervallet mellem opdateringer.
Det kan varieres fra 1 sekund til 30 sekunder, i skridt p† 1/10 sekund.
Den viste v‘rdi er tiendedele sekunder.
:p.
:hp2.Gem titellinje:ehp2.
N†r programmet ikke er minimeret, kan det vises med eller uden den s‘dvanlige
titellinje, system menu, og minim‚r knappen. Du kan stadig flytte vinduet hvis
du v‘lger at gemme denne titellinie, ved at tr‘kke i vinduet med h›jre eller venstre
museknap.
:p.
:hp2.Averaging Interval:ehp2.
This control allows you to set the interval over which averages are
computed.  It can be varied from 10 seconds to 3600 seconds (one hour).

:h3 res=IDD_CONFIG_PAGE_COLORS.Farver (Configuration Page)
Dette omr†de viser de tre farver forbundet med Normal, Advarende og Fejl 
v‘rdier for de emner der underst›tter det. Man kan ‘ndre disse farver ved 
at tr‘kke nye farver fra farvepaletterne.

:h3 res=IDD_CONFIG_PAGE_ANCHOR.Vinduesanker (Configuration Page)
Denne kontrol giver dig mulighed for at fasts‘tte et hj›rne af sk‘rmen
som fikspunt, n†r vinduesst›rrelsen ‘ndres. Vinduesst›rrelsen ‘ndres fx.
n†r menupunkter tilf›jes eller fjernes fra aktivlisten, og n†r fonten ‘ndres
fra fontpaletten.

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

:h3 res=IDD_LABEL.Emne attributter  (Dialog)
Denne dialog boks fremviser den nuv‘rende og standard navne for et 
fremvisningsemne, sammen med de eksisterende Advarende og Fejl niveauer 
hvis den egenskab er aktiveret for det aktuelle emne. For at ‘ndre navnet 
tastes det nye navn blot ind i stedet for det eksisterende. For at ‘ndre 
niveuaerne for advarende eller fejl t‘rskelv‘rdierne, kan man enten 
indtaste en ny v‘rdi, eller flytte p† indikatoren til det aktuelle emne.
:p.For at nulstille alle v‘rdier til de oprindelige standard v‘rdier, kan 
man klikke p† Nulstil knappen.

:h1 res=IDM_RESETLOAD.Nulstil belastningsm†ler (Menu valgmulighed)
:i1.Nulstil belastningsm†ler (Menu valgmulighed)
Denne valgmulighed giver brugere mulighed for at nulstille CPU
belastningsm†leren og rekalibrere den. Dette medf›rer at tidligere
CPU aktivitet i beregningerne slettes.

:note.This menu option is not available when on Warp 4.0 or better,
nor on certain higher fixpack levels of Warp 3.0, as the newer load
measuring technique makes this command irrelevant.

:h1 res=IDM_RESETAVERAGE.Reset Average Load (Menu valgmulighed)
:i1.Reset Average Load (Menu valgmulighed)
This menu option allows the user to reset the Average Load meter,
causing it to forget previous CPU activity in its calculations.
:p.
The short-cut key for this command is Alt+R.

:h1 res=IDM_RESETDRIVES.Nulstil drev (Menu valgmulighed)
:i1.Nulstil drev (Menu valgmulighed)
Denne menu valgmulighed tillader brugeren at nulstille eventuelle 
drev fejl der m†tte v‘re. Dette vil automatisk ske efter en periode, 
men kan g›res med det samme vha. denne mulighed.

:h1 res=IDM_LAUNCHEMAILER.Launch E-Mail Reader (Menu valgmulighed)
:i1.Launch E-Mail Reader (Menu valgmulighed)
This option will launch the e-mail reader configured by the user.
See :link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail:elink. for more
information.

:h1 res=IDM_COPY.Kopi‚r (Menu valgmulighed)
:i1.Kopi‚r (Menu valgmulighed)
Denne menuvalgmulighed kopierer programvinduet til systemets udklipsholder
i to formater&colon. tekst og bitmap. Tekst versionen viser alle de valgte emner
i et format egnet til at klistre ind i et tekstbehandlingsprogram. Bitmap
versionen viser en n›jagtig kopi af programvinduet, og kan bruges af f.eks.
billedbehandlingsprogrammer.
:p.Genvejstasten for denne kommando er Ctrl+Ins.

:h1 res=IDM_ABOUT.Programinformation (Menu valgmulighed)
:i1.Programinformation (Menu valgmulighed)
Denne menu valgmulighed vil, n†r den v‘lges, f›re til at programmets
Programinformations dialog vises.
For yderligere information se :link reftype=hd res=IDD_ABOUT.Programinformation (Dialog):elink..

:h2 res=IDD_ABOUT.Programinformation (Dialog)
Denne dialogboks viser programmets navn, ikon og copyright information.
For at forlade dialogen, tryk da p† ENTER, ESC eller mellemrumstasten, eller
klik p† OK knappen med musen.

:h1 res=IDM_EXIT.Luk (Menu valgmulighed)
:i1.Luk (Menu valgmulighed)
Denne menu funktion vil, n†r valgt, medf›re at programmet afsluttes.

:h1 res=IDM_KEYS_HELP.Hj‘lp for taster
:i1.Hj‘lp for taster
De f›lgende funktionstaster er defineret for dette program&colon.
:sl compact.
:li.F1 - Hj‘lp
:li.F3 - :link reftype=hd res=IDM_EXIT.Afslut:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.Konfigur‚r:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.Gem kontrollinje:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.Ctrl+Ins - :link reftype=hd res=IDM_COPY.Kopi‚r:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.S‘t profilfil stien (Dialog)
:i1.S‘t profilfil stien (Dialog)
Denne dialog vises n†r programmet ikke kan finde sin profilfil (INI), og sp›rger
om hvor filen er placeret eller hvor den kan skabes.
:p.
Kun et gyldigt eksisterende stinavn accepteres.
N†r stinavnet er indtastet, tryk da p† ENTER tasten eller klik p† OK knappen
for at programmet kan forts‘tte.
:p.
Hvis du ›nsker at afbryde programmet&apos.s initialiserings sekvens, tryk da
p† ESC tasten eller klik p† Escape.

:h1 res=60000.HRTIMER
H›jhastigheds tids device driveren HRTIMER.SYS, muligg›r at et program
kan f† adgang til en tidschip p† IBM PC&apos.ere, der normalt ikke anvendes.
Den har en frekvens p† op til 840 nanosekunder, hvilket er 64000 gange
hurtigere end den normale tidshastighed p† 55 millisekunder.
:p.
De eneste steder jeg kender til, der i ›jeblikket har dette program
tilg‘ngeligt, er :hp4.CompuServe:ehp4., hvor programmet er placeret som filem
:hp4.TIMER.ZIP:ehp4., i :hp4.Device Driver Development:ehp4. biblioteket under
:hp4.OS/2 Developers Forum I (OS2DF1):ehp4.. 
Desuden er programmet placeret p† :hp4.ftp.cdrom.com:ehp4. og :hp4.hobbes.nmsu.edu:ehp4. 
i biblioteket :hp4./pub/os2/dev16:ehp4., som filen :hp4.hrtimer.zip:ehp4.. 
Man kan logge p† begge disse ftp-sites som &odq.anonymous&cdq..
:p.
Koden til denne driver blev hentet fra efter†rsudgaven af &odq.IBM Personal
Systems Developer magazine&cdq. i 1991. Det er freeware, men IBM har copyright
til koden.

:h1 res=60001.Hvad er konflikten med PULS?
Dette program, s† vel som PULS, CPULOAD, CPUMETER og lignende programmer,
fors›ger alle at m†le hvor travlt computeren har ved at se hvor meget de
kan udf›re ved den lavest mulige prioritet. Hvis et af disse programmer
kan udf›re noget ved en given prioritet, m† det antages at der er ledig
tid p† systemet. Hvis der ingen ledig tid er p† systemet, er computeren
100 % belastet. Jo mere dette program kan udf›re ved systemets givne
ledige prioritet, desto mindre belastet antages computeren at v‘re.
:p.
Problemet ved at bruge mere end et af disse programmer samtidigt,
er at de begynder at konkurrere om den tilg‘ngelige ledige tid.
Hvis man f.eks. bruger to kopier af det samme program, og ikke andet,
vil de begge vise ca. 50 % CPU belastning. Hvis man k›rer to forskellige
programmer, som f.eks. dette og PULS, vil de begge blive ber›rt, men
normalt i forskellig grad.

:h1 res=60002.Hvordan hj‘lper HRTIMER.SYS?
Med HRTIMER.SYS installeret bliver systemets overskudskapacitet
inkluderet i CPU belastningen.
:p.CPU belastningen bliver fastsat ved at sammenligne, hvor meget der
kan udf›res ved den ledige prioritet, med det maksimale programmet har
kunnet udf›re indtil sammenligningstidspunktet.
Uden yderligere information, vil normal baggrundsaktivitet fremg† som
ingen belastning.
Hvis HRTIMER.SYS er installeret p† din computer, s† udf›rer denne
driver en kalibreringsl›kke ved start af systemet, hvor den fasts‘tter
hvor meget der kan udf›res ved den maksimale prioritet.
Denne v‘rdi bliver brugt til sammenligningen.

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
