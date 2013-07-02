#include "memsize.h"
:userdoc.
:title.Aide de &apos.Ressources systäme&apos.
:body.

:h1 res=ID_MAIN.Introduction
:i1.Introduction
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
Ce programme affiche plusieurs ÇlÇments relatifs aux ressources du
systäme, et met Ö jour ces informations une fois par seconde s&apos.il
dispose de suffisamment de ressources processeur pour cela. Les ÇlÇments
affichÇs sontˇ&colon.
:p.
:hp2.Date/heure:ehp2. - L&apos.heure et la date, dans le format dÇfini par
les informations de pays de votre systäme (d&apos.apräs la ligne &apos.COUNTRY=&apos.
de votre fichier CONFIG.SYS).
:p.
:hp2.Temps ÇcoulÇ:ehp2. - Le temps ÇcoulÇ depuis que le systäme a ÇtÇ
lancÇ pour la derniäre fois.
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.MÇmoire physique libre:ehp2. - La quantitÇ de mÇmoire systäme
libre, d&apos.apräs la fonction :hp1.DosMemAvail:ehp1..
:note.Cette fonction fait partie de l&apos.ancien systäme 16 bits, et ne compte que
la mÇmoire physique sous les 16Mo, et a aussi ÇtÇ modifiÇe juste avant
la sortie d&apos.OS/2 2.0 pour ne jamais indiquer moins de 512ko.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.MÇmoire virtuelle libre:ehp2. - La quantitÇ de mÇmoire virtuelle
disponible, d&apos.apräs la fonction :hp1.DosQuerySysInfo:ehp1.. Il s&apos.agit de
la quantitÇ de mÇmoire physique libre, plus l&apos.espace libre dans le
fichier d&apos.Çchange, plus l&apos.espace disponible pour l&apos.augmentation du
fichier d&apos.Çchange, moins l&apos.espace libre minimum rÇservÇ sur le disque
d&apos.Çchange.
:p.
:hp2.Taille fichier d&apos.Çchange:ehp2. - La taille actuelle du fichier
d&apos.Çchange du systäme, :hp4.SWAPPER.DAT:ehp4..
Pour localiser le fichier, l&apos.entrÇe :hp4.SWAPPATH:ehp4. est recherchÇe
dans le fichier :hp4.CONFIG.SYS:ehp4.. Cette entrÇe fournit le nom du
rÇpertoire oó est stockÇ le fichier d&apos.Çchange et indique l&apos.espace
minimum qui doit àtre laissÇ disponible sur le disque du fichier
d&apos.Çchange.
:p.
:hp2.Espace d&apos.Çchange libre:ehp2. - La quantitÇ d&apos.espace libre sur
le disque oó rÇside le fichier d&apos.Çchange, moins l&apos.espace libre minimum.
Ceci reprÇsente l&apos.augmentation potentielle maximale du fichier
d&apos.Çchange.
:p.
:hp2.Espace d&apos.Çchange inutilisÇ:ehp2. - L&apos.espace libre dans le fichier
d&apos.Çchange actuellement allouÇ. Quand cette quantitÇ descend Ö zÇro, le
fichier d&apos.Çchange doit àtre agrandi, normalement par incrÇment de un mÇga-
octet.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Taille fichier d&apos.impression:ehp2. - L&apos.espace disque
utilisÇ par les fichiers d&apos.attente d&apos.impression.
:p.
:hp2.Charge processeur (%):ehp2. - Le pourcentage approximatif de la
puissance du processeur utilisÇe. Il s&apos.agit d&apos.une moyenne calculÇe
pendant la seconde prÇcÇdente.
:note.Under versions of OS/2 before release 4.0, cette fonction et l&apos.application :hp4.ActivitÇ du processeur:ehp4.
ne cohabitent pas träs bien.
:link reftype=hd res=60001.Pourquoi ?:elink.
:note.Under versions of OS/2 before release 4.0, si le pilote :link reftype=hd res=60000.HRTIMER.SYS:elink.
est installÇ, ce nombre sera plus prÇcis.
:link reftype=hd res=60002.Pourquoi ?:elink.
:p.
:hp2.Average Load (%):ehp2. - The approximate percentage of the CPU&apos.s available
power that has been used in the recent past.  The number of seconds to be 
averaged may be configured by the user 
(see the :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Configuration:elink. dialog).
:p.
:hp2.Batterie:ehp2. - Le pourcentage de charge de la batterie, ou son
Çtat (Charge, Secteur) est affichÇ.
:p.
:hp2.Nombre de tÉches actives:ehp2. - Le nombre de lignes dans la
liste des tÉches du systäme, qui est la liste affichÇe quand vous
pressez les touches :hp4.Ctrl:ehp4. et :hp4.êchap:ehp4..
:note.Toutes les entrÇes de la liste des tÉches du systäme ne sont pas
affichÇes dans la :hp4.Liste de fenàtres:ehp4.. Certaines sont
configurÇes pour ne pas àtre affichÇes.
:p.
:hp2.Nombre de processus:ehp2. - Le nombre de processus en cours d&apos.exÇcution sur
le systäme.
:p.
:hp2.Nombre de threads:ehp2. - Le nombre de threads en cours d&apos.exÇcution sur
le systäme. Il y a au moins un thread par processus.
:p.
:hp2.Espace disque libre total:ehp2. - La somme de l&apos.espace libre sur
tous les disques fixes locaux.
:p.
:hp2.Libre sur X:ehp2. - L&apos.espace libre sur le disque X.
:p.
Comme vous avez dÇjÖ pu le constater, l&apos.aide en ligne est disponible, et
les commandes du programme peuvent àtre atteintes via le menu systäme de
la fenàtre. Les commandes suivantes sont disponiblesˇ&colon.
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.RÇinitialiser l&apos.environnement:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.Cacher la barre de titre:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.Configurer...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.RÇinitialiser charge processeur:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.RÇinitialiser disques:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Launch E-Mail Reader:elink.
:li.:link reftype=hd res=IDM_COPY.Copier:elink.
:li.:link reftype=hd res=IDM_ABOUT.A propos:elink.
:li.:link reftype=hd res=IDM_EXIT.Fermer:elink.
:esl.:p.
En plus des fonctionnalitÇs dÇjÖ dÇcrites, le programme accepte les
commandes provenant des gestionnaires de polices et de couleurs d&apos.OS/2,
et se comporte en serveur DDE pour tous les ÇlÇments affichÇs. Le nom
DDE de l&apos.application est ÆMEMSIZEØ, et les noms des ÇlÇments sont les
màmes que les noms affichÇs par dÇfaut (par exemple, ÆMÇmoire physique
libreØ).

