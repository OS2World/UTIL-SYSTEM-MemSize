#include "memsize.h"
:userdoc.
:title.Seg¡ts‚g a &apos.System Resources&apos.-hoz
:body.

:h1 res=ID_MAIN.Bevezet‚s
:i1.Bevezet‚s
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
A program t”bb, a rendszerer‹forr sokkal ”sszefgg‹ elemet jelen¡t meg,
‚s m sodpercenk‚nt friss¡ti a kijelz‹t (m r ha elegend‹
CPU id‹t kap). A megjelen¡tett elemek a k”vetkez‹k&colon.
:p.
:hp2.D tum/Id‹:ehp2. - Az aktu lis d tum ‚s id‹, a rendszer
CONFIG.SYS f jlj nak &apos.COUNTRY=&apos. sor ban be ll¡tott
alap‚rtelmezett orsz ginform ci¢ szerint megadott form tumban.
:p.
:hp2.šzemid‹:ehp2. - A legut¢bbi rendszerind¡t s ¢ta eltelt id‹.
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.Szabad fizikai mem¢ria:ehp2. - A szabad fizikai mem¢ria
mennyis‚ge a :hp1.DosMemAvail:ehp1. fggv‚ny visszat‚r‚si ‚rt‚ke alapj n.
:note.A fggv‚ny a r‚gi 16 bites rendszer r‚sze, csak a
16MB alatti terletet jelzi, ‚s az OS/2 2.0 kiad sa el‹tt
megv ltoztatt k, hogy mindig legal bb 512K-t adjon vissza.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Szabad virtu lis mem¢ria:ehp2. - A rendelkez‚sre  ll¢
virtu lis mem¢ria m‚rete a :hp1.DosQuerySysInfo:ehp1. fggv‚ny
visszat‚r‚si ‚rt‚ke alapj n.
Ez a szabad fizikai mem¢ria, a jelenleg haszn lt t rcseref jlb¢l
fennmarad¢ hely ‚s a t rcseref jl maxim lis n”veked‚si m‚ret‚nek
”sszege m¡nusz a t rcser‚re haszn lt meghajt¢n fenntartott szabad
hely.
:p.
:hp2.T rcseref jl m‚rete:ehp2. - A rendszer virtu lismem¢ria-f jlj nak, a
SWAPPER.DAT-nak m‚rete.
A f jl megkeres‚s‚hez a CONFIG.SYS f jl SWAPPATH bejegyz‚s‚t haszn lja a program.
A bejegyz‚s megadja a t rcseref jl alk”nyvt r nak teljes nev‚t,
‚s jelzi a t rcseref jlt tartalmaz¢ meghajt¢n szabadon hagyand¢
minim lis terlet m‚ret‚t.
:p.
:hp2.T rcser‚hez rendelkez‚sre  ll:ehp2. - A t rcseref jlt
tartalmaz¢ logikai meghajt¢n tal lhat¢ szabad terlet m¡nusz a minim lis
szabad terlet. Ez adja meg, szks‚g eset‚n mennyit n‹het m‚g a t rcseref jl.
:p.
:hp2.T rcseref jlb¢l szabad:ehp2. - Az aktu lisan lefoglalt
t rcseref jlban tal lhat¢ szabad terlet m‚rete. Ha null ra cs”kken,
a t rcseref jlt meg kell n”velni,  ltal ban egy megab jtos l‚p‚sekben.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Zsilipf jl m‚rete:ehp2. - A zsilipf jlok t rol s ra haszn lt lemezterlet m‚rete.
:p.
:hp2.CPU terhel‚s (%):ehp2. - A CPU sz m¡t si teljes¡tm‚ny‚nek k”zel¡t‹
kihaszn lts ga a jelen pillanatban. Az el‹z‹ m sodperc  tlag‚rt‚ke.
:note.Under versions of OS/2 before release 4.0, ez a funkci¢ ‚s a PULZUS nem j”nnek ki j¢l egym ssal.
:link reftype=hd res=60001.Mi‚rt?:elink.
:note.Under versions of OS/2 before release 4.0, ha telep¡tetted a :link reftype=hd res=60000.HRTIMER.SYS:elink.
eszk”zvez‚rl‹t, a sz m pontosabb lesz.
:link reftype=hd res=60002.Mi‚rt?:elink.
:p.
:hp2.Average Load (%):ehp2. - The approximate percentage of the CPU&apos.s available
power that has been used in the recent past.  The number of seconds to be 
averaged may be configured by the user 
(see the :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Configuration:elink. dialog).
:p.
:hp2.Akkumul tor:ehp2. - A jelenleg rendelkez‚sere  ll¢ kapacit st, vagy
az akkumul tor  llapot t (T”lt‚s, H l¢zat) jelzi.
:p.
:hp2.Akt¡v feladatok sz ma:ehp2. - A rendszer v lt¢list j ban,
a CTRL+ESC megnyom s ra megjelen‹ list ban l that¢ bejegyz‚sek sz ma.
:note.A rendszer v lt¢list j nak nem minden eleme jelenik meg az Akt¡v
feladatok list j ban. Egyesek megjelen¡t‚se le van tiltva.
:p.
:hp2.Folyamatok sz ma:ehp2. - A rendszerbe jelenleg bet”lt”tt
folyamatok sz ma.
:p.
:hp2.Sz lak sz ma:ehp2. - A rendszeren jelenleg fut¢ sz lak sz ma. Minden feladathoz legal bb egy sz l tartozik.
:p.
:hp2.™sszes szabad lemezterlet:ehp2. - Az ”sszes el nem t vol¡that¢
lemezen (merevlemezen) tal lhat¢ szabad terlet ”sszege.
:p.
:hp2.X: meghajt¢ szabad terlete:ehp2. - Az X: meghajt¢n tal lhat¢ szabad
lemezterlet.
:p.
Mint azt m r l thattad, a seg¡ts‚g mûk”dik, ‚s a rendelkez‚sre  ll¢
parancsok el‚rhet‹ek az ablak rendszermenj‚ben. A k”vetkez‹
parancsok haszn lhat¢k&colon.
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.Alap‚rtelmez‚sek vissza ll¡t sa:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.Vez‚rl‹elemek elrejt‚se:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.Be ll¡t sok...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.Terhel‚sm‚r‹ alaphelyzetbe:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.Meghajt¢hib k t”rl‚se:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Launch E-Mail Reader:elink.
:li.:link reftype=hd res=IDM_COPY.M sol s:elink.
:li.:link reftype=hd res=IDM_ABOUT.A programr¢l...:elink.
:li.:link reftype=hd res=IDM_EXIT.Bez r s:elink.
:esl.:p.
A m r le¡rt szolg ltat sok mellett a program elfogadja az OS/2 2.0 Betûk‚szlet-
‚s Sz¡npaletta programok parancsait, ‚s DDE szolg ltat¢k‚nt
mûk”dik az ”sszes megjelen¡tett elemre n‚zve.  A DDE alkalmaz s neve
MEMSIZE, az elemek nevei megegyeznek az alap‚rtelmezett megjelen¡t‚si
nevekkel (p‚ld ul Szabad fizikai mem¢ria).

