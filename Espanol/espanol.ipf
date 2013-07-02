#include "memsize.h"
:userdoc.
:title.&apos.Ayuda de Recursos del Sistema OS/2&apos.
:body.

:h1 res=ID_MAIN.Introducci¢n
:i1.Introducci¢n
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
Este programa muestra por pantalla diferentes informaciones sobre los
recursos del sistema, y las actualiza una vez por segundo, si es que el
procesador tiene tiempo para hacerlo. Las informaciones que se muestran
son:
:p.
:hp2.Fecha/Hora:ehp2. - La fecha y hora actual, en el formato normal
de cada pa¡s, dependiendo de como est‚ configurada la l¡nea &apos.COUNTRY=&apos. 
del fichero CONFIG.SYS
:p.
:hp2.Tiempo transcurrido:ehp2. - El tiempo que ha pasado desde la £ltima
vez que se arranc¢ el ordenador.
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.Memoria f¡sica libre:ehp2. -
La cantidad de memoria libre del sistema, de acuerdo con el valor
devuelto por la funci¢n :hp1.DosMemAvail:ehp1.. 
:note.Esta funci¢n formaba parte del sistema antiguo de 16 bits, y s¢lo
informa de la memoria f¡sica por debajo de 16Mb. Fue modificada justo antes 
de que fuese hecho p£blico el OS/2 2.0 para que nunca reportara menos
de 512K.
:note.Si est‚ presente el programa de analisis de sistemas de IBM 
:link reftype=hd res=60003.Theseus/2:elink.,
este n£mero ser  completamente correcto, aunque no se revisar 
mas a menudo que una vez cada diez segundos.
:p.
:hp2.Memoria virtual libre:ehp2. -
La cantidad de memoria virtual libre seg£n la funci¢n
:hp1.DosQuerySysInfo:ehp1..  Esta es la cantidad de memoria f¡sica
libre, m s la cantidad de espacio libre dentro del fichero de
intercambio (swap), m s la cantidad de espacio que tiene a£n el fichero
de intercambio para crecer, menos la cantidad que se debe reservar libre 
en la unidad donde se encuentra el fichero de intercambio.
:p.
:hp2.Tama¤o del fichero de intercambio:ehp2. - El tama¤o que ocupa en el
disco el fichero de memoria virtual de intercambio (swap), SWAPPER.DAT.
Para localizar este fichero, busque en el fichero CONFIG.SYS la l¡nea
que empieza con SWAPPATH, donde est  indicado el nombre completo
(incluido el directorio) del fichero de intercambio, as¡ como el espacio
que se debe reservar en la unidad d¢nde se encuentra este fichero.
:p.
:hp2.Espacio disponible para el fichero de intercambio:ehp2. - La
cantidad total de espacio en la unidad de disco d¢nde se encuentra el
fichero de intercambio, menos la cantidad de espacio que debe
reservarse. Este valor es el tama¤o m ximo que puede alcanzar el fichero
de intercambio.
:p.
:hp2.Espacio sin usar del fichero de intercambio:ehp2. - La
cantidad de espacio dentro del fichero de intercambio que est  actualmente
sin usar.  Cuando ‚sta baja a cero, el fichero de intercambio tiene
que ser aumentado de tama¤o, normalmente en un megabyte cada vez.
:note.Si est‚ presente el programa de analisis de sistemas de IBM 
:link reftype=hd res=60003.Theseus/2:elink.,
este n£mero ser  completamente correcto, aunque no se revisar 
mas a menudo que una vez cada diez segundos.
:p.
:hp2.Tama¤o del fichero del spooler:ehp2. - La cantidad total de disco
consumida por los ficheros enviados al spooler de la impresora.
:p.
:hp2.Porcentaje de utilizaci¢n de la CPU:ehp2. - El porcentaje
aproximado de utilizaci¢n de la CPU. El valor que se muestra es el
porcentaje correspondiente al promedio de utilizaci¢n del £ltimo
segundo.
:Note.Bajo versiones de OS/2 antes de 4.0, esta funci¢n y el PULSE que se incluye con el OS/2 2.0, no son muy
compatibles.
:link reftype=hd res=60001.¨Por qu‚?:elink.
:Note.Bajo versiones de OS/2 antes de 4.0, si est‚ instalado el programa :link reftype=hd res=60000.HRTIMER.SYS:elink., 
este n£mero ser  mas correcto.
:link reftype=hd res=60002.¨Por qu‚?:elink.
:p.
:hp2.Utilizaci¢n promedio (%):ehp2. - El porcentage aproximado de utilizaci¢n
de la CPU que se ha usado en el pasado reciente.  El n£mero de segundos
para usar en este c culo se puede fijar por el usuario
(v‚ase el di logo :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Configuraci¢n:elink.).
:p.
:hp2.Pila:ehp2. - La capacidad que queda en la pila, 
o su estado actual (cargando, enchufado) se muestra.
:p.
:hp2.N£mero de tareas activas:ehp2. -
El n£mero de elementos en la Lista de Ventanas, que es la lista que se
visualiza cuando pulsamos CTRL-ESC.
:note.No todas las tareas del sistema se muestran en la Lista de
Ventanas, ya que algunas de estas tareas no son visibles.
:p.
:hp2.N£mero de procesos:ehp2. - El n£mero de procesos actualmente
cargados en el sistema.
:p.
:hp2.N£mero de hilos:ehp2. - El n£mero de hilos (threads) de procesamiento
en el sistema.  Hay por lo menos un hilo (thread) por cada proceso.
:p.
:hp2.Espacio libre total:ehp2. - La suma del espacio libre en todas las
unidades locales de disco (no removibles).
:p.
:hp2.Espacio libre en la unidad X:ehp2. - La cantidad de espacio libre
en el disco X.
:p.
Esta ayuda es sensible al contexto, como ya se habr  dado cuenta. El
acceso a las siguientes funciones se realiza a trav‚s del men£ de
sistema de la ventana&colon.
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.Restaurar los valores por defecto:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.Esconder los controles:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.Configuraci¢n...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.Reinicializar medidor de uso:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Reinicializar el uso promedio:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.Reinicializar ficheros:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Lanzar lector de correo:elink.
:li.:link reftype=hd res=IDM_COPY.Copiar:elink.
:li.:link reftype=hd res=IDM_ABOUT.Informaci¢n del producto:elink.
:li.:link reftype=hd res=IDM_EXIT.Cerrar:elink.
:esl.:p.
Aparte de estas funciones, el programa acepta ordenes de los
controladores de fuentes y de la paleta de colores del OS/2 2.0, 
y es un servidor &apos.DDE&apos. para todos los elementos actualmente
mostrados.  El nombre de aplicaci¢n es MEMSIZE, y los nombres de
elementos son los mismos nombres de elementos por defecto
(como &apos.Memoria f¡sica libre&apos.).

