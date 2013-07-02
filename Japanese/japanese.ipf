#include "memsize.h"
:userdoc.
:title.&apos.システムリソース&apos. ヘルプ
:body.

:h1 res=ID_MAIN.はじめに
:i1.はじめに
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
このプログラムはシステムリソースに関する情報を表示します。
表示は１秒ごと、あるいは設定された時間ごとに更新されます。
表示されるのは以下の項目です:
:p.
:hp2.日時:ehp2. - 現在の日付と時刻を表示します。書式は CONFIG.SYS の
中の &apos.COUNTRY=&apos. 文で指定された国別情報にしたがいます。
:p.
:hp2.経過時間:ehp2. - システムが最後に起動した時刻からの経過時間です。
:p.
:hp2.Messages Waiting:ehp2. - The number of messages waiting to be downloaded.
:p.
:hp2.実メモリ空き容量:ehp2. - :hp1.DosMemAvail:ehp1. 関数によって
調べた使用可能なシステムメモリの大きさを表します。
:note.この関数は古い 16 ビットのシステム用のもので、16MB 以下のメモリに
ついてのみ報告します。また、OS/2 2.0 のリリース直前に仕様が変わり、
この関数が 512KB 以下を報告することはなくなりました。
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.仮想メモリ空き容量:ehp2. - :hp1.DosQuerySysInfo:ehp1. 関数によって
調べた使用可能な仮想メモリの大きさを表します。
この値は使用可能な実メモリに加え、現在のスワップファイル中の未使用領域の
大きさ、およびスワップファイルの大きくなれる限界までのサイズを計算します。
:p.
:hp2.スワップファイルサイズ:ehp2. - システムの仮想メモリ用スワップ
ファイルである SWAPPER.DAT の大きさを表します。
スワップファイルの場所は CONFIG.SYS の SWAPPATH 文で指定されます。
これはスワップファイルのフルパス名を指定するとともに、
スワップファイルのあるドライブに必要な最低限の空き容量も示します。
:p.
:hp2.使用可能スワップ領域:ehp2. - スワップファイルの存在する
ドライブの空き容量を示します。このサイズはスワップファイルがあとどのくらい
大きくなれるかを表します。
:p.
:hp2.未使用スワップ領域:ehp2. - スワップファイル内の未使用の領域の大きさを
表します。この値が 0 になると、スワップファイルは通常 1 メガバイト単位で
拡張されます。
:note.If you have the IBM system analysis 
program :link reftype=hd res=60003.Theseus/2:elink. installed
on your system, this number will be completely accurate,
though it will only update itself once every ten seconds at most.
:p.
:hp2.スプールファイルサイズ:ehp2. - スプールファイルによって占められている
ディスク容量を表します。
:p.
:hp2.CPU 負荷 (%):ehp2. - 現在使用可能な CPU パワーのおおよその値をパーセントで
表示します。この値は直前の 1 秒間での平均値です。
:note.Under versions of OS/2 before release 4.0, この機能と「パルス」は同時にはうまく働きません。
:link reftype=hd res=60001.なぜでしょう？:elink.
:note.Under versions of OS/2 before release 4.0, :link reftype=hd res=60000.HRTIMER.SYS:elink.
 がインストールされているとこの値はより正確になります。