:h1 res=IDM_RESET_DEFAULTS.Alap‚rtelmez‚sek vissza ll¡t sa (Mensor)
:i1.Alap‚rtelmez‚sek vissza ll¡t sa (Mensor)
Az opci¢ kiv laszt sa vissza ll¡tja a program betûk‚szlet‚t ‚s sz¡neit
az alap‚rtelmez‚sre.

:h1 res=IDM_HIDE_CONTROLS.Vez‚rl‹elemek elrejt‚se (Mensor)
:i1.Vez‚rl‹elemek elrejt‚se (Mensor)
Az opci¢ kiv laszt sa elrejti a program keret-vez‚rl‹elemeit
(a rendszerikont, a c¡msort ‚s a zsugor¡t¢gombot).
Az opci¢t ki-be kapcsolgathatod b rmelyik eg‚rgombbal dupl n kattintva.
Mivel nagyon hasznosnak tal ltam, hogy az ablakot elrejtett vez‚rl‹elemekkel
is lehessen mozgatni, b rmelyik eg‚rgombbal vontathat¢ra alak¡tottam ki.
:p.
Az Alt+H billentyûkombin ci¢ ugyanezt a funkci¢t ny£jtja.

:h1 res=IDM_CONFIGURE.Be ll¡t sok (Mensor)
:i1.Be ll¡t sok (Mensor)
Az opci¢ kiv laszt sa megjelen¡ti a program be ll¡t s ra szolg l¢
p rbesz‚dpanelt.
Tov bbi inform ci¢‚rt l sd&colon. 
:link reftype=hd res=IDD_CONFIGURE.Be ll¡t sok (Jegyzett”mb):elink..
:note.Az opci¢ gyorsbillentyûje az Alt+C.