:h1 res=IDM_RESET_DEFAULTS.Restaurar los valores por defecto (Opci¢n del men£)
:i1.Restaurar los valores por defecto (Opci¢n del men£)
Seleccionado esta opci¢n del men£, se restaura el tipo de letra y los
atributos de color a sus valores por defecto.

:h1 res=IDM_HIDE_CONTROLS.Esconder los controles (Opci¢n del men£)
:i1.Esconder los controles (Opci¢n del men£)
Cuando esta opci¢n del men£ est  seleccionada, obliga a que los controles de
la ventana del programa (el men£ de sistema, la barra con el t¡tulo y el
bot¢n de minimizar) esten ocultos. Es posible variar el valor de esta
funci¢n pulsando dos veces el bot¢n del mouse dentro de la ventana.
.br
La tecla aceleradora asignada a esta funci¢n es ALT+H.

:h1 res=IDM_CONFIGURE.Configurar (Opci¢n del men£)
:i1.Configurar (Opci¢n del men£)
Cuando se selecciona esta opci¢n del men£, aparecer  la pantalla de
configuraci¢n del programa.
Para mas informaci¢n, v‚ase :link reftype=hd res=IDD_CONFIGURE.Configurar (Caja de di logo):elink..
:note.La tecla aceleradora asignada a esta funci¢n es Alt+C.

