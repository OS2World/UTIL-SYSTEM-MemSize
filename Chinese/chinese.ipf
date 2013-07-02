#include "memsize.h"
:userdoc.
:title.&apos.系統資源監視器&apos. 解說
:body.

:h1 res=ID_MAIN.簡介
:i1.簡介
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
本程式會顯示跟系統資源有關的一些資訊，而且只要在 CPU 時間許可下
每秒會更新一次數值。 顯示的項目有：
:p.
:hp2.日期/時間:ehp2. - 目前的系統日期及時間，格式是由 CONFIG.SYS 檔內的
&apos.COUNTRY=&apos. 設定項來決定各國不同的格式。
:p.
:hp2.經過時間:ehp2. - 本次系統啟動到現在所經過的時間。
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.剩餘實體記憶體:ehp2. - 系統剩餘的實體記憶體大小，數值是依據
:hp1.DosMemAvail:ehp1. 而來。
:p.:hp2. 註：:ehp2.該指令是舊的 16 位元系統指令，不但只報告 16Mb 以下的實體記憶體，
並且在 OS/2 2.0 問世前被更改成不會報告少於 512K 的記憶體。
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.剩餘虛擬記憶體:ehp2. - 系統剩餘的虛擬記憶體大小，數值是依據
:hp1.DosQuerySysInfo:ehp1. 而來。
本數值為剩餘的實體記憶體，加上目前交換檔未使用的空間大小，再加上
交換檔可增長之磁碟空間，減掉交換磁碟上保留的空間大小。
:p.
:hp2.記憶體交換檔大小:ehp2. - 系統目前使用的虛擬記憶體交換檔，
SWAPPER.DAT 的大小。
該檔的位置是由 CONFIG.SYS 檔中的 SWAPPATH 設定項取得。
該項目提供交換檔所在目錄的完整路徑名稱並定義交換檔所在磁碟必須保留
的最小空間。
:p.
:hp2.磁碟可交換空間大小:ehp2. - 記憶體交換檔所在邏輯磁碟上剩餘的磁碟空間，
減掉最小保留空間。 這是記憶體交換檔在有必要時所能增長的最大空間。
:p.
:hp2.交換檔未使用的空間大小:ehp2. - 目前使用的記憶體交換檔中尚未使用的空間。
當此值降至 0 時，記憶體交換檔便會增長，通常一次是以 1MB 為單位增長。
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.排存器檔案大小:ehp2. - 排存器檔案在磁碟上所佔的空間大小。
:p.
:hp2.CPU 負載 (%):ehp2. - CPU 可用的馬力中已被使用的百分比。
現值是由前一秒的數值平均而來。
:p.:hp2. 註：:ehp2.Under versions of OS/2 before release 4.0, 本功能與「脈衝」（Pulse）並不相容。
:link reftype=hd res=60001.為什麼呢？:elink.
:p.:hp2. 註：:ehp2.Under versions of OS/2 before release 4.0, 若您有安裝 :link reftype=hd res=60000.HRTIMER.SYS:elink. 驅動程式,
本數值將會較為精確。
:link reftype=hd res=60002.為什麼呢？:elink.
:p.
:hp2.Average Load (%):ehp2. - The approximate percentage of the CPU&apos.s available
power that has been used in the recent past.  The number of seconds to be 
averaged may be configured by the user 
(see the :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Configuration:elink. dialog).
:p.
:hp2.電池電力:ehp2. - 會顯示電池目前所剩餘的電力或其狀態（充電中、AC 等）。
:p.
:hp2.執行中程式件數:ehp2. - 系統程式切換列表，也就是您按 CTRL+ESC 時出現的視窗，
中執行中的程式件數。
:p.:hp2. 註：:ehp2.並不是所有系統程式切換列表中的程式都會顯示在視窗列表中。
有些被標記為不顯示。
:p.
:hp2.執行中工作件數:ehp2. - 目前載入系統執行中的工作（process）數目。
:p.
:hp2.執行中工作緒數目:ehp2. - 目前載入系統執行的工作緒（thread）數目。
每個工作都至少有一個工作緒。
:p.
:hp2.剩餘磁碟空間總和:ehp2. - 您的電腦上所有固定磁碟機（如硬碟）上的剩餘空間總和。
:p.
:hp2.X 磁碟剩餘空間:ehp2. - X 磁碟上的剩餘空間大小。
:p.
如您所見，本解說系統是活動的，本程式的功能都可以經由視窗的系統選單
來執行。 有以下的指令可用:
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.重設視窗設定:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.隱藏控制框:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.設定...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.重設負載量計:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.重設磁碟:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Launch E-Mail Reader:elink.
:li.:link reftype=hd res=IDM_COPY.複製:elink.
:li.:link reftype=hd res=IDM_ABOUT.關於:elink.
:li.:link reftype=hd res=IDM_EXIT.關閉:elink.
:esl.:p.
除了已經描述的功能之外，本程式並接受 OS/2 2.0 字型及顏色設定程式的指令,
而且是所有目前顯示項目的 DDE 伺服器。 DDE 應用程式名稱為 MEMSIZE，而
項目名稱則跟內設的顯示項目名稱一樣（如「剩餘實體記憶體」）。

