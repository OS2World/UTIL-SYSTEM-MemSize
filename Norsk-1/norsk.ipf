#include "memsize.h"
:userdoc.
:title.Hjelp for Systemressurser
:body.

:h1 res=ID_MAIN.Introduksjon
:i1.Introduksjon
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
Dette programmet viser detaljert informasjon om ditt systems
ressurser. Informasjonen blir oppdatert en gang i sekundet,
forutsatt at programmet f†r CPU-tid. De viste informasjoner er&colon.
:p.
:hp2.Dato og klokkeslett:ehp2. - Systemets dato og klokkeslett, i
det standardformat som er definert for landet spesifisert i
COUNTRY-linjen i CONFIG.SYS.
:p.
:hp2.Driftstid:ehp2. - Den tid som er g†tt siden systemet sist ble
startet.
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.Fri fysisk hukommelse:ehp2. - Mengden av tilgjengelig
systemhukommelse i f›lge funksjonen :hp1.DosMemAvail:ehp1..
:note. Denne funksjonen er del av det gamle 16-bit systemet,
og rapporterer bare minne under 16Mb, og ble, like f›r 
lanseringen av OS/2 2.0, endret til aldri † rapportere mindre enn
512Kb.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Fri virtuell hukommelse:ehp2. - Mengden av fri virtuell
hukommelse i f›lge :hp1.DosQuerySysInfo:ehp1.-funksjonen.
Dette er det samme som den frie fysiske hukommelsen, pluss
mengden av ubenyttet plass i den eksisterende swapfil, pluss
ledig plass p† platelageret som st†r til r†dighet for swapfilen
(dvs. ledig plass minus reservert plass spesifisert i SWAPPATH-
linjen i CONFIG.SYS).
:p.
:hp2.Swap-fil st›rrelse:ehp2. - Den n†v‘rende st›rrelsen av
systemets swapfil SWAPPER.DAT. Lokalisering av swapfilen
gj›res ved † se p† SWAPPATH-linjen i CONFIG.SYS.
:p.
:hp2.Tilgjengelig swap-plass:ehp2. - Mengden av fri plass p† det
logiske platelageret hvor swapfilen er plassert, minus den
minimale frie plass p† dette lager. Dette tall viser hvor mye
swapfilen ved behov kan vokse f›r lageret er fullt.
:p.
:hp2.Ubrukt plass i swap-fil:ehp2. - Mengden av ledig plass innenfor
den n†v‘rende swapfil. N†r denne verdi n†r null, vil swapfilens
st›rrelse ›kes, normalt med 1 megabyte av gangen.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Spool-fil st›rrelse:ehp2. - Mengden av platelagerplass opptatt
av spool-filer.
:p.
:hp2.CPU-belastning (%):ehp2. - En tiln‘rmet verdi for utnyttelse
av CPU&apos.ens regnekraft. Verdien beregnes som et gjennomsnitt over
det siste sekund.
:note.Under versions of OS/2 before release 4.0, denne funksjonen fungerer ikke ordentlig n†r Puls-applikasjonen
er aktiv.
:link reftype=hd res=60001.Hvorfor?:elink.
:note.Under versions of OS/2 before release 4.0, hvis du har driveren :link reftype=hd res=60000.HRTIMER.SYS:elink.
installert vil vist verdi v‘re mer n›yaktig.
:link reftype=hd res=60002.Hvorfor?:elink.
:p.
:hp2.Average Load (%):ehp2. - The approximate percentage of the CPU&apos.s available
power that has been used in the recent past.  The number of seconds to be 
averaged may be configured by the user 
(see the :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Configuration:elink. dialog).
:p.
:hp2.Battery:ehp2. - The current battery capacity remaining,
or its status (Charging, AC) is displayed.
:p.
:hp2.Aktive jobber:ehp2. - Antallet av jobber vist i vindusoversikten.
Vindusoversikten f†s frem ved † taste CTRL+ESC.
:note.Ikke alt som foreg†r vises i vindusoversikten, fordi noen
jobber er merket slik at de ikke skal bli vist.
:p.
:hp2.Antall prosesser:ehp2. - Antall prosesser aktive p† systemet.
:p.
:hp2.Antall tr†der:ehp2. - Antall tr†der som er aktive p† systemet.  
Det er minst en tr†d for hver prosess.
:p.
:hp2.Ledig lagerplass i alt:ehp2. - Mengden av ledig plass p† alle
ikke-flyttbare platelagere tilsammen.
:p.
:hp2.Stasjon X: ledig plass:ehp2. - Mengden av ledig plass p†
stasjon X.
:p.
Som du har sett finnes det et hjelpesystem for Systemressurser,
og de programkommandoer som eksisterer er tilgjengelige via
vinduets systemmeny. F›lgende kommandoer er tilgjengelige&colon.
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.Sett standardinnstillinger:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.Skjul kontroller:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.Konfigurer...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.Nullstill CPU-monitor:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.Nullstill platelagre:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Launch E-Mail Reader:elink.
:li.:link reftype=hd res=IDM_COPY.Kopier:elink.
:li.:link reftype=hd res=IDM_ABOUT.Produktinformasjon:elink.
:li.:link reftype=hd res=IDM_EXIT.Avslutt:elink.
:esl.:p.
Utover de nevnte egenskaper, aksepterer dette programmet innstillinger
fra OS/2 font- og fargepalettene, og det er en DDE-tjener som kan
levere tilstandsdata for alle ressurser som overv†kes.
DDE-applikasjonens navn er MEMSIZE, og ressursnavnene er de samme
som standardnavnene p† de ulike ressursene(slik som Fri fysisk
hukommelse).