:h2 res=IDD_CONFIGURE.Configurar (Di logo)
Esta pantalla nos permite cambiar todos los aspectos configurables
del programa.  Las opciones se organizan seg£n su categor¡a en las 
p ginas sigientes&colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.Elementos:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.Opciones (P gina 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Opciones (P gina 2):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.Colores:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.Ancla:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Fecha:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.Correo (P gina 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.Correo (P gina 2):elink.
:esl.:p.
Cualquier cambio tendr  efecto inmediatamente.

:h3 res=IDD_CONFIG_PAGE_ITEMS.Elementos (P gina de configuraci¢n)
Esta p gina muestra una lista de elementos para mostrar.
Podemos seleccionar varios o todos los elementos que deber n
monitorizarse. S¢lo deberemos pulsar en un elemento para
seleccionarlo, o bien hacer otro click para desmarcarlo. Los
elementos marcados son los que se visualizar n.
Para cambiar el nombre por defecto de un elemento para mostrarse, 
o para ajustar sus niveles de aviso y/o de error, hay que hacer un
doble-click con el bot¢n n£mero uno, o un solo click con el bot¢n
n£mero dos sobre el elemento en cuesti¢n.  En los dos casos el di logo
:link reftype=hd res=IDD_LABEL.Atributos del elemento:elink. ser  desplegado.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.Opciones (P gina 1) (P gina de configuraci¢n)
Esta p gina muestra una lista de los elementos disponibles
y permite que se activen o que se inactiven.
:p.
:hp2.Esconder los controles:ehp2.
Cuando el programa no est  en forma de icono, lo podremos ver con o
sin los elementos normales de las ventanas&colon. barra de t¡tulo, men£ de
sistema y botones de maximizar y minimizar. Si escogemos esconder
los controles, todav¡a podemos cambiar la posici¢n de visualizaci¢n
arrastrando la ventana con el mouse.
:p.
:hp2.Quedar en primer plano:ehp2.
Este bot¢n activa la funci¢n de quedar siempre en primer plano.
Cuando este bot¢n esta marcado, la ventana del programa quedar 
siempre en el primer plano de visualizaci¢n, de forma que siempre
ser  visible.
:p.
:hp2.Animaci¢n:ehp2.
Este bot¢n activa la funci¢n de animaci¢n al abrir y cerrar la
ventana. Cuando est  seleccionado, la ventana del programa se abrir 
de la misma forma que lo hacen las carpetas: con sonido y
movimiento. De la misma forma que con las carpetas, no habr  ninguna
animaci¢n si esta ha sido deshabilitada a nivel de sistema.
:p.
:hp2.Borde de di logo:ehp2.
Este bot¢n determine si la ventana principal del programa tendr 
un borde simple y linear, o un borde mas elaborado.  El borde linear
requiere menos espacio en la pantalla, pero el borde di logo puede
parecer mejor, especialmente con OS/2 Warp 4.0 o mas tarde, lo cual
da a este estilo de borde una apariencia de tres dimensiones.
:p.
:hp2.Formato tabla:ehp2.
Este bot¢n activa la funci¢n de mostrar la ventana siempre en 
formato tabla, o sea, con los elementos distribuidos 
horizontalmente, en vez de verticalmente.  Se dispone para
ocupar tan poco espacio vertical como sea posible.
Cuando no se marque este bot¢n, la ventana se colocar  en
forma vertical, con una sola columna de elementos.
:p.
:hp2.Sonido a la hora:ehp2.
Este bot¢n causa que suene un sonido al comienzo de cada hora.
:p.
:hp2.Mostrar ficheros:ehp2.
Cuando se marque este bot¢n, se muestra el espacio libre en los
diferentes mandos de disco, sujeto a las restricciones puestos
sobre esto por las otras opciones.  Este control existe principalmente
como una manera para apagar la funci¢n de vigilarlos a los discos.
:p.
:hp2.Mostrar ficheros remotos:ehp2.
Cuando se marque este bot¢n, los ficheros remotos (LAN) que est n
conectados actualmente al sistema se muestra igual que los ficheros
locales, salvo que no se incluyen en el total de espacio libre local.
:p.
:hp2.Mostrar el nombre del sistema de ficheros:ehp2.
Si este bot¢n est  marcado se indicar , adem s del espacio
libre el nombre del sistema de ficheros de la unidad. El uso de esta
opci¢n implica una ventana un poco m s ancha.
:p.
:hp2.Mostrar los nombres de los ficheros:ehp2.
Si este bot¢n est  marcado se indicar , adem s del espacio
libre el nombre de la unidad en si.  El uso de esta opci¢n implica
una ventana un poco m s ancha.
:p.
:hp2.Mostrar segundos:ehp2.
Este bot¢n causa que el reloj y el tiempo transcurrido incluyen
los segundos del minuto.
:p.
:hp2.Usar Theseus/2:ehp2.
Este bot¢n causa que el programa use el programa &apos.Theseus/2&apos. de
IBM, cuando est‚ presente, para tales datos que puede medir mas 
correctamente que de otros modos.  Debido al costo al CPU de usarlo, 
de ninguna manera se usa Theseus/2 mas que una vez por cada diez segundos.
Actualmente, esta t‚cnica provee informaci¢n mejor para la cantidad
de memoria f¡sica libre y para el espacio sin usar adentro del fichero
de intercambio.
:p.
:hp2.Centrar Reloj:ehp2.
Este bot¢n causa que el programa fije su reloj al centro de su cellulo,
en vez de para el lado izquierdo.

:h3 res=IDD_CONFIG_PAGE_OPTIONS2.Opciones (P gina 2) (P gina de configuraci¢n)
Esta p gina muestra otras opciones del programa que son mas
complejas que simplemente activado o desactivado.
:p.
:hp2.Mostrar &apos.K&apos.ilobytes:ehp2.
Este grupo de botones &apos.radio&apos. permite que Ud. escoja si los tama¤os
grandes ser n mostrados en kilobytes siempre, nunca, o s¢lo cuando
sus valores excedan 512K.
Lo normal es mostrar todo tama¤o mas grande que 512K en kilobytes.
.br
El bot¢n de &apos.K=1024&apos., cuando se marque, har  que los n£meros sean
mostrados en kilobytes verdaderos (1024).  Cuando no se marque el
bot¢n, los n£meros se muestran en kilobytes decimales (1000).
.br
El bot¢n de &apos.Megabytes&apos., cuando se marque, har  que los n£meros
sean mostrados en megabytes una vez que sus valores pasen ocho megabytes.
.br
El bot¢n de &apos.G&apos.igabytes&apos., cuando se marque, har  que los n£meros
sean mostrados en gigabytes una vez que sus valores pasen cuatro gigabytes.
:p.
:hp2.Prioridad de monitorizaci¢n:ehp2.
Esta opci¢n nos permitir  establecer el valor de prioridad del
thread de monitorizaci¢n, dentro de la categoria de tiempo-cr¡tico.
Podemos variar la prioridad entre 0 y 31. La prioridad 31 es el
valor m s elevado del sistema, por debajo de las interrupciones
f¡sicas. La prioridad 0 es el valor m s bajo dentro de la categor¡a
tiempo-cr¡tico. Normalmente, este valor tiene que estar configurado
al m ximo para obtener un valor m s preciso. No obstante, algunos
usuarios han informado que, ocasionalmente, pierden algunos
caracteres en las comunicaciones que se realizan mientras se est 
ejecutando el programa. Es por esto que la prioridad es, ahora,
configurable.
:p.
:hp2.Intervalo de tiempo:ehp2.
Esta opci¢n nos permite establecer el intervalo de actualizaci¢n de
la ventana. Puede variar entre 1.0 segundos hasta 30.0 segundos, en
intervalos de 1 d‚cima de segundo. El n£mero se muestra en d‚cimas
de segundo.
:p.
:hp2.Averaging Interval:ehp2.
This control allows you to set the interval over which averages are
computed.  It can be varied from 10 seconds to 3600 seconds (one hour).

:h3 res=IDD_CONFIG_PAGE_COLORS.Colores (P gina de configuraci¢n)
Este p gina muestra los tres colores asignados a los niveles de
Normal, Aviso y de Error para los elementos a mostrar que usen tales
colores.  Se pueden elegir los colores arrastr ndolos desde una de 
las paletas de colores del sistema operativo.
:note.Bajo OS/2 Warp revisi¢n 3.0 o superior, si uno hace un click
sobre una de las muestras de colores, la paleta de colores s¢lidos 
del sistema se mostrar  para mayor comodidad.
Tambi‚n se puede usar la paleta de colores mixtos, pero esta
paleta tiene que elegirse en Configuraci¢n del Sistema.

:h3 res=IDD_CONFIG_PAGE_ANCHOR.Ancla (P gina de configuraci¢n)
Este p gina permite que el usuario elija la esquina de la ventana del
programa que quedar  fija cuando el tama¤o de la ventana cambie.
La ventana puede cambiar de tama¤o cuando se a¤aden o se eliminan elementos
de la lista activa, y cuando la fuente se cambie utilizando la paleta de
fuentes.

:h3 res=IDD_CONFIG_PAGE_DATEFMT.Fecha (P gina de configuraci¢n)
Esta p gina le permite a uno especificar el formato en lo cual se
mostrar  la fecha y la hora.  Cualquier texto que uno quiera se puede
entrar, mas unos c¢digos especiales.  La lista entera de los c¢digos
especiales se ve por el fondo de la p gina, y por hacer un doble click
sobre cualquier de las entradas el c¢digo se incluye en el texto de
formato actual de una vez.  Un texto resultado de ejemplo, con los
c¢digos elaborados, se da inmediatamente por debajo del campo de 
entrar el texto formato.

:h3 res=IDD_CONFIG_PAGE_POP31.E-Mail (P gina de configuraci¢n)
Esta p gina le permite a uno especificar los par metros por los cu les
se muestra el n£mero de mensajes esperando en el correo.  Uno tiene que
entrar el nombre del servidor 'POP3', el nombre del usuario y su clave.
Se puede especificar un n£mero de puerto 'POP3' si el servidor usa uno
que es fuera de lo normal.  Tambi‚n se puede especificar una direcci¢n
espec¡fica de IP.
:p.La opci¢n para usar APOP le permite a uno que use un protocolo avanzado
de verificaci¢n del clave lo cual reduce el riesgo de pasar la identificaci¢n
y clave de cuenta en la red abierta.
:p.Una vez que est‚ entrado el nombre del servidor, el nombre del usuario,
y el clave del usuario, se puede tocar al bot¢n de verificar para comprobar
los datos, o al bot¢n de aceptar para guardar la informaci¢n nueva (ser 
verificada primeramente), o al bot¢n de restaurar para borrar los cambios
hechos desde la £ltima tocada al bot¢n de aceptar.
:p.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.Pr¢xima p gina...:elink.

:h3 res=IDD_CONFIG_PAGE_POP32.E-Mail (P gina de configuraci¢n)
Esta p gina le permite a uno que especifique los par metros por los cuales
la entrada de mensajes esperando reaccione a cambios de su informaci¢n.
:p.El campo para el intervalo de chequear le permite a uno que fije 
el n£mero de segundos para demorar entre cada chequeada del correo
para mensajes nuevas.
:p.El campo para el lector de correo es para especificar la l¡nea completa
para usarse para lanzar su programa preferido para leer el correo.
:p.Si uno quiera que haya un sonido de alerta cuando llegue correo
nuevo, entonces se puede chequear la casilla provista.
:p.El campo para el sonido es para especificar un fichero 'WAV' para 
ser sonado al llegar correo nuevo.  Si ning£n fichero WAV se especifica,
pero s¡ se especific¢ sonido, entonces una sequencia ascendiente de tonos
ser  generado por el parlante.

:h3 res=IDD_LABEL.Atributos del elemento (Di logo)
Este dialogo muestra los nombres actuales y por defecto del elemento
que mostrar, junto con sus niveles de Aviso y de Error, si esta funci¢n
funciona para el elemento actual.  Para cambiar el nombre, solo hay
que introducir un nombre nuevo sobreescribiendo el actual.  Para cambiar 
los niveles de Aviso o de Error, hay que entrar un valor nuevo en la 
cajita al efecto, o mover el control situado a la derecha de la cajita.
:p.Para restaurar todos los valores a la opci¢n por defecto, solo hay que 
hacer un click sobre el bot¢n &apos.Defecto&apos..

:h1 res=IDM_RESETLOAD.Reinicializar medidor de uso (Opci¢n del men£)
:i1.Reinicializar medidor de uso (Opci¢n del men£)
Esta opci¢n causa que se reinicie el medidor de porcentaje de
utilizaci¢n del sistema, haciendo que se olvide de la actividad
previa de la CPU en sus c lculos.

:note.Esta opci¢n del men£ no se ve bajo OS/2 Warp 4.0 o mas nuevo,
ni con ciertas niveles mas actualizados de Warp 3.0 tampoco, como
la nueva t‚cnica para medir el uso del sistema hace este
mandato carecer de utilidad.

:h1 res=IDM_RESETAVERAGE.Reinicializar el uso promedio (Opci¢n del men£)
:i1.Reinicializar el uso promedio (Opci¢n del men£)
Esta opci¢n causa que se reinicie el medidor de porcentaje de utilizaci¢n
promedio del sistema, haciendo que se olvide de la actividad previa
de la CPU en sus c culos.
:p.
La tecla aceleradora para esta funci¢n es Alt+R.

:h1 res=IDM_RESETDRIVES.Reinicializar ficheros (Opci¢n del men£)
:i1.Reinicializar ficheros (Opci¢n del men£)
Esta opci¢n permite al usuario reinicializar ante cualquier error en
medir los ficheros que se haya producido. Esto pasar  autom ticamente de
vez en cuando, pero esta opci¢n le permite al usuario eliminar
los errores en ese mismo momento.

:h1 res=IDM_LAUNCHEMAILER.Lanzar lector de correo (Opci¢n del men£)
:i1.Lanzar lector de correo (Opci¢n del men£)
Esta opci¢n permite que el usuario lanze el lector de correo electr¢nico
que quiera.  V‚ase :link reftype=hd res=IDD_CONFIG_PAGE_POP31.Correo:elink. para mas
informaci¢n.

:h1 res=IDM_COPY.Copiar (Opci¢n del men£)
:i1.Copiar (Opci¢n del men£)
Esta opci¢n del men£ hace una copia de la ventana actual en el portapapeles 
del sistema en dos formatos&colon. texto y bitmap.  La versi¢n en texto
muestra todos los elementos actualmente activos en una forma adecuada
para pegar en un documento con un editor de textos.  La versi¢n en
bitmap muestra una copia exacta de la ventana actual, y se puede
usar con editores y visores de gr ficos.
:p.
La tecla aceleradora para esta funci¢n es Ctrl+Ins.

:h1 res=IDM_ABOUT.Informaci¢n del producto (Opci¢n del men£)
:i1.Informaci¢n del producto (Opci¢n del men£)
Esta opci¢n muestra, cuando se selecciona, informaci¢n sobre el
programa.
Para mas informaci¢n, v‚ase :link reftype=hd res=IDD_ABOUT.Informaci¢n del programa (Caja de di logo):elink..

:h2 res=IDD_ABOUT.Informaci¢n del producto (Caja de di logo)
Esta caja de di logo muestra el nombre del programa, el icono e
informaci¢n sobre copyright y el autor.

Para salir de esta caja de di logo, se puede pulsar cualquiera de estas
teclas: ENTER, la barra de espacios o bien ESCAPE. Tambi‚n se puede
salir seleccionando el bot¢n OK con el mouse.

:h1 res=IDM_EXIT.Cerrar (Opci¢n del men£)
:i1.Cerrar (Opci¢n del men£)
Cuando se selecciona esta opci¢n del men£ se termina el programa.

:h1 res=IDM_KEYS_HELP.Ayuda de las teclas
:i1.Ayuda de las teclas
Las siguientes teclas de funci¢n se han definido para el uso de este
programa&colon.
:sl compact.
:li.F1 - Ayuda
:li.F3 - :link reftype=hd res=IDM_EXIT.Salir:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.Configurar:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.Esconder los controles:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Reinicializar el uso promedio:elink.
:li.Ctrl+Ins - :link reftype=hd res=IDM_COPY.Copiar:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.Indicar el directorio del fichero .INI. (Caja de di logo)
:i1.Indicar el directorio del fichero .INI. (Caja de di logo)
Esta caja de di logo se muestra cuando el programa no puede
encontrar su fichero .INI y pregunta cual es el nombre de este
fichero y en qu‚ directorio se encuentra.
:p.
S¢lo se acceptan nombres
v lidos de directorios ya existentes. Una vez que se haya escrito
el nombre, pulsando INTRO (RETURN) o bien haciendo un click en el
bot¢n "OK" se permite al programa continuar su secuencia de
inicializaci¢n.
:p.
Si, por contra, se desea cortar la inicializaci¢n
del programa, se debe pulsar la tecla ESC o bien hacer un click en
el bot¢n de anulaci¢n.

:h1 res=60000.HRTIMER - Driver reloj de alta resoluci¢n
El driver de reloj de alta resoluci¢n &apos.HRTIMER.SYS&apos. facilita a los programas el
aceso a un circuito de reloj normalmente sin usar que se encuentra en los 
PCs &apos.IBM&apos..
Tiene una resoluci¢n hasta 840 nanosegundos, 64 mil veces mejor que la
resoluci¢n normal de 55 millisegundos.
:p.
Los £nicos lugares que conozco actualmente que tienen este programa son
:hp4.CompuServe:ehp4., 
donde el program se puede encontrar en la biblioteca
:hp4.Device Driver Development:ehp4.
del foro
:hp4.OS/2 Developer&apos.s Forum I (OS2DF1):ehp4., 
bajo el nombre de 
:hp4.TIMER.ZIP:ehp4.,
y por los sitios de servicio ftp an¢nimo
:hp4.ftp.cdrom.com:ehp4. 
y
:hp4.hobbes.nmsu.edu:ehp4..
El fichero se encuentra en el directorio
:hp4./pub/os2/dev16:ehp4.,
como
:hp4.hrtimer.zip:ehp4..
:p.El c¢digo fuente para este driver fue conseguido de la revista &apos.IBM Personal
Systems Developer&apos., edici¢n del oto¤o 1991.  Es &apos.freeware&apos., pero el copyright 
original pertenece a IBM.

:h1 res=60001.¨Qu‚ es el conflicto con PULSE?
Este programa, al igual que PULSE, CPULOAD, CPUMETER y programas semejantes,
intenta medir como est  de ocupado el ordenador tratando de ver
cu nto trabajo se puede realizar a la prioridad m¡nima.
Supuestamente, si uno de estos programas puede hacer algo a esa prioridad,
entonces estar  usando el tiempo sin permitir otro uso.
Si no hay tiempo sin uso, entonces el ordenador est  ocupado al 100%.
Cuanto mas puede hacer este programa con el tiempo sin otro uso, menos
ocupado se supone que est  el sistema.
:p.El problema con ejecutar m s de uno de estos programas a la vez es que
todos ellos empiezan a competir por el mismo tiempo sin uso.
Si uno intenta ejecutar dos copias del mismo programa, y nada mas, los dos
deben mostrar un 50% de uso del sistema.  Si uno ejecuta dos programas 
diferentes, como este programa y PULSE, entonces los dos ser n afectados,
pero usualmente en diferentes grados.

:h1 res=60002.¨C¢mo ayuda HRTIMER.SYS?
Cuando est‚ instalado HRTIMER.SYS, el uso normal del CPU por el sistema
operativo ser  incluido en el porcentaje de uso del sistema.
:p.
El porcentaje de uso del sistema se determina comparando cu nto trabajo
se puede hacer a la prioridad m¡nima con el m ximo que el programa ha podido
hacer hasta el momento presente.  Sin mas informaci¢n, el uso normal del CPU
por el sistema operativo aparecer¡a como un uso de porcentaje cero.
Si est‚ instalado HRTIMER.SYS en el sistema, entonces el programa puede
hacer una calibraci¢n al comenzar, mediante la cual se puede determinar cu nto
trabajo se podr¡a hacer a la prioridad m xima del sistema.
Esta cantidad se usa entonces para posteriores comparaciones.

:h1 res=60003.¨Qu‚ es Theseus/2?
El programa Theseus/2 fue desarollado por la IBM para el analisis de la
memoria del sistema, y una vez formaba parte de un producto que se llamaba
:hp5.System Performance Monitor/2:ehp5..
Eventualmente, dejaron de venderlo, y alguien adentro de la IBM lo empaquet¢
para distribuci¢n como freeware hacia los comienzos del a¤o 1997.
Se puede conseguir de la siguiente direcci¢n&colon.
:p.:lm margin=6.
ftp&colon.//hobbes.nmsu.edu/pub/os2/util/system/t2spm.zip
:lm margin=1.:p.
Una versi¢n mas nueva (Theseus/3) se puede encontrar en&colon.
:p.:lm margin=6.
ftp&colon.//ftp.boulder.ibm.com/ps/products/os2/fixes/v4warp/theseus3/thes3000.exe
:lm margin=1.:p.
Theseus/2 tiene dos cualidades que el programa mio probablemente nunca
tendr &colon. un conocimiento profundo de la estructura interna de OS/2,
y un driver especial que le permite inspeccionar lo interior de OS/2
directamente.  Sin embargo, estas ventajas tienen un precio&colon.
el programa demora un tiempo relativamente largo para hacer su analisis.
Por esa raz¢n, MemSize no lo usa mas que una vez cada diez segundos, 
as¡ para evitar sobrecargar al sistema.

:euserdoc.