:h2 res=IDD_CONFIGURE.Be ll¡t sok (Jegyzett”mb)
A jegyzett”mb seg¡ts‚g‚vel a program mûk”d‚s‚nek minden  ll¡that¢
jellemz‹j‚t megv ltoztathatod. Az al bbi be ll¡t si lehet‹s‚geket
tal lhatod kateg¢ria szerint csoportos¡tva a k”vetkez‹ oldalakon&colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.Elemek:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.Be ll¡t sok (1. oldal):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Be ll¡t sok (2. oldal):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.Sz¡nek:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.Ablakhorgony:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Date Format:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (1. oldal):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (2. oldal):elink.
:esl.:p.
Minden v ltoztat s azonnal ‚letbe l‚p.

:h3 res=IDD_CONFIG_PAGE_ITEMS.Elemek (Be ll¡t¢oldal)
Az oldal felsorolja a megjelen¡thet‹ elemeket.
A Megjelen¡tend‹ elemek lista b rmely vagy ak r minden elem‚t kiv laszthatod, ha
az(oka)t figyeltetni ‚s l tni akarod.
Egyszerû kattint ssal kijel”lheted b rmely elemet, illetve t”r”lheted
annak kiv laszt s t.
A program a kijel”lt elemeket figyeli.
To change an item&apos.s default label, or to adjust its warning and/or error 
thresholds, either double-click mouse button one on the item list entry, or single-click
mouse button two on the entry.  In either case, the 
:link reftype=hd res=IDD_LABEL.Item Attribute:elink. dialog will be displayed.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.Be ll¡t sok (1. oldal) (Be ll¡t¢oldal)
Az oldal sz mos, a programban ki/bekapcsolhat¢ opci¢t sorol fel.
:p.
:hp2.Vez‚rl‹elemek elrejt‚se:ehp2.
Ha a programablakot nem ikoniz ltad, megjelen¡theted a szok sos
c¡msorral, rendszerikonnal ‚s minimaliz l¢/maximaliz l¢ gombokkal,
illetve ezek n‚lkl. Ha elrejted a vez‚rl‹elemeket, az ablak tov bbra
is mozgathat¢ k”z‚ppontj nak vontat s val.
:p.
:hp2.Mindig l that¢:ehp2.
A jel”l‹n‚gyzet bekapcsolja az ablak lebegtet‹ funkci¢j t.
A jel”l‹n‚gyzetet kipip lva az ablak a t”bbi ablak felett lebeg,
¡gy mindig l that¢ marad.
:p.
:hp2.Anim ci¢:ehp2.
A jel”l‹n‚gyzet bekapcsolja az ablaknyit s/z r s anim ci¢ funkci¢t.
Ha bekapcsolod, a programablakok £gy ny¡lnak/z r¢dnak, mint a
gyûjt‹objektumok&colon. hanggal ‚s mozg ssal.
Mint a gyûjt‹k eset‚n, az anim ci¢ csak akkor mûk”dik, ha rendszerszinten
enged‚lyezted.
:p.
:hp2.Dialog Border:ehp2.
This button determines whether the main program window will have 
a simple line border, or a more elaborate dialog border.  The line
border requires less space on the screen, but the dialog border can
look better, especially on OS/2 Warp 4.0 or later, which gives this
border style a three-dimensional appearance.
:p.
:hp2.T bl zat alak:ehp2.
Ha kipip lod, az ablak t bl zat form j ban lesz l that¢, a bejegyz‚sek
sorokba ‚s oszlopokba rendezve jelennek meg.
Az ablak optimaliz lni fogja mag t, hogy a lehet‹ legkisebb fgg‹leges kiterjed‚st ‚rje el.
Ha a jel”l‹n‚gyzet nincs kipip lva, az ablak egyetlen oszlopban jelen¡ti meg az
”sszes k‚rt infom ci¢t.
:p.
:hp2.àr nk‚nti hangjelz‚s:ehp2.
A gomb bekapcsolja az ¢r nk‚nti hangjelz‚s szolg ltat st. Ha akt¡v, a program
minden eg‚sz ¢rakor k‚tszer csippant.
:p.
:hp2.Show Disk Drives:ehp2.
When this button is checked, disk drive free space is monitored, subject
to the restrictions placed on this monitoring by other options.  
This control mainly exists as a way to disable disk monitoring.
:p.
:hp2.T voli meghajt¢k megjelen¡t‚se:ehp2.
A jel”l‹n‚gyzetet kipip lva a jelenleg a rendszerhez kapcsol¢d¢ t voli (h l¢zati) meghajt¢kat
ugyan£gy figyeli a program, mint a helyi nem elt vol¡that¢ meghajt¢kat, azzal a kl”nbs‚ggel,
hogy az ”sszes szabad lemezkapacit sba nem sz m¡tanak bele.
:p.
:hp2.F jlrendszern‚v megjelen¡t‚se:ehp2.
A jel”l‹mez‹t kipip lva az egyes meghajt¢k szabad kapacit s t tartalmaz¢
sorokban megjelenik a meghajt¢n haszn lt f jlrendszer neve. Az opci¢ haszn lata sz‚lesebb
megfigyel‹ablakot eredm‚nyez.
:p.
:hp2.Lemezc¡mke megjelen¡t‚se:ehp2.
A jel”l‹mez‹t kipip lva az egyes meghajt¢k szabad kapacit s t tartalmaz¢
sorokban megjelenik azok lemezc¡mk‚je. Az opci¢ haszn lata sz‚lesebb
megfigyel‹ablakot eredm‚nyez.
:p.
:hp2.M sodpercek megjelen¡t‚se:ehp2.
A jel”l‹n‚gyzet kipip lva az ¢ra ‚s zemid‹ sorokban megjelennek
a m sodpercek.
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

