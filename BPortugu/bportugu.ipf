#include "memsize.h"
:userdoc.
:title.&apos.Recursos do Sistema OS/2&apos. - Ajuda
:body.

:h1 res=ID_MAIN.Introduá∆o
:i1.Introduá∆o
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
Este programa mostra v†rios itens relacionados aos recursos do sistema 
e os atualiza uma vez por segundo, desde que haja tempo de CPU para isto. 
Os itens mostrados s∆o&colon.
:p.
:hp2.Data/Hora:ehp2. - A data e hora corrente no formato de cada pa°s, 
dependendo de como esta configurada a linha &apos.COUNTRY=&apos. 
do arquivo CONFIG.SYS.
:p.
:hp2.Tempo decorrido:ehp2. - O tempo decorrido desde a £ltima 
reinicializaá∆o.
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.Mem¢ria f°sica livre:ehp2. - A quantidade de mem¢ria do sistema
dispon°vel, de acordo com o valor retornado pela funá∆o :hp1.DosMemAvail:ehp1.. 
:Note.Esta funá∆o Ç parte do antigo sistema de 16 bits, e somente
informa a mem¢ria f°sica abaixo de 16Mb, e foi modificada pouco antes
do lanáamento do OS/2 2.0 para nunca retornar menos que 512K.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Mem¢ria virtual livre:ehp2. - A quantidade de mem¢ria virtual livre
de acordo com a funá∆o :hp1.DosQuerySysInfo:ehp1.. Esta Ç a quantidade de
mem¢ria f°sica livre mais a quantidade de espaáo livre no arquivo de swap
mais a quantidade de espaáo que o swap pode crescer e menos a quantidade
e espaáo livre reservado no disco de swap.
:p.
:hp2.Tamanho do Swap:ehp2. - O tamanho atual do arquivo de swap 
SWAPPER.DAT. Para localizar este arquivo Ç procurada a linha que comeáa 
com SWAPPATH no arquivo CONFIG.SYS. Esta linha informa o nome completo do 
diret¢rio onde esta o arquivo de swap, o m°nimo espaáo livre que deve ser 
deixado no disco de swap e o tamanho inicial do arquivo de swap.
:p.
:hp2.Espaáo dispon°vel para swap:ehp2. - A quantidade de espaáo livre no 
disco l¢gico onde reside o swap menos o espaáo reservado. Isto mostra o 
quanto o arquivo de swap pode crescer.
:p.
:hp2.Espaáo de swap n∆o utilizado:ehp2. - A quantidade de espaáo livre 
dentro do arquivo de swap. Quando se torna igual a zero, o arquivo de 
swap deve crescer, normalmente um megabyte por vez.
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.Tamanho do spool:ehp2. - A quantidade total de disco comsumida 
pelos arquivos enviados ao spooler de impress∆o.
:p.
:hp2.Carga da CPU:ehp2. - O percentual aproximado da capacidade da CPU 
dispon°vel em uso no momento. ê uma mÇdia do intervalo anterior.
:Note.Under versions of OS/2 before release 4.0, esta funá∆o e o PULSE n∆o apresentam resultados equivalentes.
:link reftype=hd res=60001.Por que?:elink.
:Note.Under versions of OS/2 before release 4.0, se o driver :link reftype=hd res=60000.HRTIMER.SYS:elink. estiver instalado, 
este n£mero ser† mais correto.
:link reftype=hd res=60002.Por que?:elink.
:p.
:hp2.Average Load (%):ehp2. - The approximate percentage of the CPU&apos.s available
power that has been used in the recent past.  The number of seconds to be 
averaged may be configured by the user 
(see the :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Configuration:elink. dialog).
:p.
:hp2.Bateria:ehp2. - ê mostrada a carga restante da bateria ou o status 
(Carregando, A/C).
:p.
:hp2.N£mero de tarefas ativas:ehp2. - O n£mero de entradas na lista de 
tarefas do sistema, que Ç a lista mostrada quando se pressiona CTRL+ESC.
:note.Nem todas as tarefas s∆o mostradas na lista de tarefas. Algumas s∆o 
marcadas como invis°veis.
:p.
:hp2.N£mero de processos:ehp2. - O n£mero de processos atualmente carregados 
pelo sistema.
:p.
:hp2.N£mero de threads:ehp2. - O n£mero de threads de processamento no 
sistema. H† pelo menos uma thread para cada processo.
:p.
:hp2.Espaáo livre total em disco:ehp2. - A quantidade de espaáo livre em 
todos os discos locais n∆o remov°veis.
:p.
:hp2.Drive X&colon. espaáo livre:ehp2. - A quantidade de espaáo livre no 
drive X&colon..
:p.
O sistema de ajuda est† ativo, como j† se deve ter visto, e os comandos 
existentes podem ser acessados pela janela do menu do sistema. Os seguintes 
comandos est∆o dispon°veis&colon.
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.Inicializar pelo padr∆o:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.Esconder t°tulo:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.Configurar...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.Reinicializar medidor de carga:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.Reinicializar drives:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Launch E-Mail Reader:elink.
:li.:link reftype=hd res=IDM_COPY.Copiar:elink.
:li.:link reftype=hd res=IDM_ABOUT.Informaá‰es do produto:elink.
:li.:link reftype=hd res=IDM_EXIT.Encerrar:elink.
:esl.
:p.
AlÇm das funá‰es j† descritas, este programa aceita comandos das Paletas 
de Fontes e de Cores do OS/2, e Ç tambÇm um servidor DDE para todos os 
dados apresentados. O nome da aplicaá∆o DDE Ç MEMSIZE e os nomes dos itens 
s∆o os iguais aos que aparecem (como Tempo decorrido).

