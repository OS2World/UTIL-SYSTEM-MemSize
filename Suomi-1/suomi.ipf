#include "memsize.h"
:userdoc.
:title.J„rjestelm„monitorin opastus
:body.

:h1 res=ID_MAIN.Esittely
:i1.Esittely
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
T„m„ ohjelma monitoroi ja n„ytt„„ useita j„rjestelm„resursseja
p„ivitt„en ne kerran sekunnissa prosessorikuorman niin salliessa.
N„ytett„v„t ja monitoroitavat resurssit ovat&colon.
:p.
:hp2.P„iv„ ja kellonaika:ehp2. - P„iv„ ja kellonaika siin„ muodossa, jossa
ne CONFIG.SYS-tiedoston &apos.COUNTRY=&apos.-rivin m„„r„„m„n
maan oletusasetuksen mukaan esitet„„n.
:p.
:hp2.Aika alkulatauksesta:ehp2. - Aika siit„ kun ty”asema on viimeksi
k„ynnistetty ja OS/2 ladattu.
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.Fyysist„ muistia vapaana:ehp2. - Fyysisen vapaan muistin m„„r„
j„rjestelm„kutsun :hp1.DosMemAvail:ehp1. mukaan.
:note.T„m„ j„rjestelm„kutsu on osa OS/2&colon. vanhaa 16 bittist„ osaa, ja
ainoastaan palauttaa tiedon muistista, joka sijaitsee 16 megatavun rajan
alapuolella. T„t„ j„rjestelm„kutsua muutettiin juuri ennen OS/2 2.0&colon.
julkaisua siten, ettei se koskaan palauta lukua alle 512 kilotavua.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Virtuaalimuistia vapaana:ehp2. - Virtuaalimuistin vapaa m„„r„
j„rjestelm„kutsun :hp1.DosQuerySysInfo:ehp1. mukaan.
Laskentakaava on seuraava: vapaa fyysinen muisti + t„m„n hetkisen
swapper.dat-tiedoston vapaa tila + vapaa levytila swapper.dat-tiedoston
levyll„ - varattu vapaa tila swapper.dat-tiedoston levyll„, joka
m„„ritell„„n CONFIG.SYS-tiedostossa SWAPPATH-rivill„.
:p.
:hp2.swapper.dat&apos.in koko:ehp2. - T„m„n hetkinen SWAPPER.DAT-tiedoston koko.
T„t„ sivutustiedostoa k„ytet„„n OS/2&colon. virtuaalimuistin hallinnassa.  Tiedosto
etsit„„n tutkimalla CONFIG.SYS-tiedoston SWAPPATH-rivi„.  T„ll„ rivill„
on t„ydellinen polku swapper.dat-tiedoston hakemistoon.
:p.
:hp2.Swap-tilaa vapaana:ehp2. - Vapaa levytila levyosiolla, jossa
swapper.dat-tiedosto sijaitsee, poislukien varattu vapaa tila.  T„m„
arvo kertoo paljonko swapper.dat-tiedosto voi tarvittaessa kasvaa.
:p.
:hp2.K„ytt„m„t”n swap-tila:ehp2. - T„m„n hetkisess„ swapper.dat-tiedostossa
oleva k„ytt„m„t”n vapaa tila.  Kun t„m„ arvo putoaa nollaan, kasvatetaan
swapper.dat-tiedoston kokoa.  Normaalisti koko kasvaa yhden megatavun
(1024 kilotavua) kerrallaan.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Spool-hakemiston koko:ehp2. - OS/2&colon. tulostuspalvelun
tulostustiedostojen kuluttama levytila.  Tieto kattaa kaikkien tulostimien
kaikki tulostustiedostot.
:p.
:hp2.Prosessorikuorma:ehp2. - Arvio ty”aseman mikroprosessorin kuormasta
prosentteina.  Arvio lasketaan keskiarvona edellisen sekunnin kuormasta.
:note.OS/2&colon.t ennen versiota 4.0&colon. T„m„ toiminto ja PULSE-ohjelma eiv„t toimi yht„aikaa.
:link reftype=hd res=60001.Miksi?:elink.
:note.OS/2&colon.t ennen versiota 4.0&colon. Jos koneeseen on asennettu :link reftype=hd res=60000.HRTIMER.SYS:elink.
-ajuri, on arvio tarkempi.
:link reftype=hd res=60002.Miksi?:elink.
:p.
:hp2.Keskim„„r„inen prosessorikuorma (%):ehp2. - Arvioitu keskim„„r„inen prosessorin kuormitusaste.
Luku ilmaistaan prosenttina.
Aika jolta kuormitusaste lasketaan, voidaan s„„t„„.
(Katso kohtaa :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Muokkaa asetuksia:elink.)
:p.
:hp2.Akku:ehp2. - Akun t„m„n hetkien varausaste tai sen tila
(varaa/verkko).
:p.
:hp2.Aktiivisia t”it„:ehp2. - OS/2&colon. ohjelmaluettelon koko, joka on
likim„„rin sama kuin ikkunaluettelon (Window List) koko.  Ikkunaluettelo
saadaan esiin painamalla CTRL+ESC.
:note.Kaikkien ohjelmia ei n„ytet„ ikkunaluettelossa.  Jotkin
j„rjestelm„n perusohjelmat on tarkoituksella piilotettu.
:p.
:hp2.Prosessien m„„r„:ehp2. - T„m„n hetkinen prosessien m„„r„ OS/2&colon.ssa.
:p.
:hp2.S„ikeiden m„„r„:ehp2. - T„m„n hetkinen ohjelmas„ikeiden m„„r„
OS/2&colon.ssa.  Jokaisessa prosessissa on ainakin yksi ohjelmas„ie.
:p.
:hp2.Vapaa levytila yhteens„:ehp2. - Kaikkien paikallisten
kiintolevyosioiden yhteen laskettu vapaa levytila.
:p.
:hp2.Aseman X vapaa tila:ehp2. - Vapaa levytila asemalla X.
:p.
Opastus on aktiivinen, kuten olet jo huomannut.  Ohjelmalle voidaan antaa
komentoja sen j„rjestelm„valikon kautta.  Seuraavat komennot ovat k„yt”ss„&colon.
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.Palauta oletusasetukset:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.Piilota otsikkorivi:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.Muokkaa asetuksia...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.Kalibroi kuormamittari:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Laske keskim. kuorma:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.Poista levyvirheet:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Launch E-Mail Reader:elink.
:li.:link reftype=hd res=IDM_COPY.Kopioi leikep”yd„lle:elink.
:li.:link reftype=hd res=IDM_ABOUT.Tuoteinformaatio:elink.
:li.:link reftype=hd res=IDM_EXIT.Sulje:elink.
:esl.:p.
Jo kuvattujen ominaisuuksien lis„ksi t„m„ ohjelma hyv„ksyy komentoja
OS/2&colon. kirjasin- ja v„ripaleteista.  Ohjelma toimii my”s DDE-palvelimena
kaikille n„ytett„ville resursseille.  DDE-sovelluksen nimi on MEMSIZE
ja resurssien nimet ovat resurssien oletusnimi„, esim.
Fyysist„ muistia vapaana.