:h3 res=IDD_CONFIG_PAGE_OPTIONS2.Be ll¡t sok (2. oldal) (Be ll¡t¢oldal)
Ez az oldal az ”sszetettebb, nem csak ki/bekapcsolhat¢ opci¢kat tartalmazza.
:p.
:hp2.&apos.K&apos.ilob jt kijelz‚s:ehp2.
A r di¢gombok ezen csoportj val kiv laszthatod, hogy a nagy m‚reteket
mindig, soha, vagy csak 512K f”l”tt szeretn‚d-e kilob jt jel”l‚sben
l tni.
Az alap‚rtelmez‚s szerint az 512K feletti m‚retek jelennek meg ¡gy.
.br
Ha kiv lasztod a &apos.K=1024&apos. gombot, a sz mok t‚nyleges K (1024 egys‚g) felbont sban
jelennek meg. Ha nem, a K t¡zes rendszerben ‚rtend‹
(azaz 1000 egys‚get jelent).
.br
Ha kipip lod a &apos.M&apos.egab jt gombot, a nyolc megab jtot meghalad¢ m‚retek
megab jt egys‚gben jelennek meg.
.br
The &apos.G&apos.igabytes button, if checked, will cause the numbers to be
displayed in gigabytes once their values exceed four gigabytes.
:p.
:hp2.Megfigyel‹sz l priorit sa:ehp2.
A vez‚rl‹mez‹ seg¡ts‚g‚vel be ll¡thatod a program megfigyel‹sz l nak
abszol£t priorit s t az id‹kritikus priorit soszt lyon bell.
A priorit s‚rt‚k 0-t¢l 31-ig v ltozhat. A 31-es priorit s a
rendszerben el‚rhet‹ legmagasabb priorit s a fizikai megszak¡t sokt¢l eltekintve.
A 0-s priorit s egyszerûen a legalacsonyabb priorit s az id‹kritikus oszt lyon bell. 
Norm lis esetben a pontosabb m‚r‚s el‚r‚s‚hez ezt a lehet‹ legmagasabbra kell be ll¡tani. 
P ran azonban panaszkodtak, hogy kommunik ci¢s programjaik id‹nk‚nt adatot vesztenek a figyel‹program
fut sa k”zben, ez‚rt a priorit st be ll¡that¢v  tettem.
:p.
:hp2.Friss¡t‚si id‹k”z:ehp2.
Ez hat rozza meg a figyel‹ablak k‚t friss¡t‚se k”zt eltel‹ id‹t. rt‚ke 1,0 m sodperct‹l
30,0 m sodpercig v ltoztathat¢ tizedm sodperces l‚p‚sekben. A megjelen‹ sz m tizedm sodpercben ‚rtend‹.
:p.
:hp2.Averaging Interval:ehp2.
This control allows you to set the interval over which averages are
computed.  It can be varied from 10 seconds to 3600 seconds (one hour).