:h1 res=IDM_RESET_DEFAULTS.Sett standardinnstillinger (Meny-valg)
:i1.Sett standardinnstillinger (Meny-valg)
Ved dette meny-valg tilbakestilles programmets font og farge til 
standardverdiene.

:h1 res=IDM_HIDE_CONTROLS.Skjul kontroller (Meny-valg)
:i1.Skjul kontroller (Meny-valg)
Dette meny-valg medf›rer at programmets vinduskontroll (system-menyen,
tittellinjen og minimaliseringsknappen) skjules.
Dette valg kan gj›res - og omgj›res - ved † dobbeltklikke p† vinduet med 
pekefinger-knappen p† musa. Programvinduet kan flyttes ved † trekke det
med samme knapp ogs† n†r tittellinjen er skjult.
:p.
Tastekombinasjonen Alt+H vil ogs† utf›re denne funksjonen.

:h1 res=IDM_CONFIGURE.Konfigurer (Meny-valg)
:i1.Konfigurer (Meny-valg)
Dette meny-valg f›rer til at programmets konfigurasjonsmeny vises.
For ytterligere informasjon se :link reftype=hd res=IDD_CONFIGURE.Konfigurer (Dialog):elink..
:note.Tastkombinasjonen for dette valg er Alt+C.

:h2 res=IDD_CONFIGURE.Konfigurer (Dialog)
This notebook dialog allows you to change all the configurable 
aspects of this program&apos.s operations.  The configuration options
are grouped by category on the following pages&colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.Ressursoverv†kning:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.Options (Page 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Options (Page 2):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.Farger:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.Vindusanker:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Date Format:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (Page 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (Page 2):elink.
:esl.:p.
All changes take effect immediately.

:h3 res=IDD_CONFIG_PAGE_ITEMS.Ressursoverv†kning (Configuration Page)
This page displays a list of displayable items.
Du kan velge enkelte eller alle elementene i listen ressursoverv†kning
ved † fremheve dem. Klikk p† et punkt for † fremheve det, og klikk
igjen for at fjerne fremhevingen. 
To change an item&apos.s default label, or to adjust its warning and/or error 
thresholds, either double-click mouse button one on the item list entry, or single-click
mouse button two on the entry.  In either case, the 
:link reftype=hd res=IDD_LABEL.Item Attribute:elink. dialog will be displayed.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.Options (Page 1) (Configuration Page)
This page shows a number of program options that may be
activated or deactivated.
:p.
:hp2.Skjul kontroller:ehp2.
N†r programmet ikke er minimert, kan det vises med eller uten
tittellinje, systemmeny og minimaliseringsknapp. Du kan fortsatt
flytte vinduet hvis du velger † skjule kontrollene, ved † trekke
vinduet med pekefingerknappen p† musa.
:p.
:hp2.Alltid synlig:ehp2.
Denne knappen aktiverer vinduets &apos.alltid synlig&apos. funksjon. N†r denne
velges vil programvinduet alltid &apos.flyte opp&apos. og bli liggende over
andre vinduer.
:p.
:hp2.Animer:ehp2.
Denne knappen aktiverer vinduets animasjonsegenskaper ved †pning
og lukking. N†r disse er aktive vil programvinduet †pne og lukke p†
samme m†te som folderobjekter gj›r - med lyd og bevegelse.
Liksom med foldere, vil animering ikke forekomme med mindre det er
blitt valgt i system-oppsettet.
:p.
:hp2.Dialog Border:ehp2.
This button determines whether the main program window will have 
a simple line border, or a more elaborate dialog border.  The line
border requires less space on the screen, but the dialog border can
look better, especially on OS/2 Warp 4.0 or later, which gives this
border style a three-dimensional appearance.
:p.
:hp2.Tabellformat:ehp2.
N†r denne knappen markeres vil vinduet bli arrangert i tabellformat, med viste  
ressurser ordnet i rekker og kolonner. Vinduet vil optimeres slik at det tar s† lite  
plass som mulig vertikalt. N†r knappen ikke er markert vil vinduet v‘re arrangert  
som en enkelt kolonne.
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
:hp2.Vis platelagre p† nettet:ehp2.
N†r denne knappen er markert vil platelagre som for ›yeblikket er koblet til  
systemet via et nettverk bli overv†ket p† samme vis om lokale, faste disker. Unntak:  
Bare lokale, faste platelagre teller med n†r verdien for total ledig lagerplass  
beregnes.
:p.
:hp2.Vis filsystem-type:ehp2.
Denne knappen bevirker at hvert enkelt &apos.Stasjon X&colon. ledig plass&apos.-felt
viser hvilken type filsystem som platelageret er formatert med. Bruken
av denne funksjon gj›r programvinduet noe bredere.
:p.
:hp2.Vis stasjonsnavn:ehp2.
Denne knappen bevirker at hvert enkelt &apos.ledig platelager plass&apos.-felt
viser platelagerets navn (label). Bruken av denne funksjon medf›rer
et bredere programvindu.
:p.
:hp2.Vis sekunder:ehp2.
Denne knappen medf›rer at klokkeslett og driftstid vises med
sekundangivelse.
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
:hp2.Vis &apos.K&apos.ilobyte:ehp2.
Denne gruppen av knapper gj›r det mulig † velge hvorvidt store
st›rrelser skal vises i kilobyte notasjon; alltid, aldri, eller bare
n†r deres verdier overstiger 512 Kb. Standardvalget er † vise
alle st›rrelser over 512 Kb p† denne m†ten. 
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
:hp2.Overv†kningsprioritet:ehp2.
Denne kontroll gj›r det mulig † sette den absolutte prioritet for
overv†kningstr†den, innenfor klassen av tidskritiske prioriteter.
Man kan sette prioriteten mellem 0 og 31. Prioritet 31 er den
h›yest mulige prioritet i systemet, bortsett fra fysiske avbrytelser.
Prioritet 0 er den laveste prioritet i klassen av tidskritiske
prioriteter. Man b›r normalt sette verdien til maksimum (31) for
† f† st›rre n›yaktighet i m†lingene av systemaktiviteten. Fordi
noen brukere har klaget over at deres kommunikasjonsprogrammer
noen ganger mister data s† lenge dette programmet er aktivt, er
prioriteten n† variabel.
:p.
:hp2.Tidsintervall:ehp2.
Denne kontrollen gj›r det mulig † justere intervallet mellom
oppdateringer. Det kan varieres fra 1 sekund til 30 sekunder,
i skritt p† 1/10 sekund. Den viste verdi er i tiendedels sekunder.
:p.
:hp2.Averaging Interval:ehp2.
This control allows you to set the interval over which averages are
computed.  It can be varied from 10 seconds to 3600 seconds (one hour).

:h3 res=IDD_CONFIG_PAGE_COLORS.Farger (Configuration Page)
Dette omr†det viser de tre fargene assosiert med Normal-, Advarsel- og Feil-niv† for de  
ressurser for hvilke disse niv†ene er definert. Du kan sette disse fargene ved †  
trekke farger over fra en av systemets fargepaletter.  

:h3 res=IDD_CONFIG_PAGE_ANCHOR.Vindusanker (Configuration Page)
Denne kontroll gir deg mulighet til † bestemme et hj›rne av skjermen
som festepunkt, n†r vindusst›rrelsen endres. Vindusst›rrelsen endres
for eksempel n†r menypunkter tilf›yes eller fjernes fra aktivlisten,
og n†r fonten forandres fra fontpaletten.

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
Denne dialogen viser n†v‘rende og standerd betegnelse for en ressurs,  
sammen med n†v‘rende advarsel- og feilniv†, hvis slike er definert  
for vedkommende ressurs. For † forandre betegnelsen kan du ganske enkelt skrive 
inn en ny betegnelse over den gamle. For † forandre advarsel- og/eller feilniv†ene kan du enten 
skrive inn en ny verdi over den gamle, eller flytte skyveknappen som tilh›rer vedkommende 
ressurs 
:p.For † stille alle verdier tilbake til standardverdiene, klikk p† Standard knappen. 

:h1 res=IDM_RESETLOAD.Nullstill CPU-monitor (Meny-valg)
:i1.Nullstill CPU-monitor (Meny-valg)
Dette valg gir brukere mulighet til † nullstille belastningsm†leren
for sentralprosessoren i systemet, og † rekalibrere den. Dette medf›rer
at tidligere CPU-aktivitet ignoreres i beregningene.

:note.This menu option is not available when on Warp 4.0 or better,
nor on certain higher fixpack levels of Warp 3.0, as the newer load
measuring technique makes this command irrelevant.

:h1 res=IDM_RESETAVERAGE.Reset Average Load (Menyvalg)
:i1.Reset Average Load (Menyvalg)
This menu option allows the user to reset the Average Load meter,
causing it to forget previous CPU activity in its calculations.
:p.
The short-cut key for this command is Alt+R.

:h1 res=IDM_RESETDRIVES.Nullstill platelagre (Menyvalg)
:i1.Nullstill platelagre (Menyvalg)
Dette menyvalget tillater brukeren † nullstille eventuelle feil som m†tte oppst†.   
Nullstilling vil skje automatisk etter en tid, men kommandoen tillater brukeren †  
nullstille umiddelbart.

:h1 res=IDM_LAUNCHEMAILER.Launch E-Mail Reader (Menyvalg)
:i1.Launch E-Mail Reader (Menyvalg)
This option will launch the e-mail reader configured by the user.
See :link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail:elink. for more
information.

:h1 res=IDM_COPY.Kopier (Meny-valg)
:i1.Kopier (Meny-valg)
Dette menyvalget kopierer det n†v‘rende vindusinnhold til systemets
utklippstavle i to formater&colon. Tekst og grafikk (bit-kart). Tekstversjonen
viser alle linjene i vinduet i en form som er passende for kopiering
inn i en tekstbehandler. Grafikkversjonen viser en eksakt kopi av
det n†v‘rende vinduet, og kan bli brukt av grafikkredigerings- og
bildeframvisningsprogrammer.
:p.
Tastkombinasjonen for denne kommandoen er Ctrl+Ins.

:h1 res=IDM_ABOUT.Produktinformasjon (Meny-valg)
:i1.Produktinformasjon (Meny-valg)
Denne meny-valg f›rer til at programmets informasjonsdialog vises.
For mer informasjon se :link reftype=hd res=IDD_ABOUT.Produktinformasjon (Dialog):elink..

:h2 res=IDD_ABOUT.Produktinformasjon (Dialog)
Denne dialogboks viser programmets navn, ikon og informasjon om opphavsrettigheter.
For † forlate dialogen, trykk p† ENTER, ESC eller mellomromstasten, eller
klikk p† OK-knappen med musa.

:h1 res=IDM_EXIT.Avslutt (Meny-valg)
:i1.Avslutt (Meny-valg)
Dette valg gj›r at programmet avsluttes.

:h1 res=IDM_KEYS_HELP.Hjelp for taster
:i1.Hjelp for taster
De f›lgende funksjonstaster er defineret for dette program&colon.
:sl compact.
:li.F1 - Hjelp
:li.F3 - :link reftype=hd res=IDM_EXIT.Avslutt:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.Konfigurer:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.Skjul kontroller:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.Ctrl+Ins - :link reftype=hd res=IDM_COPY.Kopier:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.Sett initialiseringsbane (Dialog)
:i1.Sett initialiseringsbane (Dialog)
Denne dialog vises n†r programmet ikke kan finne sin initialiseringsfil
(INI), og sp›r hvor filen er plassert eller hvor den skal opprettes.
:p.
Kun en gyldig og eksisterende bane aksepteres.
N†r banen er inntastet: trykk p† ENTER-tasten eller klikk p† OK-knappen
for † f† programmet til † fortsette.
:p.
Hvis du ›nsker † avbryte programmets initialiseringsrutine: trykk p†
ESC-tasten eller klikk p† Avbryt.

:h1 res=60000.HRTIMER - H›yoppl›sning klokkedriver
H›yoppl›snings klokkedriveren, HRTIMER.SYS, gj›r det mulig for et
program † bruke en klokke-brikke som finnes i IBM PCer, men som
vanligvis ikke benyttes. Driveren har en oppl›sning p† hele 840
nanosekunder, 64K ganger bedre enn den normale klokkeoppl›sningen p†
55 millisekunder.
:p.
De eneste steden jeg vet om som n† har dette programmet tilgjengelig er
:hp4.CompuServe:ehp4., 
hvor programmet kan bli funnet i
:hp4.Device Driver Development:ehp4.
biblioteket i
:hp4.OS/2 Developer&apos.s Forum I (OS2DF1):ehp4., 
under navnet
:hp4.TIMER.ZIP:ehp4.,
og f›lgende tjenere for anonym ftp:
:hp4.ftp.cdrom.com:ehp4.  
:hp4.hobbes.nmsu.edu:ehp4..
Filen kan bli funnet i katalogen
:hp4./pub/os2/dev16:ehp4.,
som
:hp4.hrtimer.zip:ehp4..
:p.Programkoden for denne driveren ble hentet fra H›st 1991 utgaven
av IBM Personal Systems Developer Magazine.  Programmet er freeware,
men de originale opphavsrettighetene tilh›rer IBM.

:h1 res=60001.Hva er problemet med PULSE?
Memsize, p† samme m†te som PULSE, CPULOAD, CPUMETER og andre
lignende programmer, pr›ver † m†le hvor opptatt datamaskinen er ved
† se hvor mye arbeid som kan utf›res med den lavest mulige prioritet.
Hvis programmet kan utf›re noe som helst ved denne minimale prioritet,
m† det bruke ledig prosessortid. Hvis det ikke er noe tid tilgjengelig, m†
datamaskinens prosessor v‘re 100% utnyttet. Jo mer programmet kan
gj›re, jo mindre belastet antas prosessoren † v‘re.
:p.Problemet med † kj›re flere slike programmer samtidig er at de
begynner † konkurrere om ledig prosessortid. Hvis du kj›rer to kopier
av det samme programmet, og ikke noe annet, skulle begge vise omtrent
50% prosessorutnyttelse. Hvis du kj›rer to ulike programmer, slik som
dette programmet og PULSE, vil begge bli p†virket, men vanligvis i ulik
grad.

:h1 res=60002.Hvorfor hjelper HRTIMER.SYS?
Med HRTIMER.SYS installert, blir system-overhead inkludert i CPU-
belastningen.
:p.
CPU-belastningen blir bestemt ved † sammenligne hvor mye programmet
f†r gjort ved minimal prioritet, med hvor mye programmet har v‘rt i
stand til † f† gjort opp til det n†v‘rende tidspunkt. Uten mer
informasjon vil normal bakgrunnsaktivitet fremst† som null
belastning. Dette problemet motvirkes av HRTIMER.SYS, som kj›rer en
kalibreringsrutine ved oppstart. Programmet bestemmer hvor mye
som kan bli gjort ved h›yest mulig prioritet, og den m†lte verdi
brukes som referansepunkt for senere belastningsberegninger.

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