:h1 res=IDM_RESET_DEFAULTS.RÇinitialiser l&apos.environnement (Option de menu)
:i1.RÇinitialiser l&apos.environnement (Option de menu)
Cette option de menu entraåne la rÇinitialisation des attributs de
police et de couleur du programme Ö leurs valeurs par dÇfaut.

:h1 res=IDM_HIDE_CONTROLS.Cacher la barre de titre (Option de menu)
:i1.Cacher la barre de titre (Option de menu)
La sÇlection de cette option de menu entraåne la disparition de la barre
de titre de la fenàtre, et des contrìles qui y sont associÇs (le menu
systäme, la plaquette de rÇduction). Cette option peut àtre inversÇe en
cliquant deux fois l&apos.un des boutons de la souris. De plus, comme il
m&apos.a semblÇ träs utile de pouvoir dÇplacer la fenàtre quand la barre
de titre est absente, la fenàtre est configurÇe de telle sorte que vous
pouvez la faire glisser en pressant l&apos.un des boutons de la souris.
:p.
:note.La touche de raccourci pour cette fonction est :hp4.Alt+H:ehp4..

:h1 res=IDM_CONFIGURE.Configurer (Option de menu)
:i1.Configurer (Option de menu)
La sÇlection de cette option de menu entraåne l&apos.apparition de la boåte
de dialogue de configuration du programme.
Pour plus d&apos.informations, voir
:link reftype=hd res=IDD_CONFIGURE.Configurer (Dialogue):elink..
:note.Le raccourci clavier pour cette commande est :hp4.Alt+C:ehp4..