:h3 res=IDD_CONFIG_PAGE_COLORS.Sz¡nek (Be ll¡t¢oldal)
Ez az oldal mutatja a Norm l, Figyelmeztet‚s ‚s Hiba szinteket t mogat¢ elemekhez
rendelt h rom sz¡nt. A sz¡neket £gy v ltoztathatod meg, hogy a rendszer
sz¡npalett inak valamelyik‚b‹l idevontatod a k¡v nt sz¡neket.

:note.OS/2 Warp 3.0 vagy £jabb eset‚n k‚nyelmi szolg ltat sk‚nt a sz¡nmint kon
b rmelyik eg‚rgombbal kattintva megjelen¡theted a Tisza sz¡nek palett j t.
A Kevert sz¡nek palett j t is haszn lhatod a sz¡nek kiv laszt s ra, de ezt
a Rendszerbe ll¡t sok gyûjt‹ben neked kell megnyitnod.

:h3 res=IDD_CONFIG_PAGE_ANCHOR.Ablakhorgony (Be ll¡t¢oldal)
Ez az oldal tartalmazza azt a vez‚rl‹elemet, amellyel kijel”lheted a
megfigyel‹ablaknak azt a sark t, amely az ablak m‚ret‚nek
megv ltoz sakor hely‚n marad. A megfigyel‹ablak m‚rete megv ltozhat,
ha elemeket veszel fel a list ra illetve t vol¡tasz el onnan, vagy
a Betûk‚szlet-paletta seg¡ts‚g‚vel megv ltoztatod a megjelen¡t‚shez
haszn lt betûk‚szletet.

:h3 res=IDD_CONFIG_PAGE_DATEFMT.Date Format (Be ll¡t¢oldal)
This page allows you to specify the format with which the date &amp. time
item will be displayed.  Any text you would like may be entered, and
special function codes can be included.  The entire list of valid 
function codes is provided in a list at the bottom of the page, and
by double-clicking on any item in that list the item is immediately
inserted into the current format text.  A sample result text, with 
the function codes expanded, is given right below the format text
entry field.

:h3 res=IDD_CONFIG_PAGE_POP31.E-Mail (Be ll¡t¢oldal)
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

:h3 res=IDD_CONFIG_PAGE_POP32.E-Mail (Be ll¡t¢oldal)
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

:h3 res=IDD_LABEL.Elemtulajdons gok (P rbesz‚dpanel)
A p rbesz‚dpanel a megjelen¡tend‹ elemek jelenlegi ‚s alap‚rtelmezett c¡mk‚it,
valamint - ha az adott elem rendelkezik ilyen be ll¡t sokkal - az aktu lis
Figyelmeztet‚s ‚s Hiba szinteket jelen¡ti meg. A c¡mke megv ltoztat s hoz egyszerûen ¡rd fell az aktu lis
c¡mk‚t az £jjal! A hiba ‚s figyelmeztet‚s szint megv ltoztat s hoz add meg a
k¡v nt szintet a beviteli mez‹ben, vagy haszn ld az adott szinthez tartoz¢ cs£szk t!
:p.Az Alap‚rtelmez‚s gomb megnyom s val az ”sszes be ll¡t st vissza ll¡thatod az
alap‚rtelmezett ‚rt‚kekre.

:h1 res=IDM_RESETLOAD.Terhel‚sm‚r‹ alaphelyzetbe (Menelem)
:i1.Terhel‚sm‚r‹ alaphelyzetbe (Menelem)
Az opci¢ seg¡ts‚g‚vel alap llapotba hozhatod a CPU terhel‚sm‚r‹t,
£jrakalibr lva azt. Ennek hat s ra a program nem veszi figyelembe a
kor bbi CPU aktivit st a sz m¡t sok folyam n.