:h1 res=IDM_RESET_DEFAULTS.Palauta oletusasetukset (Valikossa)
:i1.Palauta oletusasetukset (Valikossa)
T„m„ toiminto palauttaa ohjelman kirjasin ja v„riarvot niiden
oletusarvoihin.

:h1 res=IDM_HIDE_CONTROLS.Piilota otsikkorivi (Valikossa)
:i1.Piilota otsikkorivi (Valikossa)
T„m„ toiminto vuorotellen piilottaa ja palauttaa n„kyviin ohjelman
otsikkorivin.  Toiminto voidaan toteuttaa my”s kaksoinapsauttamalla
ohjelman ikkunaa kummalla tahansa hiiren painikkeella.  Koska ohjelman
ikkunan helppo siirt„minen OS/2&colon. ty”p”yd„ll„ on k„yt„nn”llist„,
on ikkunan siirt„minen mahdollista my”s tarttumalla kummalla tahansa
hiiren painikkeella ikkunasta ja liikuttamalla hiirt„.
:p.
My”s n„pp„inyhdistelm„ Alt+H tekee t„m„n toiminnon.

:h1 res=IDM_CONFIGURE.Muokkaa asetuksia... (Valikossa)
:i1.Muokkaa asetuksia... (Valikossa)
T„m„n toiminnon valitseminen avaa ohjelman muokkausdialogi-ikkunan.