:h2 res=IDD_CONFIGURE.Configuration (Boåte de dialogue)
Ce bloc-notes vous permet de modifier les options de configuration
du programme. Les options de configuration sont groupÇes par
catÇgorie dans les pages suivantesˇ&colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.êlÇments:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.Options (Page 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Options (Page 2):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.Couleurs:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.Point d&apos.ancrage:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Date Format:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (Page 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (Page 2):elink.
:esl.:p.
Toutes les modifications prennent immÇdiatement effet.

:h3 res=IDD_CONFIG_PAGE_ITEMS.Items (Page de configuration)
Cette page affiche la liste des ÇlÇments affichables.
Les ÇlÇments que vous sÇlectionnerez dans la liste des :hp4.DonnÇes
affichÇes:ehp4. seront surveillÇs et affichÇs par le programme. Les
items en surbrillance sont surveillÇs. Pour mettre un item en
surbrillance, sÇlectez-le Ö l&apos.aide de la souris. Pour supprimer la
surbrillance, sÇlectez Ö nouveau l&apos.item.
To change an item&apos.s default label, or to adjust its warning and/or error 
thresholds, either double-click mouse button one on the item list entry, or single-click
mouse button two on the entry.  In either case, the 
:link reftype=hd res=IDD_LABEL.Item Attribute:elink. dialog will be displayed.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.Options (Page 1) (Page de configuration)
Cette page affiche un groupe d&apos.options qui peuvent àtre activÇes ou
dÇsactivÇes.
:p.
:hp2.Cacher la barre de titre:ehp2.
Quand la fenàtre du programme n&apos.est pas rÇduite, elle peut apparaåtre
avec ou sans la barre de titre, le menu systäme et les boutons de
rÇduction et d&apos.agrandissement. La fenàtre peut àtre dÇplacÇe en la
faisant glisser.
:p.
:hp2.Toujours visible:ehp2.
Quand cette case est cochÇe, &apos.Ressources systäme&apos. reste visible en
permanence.
:p.
:hp2.Animer:ehp2.
Cette case active l&apos.animation lors de l&apos.ouverture et de la fermeture de
la fenàtre. Quand cette case est cochÇe, la fenàtre du programme s&apos.ouvre
et se ferme de la màme faáon que les dossiersˇ&colon. avec un accompagnement
sonore et une animation. De màme que pour les dossiers, cette
fonctionnalitÇ doit àtre activÇe dans la configuration du systäme.
:p.
:hp2.Dialog Border:ehp2.
This button determines whether the main program window will have 
a simple line border, or a more elaborate dialog border.  The line
border requires less space on the screen, but the dialog border can
look better, especially on OS/2 Warp 4.0 or later, which gives this
border style a three-dimensional appearance.
:p.
:hp2.Format tableau:ehp2.
Quand cette case est cochÇe, la fenàtre est disposÇe comme un tableau,
les ÇlÇments Çtant arrangÇs en lignes et colones. L&apos.affichage sera
optimisÇ de faáon Ö utiliser le moins d&apos.espace vertical possible. Quand
cette case n&apos.est pas cochÇe, la fenàtre affiche tous les ÇlÇments en une
seule colone.
:p.
:hp2.Signal horaire:ehp2.
Cette case active la fonction de signal horaire. Le programme Çmettra
deux bips Ö chaque changement d&apos.heure.
:p.
:hp2.Show Disk Drives:ehp2.
When this button is checked, disk drive free space is monitored, subject
to the restrictions placed on this monitoring by other options.  
This control mainly exists as a way to disable disk monitoring.
:p.
:hp2.Afficher unitÇs distantes:ehp2.
Quand cette case est cochÇe, les unitÇs distantes (rÇseau) qui sont
actuellement connectÇes au systäme sont surveillÇes comme des unitÇs
locales non amovibles, mais elles ne sont pas inclues dans le calcul de
l&apos.Espace disque libre total.
:p.
:hp2.Afficher systäme de fichiers:ehp2.
Lorsque cette case est cochÇe, chaque ligne affichant l&apos.espace
disponible sur un disque indique le type de systäme de fichiers utilisÇ
par le disque. L&apos.utilisation de cette option entraåne une fenàtre de
surveillance plus large.
:p.
:hp2.Afficher noms d&apos.unitÇs:ehp2.
Lorsque cette case est cochÇe, chaque ligne affichant l&apos.espace
disponible sur un disque indique le label de volume du disque.
L&apos.utilisation de cette option entraåne une fenàtre de surveillance plus
large.
:p.
:hp2.Afficher les secondes:ehp2.
Quand cette case est cochÇe, les secondes sont affichÇes pour l&apos.heure
et le temps ÇcoulÇ.
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

:h3 res=IDD_CONFIG_PAGE_OPTIONS2.Options (Page 2) (Page de configuration)
Cette page propose d&apos.autres options de configuration du programme.
:p.
:hp2.Afficher ÆKØilo-octets:ehp2.
Ces boutons-radio vous permettent de dÇfinir si les grandes quantitÇs
sont affichÇes en kilo-octets jamais, toujours ou uniquement si leur
valeur excäde 512ko.
Par dÇfaut, les tailles de plus de 500ko sont affichÇes en kilo-octets.
.br
Si la case &apos.K=1024&apos. est cochÇe, les nombres affichÇs sont de vrais
kilo-octets. Dans le cas contraire, le programme affiche des milliers
d&apos.octets (1k=1000).
.br
Si la case &apos.M&apos.Çgaoctets est cochÇe, les valeurs supÇrieures Ö 8 mÇgaoctets
sont affichÇes en mÇgaoctets.
.br
The &apos.G&apos.igabytes button, if checked, will cause the numbers to be
displayed in gigabytes once their values exceed four gigabytes.
:p.
:hp2.PrioritÇ:ehp2.
Ce champ vous permet de dÇfinir la prioritÇ de la routine de
surveillance du programme, dans la classe de prioritÇ Ætime-criticalØ.
Vous pouvez faire varier la prioritÇ de 0 Ö 31. Une prioritÇ de 31 est
la plus haute prioritÇ possible dans le systäme, exception faite des
interruptions matÇrielles. La prioritÇ 0 est la prioritÇ la plus basse
de la classe Ætime-criticalØ. Pour obtenir la plus grande prÇcision dans
la mesure de la charge du processeur, vous devriez utiliser la valeur
maximale. Toutefois, certains utilisateurs se sont plaints du fait que
leur programme de communication manquait occasionnellement des donnÇes
quand le programme s&apos.exÇcute. C&apos.est la raison pour laquelle la prioritÇ
est maintenant modifiable.
:p.
:hp2.PÇriode de mise Ö jour:ehp2.
Ce champ vous permet de dÇfinir le temps s&apos.Çcoulant entre deux mises Ö
jour de la fenàtre de surveillance. Cette pÇriode peut varier de une Ö
trente secondes, par palier d&apos.un dixiäme de seconde. Le nombre affichÇ
exprime des dixiämes de seconde.
:p.
:hp2.Averaging Interval:ehp2.
This control allows you to set the interval over which averages are
computed.  It can be varied from 10 seconds to 3600 seconds (one hour).

:h3 res=IDD_CONFIG_PAGE_COLORS.Couleurs (Configuration Page)
Cette zone affiche les trois couleurs associÇes avec les conditions
Normal, Alerte et Erreur pour les ÇlÇments qui en disposent. Vous pouvez
dÇfinir les couleurs en faisant glisser une couleur depuis la palette du
systäme.

:h3 res=IDD_CONFIG_PAGE_ANCHOR.Point d&apos.ancrage (Configuration Page)
Ce contrìle vous permet de dÇfinir un coin de la fenàtre dont la
position reste fixe quand la taille de la fenàtre est modifiÇe. La
taille de la fenàtre peut changer quand la liste des items surveillÇs
est modifiÇe, ou quand la police est modifiÇe Ö l&apos.aide de la palette de
polices.

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

:h3 res=IDD_LABEL.Item Attributes (Boåte de dialogue)
Cette boåte de dialogue affiche le libellÇ actuel et par dÇfaut d&apos.un
ÇlÇment, ainsi que les seuils d&apos.alerte et d&apos.erreur, si cette fonction
est disponible pour l&apos.ÇlÇment considÇrÇ. Pour modifier le libellÇ,
saisissez le nouveau libellÇ Ö la place de l&apos.ancien. Pour modifier les
seuils d&apos.alerte et d&apos.erreur, saisissez une nouvelle valeur dans le
champ de saisie ou utilisez la rÇglette associÇe.
:p.Pour restaurer les valeurs par dÇfaut, sÇlectez la plaquette
:hp2.DÇfaut:ehp2..

:h1 res=IDM_RESETLOAD.RÇinitialiser charge processeur (Option de menu)
:i1.RÇinitialiser charge processeur (Option de menu)
Cette option de menu permet de rÇinitialiser la fonction de mesure de la
charge du processeur.

:note.This menu option is not available when on Warp 4.0 or better,
nor on certain higher fixpack levels of Warp 3.0, as the newer load
measuring technique makes this command irrelevant.

:h1 res=IDM_RESETAVERAGE.Reset Average Load (Option de menu)
:i1.Reset Average Load (Option de menu)
This menu option allows the user to reset the Average Load meter,
causing it to forget previous CPU activity in its calculations.
:p.
The short-cut key for this command is Alt+R.

:h1 res=IDM_RESETDRIVES.RÇinitialiser erreurs disque (Option de menu)
:i1.RÇinitialiser erreurs disque (Option de menu)
Cette option vous permet de rÇinitialiser une Çventuelle erreur disque.
Ceci est fait automatiquement pÇriodiquement, mais cette commande vous
permet de le faire sans attendre.

:h1 res=IDM_LAUNCHEMAILER.Launch E-Mail Reader (Option de menu)
:i1.Launch E-Mail Reader (Option de menu)
This option will launch the e-mail reader configured by the user.
See :link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail:elink. for more
information.

:h1 res=IDM_COPY.Copier (Option de menu)
:i1.Copier (Option de menu)
Cette option de menu copie le contenu de la fenàtre dans le presse-
papiers du systäme dans deux formats &colon. texte et bitmap. La version texte
contient tous les ÇlÇments affichÇs dans un format permettant son
insertion dans un Çditeur de textes. La version bitmap est une copie de
la fenàtre, et peut àtre utilisÇe dans les programmes manipulant des
graphiques.
:p.
La touche de raccourci pour cette commande est Ctrl+Inser.

:h1 res=IDM_ABOUT.A propos (Option de menu)
:i1.A propos (Option de menu)
Cette option fait apparaåtre la fenàtre d&apos.informations sur le
programme.
Pour plus d&apos.informations, voir
:link reftype=hd res=IDD_ABOUT.A propos (Dialogue):elink..

:h2 res=IDD_ABOUT.A propos (Dialogue)
Cette boåte de dialogue affiche le nom du programme, l&apos.icìne et les
informations de Copyright. Pour quitter le dialogue, pressez la touche
:hp4.EntrÇe:ehp4., la barre d&apos.espacement ou la touche :hp4.êchap:ehp4..

:h1 res=IDM_EXIT.Fermer (Option de menu)
:i1.Fermer (Option de menu)
Le choix de cette option de menu entraine l&apos.arràt du programme.

:h1 res=IDM_KEYS_HELP.Aide sur les touches
:i1.Aide sur les touches
Les touches de fonction suivantes sont disponibles dans ce
programmeˇ&colon.
:sl compact.
:li.F1 - Aide
:li.F3 - :link reftype=hd res=IDM_EXIT.Sortie:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.Configurer:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.Cacher la barre de titre:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.Ctrl+Inser - :link reftype=hd res=IDM_COPY.Copier:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.RÇpertoire configuration
:i1.RÇpertoire configuration
Cette boåte de dialogue est affichÇe quand le programme ne trouve pas
son fichier de configuration (.INI), et demande oó ce fichier se trouve
ou l&apos.endroit oó il doit àtre crÇÇ.
:p.
Seul un nom de rÇpertoire existant sera acceptÇ. Une fois le nom entrÇ,
pressez la touche :hp4.EntrÇe:ehp4. ou sÇlectez la plaquette
:hp4.Ok:ehp4. pour que le programme continue son exÇcution.
:p.
Si vous voulez interrompre l&apos.initialisation du programme, pressez la
touche :hp4.êchap:ehp4. ou sÇlectez la plaquette :hp4.Annuler:ehp4..

:h1 res=60000.HRTIMER - Pilote de chronomätre haute rÇsolution.
Le pilote de chronomätre haute rÇsolution, HRTIMER.SYS, permet aux
programmes d&apos.accÇder Ö un composant normalement inutilisÇ sur les IBM
PCs. Il possäde une rÇsolution de 840 nanosecondes, 64k fois mieux que
la rÇsolution normale de l&apos.horloge, qui est de 55 millisecondes.
:p.Ce programme peut àtre trouvÇ, entre autres, aux endroits suivantsˇ: sur
:hp4.CompuServe:ehp4.,
dans la librairie
:hp4.Device Driver Development:ehp4.
du forum
:hp4.OS/2 Developer&apos.s Forum I (OS2DF1):ehp4.,
sous le nom
:hp4.TIMER.ZIP:ehp4. ;
sur Internet, par ftp anonyme sur les sites
:hp4.ftp.cdrom.com:ehp4.
et
:hp4.hobbes.nmsu.edu:ehp4.,
dans le rÇpertoire
:hp4./pub/os2/dev16:ehp4.,
sous le nom
:hp4.hrtimer.zip:ehp4..
:p.Le code source de ce pilote de pÇriphÇrique provient du numÇro d&apos.automne
du magazine IBM Personal Systems Developer. Il est gratuit, mais le
copyright original appartient Ö IBM.

:h1 res=60001.D&apos.oó vient le conflit avec ÆActivitÇ du processeurØ ?
Ce programme, ainsi que ÆActivitÇ du processeurØ, CPULOAD, CPUMETER et
les programmes similaires, essaie de mesurer l&apos.activitÇ du processeur en
Çvaluant le nombre de cycles processeurs qui lui sont allouÇs Ö la
prioritÇ la plus basse. Si un de ces programme arrive Ö s&apos.exÇcuter Ö une
si basse prioritÇ, il utilise le temps d&apos.inactivitÇ. S&apos.il n&apos.y a pas de
temps d&apos.inactivitÇ, l&apos.ordinateur est occupÇ Ö 100%. Plus le programme
s&apos.exÇcute Ö la prioritÇ systäme ÆinactifØ, moins le processeur est
chargÇ.
:p.
ExÇcuter deux programmes de ce type pose un probläme : ils essaient tous
les deux d&apos.utiliser ce temps d&apos.inactivitÇ. Si vous exÇcutez deux copies
du màme programme, et rien d&apos.autre, ils devraient tous les deux indiquer
une charge processeur de 50%. Si vous exÇcutez deux programmes
diffÇrents, ils seront tous les deux affectÇs, en principe Ö des degrÇs
diffÇrents.

:h1 res=60002.A quoi sert HRTIMER.SYS ?
Quand HRTIMER.SYS est installÇ, la surcharge que reprÇsente le systäme
est prise en compte dans la charge CPU.
:p.
La charge du processeur est dÇterminÇe en comparant la quantitÇ de
travail qui peut àtre effectuÇe Ö la prioritÇ ÆinactifØ avec la quantitÇ
maximale qu&apos.il a ÇtÇ possible d&apos.effectuer jusqu&apos.ici. Sans information
complÇmentaire, l&apos.activitÇ normale du systäme apparaåt comme une charge
processeur de zÇro.
HRTIMER.SYS, quand il s&apos.installe, effectue un calibrage pour dÇterminer
la quantitÇ de travail rÇalisable Ö la plus haute prioritÇ. Cette mesure
est utilisÇe par le programme.

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