:note.This menu option is not available when on Warp 4.0 or better,
nor on certain higher fixpack levels of Warp 3.0, as the newer load
measuring technique makes this command irrelevant.

:h1 res=IDM_RESETAVERAGE.Reset Average Load (Menelem)
:i1.Reset Average Load (Menelem)
This menu option allows the user to reset the Average Load meter,
causing it to forget previous CPU activity in its calculations.
:p.
The short-cut key for this command is Alt+R.

:h1 res=IDM_RESETDRIVES.Meghajt¢hib k t”rl‚se (Menelem)
:i1.Meghajt¢hib k t”rl‚se (Menelem)
Az opci¢ seg¡ts‚g‚vel t”r”lheted az esetleg bek”vetkezett meghajt¢hib kat. Ez id‹vel ”nmûk”d‹en is megt”rt‚nik, de a parancs seg¡ts‚g‚vel azonnal is t”r”lheted
a hib t.

:h1 res=IDM_LAUNCHEMAILER.Launch E-Mail Reader (Menelem)
:i1.Launch E-Mail Reader (Menelem)
This option will launch the e-mail reader configured by the user.
See :link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail:elink. for more
information.

:h1 res=IDM_COPY.M sol s (Menelem)
:i1.M sol s (Menelem)
Az opci¢ az aktu lis ablaktartalmat a v g¢lapra helyezi k‚t
form tumban&colon. sz”vegesen ‚s bitt‚rk‚pesen. A sz”veges verzi¢
az aktu lisan megjelen¡tett elemeket mutatja sz”vegszerkeszt‹be
beilleszthet‹ form ban.  A bitt‚rk‚pes v ltozat az ablak pontos m solata,
grafikus szerkeszt‹kben ‚s n‚zeget‹kben haszn lhat¢.
:p.
A parancs gyorsbillentyûje a Ctrl+Ins.

:h1 res=IDM_ABOUT.A programr¢l... (Menelem)
:i1.A programr¢l... (Menelem)
Az opci¢ kiv laszt sa megjelen¡ti a programinform ci¢t tartalmaz¢
p rbesz‚dpanelt.
Tov bbi inform ci¢‚rt l sd&colon. :link reftype=hd res=IDD_ABOUT.A programr¢l (P rbesz‚dpanel):elink..
:h2 res=IDD_ABOUT.A programr¢l (P rbesz‚dpanel)
A p rbesz‚dpanel a program nev‚t, ikonj t ‚s szerz‹i jogi inform ci¢j t
jelen¡ti meg.
A p rbesz‚dpanelb‹l az ENTER, SPACE vagy ESCAPE billentyû
megnyom s val, illetve az OK gombra kattintva l‚phetsz ki.

:h1 res=IDM_EXIT.Bez r s (Menelem)
:i1.Bez r s (Menelem)
Az opci¢ kiv laszt sa a program befejez‚s‚t eredm‚nyezi.

:h1 res=IDM_KEYS_HELP.Seg¡ts‚g a billentyûkh”z
:i1.Seg¡ts‚g a billentyûkh”z
A programban a k”vetkez‹ gyorsbillentyûk haszn lhat¢k&colon.
:sl compact.
:li.F1 - Seg¡ts‚g
:li.F3 - :link reftype=hd res=IDM_EXIT.Bez r s:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.Be ll¡t sok:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.Vez‚rl‹elemek elrejt‚se:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.Ctrl+Ins - :link reftype=hd res=IDM_COPY.M sol s:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.Be ll¡t sf jl el‚r‚si £tvonala (P rbesz‚dpanel)
:i1.Be ll¡t sf jl el‚r‚si £tvonala (P rbesz‚dpanel)
Ez a p rbesz‚dpanel akkor jelenik meg, ha a program nem tal lja a be ll¡t sokat
tartalmaz¢ (INI) f jlt, ‚s megk‚rdezi, hol van a f jl, illetve hol hozza l‚tre.
:p.
Csak ‚rv‚nyes, l‚tez‹ alk”nyvt rn‚v fogadhat¢ el.
A n‚v megad sa ut n a program folytat s hoz nyomd meg az ENTER
billentyût vagy kattints az OK nyom¢gombra!
:p.
Ha le akarod l‹ni a program inicializ l¢ l‚p‚seit, nyomd meg az
ESC billentyût vagy kattints az ESC = M‚gsem gombra!