:h1 res=IDM_RESET_DEFAULTS.重設視窗設定（選單選項）
:i1.重設視窗設定（選單選項）
選擇此選單選項會將程式的字型及顏色設定都重設回系統內定值。

:h1 res=IDM_HIDE_CONTROLS.隱藏控制框（選單選項）
:i1.隱藏控制框（選單選項）
當選擇此選單選項時，程式會將其控制框（系統選單、視窗標題棒及縮至最小鈕）
隱藏起來。 本選項可用左右任一滑鼠鍵雙擊來切換。 而且，因為我認為即使隱藏
控制框，視窗的移動還是很重要，所以我把視窗設定成左右任一個滑鼠按鈕都
可以將視窗拖著走。
:p.
Alt+H 組合鍵也可以切換此選項。

:h1 res=IDM_CONFIGURE.設定（選單選項）
:i1.設定（選單選項）
當選擇此選單選項時，會顯示程式的設定對話視窗。
進一步說明，見 :link reftype=hd res=IDD_CONFIGURE.設定（對話視窗）:elink..
:p.:hp2. 註：:ehp2.本選項的快鍵為 Alt+C。

:h2 res=IDD_CONFIGURE.設定（對話框）
本對話框中的筆記簿可以讓您改變本程式所有可設定旳屬性。
這些設定選項分別被歸類成下列幾頁：
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.顯示項目:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.選項（第 1 頁）:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.選項（第 2 頁）:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.顏色:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.視窗固定點:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Date Format:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (Page 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (Page 2):elink.
:esl.:p.
所有的設定改變都會立即生效。

:h3 res=IDD_CONFIG_PAGE_ITEMS.顯示項目（設定頁）
本頁顯示的是可顯示項目的清單。
您可以在顯示項目清單中選擇顯示部分或者是所有的監視項目。
只要用滑鼠在欲顯示的項目上點一下使該項反白即可。 取消選擇只要再用滑鼠點一下
即可。
反白的項目就會顯示在視窗中受到監視。
欲更改某項目的標題，或是復原成內定的標題，用滑鼠雙擊沒有被選擇顯示的項目。
該項目便會被選擇顯示並會出現一個對話視窗讓您更改該項目的標題。
To change an item&apos.s default label, or to adjust its warning and/or error 
thresholds, either double-click mouse button one on the item list entry, or single-click
mouse button two on the entry.  In either case, the 
:link reftype=hd res=IDD_LABEL.Item Attribute:elink. dialog will be displayed.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.選項（第 1 頁）（設定頁）
本頁顯示一些可以使用或關閉的程式功能選項。
:p.
:hp2.隱藏控制框:ehp2.
當視窗沒有被圖示化時，您可以選擇要不要顯示上面的標題棒、系統選單以及最大/
最小化按鈕。 若您選擇隱藏控制框，您一樣可以用滑鼠點視窗內部做拖曳視窗的
動作。
:p.
:hp2.浮在最上面:ehp2.
本按鈕會啟動監視視窗的浮在最上方功能。
當此按鈕被選擇後，監視視窗會保持在其它視窗的上方，使監視資料永遠看得到。
:p.
:hp2.動畫:ehp2.
本按鈕會啟動視窗的開啟 &amp. 關閉動畫功能。
本功能打開時，程式視窗的開關會有如同資料夾物件一樣的音效及動作。
也如同資料夾一樣，若您在系統中就把動畫功能關掉，則這裡的動畫也無法顯示。
:p.
:hp2.Dialog Border:ehp2.
This button determines whether the main program window will have 
a simple line border, or a more elaborate dialog border.  The line
border requires less space on the screen, but the dialog border can
look better, especially on OS/2 Warp 4.0 or later, which gives this
border style a three-dimensional appearance.
:p.
:hp2.表格格式:ehp2.
當選取此按鈕時，視窗內的資訊會以行列的方式，以表格的格式來顯示。
該表格會以最低的高度，也就是會以扁平的格式來顯示資訊。
當不選取此按鈕時，資訊視窗會以一列的格式，由上至下顯示所有的資訊。
:p.
:hp2.整點鐘響:ehp2.
本按鈕可啟動整點鐘響功能。本功能啟動後，程式會在每個整點鐘時嗶兩聲。
:p.
:hp2.Show Disk Drives:ehp2.
When this button is checked, disk drive free space is monitored, subject
to the restrictions placed on this monitoring by other options.  
This control mainly exists as a way to disable disk monitoring.
:p.
:hp2.顯示遠端磁碟:ehp2.
當選取此按鈕時，連接到系統上的遠端（網路）磁碟也會跟硬碟一樣，
一併被列入監視之列。只是遠端磁碟的剩餘空間不會被計入總剩餘磁碟
空間內。
:p.
:hp2.顯示檔案系統名稱:ehp2.
本按鈕可使磁碟在顯示剩餘空間的同時一併顯示該磁碟所使用的檔案系統。
選用本選項會使視窗的寬度加寬。
:p.
:hp2.顯示磁碟標號:ehp2.
本按鈕可使磁碟在顯示剩餘空間的同時一併顯示該磁碟的磁碟標號（label）。
選用本選項會使視窗的寬度加寬。
:p.
:hp2.顯示秒數:ehp2.
本按鈕會使時鐘和系統經過時間都顯示至秒。
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

:h3 res=IDD_CONFIG_PAGE_OPTIONS2.選項（第 2 頁）（設定頁）
本頁有一些設定較複雜的程式選項。
:p.
:hp2.顯示 &apos.K&apos. 數:ehp2.
本組選項鈕可以讓您何時將較大的數值以千位元組（KB）來顯示。
選項有「總是」、「從不」，或是「當數值超過 512K 時」以 KB 顯示。
程式內設值是「當數值超過 512K 時」以 KB 顯示。
.br
當選取 &apos.K=1024&apos. 鈕時，數值會以 1K=1024 來顯示。
沒有選取 &apos.K=1024&apos. 鈕時，數值會以 1K=1000 來顯示。
.br
當選取 &apos.M&apos.egabytes 按鈕時，數值超過 8MB 時會以 MB 來顯示。bytes.
.br
The &apos.G&apos.igabytes button, if checked, will cause the numbers to be
displayed in gigabytes once their values exceed four gigabytes.
:p.
:hp2.監視優先權:ehp2.
本項控制可以讓您設定程式在即時優先層級內對監視流程的絕對優先權。
您可以設定 0 到 31 的優先權級。 31 級是系統中最高的權級，最少的實體中斷。
0 級即時層級中的最低權級。 一般而言，您應將此值設至最大以便對系統做
更精確的監視。 儘管如此，有些使用者曾把怨如此會使一些通訊程式漏失一些
資料。 因此本優先權級現在是可調整的。
:p.
:hp2.間隔時間:ehp2.
本控制可以讓您設定監視視窗更新的時間間隔。 最短 1.0 秒，最長 30.0 秒，
以 1/10 秒為單位調整。 設定的數字是以 1/10 秒為單位。
:p.
:hp2.Averaging Interval:ehp2.
This control allows you to set the interval over which averages are
computed.  It can be varied from 10 seconds to 3600 seconds (one hour).

:h3 res=IDD_CONFIG_PAGE_COLORS.顏色（設定頁）
本區域是用來展示某些顯示項目的正常、警告，以及錯誤等級的顏色。
您可以從顏色選用區將顏色直接丟在上面來改變設定的顏色。

:h3 res=IDD_CONFIG_PAGE_ANCHOR.視窗固定點（設定頁）
本設定可以讓您設定在改變監視視窗大小時保持不動的視窗固定點。
您在新增或刪除顯示項目或是改變字體時會改變監視視窗大小。

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

:h3 res=IDD_LABEL.Item Attributes（對話框）
本對話框顯示目前使用以及系統內定的顯示項目標題，以及特定項目的
警告及錯誤等級。
若要更改標題，只要在原來的標題上直接輸入新的標題即可。
若要更改錯誤或警告等級，只要在項目欄上輸入新的等級，或是移動伴隨
該等級的滑片即可。
:p.若要將設定值還原為系統內定值，請按「預設值」(Default）按鈕。

:h1 res=IDM_RESETLOAD.重設負載量計（選單選項）
:i1.重設負載量計（選單選項）
本選單選項讓使用者可以重設 CPU 的負載量計，重新校正，不將之前的 CPU
運作列入計算。

:note.This menu option is not available when on Warp 4.0 or better,
nor on certain higher fixpack levels of Warp 3.0, as the newer load
measuring technique makes this command irrelevant.

:h1 res=IDM_RESETAVERAGE.Reset Average Load (Menu Option)
:i1.Reset Average Load (Menu Option)
This menu option allows the user to reset the Average Load meter,
causing it to forget previous CPU activity in its calculations.
:p.
The short-cut key for this command is Alt+R.

:h1 res=IDM_RESETDRIVES.重設磁碟機（選單選項）
:i1.重設磁碟機（選單選項）
本選單選單可以讓使用者重設目前可能存在的任何磁碟錯誤。
程式本身會在一段時間後自動做重設的動作，但是本命令可以讓程式
馬上重設錯誤。

:h1 res=IDM_LAUNCHEMAILER.Launch E-Mail Reader (Menu Option)
:i1.Launch E-Mail Reader (Menu Option)
This option will launch the e-mail reader configured by the user.
See :link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail:elink. for more
information.

:h1 res=IDM_COPY.複製（選單選項）
:i1.複製（選單選項）
本選單選項可以將目前的視窗顯示資料複製到系統的剪輯暫存區（clipboard），
格式有兩種：文字（Text）和位元圖形（Bitmap）。 文字版本的格式適用於
將所有的顯示項目剪貼到文字編輯器中。 位元圖形版本則是將目前的視窗顯示
照樣複製一份，可供圖形編輯或閱讀程式使用。
:p.
本指令的快鍵為 Ctrl+Ins。

:h1 res=IDM_ABOUT.關於（選單選項）
:i1.關於（選單選項）
當選擇此選單選項時，程式的關於（About）視窗會顯示。
進一步說明，見 :link reftype=hd res=IDD_ABOUT.關於（對話框）:elink..

:h2 res=IDD_ABOUT.關於（對話框）
本對話框顯示程式的名稱、圖示及版權聲明。
要離開本對話框，只要按 ENTER 鍵、空白鍵或 ESCAPE 鍵，
或用滑鼠點 OK 鈕即可。

:h1 res=IDM_EXIT.關閉（選單選項）
:i1.關閉（選單選項）
當選擇此選單選項時，會中止本程式的執行。

:h1 res=IDM_KEYS_HELP.按鍵解說
:i1.按鍵解說
本程式定義的功能鍵如下:
:sl compact.
:li.F1 - 解說
:li.F3 - :link reftype=hd res=IDM_EXIT.離開:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.設定:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.隱藏控制框:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.Ctrl+Ins - :link reftype=hd res=IDM_COPY.複製:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.設定資料檔路徑（對話框）
:i1.設定資料檔路徑（對話框）
程式在找不到資料檔時便會出現此對話框，詢問檔案所在的路徑或是新建該檔
所要放置的路徑位置。
:p.
只有現在已存在的合法路徑名稱會被接受。
您在輸入好路徑後，只要按 ENTER 鍵或用滑鼠點 OK 鈕即可繼續程式的執行。
:p.
若您想離開此程式的起始設定過程，只要按 ESC 鍵或用滑鼠點離開鈕即可。

:h1 res=60000.HRTIMER - 高準度時計驅動程式
高精準度計時器驅動程式，HRTIMER.SYS，可以讓程式使用 IBM PC 上一顆很少
被用到的計時器晶片。 它最高可以精確到 840 十億分之一秒（nanosecond），
精準度是一般計時器 55 毫秒（millisecond）的 64K 倍。
:p.我所知道在 :hp4.CompuServe:ehp4. 有這個程式，在其 OS/2 發展論壇（OS2DF1）下的
:hp4.Device Driver Development:ehp4. 程式庫中，檔名為 :hp4.TIMER.ZIP:ehp4.。
另外在 :hp4.ftp.cdrom.com:ehp4. 和 :hp4.hobbes.nmsu.edu:ehp4. 這兩個 ftp sites 也可以找到，
檔名為 :hp4.hrtimer.zip:ehp4.，位於 :hp4./pub/os2/dev16:ehp4. 路徑中。
:p.此驅動程式的原始碼是從 1991 年秋季號的 IBM 個人系統發展者（IBM Personal
Systems Developer）雜誌取得。 它是免費軟體，但原始版權是屬於 IBM 的。

:h1 res=60001.哪裡和脈衝（PULSE）不相容?
本程式，以及其它如 PULSE，CPULOAD，CPUMETER 的類似程式，都嘗試以探測在
最低可能的優先權下該程式能做多少事來測知電腦的忙碌程度。 大約說來，若一個
這類程式在該優先權下能做事，它一定有佔用到閒置時間（idle time）。 若已經沒有
閒置時間，電腦是 100% 忙碌的。 該程式在系統閒置優先權下能做的越多，電腦大概
就越不忙。
:p.同時執行一種以上這種測 CPU 程式的問題在於它們會爭奪閒置時間。 若您同時
執行兩個同類型的測 CPU 程式而不執行其它的程式，它們應該都會顯示 50% 的
CPU 負載。 若您執行的兩個程式是不同類型的，例如本程式和脈衝（PULSE），
它們之間會有不同程度的相互影響。

:h1 res=60002.HRTIMER.SYS 如何幫助您？
當安裝 HRTIMER.SYS 時，系統的運算時間會被納入 CPU 負載中。
:p.
CPU 負載的測定是將在閒置優先權時能做的工作量拿來和測定程式在當時所能做的
工作量兩相比較。 若沒有其它的資訊輔助，正常的系統背景活動會被視為零負載。
若您的系統有安裝 HRTIMER.SYS，則本程式在啟始時會先執行一個校正迴圈，同時
測定在最高可能的優先權下所能完成的工作量。 再拿這個數值來做比較。

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