:link reftype=hd res=60002.なぜでしょう？:elink.
:p.
:hp2.Average Load (%):ehp2. - The approximate percentage of the CPU&apos.s available
power that has been used in the recent past.  The number of seconds to be 
averaged may be configured by the user 
(see the :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Configuration:elink. dialog).
:p.
:hp2.電池:ehp2. - 現在の電池の残り、
またはその状態 (充電、交流電源) が表示されます。
:p.
:hp2.タスク数:ehp2. - CTRL+ESC によって現れるウィンドウリストに
含まれるエントリーの数です。
:note. 必ずしもすべてのエントリーがウィンドウリストに表示されるとは
限りません。いくつかは不可視のエントリーです。
:p.
:hp2.プロセス数:ehp2. - システムの現在のプロセス数です。
:p.
:hp2.スレッド数:ehp2. - システムの現在のスレッド数です。各プロセスには
最低 1 つのスレッドがあります。
:p.
:hp2.全ディスク空き容量:ehp2. - すべての着脱不能ローカルディスクの
空き容量の合計です。
:p.
:hp2.ドライブ X 空き容量:ehp2. - ドライブ X の空き容量です。
:p.
ヘルプ機能は現在見ているとおり使用可能になっています。
その他、以下の機能がメニューから使用可能です。
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.省略時値:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.コントロールを隠す:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.詳細設定...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.負荷計のリセット:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.ドライブのリセット:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.Launch E-Mail Reader:elink.
:li.:link reftype=hd res=IDM_COPY.コピー:elink.
:li.:link reftype=hd res=IDM_ABOUT.製品情報:elink.
:li.:link reftype=hd res=IDM_EXIT.クローズ:elink.
:esl.:p.
これらの機能に加え、プログラムは OS/2 2.0 のフォントパレット及び
カラーパレットを受け入れることができます。またすべての表示項目に
関して DDE サーバになることができます。DDE アプリケーション
名は MEMSIZE で、項目名は省略時の項目名と同じです
（例：実メモリ空き容量）。

:h1 res=IDM_RESET_DEFAULTS.省略時値（メニュー）
:i1.省略時値（メニュー）
このメニューを選ぶと、プログラムのフォントと配色が初期の状態に戻ります。

:h1 res=IDM_HIDE_CONTROLS.コントロールを隠す（メニュー）
:i1.コントロールを隠す（メニュー）
このメニュー項目をチェックすると、フレームコントロールを表示しなくなります。
フレームコントロールとはタイトルバーやシステムメニュー、最小化ボタンの
ことです。どちらかのマウスボタンによるダブルクリックでも同じように
コントロールの表示／非表示を切り換えられます。コントロールを隠した状態でも、
マウスでドラッグしてウィンドウを移動できます。
:p.
ショートカットキーは Alt+H（前面+H）です。

:h1 res=IDM_CONFIGURE.詳細設定（メニュー）
:i1.詳細設定（メニュー）
プログラムの細かい設定を行うダイアログボックスを表示するメニューです。
詳しい情報は :link reftype=hd res=IDD_CONFIGURE.詳細設定（ダイアログ）:elink.
の方に書いてあります。
:note.ショートカットキーは Alt+C（前面+C）です。