:h1 res=IDM_RESET_DEFAULTS.Inicializar pelo padr∆o (Opá∆o do menu)
:i1.Inicializar pelo padr∆o (Opá∆o do menu)
Selecionando esta opá∆o do menu, os atributos de fonte e cor ser∆o 
retornados aos valores padr∆o.

:h1 res=IDM_HIDE_CONTROLS.Esconder t°tulo (Opá∆o de menu)
:i1.Esconder t°tulo (Opá∆o de menu)
Esta opá∆o de menu, quando selecionada, ir† fazer com que os controles da 
moldura do programa (menu do sistema, barra de t°tulo e bot∆o de minimizar) 
n∆o apareáam. Esta opá∆o pode ser alternada com um duplo clique com qualquer 
dos bot‰es do mouse. TambÇm, uma vez que percebi que Ç muito £til poder se 
mover a janela enquanto os controles est∆o escondidos, a janela foi 
configurada para poder ser movida com qualquer dos bot‰es do mouse.
.br
:Note.A tecla de atalho para esta funá∆o Ç ALT+H.

:h1 res=IDM_CONFIGURE.Configurar (Opá∆o de menu)
:i1.Configurar (Opá∆o de menu)
Esta opá∆o do menu, quando selecionada, faz com que a janela de 
configuraá∆o do programa seja mostrada. Para maiores informaá‰es veja 
:link reftype=hd res=IDD_CONFIGURE.Configurar (Caixa de di†logo):elink..
:Note.A tecla de atalho para esta funá∆o Ç Alt+C.