:h1 res=60000.HRTIMER - High Resolution Timer (nagyfelbont s£ id‹z¡t‹) eszk”zvez‚rl‹
A nagyfelbont s£ id‹z¡t‹ eszk”zvez‚rl‹, a HRTIMER.SYS seg¡ts‚g‚vel a program
hozz f‚rhet az IBM PC-kben tal lhat¢, egy‚bk‚nt nem haszn lt
id‹z¡t‹- ramk”rh”z. Ennek felbont sa 840 nanoszekundum, 64K-szor jobb,
mint a norm l id‹z¡t‹ 55 milliszekundumos felbont sa.
:p.
Azok a helyek, ahol biztosan tudom, hogy ez a program el‚rhet‹, a k”vetkez‹k&colon.
:hp4.CompuServe:ehp4.,
ahol a program az
:hp4.OS/2 Developer&apos.s Forum I (OS2DF1):ehp4.
:hp4.Device Driver Development:ehp4.
k”nyvt r ban tal lhat¢
:hp4.TIMER.ZIP:ehp4. n‚ven, illetve
az
:hp4.ftp.cdrom.com:ehp4.
‚s a
:hp4.hobbes.nmsu.edu:ehp4.
anonymous ftp kiszolg l¢k.
A f jl a
:hp4./pub/os2/dev16:ehp4.
alk”nyvt rban,
:hp4.hrtimer.zip:ehp4.
n‚ven tal lhat¢.
:p.Az eszk”zmeghajt¢ forr sk¢dja az IBM Personal Systems Developer magazin
1991. ‹szi sz m ban tal lhat¢. A program freeware, de a szerz‹i jog
az IBM birtok ban van.

:h1 res=60001.Mi a PULZUS-konfliktus oka?
Ez a program, mint ahogy a PULZUS, a CPULOAD, CPUMETER ‚s m s,
a sz m¡t¢g‚p kihaszn lts g t m‚r‹ programok, ezt a legalacsonyabb
priorit ssal elv‚gezhet‹ munka mennyis‚g‚t m‚rve teszi. K‚ts‚gk¡vl, ha ezek a programok b rmit is
k‚pesek tenni ezen a priorit son, azt a rendszer t‚tlens‚g‚nek
idej‚n teszik. Ha nincs t‚tlens‚gi id‹, a sz m¡t¢g‚p 100%-ig
ki van haszn lva. Min‚l t”bb munk t v‚gez a t‚tlen priorit s£
program, ann l kev‚sb‚ foglalt a g‚p.
:p.A probl‚ma az, hogy ha egyszerre egyn‚l t”bb ilyen programot
futtatsz, azok versengeni fognak a t‚tlens‚gi id‹‚rt. Ha ugyanannak a programnak k‚t p‚ld ny t futtatn d, ‚s semmi m st,
mindkett‹ k”rlbell 50% CPU terhel‚st jelezne. Ha k‚t elt‚r‹
programot futtatsz, mint p‚ld ul ez ‚s a PULZUS, az mindkett‹re
hat ssal lesz, de  ltal ban elt‚r‹ m‚rt‚kben.

:h1 res=60002.Hogyan seg¡t a HRTIMER.SYS?
Ha telep¡ted a HRTIMER.SYS-t, a rendszer holtideje is megjelenik a CPU terhel‚sben.
:p.
A CPU terhel‚s meghat roz s hoz a t‚tlen priorit s£ program munk j t
az eddigi legmagasabb teljes¡tm‚nnyel hasonl¡tja ”ssze a program. Tov bbi inform ci¢ n‚lkl a rendszer norm l h tt‚rtev‚kenys‚ge
nulla terhel‚snek l tszik. Ha rendszeredre telep¡tetted a HRTIMER.SYS-t,
a program kalibr l¢ciklust futtat indul skor, hogy meghat rozza
a legmagasabb priorit ssal elv‚gezhet‹ munka mennyis‚g‚t. Ezt a mennyis‚get haszn lja k‚s‹bb az ”sszehasonl¡t shoz.

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