:h2 res=IDD_CONFIGURE.詳細設定 (ダイアログ)
このノートブックでは、プログラムの各種動作を設定できます。
設定項目は以下のように分類されています &colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.表示項目:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.各種設定 (ページ 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.各種設定 (ページ 2):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.配色:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.ウィンドウアンカー:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Date Format:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (ページ 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (ページ 2):elink.
:esl.:p.
設定変更はただちにプログラムに反映されます。

:h3 res=IDD_CONFIG_PAGE_ITEMS.表示項目 (詳細設定ページ)
This page displays a list of displayable items.
すべての項目について、表示する／しないを自由に決められます。マウスでクリック
すれば選択／非選択が切り替わります。濃い表示のものが選択された項目です。
各項目の名前を変えたい場合は、項目をダブルクリックしてください。
新しい名前を入力するダイアログが現れます。
To change an item&apos.s default label, or to adjust its warning and/or error 
thresholds, either double-click mouse button one on the item list entry, or single-click
mouse button two on the entry.  In either case, the 
:link reftype=hd res=IDD_LABEL.Item Attribute:elink. dialog will be displayed.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.各種設定 (ページ 1) (詳細設定ページ)
このページではオン／オフの切り換えができる項目を設定できます。
:p.
:hp2.コントロールを隠す:ehp2.
タイトルバーなどのフレームコントロールを表示するかしないかを指定します。
タイトルバーがなくても、ウィンドウ自身をドラッグすることで移動できます。
:p.
:hp2.常に前面に表示:ehp2.
このオプションが指定されると、ウィンドウが他のプログラムの
ウィンドウよりも常に上に来るようになります。
:p.
:hp2.アニメーション:ehp2.
このオプションをオンにするとウィンドウ開閉時のアニメーションが有効になります。
ただしシステム設定でアニメーションがオフになっている場合はアニメーションは
起こりません。
:p.
:hp2.Dialog Border:ehp2.
This button determines whether the main program window will have 
a simple line border, or a more elaborate dialog border.  The line
border requires less space on the screen, but the dialog border can
look better, especially on OS/2 Warp 4.0 or later, which gives this
border style a three-dimensional appearance.
:p.
:hp2.テーブル形式:ehp2.
このボタンがチェックされると、情報をテーブル形式で表示します。
それぞれの項目は縦横の行列に配置されます。
このとき、縦方向の大きさは可能な限り小さくなります。
このボタンがチェックされていないと、各項目は縦１列に並びます。
:p.
:hp2.時報:ehp2.
このボタンをチェックすると１時間毎に時報としてビープを 2 回鳴らします。
:p.
:hp2.Show Disk Drives:ehp2.
When this button is checked, disk drive free space is monitored, subject
to the restrictions placed on this monitoring by other options.  
This control mainly exists as a way to disable disk monitoring.
:p.
:hp2.リモートドライブを表示:ehp2.
このボタンがチェックされていると、リモートドライブ（ネットワーク上のドライブ）に
関する情報も表示するようになります。ただし全ディスク空き容量の計算には
含まれません。
:p.
:hp2.ファイルシステム名を表示:ehp2.
各ドライブの空き容量を表示する際に、ドライブのファイルシステム名も表示するか
どうかを指定します。このオプションがオンの場合、
プログラムのウィンドウが多少幅広くなってしまいます。
:p.
:hp2.ディスクラベルを表示:ehp2.
各ドライブの空き容量を表示する際に、ドライブのディスクラベルも表示するか
どうかを指定します。このオプションがオンの場合、
プログラムのウィンドウが多少幅広くなってしまいます。
:p.
:hp2.秒を表示:ehp2.
日時表示と経過時間表示で、秒単位で時間を表示するかどうかを指定する
オプションです。
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

:h3 res=IDD_CONFIG_PAGE_OPTIONS2.各種設定 (ページ 2) (詳細設定ページ)
このページでは単純なオン／オフではない項目を設定できます。
:p.
:hp2.キロバイト表示:ehp2.
このラジオボタンでは、大きいサイズをどのように表示するかを指定します。
キロバイト表示をするか、しないか、あるいは 512K を越えた時のみするか、
の中から選択できます。標準では 512K 以上の時のみです。
.br
&apos.K=1024&apos. ボタンは「キロバイト」が 1024 バイトであることを表します。
このボタンがチェックされていない時は「キロバイト」は 1000 バイトです。
.br
「メガバイト」ボタンをチェックすると、8 メガバイトを越えるサイズは
メガバイト単位で表示されます。
.br
The &apos.G&apos.igabytes button, if checked, will cause the numbers to be
displayed in gigabytes once their values exceed four gigabytes.
:p.
:hp2.モニタ優先順位:ehp2.
この項目はプログラムがシステムを調べるときのスレッド優先順位を指定します。
このスレッドはタイムクリティカルスレッドで、その中での優先順位として
0 から 31 までを指定できます（大きい方が優先順位が高い）。
通常は最高の優先順位を指定して、精度を高める方がよいでしょう。ただし
通信プログラムは高い優先順位を必要とするため、このプログラムが動いている間は
取りこぼしがあるという報告がありました。優先順位をユーザが決められるように
なったのはこのためです。
:p.
:hp2.時間間隔:ehp2.
各表示項目の値を更新する時間間隔を指定します。この値は 1 秒から 30 秒
まで 0.1 秒きざみで指定できます。表示される値は 0.1 秒単位です。
:p.
:hp2.Averaging Interval:ehp2.
This control allows you to set the interval over which averages are
computed.  It can be varied from 10 seconds to 3600 seconds (one hour).

:h3 res=IDD_CONFIG_PAGE_COLORS.配色 (Configuration Page)
この部分は各項目の通常／警告／エラーレベルを表示するのに使う色を示します。
OS/2 のカラーパレットから適当な色をドラッグすることで色を変えられます。

:h3 res=IDD_CONFIG_PAGE_ANCHOR.ウィンドウアンカー (Configuration Page)
これはウィンドウサイズが変化した時にどの点を動かない基準点とするかを
指定します。表示する項目数が変わったりフォントパレットで
新しいフォントが指定されたりした時にウィンドウサイズが変化します。

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
このダイアログボックスは現在の項目名、標準の項目名、およびその項目の
警告レベル、エラーレベルを表示します（ただし一部の項目では警告、エラーは
サポートされていません）。項目名を変更するには、現在の項目名のところに
入力して下さい。警告／エラーレベルを変えるには、直接数値を入力するか
スライダーを動かして適当な値に設定して下さい。
:p.標準値ボタンを押すと警告／エラーレベルも標準の値に戻ります。

:h1 res=IDM_RESETLOAD.負荷計のリセット（メニュー）
:i1.負荷計のリセット（メニュー）
このメニューを選ぶと、CPU の負荷計をリセットし再調整します。
今までの CPU 負荷は計算には使われません。

:note.This menu option is not available when on Warp 4.0 or better,
nor on certain higher fixpack levels of Warp 3.0, as the newer load
measuring technique makes this command irrelevant.

:h1 res=IDM_RESETAVERAGE.Reset Average Load (Menu Option)
:i1.Reset Average Load (Menu Option)
This menu option allows the user to reset the Average Load meter,
causing it to forget previous CPU activity in its calculations.
:p.
The short-cut key for this command is Alt+R.

:h1 res=IDM_RESETDRIVES.ドライブのリセット（メニュー）
:i1.ドライブのリセット（メニュー）
このメニューを選ぶと、ドライブエラーがあればそれをリセットします。
一定時間が立つと自動的にドライブエラーは解消されますが、
このメニューでは直ちに解消されます。

:h1 res=IDM_LAUNCHEMAILER.Launch E-Mail Reader (Menu Option)
:i1.Launch E-Mail Reader (Menu Option)
This option will launch the e-mail reader configured by the user.
See :link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail:elink. for more
information.

:h1 res=IDM_COPY.コピー（メニュー）
:i1.コピー（メニュー）
現在の表示内容をクリップボードにコピーするためのメニューです。
クリップボードにはテキストとビットマップの両方がコピーされます。
テキストは、現在表示されている内容がエディタなどへの張り付けに
適した書式でコピーされます。ビットマップは現在のウィンドウ状態と
まったく同じもので、グラフィックビューアなどで使うことができます。
:p.
ショートカットキーは Ctrl+Ins（Ctrl+挿入）です。

:h1 res=IDM_ABOUT.製品情報（メニュー）
:i1.製品情報（メニュー）
このメニューはこのプログラムの製品情報ダイアログを表示するためのものです。
より詳しい情報は、:link reftype=hd res=IDD_ABOUT.製品情報（ダイアログ）:elink. を
見て下さい。

:h2 res=IDD_ABOUT.製品情報（ダイアログ）
このダイアログボックスにはプログラム名、アイコン、および著作権が
含まれています。ダイアログを消すには、ENTER（改行）、スペース、ESC キーの
いずれかを押すか、「了解」ボタンをクリックして下さい。

:h1 res=IDM_EXIT.クローズ（メニュー）
:i1.クローズ（メニュー）
このメニューを選ぶとプログラムが終了します。

:h1 res=IDM_KEYS_HELP.キー・ヘルプ
:i1.キー・ヘルプ
このプログラムでは以下のキーが有効です。
:sl compact.
:li.F1 - ヘルプ
:li.F3 - :link reftype=hd res=IDM_EXIT.終了:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.詳細設定:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.コントロールを隠す:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Reset Average Load:elink.
:li.Ctrl+Ins - :link reftype=hd res=IDM_COPY.コピー:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.設定の保存場所（ダイアログ）
:i1.設定の保存場所（ダイアログ）
このダイアログはプログラムが設定ファイル（INI）を見つけられない時に現れ、
どこに設定ファイルを作るかを質問します。
:p.
存在しないディレクトリは指定できません。ディレクトリ名を入力したら、
ENTER（改行）を押すか「了解」ボタンをクリックしてプログラムを続行して下さい。
:p.
もしこのプログラムの初期化を中止したい場合は、ESC キーを押すか「取消」ボタンを
クリックして下さい。

:h1 res=60000.HRTIMER - 高精度タイマードライバ
高精度タイマードライバ HRTIMER.SYS は、IBM PC で通常使われていないタイマーチップを
使えるようにするものです。このドライバがあるとタイマー間隔は通常の 55 ミリ秒
より 64K 倍細かい 840 ナノ秒になります。
:p.
私の知る限りでは、このドライバは :hp4.CompuServe:ehp4. の 
 :hp4.Device Driver Development:ehp4. にある 
 :hp4.OS/2 Developer&apos.s Forum I (OS2DF1):ehp4. というライブラリにあります。
名前は :hp4.TIMER.ZIP:ehp4. です。
また、anonymous ftp サイトの :hp4.ftp.cdrom.com:ehp4. と
 :hp4.hobbes.nmsu.edu:ehp4. にも存在します。
ディレクトリは :hp4./pub/os2/dev16:ehp4. で、
名前は :hp4.hrtimer.zip:ehp4. です。
:p.このデバイスドライバのソースコードは IBM Personal Systems Developer magazine の
 1991 年秋号からのものです。これはフリーウェアですが、オリジナルの著作権は IBM に
属します。

:h1 res=60001.「パルス」との衝突
このシステムリソースプログラムの他、パルス、CPULOAD、CPUMETER などのプログラムは
皆コンピュータがどの程度忙しいかを計るのに、最低レベルの優先順位で行える
仕事の量を用いています。これらのプログラムが最低の優先順位でなにがしかの仕事を
行うことができたら、その仕事はシステムの空き時間に行われたことを意味し、
そうでないなら空き時間はないということで現在の負荷は 100% ということになります。
最低の優先順位で行える仕事の量が増えれば、コンピュータは負荷が少ないと
推定されます。
:p.このようなプログラムを複数走らせると、お互いでシステムの空き時間を
取り合うことになります。例えばまったく同じプログラムを２つ走らせた場合、
空き時間を折半することになるので負荷は 50% と推定されてしまいます。
異なるプログラム、例えばこのシステムリソースとパルスを同時に走らせた場合は
お互い影響が出るものの、その度合いが同じになるとは限りません。

:h1 res=60002.HRTIMER.SYS はどのように働くのでしょうか？
HRTIMER.SYS がインストールされていると、システムのオーバーヘッドも
 CPU の負荷の計算に入れられます。
:p.
CPU の負荷は最低レベルの優先順位でどれだけの仕事が行えたかで求められます。
それ以上の情報がないなら、何もしない時のシステム自身の負荷はないものとして
計算されます。HRTIMER.SYS があれば、プログラムは起動時に最高レベルの
優先順位でどれだけの仕事が行えるかを計測する調整ループを実行します。
ここで得られた値が後で比較用に使われます。

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