:h2 res=IDD_CONFIGURE.Configurar (Di†logo)
Este bloco de notas de di†logo permite modificar todos os aspectos 
configur†veis das operaá‰es deste programa. As opá‰es de configuraá∆o 
est∆o agrupadas por categorias nas seguintes p†ginas&colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.Itens:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.Opá‰es (P†gina 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Opá‰es (P†gina 2):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.Cores:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.Canto fixo:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Date Format:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (P†gina 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (P†gina 2):elink.
:esl.:p.
Todas as mudanáas tem efeito imediato.

:h3 res=IDD_CONFIG_PAGE_ITEMS.Itens (P†gina de Configuraá∆o)
Esta p†gina mostra uma lista dos itens que podem ser apresentados.
ê poss°vel selecionar v†rios ou todos os elementos que devem ser 
mostrados. Simplesmente dà um clique para marc†-lo, dà outro clique para 
desmarcar. Os itens marcados ser∆o mostrados. 
To change an item&apos.s default label, or to adjust its warning and/or error 
thresholds, either double-click mouse button one on the item list entry, or single-click
mouse button two on the entry.  In either case, the 
:link reftype=hd res=IDD_LABEL.Item Attribute:elink. dialog will be displayed.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.Opá‰es (P†gina 1) (P†gina de Configuraá∆o)
Esta p†gina mostra um n£mero de opá‰es do programa que podem ser 
ativadas ou desativadas.
:p.
:hp2.Esconder T°tulo:ehp2.
Quando o programa n∆o esta minimizado, este pode ser mostrado com ou sem 
a barra de t°tulo, menu do sistema e bot‰es de maximizar/minimizar. A 
janela poder† ser movida pela sua †rea central, mesmo que a barra de 
t°tulo esteja escondida.
:p.
:hp2.Permanecer Vis°vel:ehp2.
Este bot∆o ativa a funá∆o de permanecer vis°vel, fazendo com que a janela 
esteja sempre a frente das demais.
:p.
:hp2.Animaá∆o:ehp2.
Este bot∆o ativa a caracter°stica de animaá∆o ao abrir &amp. fechar. 
Quando estiver ativa, a janela do programa ir† abrir e fechar da mesma 
maneira que os objetos pasta fazem&colon. com som e movimento. Como as 
pastas, a animaá∆o s¢ ir† ocorrer se estiverem habilitados pelo sistema.
:p.
:hp2.Dialog Border:ehp2.
This button determines whether the main program window will have 
a simple line border, or a more elaborate dialog border.  The line
border requires less space on the screen, but the dialog border can
look better, especially on OS/2 Warp 4.0 or later, which gives this
border style a three-dimensional appearance.
:p.
:hp2.Formato Tabela:ehp2.
Este bot∆o, quando estiver marcado, foráar† a janela a se arrumar em 
formato de tabela, mostrados os itens em linhas e colunas. Ser† mostrada 
de forma a ocupar o menor espaáo vertical poss°vel. Quando n∆o estiver 
marcado os itens ser∆o mostrados em uma £nica coluna.
:p.
:hp2.Toque nas Horas Cheias:ehp2.
Este bot∆o ativa a caracter°stica de tocar na hora cheia. Quando ativo o
programa ir† dar dois bips quando chegar uma nova hora.
:p.
:hp2.Show Disk Drives:ehp2.
When this button is checked, disk drive free space is monitored, subject
to the restrictions placed on this monitoring by other options.  
This control mainly exists as a way to disable disk monitoring.
:p.
:hp2.Mostrar Drives Remotos:ehp2.
Quando este bot∆o esta marcado os drives remotos (de rede) que est∆o 
conectados ao sistema s∆o monitorados da mesma forma que os drives 
locais n∆o remov°veis, exceto que eles n∆o ser∆o incluidos no item 
Espaáo livre total em disco.
:p.
:hp2.Mostrar Sistema de Arquivos:ehp2.
Este bot∆o foráa as entradas de Espaáo livre em disco a mostrarem o 
sistema de arquivos usado em cada drive. O uso desta opá∆o resulta em 
uma janela mais larga.
:p.
:hp2.Mostrar Nome do Drive:ehp2.
Este bot∆o foráa as entradas de Espaáo livre em disco a mostrarem o 
nome de cada drive. O uso desta opá∆o resulta em uma janela mais larga.
:p.
:hp2.Mostrar Segundos:ehp2.
Este bot∆o faz com que sejam mostrados os segundos no rel¢gio e no 
tempo decorrido.
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

:h3 res=IDD_CONFIG_PAGE_OPTIONS2.Opá‰es (P†gina 2) (P†gina de Configuraá∆o)
Esta p†gina mostra opá‰es adicionais do programa que s∆o mais complexas 
do que simples liga/desliga.
:p.
:hp2.Mostrar &apos.K&apos.ilobytes:ehp2.
Este grupo de bot‰es permite que se selecione como grandes n£meros devem 
ser mostrados. Se devem ser mostrados em Kbytes sempre, nunca ou somente 
se os valores ultrapassarem 512K. O padr∆o Ç para mostrar qualquer valor 
maior que 512K desta maneira.
.br
O bot∆o &apos.K=1024&apos., se marcado, faz com que os n£meros sej∆o mostrados em
reais K (1024). Se n∆o estiver marcado, os n£meros ser∆o mostrados em 
decimal K (1000).
.br
O bot∆o de &apos.M&apos.egabyte, se marcado, foráa que os n£meros sej∆o mostrados 
em megabytes quando seus valores ultrapassarem oito megabytes.
.br
The &apos.G&apos.igabytes button, if checked, will cause the numbers to be
displayed in gigabytes once their values exceed four gigabytes.
:p.
:hp2.Prioridade do monitor:ehp2.
Este controle permite indicar a prioridade absoluta da thread de 
monitoraá∆o do programa, dentro da classe de prioridade &odq.time-critical&cdq..
Pode-se variar a prioridade de 0 Ö 31. Prioridade 31 Ç a mais alta 
prioridade poss°vel no sistema, pr¢ximo de interrupá‰es f°sicas. 
Prioridade 0 Ç a mais baixa dentro da classe &odq.time-critical&cdq.. 
Normalmente deve-se colocar no m†ximo para uma melhor aproximaá∆o na 
monitoraá∆o do sistema. Entretanto alguns reclamaram que seus programas 
de comunicaá∆o eventualmente perdiam dados enquanto o programa estava 
rodando. Por esta raz∆o a prioridade agora Ç variavel.
:p.
:hp2.Intervalo de tempo:ehp2.
Este controle permite que seja dado um intervalo de tempo entre as 
atualizaá‰es dos dados. O valor pode variar de 1,0 Ö 30,0 segundos em 
intervalos de 1/10 de segundo. O n£mero mostrado Ç em dÇcimos de segundos.
:p.
:hp2.Averaging Interval:ehp2.
This control allows you to set the interval over which averages are
computed.  It can be varied from 10 seconds to 3600 seconds (one hour).

:h3 res=IDD_CONFIG_PAGE_COLORS.Cores (P†gina de Configuraá∆o)
Esta †rea mostra os tràs conjunto de cores associados com os n°veis de 
Normal, Aviso e Erro para os itens que suportam estas condiá‰es. ê 
poss°vel alterar as cores usando a Paleta de Cores do sistema.

:h3 res=IDD_CONFIG_PAGE_ANCHOR.Canto fixo (P†gina de Configuraá∆o)
Este controle permite que se informe qual o canto da janela que 
permanecer† fixo na posiá∆o quando a janela muda de tamanho. A janela do 
monitor poder† mudar de tamanho quando itens s∆o adicionados ou removidos 
da lista ativa ou quando Ç mudada a fonte atravÇs da Paleta de Fontes.

:h3 res=IDD_CONFIG_PAGE_DATEFMT.Date Format (P†gina de Configuraá∆o)
This page allows you to specify the format with which the date &amp. time
item will be displayed.  Any text you would like may be entered, and
special function codes can be included.  The entire list of valid 
function codes is provided in a list at the bottom of the page, and
by double-clicking on any item in that list the item is immediately
inserted into the current format text.  A sample result text, with 
the function codes expanded, is given right below the format text
entry field.

:h3 res=IDD_CONFIG_PAGE_POP31.E-Mail (P†gina de Configuraá∆o)
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

:h3 res=IDD_CONFIG_PAGE_POP32.E-Mail (P†gina de Configuraá∆o)
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

:h3 res=IDD_LABEL.Atributos do Item (Di†logo)
Esta caixa de di†logo mostra os nomes padr∆o e atual de um item 
juntamente com os n°veis de Aviso e Erro, caso esta caracter°stica estiver 
habilitada para este item. Para mudar o nome simplesmente digite o novo 
nome sobre o atual. Para mudar os n°veis de aviso ou erro, pode ser 
digitado o valor no campo de entrada ou movido pelo slider correspondente.
:p.
Para retornar aos valores normais, basta pressionar o bot∆o &apos.Padr∆o&apos..

:h1 res=IDM_RESETLOAD.Reinicializar medidor de carga (Opá∆o de menu)
:i1.Reinicializar medidor de carga (Opá∆o de menu)
Esta opá∆o permite ao usu†rio reinicializar o medidor de carda da CPU, 
recalibrando-o, fazendo-o desconsiderar as atividades prÇvias da CPU em 
seus c†lculos.

:note.This menu option is not available when on Warp 4.0 or better,
nor on certain higher fixpack levels of Warp 3.0, as the newer load
measuring technique makes this command irrelevant.

:h1 res=IDM_RESETAVERAGE.Reset Average Load (Opá∆o de menu)
:i1.Reset Average Load (Opá∆o de menu)
This menu option allows the user to reset the Average Load meter,
causing it to forget previous CPU activity in its calculations.
:p.
The short-cut key for this command is Alt+R.

:h1 res=IDM_RESETDRIVES.Reinicializar drives (Opá∆o de menu)
:i1.Reinicializar drives (Opá∆o de menu)
Esta opá∆o permite que o usu†rio zere qualquer condiá∆o que esteja 
presente. A reinicializaá∆o ocorre periodicamente, mas o comando permite 
ao usu†rio executar o comando imediatamente.

:h1 res=IDM_LAUNCHEMAILER.Launch E-Mail Reader (Opá∆o de menu)
:i1.Launch E-Mail Reader (Opá∆o de menu)
This option will launch the e-mail reader configured by the user.
See :link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail:elink. for more
information.

:h1 res=IDM_COPY.Copiar (Opá∆o de menu)
:i1.Copiar (Opá∆o de menu)
Esta opá∆o copia a janela corrente para o bloco de rascunho do sistema 
sob duas formas&colon. Texto e Bitmap. A vers∆o texto mostra todos os 
itens mostrados em um formato apropriado para a colagem em um editor de 
textos. A vers∆o bitmap mostra uma c¢pia exata da janela corrente e pode 
ser usada por editores gr†ficos.
:p.
:Note.A tecla de atalho para esta funá∆o Ç Ctrl+Ins.

:h1 res=IDM_ABOUT.Informaá‰es do produto (Opá∆o de menu)
:i1.Informaá‰es do produto (Opá∆o de menu)
Quando selecionada, esta opá∆o mostra a caixa de di†logo de Informaá‰es 
do produto. Para maiores informaá‰es, veja 
:link reftype=hd res=IDD_ABOUT.Informaá‰es do produto (Caixa de di†logo):elink..

:h2 res=IDD_ABOUT.Informaá‰es do produto (Caixa de di†logo)
Esta caixa de di†logo mostra o nome do programa, °cone e informaá‰es de 
direitos. Para sair do di†logo, tecle ENTER, ESPAÄO ou ESCAPE ou dà um 
clique no bot∆o OK com o mouse.

:h1 res=IDM_EXIT.Encerrar (Opá∆o de menu)
:i1.Encerrar (Opá∆o de menu)
Esta opá∆o encerra o programa, caso seja selecionada.
:p.
:Note.A tecla de atalho para esta funá∆o Ç F3.

:h1 res=IDM_KEYS_HELP.Teclas - Ajuda
:i1.Teclas - Ajuda
As seguintes teclas de funá∆o foram definidas para este programa&colon.
:sl compact.
:li.F1 - Ajuda
:li.F3 - :link reftype=hd res=IDM_EXIT.Encerrar:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.Configurar:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.Esconder t°tulo:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.Ctrl+Ins - :link reftype=hd res=IDM_COPY.Copiar:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.Indica local da configuraá∆o. (Caixa de di†logo)
:i1.Indica local da configuraá∆o. (Caixa de di†logo)
Esta caixa de di†logo Ç mostrada quando o programa n∆o pìde encontrar seu
arquivo de configuraá∆o (INI) e pergunta onde esta o arquivo ou onde deve 
ser criado.
:p.
Somente um nome v†lido de diret¢rio j† existente ser† aceito. Uma vez 
digitado o nome, tecle ENTER ou dà um clique no bot∆o OK para o programa 
continuar.
:p.
Se desejar abortar a sequància de inicializaá∆o do programa tecle ESC ou 
dà um clique no bot∆o de escape.

:h1 res=60000.HRTIMER - Driver de clock de alta resoluá∆o
O device driver do rel¢gio de alta resoluá∆o, HRTIMER.SYS, possibilita um 
programa a acessar um chip de tempo normalmente n∆o utilizado nos IBM PCs. 
Este chip tem uma resoluá∆o de atÇ 840 nanosegundos, 64K vezes melhor do 
que a resoluá∆o normal de 55 milisegundos.
:p.
Os £nicos lugares que conheáo atualmente que tem este programa dispon°vel
s∆o&colon.
:hp4.CompuServe:ehp4., 
onde o programa pode ser encontrado na biblioteca 
:hp4.Device Driver Development:ehp4.
do forum
:hp4.OS/2 Developer&apos.s Forum I (OS2DF1):ehp4., 
com o nome de
:hp4.TIMER.ZIP:ehp4.,
e nos sites de ftp anìnimo
:hp4.ftp.cdrom.com:ehp4. 
e
:hp4.hobbes.nmsu.edu:ehp4..
O arquivo se encontra no diret¢rio
:hp4./pub/os2/dev16:ehp4.,
como
:hp4.hrtimer.zip:ehp4..
:p.
O c¢digo fonte para este driver foi conseguido da revista &apos.IBM Personal 
System Developer&apos., ediá∆o de outono de 1991. ê freeware, porÇm o copyright 
original pertence Ö IBM.

:h1 res=60001.Qual Ç o conflito com o PULSE?
Este programa, como o PULSE, CPULOAD, CPUMETER e programas semelhantes, 
tentam medir o qu∆o ocupado o computador esta, verificando quanto trabalho 
pode ser realizado com a menor prioridade poss°vel. Supostamente se um 
destes programas pode fazer algo nesta prioridade ent∆o deve estar usando 
tempo vago. Se n∆o h† tempo vago ent∆o o computador esta 100% ocupado. 
Quanto mais este programa pode fazer com prioridade reduzida, menos ocupado 
o computador deve estar.
:p.
O problema Ç que rodando mais de um destes programas ao mesmo tempo, 
estes estar∆o competindo pelo tempo vago. Se se rodar duas c¢pias do mesmo 
programa e nada mais, os dois dever∆o mostrar uma carga de 50% na CPU. Se 
se rodar dois programas diferentes, como este e o PULSE, ambos devem ser 
afetados, mas normalmente em graus diferentes.

:h1 res=60002.Como o HRTIMER.SYS pode ajudar?
Com o HRTIMER.SYS instalado, a sobre carga do sistema Ç incluida na carga 
da CPU.
:p.
A carga da CPU Ç determinada comparando-se quanto trabalho pode ser 
realizado em baixa prioridade com o m†ximo que o programa foi capaz de 
fazer atÇ o presente momento. Sem maiores informaá‰es, as atividades de 
retaguarda normais do sistema devem aparecer como carga zero. Se o 
HRTIMER.SYS estiver instalado na sua m†quina, ent∆o este programa executa 
um laáo de calibragem na inicializaá∆o, no qual ele determina quanto 
trabalho pode ser feito na prioridade mais alta. Este valor Ç ent∆o usado 
para comparaá∆o.

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