Lis„tietoja: Valitse :link reftype=hd res=IDD_CONFIGURE.Muokkaa asetuksia... (Dialogi):elink..
:note.My”s n„pp„inyhdistelm„ Alt+C tekee t„m„n toiminnon.

:h2 res=IDD_CONFIGURE.Muokkaa asetuksia... (Dialogi)
T„m„n muistikirjadialogin avulla voit muuttaa ohjelman asetuksia.
Muutettavat asetukset on ryhmitelty kategorioihin
seuraaville sivuille&colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.Resurssit:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.Asetukset (Sivu 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Asetukset (Sivu 2):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.V„rit:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.Ikkunan kiinnitys:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.P„iv„m„„r„n muotoilu:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (Sivu 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (Sivu 2):elink.
:esl.:p.
Kaikki muutokset astuvat heti voimaan.

:h3 res=IDD_CONFIG_PAGE_ITEMS.Resurssit (Muistikirjan sivu)
T„ll„ sivulla on luettelo n„ytett„vist„ resursseista.
Voit valita mink„ tai mitk„ tahansa resurssit Asetusten muokkaus
-listasta.  Valitseminen tehd„„n korostamalla resurssin nime„ yhdell„
hiiren napsautuksella.  Valinnan poistaminen k„y samalla tavalla.  Voit
valita listalta haluamasi resurssit.
Muuttaaksesi resurssin otsikkoa tai s„„t„„ksesi sen varoitus- ja/tai
virheraja-arvoa joko kaksoisnapsauta ao. resurssia hiiren 1-n„pp„imell„
tai napsauta sit„ hiiren 2-n„pp„imell„.
the entry.  Molemmissa tapauksissa esiin tulee
:link reftype=hd res=IDD_LABEL.resurssiattribuutti:elink.-dialogi.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.Asetukset (Sivu 1) (Muistikirjan sivu)
T„ll„ sivulla on useita ohjelman asetuksia, jotka voidaan yksinkertaisesti
joko aktivoida tai kytke„ pois p„„lt„.
:p.
:hp2.Piilota otsikkorivi:ehp2.
Kun ohjelmaikkuna on normaalisti esill„ n„yt”ll„, voidaan se n„ytt„„
otsikkorivin kera tai ilman sit„.  Ohjelmaikkunan paikkaa voidaan aina
siirt„„ tarttumalla siit„ kiinni ja siirt„m„ll„ sit„.  Otsikkorivi ei
vaikuta t„h„n.  Yleens„ orsikkorivi kannattaa pit„„ piilossa.
:p.
:hp2.Kellu p„„limm„isen„:ehp2.
T„ll„ asetuksella aktivoidaan ohjelmaikkunan kelluminen.  Kun asetuksessa
on rasti paikoillaan, kelluu ohjelmaikkuna muiden ikkunoiden p„„ll„.
:p.
:hp2.Animoi:ehp2.
T„ll„ asetuksella asetetaan ikkunan avautumisen ja sulkeutumisen
animoiminen.  Asetuksen ollessa aktiivinen avautuu ja sulkeutuu
ohjelmaikkuna kuten muutkin ty”p”yd„n kansiot „„nineen ja
animointineen, ellei n„it„ asetuksia ole kytketty erikseen pois
k„ytt”j„rjestelm„tasolla.
:p.
:hp2.Dialogikehys:ehp2.
T„ll„ asetuksella m„„ritell„„n onko resurssimonitorissa
yksinkertainen viivakehys vai visuaalisempi dialogikehys.
Viivakehys vie v„hemm„n tilaa, mutta dialogikehys n„ytt„„
paremmalta varsinkin OS/2 Warp 4.0- ja uudemmissa versioissa.
Niiss„ dialogikehys on n„ytt„„ kolmiuloitteiselta.
:p.
:hp2.Taulukkomuoto:ehp2.
T„ll„ asetuksella muutetaan monitoroitavien resurssien n„ytt„minen
sellaiseksi, ett„ muotoillaan taulukkoon joka vie mahdollisimman
v„h„n tilaa pystysuunnassa.  Kun asetus ei ole aktivoituna,
n„ytet„„n resurssit yhdess„ sarakkeessa.
:p.
:hp2.Tasatuntih„lytys:ehp2.
T„ll„ asetuksella otetaan k„ytt””n tasatuntih„lytys.  Ohjelma
h„lytt„„ t„ll”in kahdesti tasatunnein.  H„lytys hy”dynt„„ OS/2&colon.n
multimediaa jos se on k„ytett„viss„.
:p.
:hp2.Show Disk Drives:ehp2.
When this button is checked, disk drive free space is monitored, subject
to the restrictions placed on this monitoring by other options.  
This control mainly exists as a way to disable disk monitoring.
:p.
:hp2.N„yt„ verkkolevyt:ehp2.
T„ll„ asetuksella otetaan verkkolevyt mukaan monitoroitavien resurssien
joukkoon.  Verkkolevyjen vapaata levytilaa ei kuitenkaan lasketa mukaan
resurssiin Vapaa levytila yhteens„.
:p.
:hp2.N„yt„ tied.j„rj. tyypit:ehp2.
T„ll„ asetuksella saadaan kunkin n„ytett„v„n levyn vapaan tilan yhteyteen
tieto ko. levyn tiedostoj„rjestelm„.  T„m„ asetus levent„„ ohjelmaikkunan
kokoa.
:p.
:hp2.N„yt„ asemien nimet:ehp2.
T„ll„ asetuksella saadaan kunkin n„ytett„v„n levyn vapaan tilan yhteyteen
tieto ko. levyn nimi”st„.  T„m„ asetus levent„„ ohjelmaikkunan kokoa.
:p.
:hp2.N„yt„ sekunnit:ehp2.
T„m„ asetus lis„„ kaikkiin n„ytett„viin kellonaikoihin mukaan sekunnit
tuntien ja minuuttien yhteyteen.
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

:h3 res=IDD_CONFIG_PAGE_OPTIONS2.Asetukset (Sivu 2) (Muistikirjan sivu)
T„ll„ sivulla on ohjelman asetuksia, jotka ovat monimutkaisempia kuin
pelkk„ p„„lle/pois-kytkent„.
:p.
:hp2.N„yt„ &apos.K&apos.ilotavuina:ehp2.
N„m„ valintanapit antavat mahdollisuuden valita n„ytet„„nk”
suuret luvut kilotavuina aina, ei koskaan, vai silloin kun n„ytett„v„
luku on kooltaan yli 512 kilotavua (oletus).
.br
&apos.K=1024&apos.-asetus muuttaa ohjelman n„ytt„m„t numerot todellisiksi
ATK-kiloiksi (1K = 1024 = 2^10).  Jos asetus ei ole aktivoituna,
n„ytet„„n numerot desimaalisina siten, ett„ kilo on 1000.
.br
T„ll„ asetuksella aktivoidaan muunnos, jolloin kaikki
numerot arvoltaan yli 8 megatavua n„ytet„„n megatavuina (M).
.br
The &apos.G&apos.igabytes button, if checked, will cause the numbers to be
displayed in gigabytes once their values exceed four gigabytes.
:p.
:hp2.Monitorin prioriteetti:ehp2.
T„ll„ asetuksella m„„r„t„„n ohjelman monitorointis„ikeen absoluuttinen
prioriteetti.  S„ie on luokaltaan aikakriittinen (time-critical).
S„ikeen prioriteetti voi vaihdella 0&colon.sta 31&colon.een.  Prioriteetti 31 on
kaikkein korkein prioriteetti j„rjestelm„ss„.  Vain fyysiset
keskeytykset ovat prioriteetiltaan korkeampia.  Prioriteettitaso 0 on
aikakriittisen luokan matalin prioriteetti.  Normaalisti monitorointis„ikeen
prioriteetilla ei ole suurta merkityst„.  Korkeampi prioriteetti tekee
monitoroitavien resurssien arvoista kuitenkin tarkempia.  Toisaalta
korkea prioriteetti saattaa aiheuttaa puuttuvia merkkej„ esim.
kommunikointiohjelmissa.  Prioriteetti 1 voi olla hyv„ l„ht”kohta
kokeiluille.
:p.
:hp2.P„ivitysv„li:ehp2.
T„ll„ asetuksella voit muuttaa monitoroitavien resurssien arvojen
p„ivitysv„li„ n„yt”ll„.  Arvo voi vaihdella 1.0 sekunnin ja 30.0
sekunnin v„lill„ 0.1 sekunnin v„lein.  Asetettava numeroarvo on
siis sekunnin kymmenesosia.
:p.
:hp2.Keskim„„r„isen prosessorikuorman intervalli:ehp2.
T„ll„ asetuksella muutetaan keskim„„r„isen prosessorikuorman
laskennan intervalli.  Arvo voi vaihdella 10 sekunnista 3600
sekuntiin (1 tunti).

:h3 res=IDD_CONFIG_PAGE_COLORS.V„rit (Muistikirjan sivu)
T„ll„ alueella on kolme v„ri„.  N„it„ v„rej„ k„ytet„„n ilmaisemaan
mittarin tasoa&colon. normaali, varoitus tai virhe.  Kaikilla mittareilla ei
ole k„yt”ss„ n„m„ tasot.  Voit asettaa v„rit vet„m„ll„ ja pudottamalla
ne OS/2&colon.n v„ripaletista.

:h3 res=IDD_CONFIG_PAGE_ANCHOR.Ikkunan kiinnitys (Muistikirjan sivu)
N„ill„ valintanapeilla m„„r„t„„n se ohjelman ikkunan kulma, joka
pysyy paikallaan kun ikkunan koko muuttuu.  Koko muuttuu kun
monitoroitavien resurssien m„„r„ muuttuu tai kun k„ytett„v„ merkkilaji
muuttuu.

:h3 res=IDD_CONFIG_PAGE_DATEFMT.P„iv„m„„r„n muotoilu (Muistikirjan sivu)
T„ll„ muistikirjan sivulla voit m„„ritell„ miss„ muodossa
p„iv„m„„r„ ja kellonaika n„ytet„„n.  Voit lis„t„ haluamaasi
vapaamuotoista teksti„ ja laittaa sen joukkoon varsinaiset
muotoilukoodit.  T„ydellinen luettelo muotoilukoodeista on
sivun alareunassa.  Muotoilukoodeja k„ytet„„n kaksoisnapsauttamalla
niit„.  Esimerkki siit„ milt„ p„iv„m„„r„ kulloinkin n„ytt„„,
n„kyy koodien sy”tt”kent„n alla.

:h3 res=IDD_CONFIG_PAGE_POP31.E-Mail (Muistikirjan sivu)
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

:h3 res=IDD_CONFIG_PAGE_POP32.E-Mail (Muistikirjan sivu)
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

:h3 res=IDD_LABEL.Resurssiattribuutit (Dialogi)
T„m„ dialogi-ikkuna n„ytt„„ k„yt”ss„ olevan nimen ja oletusnimen
n„ytett„v„lle mittarille.  Ikkunassa on my”s mittareiden varoitus- ja
virhetasot, jos sellaiset mittarille on m„„r„tt„viss„.  Voit muuttaa
kaikkia n„it„ arvoja.  Kenttiin voit kirjoittaa mittarin nimen tai
uuden varoitus- ja virhetason arvon.  Varoitus- ja virhetason
arvoa voit muuttaa my”s liikuttamalla liukureita.
:p.Kaikki arvot on palautettavissa oletusarvoihin painamalla
Oletus-painiketta.

:h1 res=IDM_RESETLOAD.Kalibroi kuormamittari (Valikossa)
:i1.Kalibroi kuormamittari (Valikossa)
T„m„ valikon kohta poistaa aiemman prosessorikuorman vaikutuksen
laskettaessa uutta prosessorikuormaa.

:note.T„m„ valikon kohta ei tarpeettomana ole k„yt”ss„ OS/2 Warp 4.0&colon.ssa tai
uudemmissa eik„ joissakin uusimmissa OS/2 Warp 3.0&colon.n
korjaavissa huolloissa.  N„iss„ ohjelmistotasoissa on k„yt”ss„ uusi
luotettavampi tapa mitata prosessorikuormaa.

:h1 res=IDM_RESETAVERAGE.Laske keskim„„r„inen prosessorikuorma (Valikossa)
:i1.Laske keskim„„r„inen prosessorikuorma (Valikossa)
T„ll„ valikon kohdalla voidaan aloittaa keskim„„r„isen prosessorikuorman
laskenta uudelleen ja aiempi kuormitus j„tet„„n huomioimatta.
:p.
My”s n„pp„inyhdistelm„ Alt+R tekee t„m„n toiminnon.

:h1 res=IDM_RESETDRIVES.Poista levyvirheet (Valikossa)
:i1.Poista levyvirheet (Valikossa)
T„m„ valikon kohta sallii k„ytt„j„n poistaa kaikki levyvirheet joita
saattaa ohjelmaikkunassa olla.  Levyvirheet poistuvat itsest„„n
ajan my”t„, mutta t„ll„ komennolla ne voidaan poistaa heti.

:h1 res=IDM_LAUNCHEMAILER.Launch E-Mail Reader (Valikossa)
:i1.Launch E-Mail Reader (Valikossa)
This option will launch the e-mail reader configured by the user.
See :link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail:elink. for more
information.

:h1 res=IDM_COPY.Kopioi leikep”yd„lle (Valikossa)
:i1.Kopioi leikep”yd„lle (Valikossa)
T„m„ valikon kohta kopioi t„m„n hetkisen ohjelmaikkunan sis„ll”n
leikep”yd„lle kahdessa eri muodossa&colon. tekstin„ ja
bittikarttana.  Tekstiversio voidaan liitt„„ suoraan mihin tahansa
tekstieditoriin.  Bittikarttaversio on graafinen kopio ohjelmaikkunan
sis„ll”st„.
:p.
My”s n„pp„inyhdistelm„ Ctrl+Ins tekee t„m„n toiminnon.

:h1 res=IDM_ABOUT.Tuoteinformaatio (Valikossa)
:i1.Tuoteinformaatio (Valikossa)
T„m„ valikon kohta n„ytt„„ ohjelman Tuoteinformaatio-dialogin.
Katso&colon. :link reftype=hd res=IDD_ABOUT.Tuoteinformaatio (Dialogi):elink..

:h2 res=IDD_ABOUT.Tuoteinformaatio (Dialogi)
T„m„ dialogi-ikkuna n„ytt„„ ohjelman nimen, kuvakkeen ja
copyright-informaation.  Paina ENTER-, v„lily”nti- tai ESCAPE-n„pp„int„
poistuaksesi tai paina OK-painiketta hiirell„.

:h1 res=IDM_EXIT.Sulje (Valikossa)
:i1.Sulje (Valikossa)
T„ll„ valikon kohdalla poistutaan ohjelmasta ja suljetaan ohjelmaikkuna.

:h1 res=IDM_KEYS_HELP.N„pp„inopastus
:i1.N„pp„inopastus
Seuraavat n„pp„imet ovat k„yt”ss„&colon.
:sl compact.
:li.F1 - Opastus
:li.F3 - :link reftype=hd res=IDM_EXIT.Sulje:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.Muokkaa asetuksia:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.Piilota otsikkorivi:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Laske keskim. prosessorikuorma:elink.
:li.Ctrl+Ins - :link reftype=hd res=IDM_COPY.Kopioi leikep”yd„lle:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.Aseta alustustiedoston polkunimi (Dialogi)
:i1.Aseta alustustiedoston polkunimi (Dialogi)
T„m„ dialogi-ikkuna tulee esiin jos ohjelma ei l”yd„
alustustiedostoaan (INI).  Dialogissa kysyt„„n miss„ tiedosto sijaitse
tai mihin se tulisi luoda.
:p.
Ainoastaan olemassa olevan hakemiston nimi hyv„ksyt„„n.
Kun olet kirjoittanut hakemiston nimen, paina ENTER tai OK.
:p.
Jos haluat keskeytt„„ ohjelman k„ynnistymisen, paina ESC.

:h1 res=60000.HRTIMER - Korkean tarkkuuden ajastin -ajuri
K„ytt„m„ll„ korkean tarkkuuden ajuria HRTIMER.SYS voidaan k„ytt„„
IBM PC -koneissa olevaa, mutta normaalisti k„ytt„m„t”nt„ ajastinpiiri„.
Sen tarkkuus on 840 nanosekuntia, eli 65536 kertaa parempi kuin normaali
ajastin (55 millisekuntia).
:p.
Ainoat t„ll„ hetkell„ tiedossa olevat paikat, josta ajuri sijaitsee, ovat&colon.
:hp4.CompuServe:ehp4., 
ja sen
:hp4.Device Driver Development:ehp4.
kirjastossa kohdassa
:hp4.OS/2 Developer&apos.s Forum I (OS2DF1):ehp4., 
nimell„
:hp4.TIMER.ZIP:ehp4.,
ja anonyymill„ FTP&colon.ll„ paikasta
:hp4.ftp.cdrom.com:ehp4. 
ja
:hp4.hobbes.nmsu.edu:ehp4..
Tiedosto on hakemistossa
:hp4./pub/os2/dev16:ehp4.,
nimell„
:hp4.hrtimer.zip:ehp4..
:p.T„m„n ajurin l„hdekoodi oli IBM Personal Systems Developer -lehden
Fall 1991 -numerossa.  Ajuri on ilmainen, mutta sen alkuper„inen
copyright kuuluu IBM&colon.lle.

:h1 res=60001.Mit„ konflikti PULSE-ohjelman kanssa tarkoittaa?
T„m„ ohjelma, kuten PULSE, CPULOAD, CPUMETER ja vastaavat ohjelmat
kaikki yritt„v„t mitata j„rjestelm„n kuormaa yritt„m„ll„ tutkia
kuinka paljon niiden idle-tason matalimman prioriteetin s„ie pystyy tekem„„n
tiettyn„ aikana (likim„„rin&colon. saavat prosessoriaikaa).  Oletettavasti,
jos joku n„ist„ ohjelmista pystyy tekem„„n mit„ tahansa idle-tasolla,
on j„rjestelm„n prosessorikuorma minimaalinen.  Jos hukka-aikaa (idle time)
ei ole, on j„rjestelm„n prosessorikuorma 100 %.  Mit„ enemm„n t„m„
ohjelma pystyy tekem„„n idle-tasolla (itse asiassa&colon. ohjelman idle-tason s„ie),
sit„ pienempi prosessorikuorman katsotaan olevan.
:p.
Jos k„ynniss„ on useampia kuin yksi t„m„n tyyppinen ohjelma, ne alkavat
kilpailla hukka-ajasta.  Jos k„yt”ss„ on kaksi samaa ohjelmaa, niiden
molempien tulisi n„ytt„„ noin 50 % prosessorikuormaa.  Jos k„yt”ss„
on kaksi erilaista ohjelmaa, kuten t„m„ ja PULSE, ne molemmat h„iriintyv„t.

:h1 res=60002.Miten HRTIMER.SYS-ajuri auttaa?
Kun HRTIMER.SYS on k„yt”ss„, lasketaan my”s k„ytt”j„rjestelm„n aiheuttama
kuormitus mukaan prosessorikuormaan ja tulos on todenmukaisempi.
:p.
Prosessorikuorma m„„ritell„„n vertaamalla kesken„„n sit„ kuinka paljon
idle-prioriteetin s„ie saa prosessoriaikaa siihen kuinka paljon
ohjelman monitoroiva aikakriittinen s„ie saa prosessoriaikaa.  Jos
ohjelmalla ei ole k„ytett„viss„ lis„apua, lasketaan normaali
k„ytt”j„rjestelm„n ja sen k„ynniss„ olevien ohjelmien kuormitus
nollakuormaksi (0 %).  Jos k„yt”ss„ on HRTIMER.SYS tekee ohjelma
k„ynnistyksen yhteydess„ kalibroinnin jossa m„„ritell„„n maksimaalinen
prosessorikuorma korkeimmalla prioriteetilla.  T„t„ arvoa k„ytet„„n
my”hemmin vertailussa 100 % tasona.

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
