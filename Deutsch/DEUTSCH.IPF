#include "memsize.h"
:userdoc.
:title.&apos.Systemressourcen&apos. Hilfe
:body.

:h1 res=ID_MAIN.Einfhrung
:i1.Einfhrung
:artwork name='..\MEMSIZE2.bmp' align=center.
:p.
Dieses Programm zeigt verschiedene Informationen im Zusammenhang mit
Systemressourcen.  Die Anzeige wird einmal pro Sekunde angepaát, sofern
das Programm hierfr CPU-Zeit erh„lt.  Die angezeigten Informationen sind:
:p.
:hp2.Datum/Zeit:ehp2. - Laufendes Datum und Uhrzeit in dem Format, wie
es die L„nderspezifikation fr Ihr System in dem COUNTRY-Eintrag Ihrer
CONFIG.SYS-Datei vorschreibt.
:p.
:hp2.Abgelaufene Zeit:ehp2. - Die Zeit, die seit dem letzten Neustart des
Computers abgelaufen ist.
:p.
:hp2.Anstehende Nachrichten:ehp2. - Die Anzahl der Nachrichten die zum download anstehen.
:p.
:hp2.Freier physischer Speicher:ehp2. - Der verfgbare Systemspeicher nach Angabe
der Funktion :hp1.DosMemAvail:ehp1..
:note.Diese Funktion geh”rt zu dem alten 16-bit System und meldet nur 
physischen Speicher unterhalb von 16Mb.  Auáerdem wurde sie kurz vor dem 
Erscheinen von OS/2 2.0 so ge„ndert, daá sie niemals weniger als 512K meldet.
:note.Falls Sie das IBM System Analyse 
Programm :link reftype=hd res=60003.Theseus/2:elink. auf ihrem System 
installiert haben, wird diese Angabe vollkommen korrekt sein,
allerdings erfolgt eine Autoaktualisierung nur alle zehn Sekunden.
:p.
:hp2.Freier virtueller Speicher:ehp2. - Der Betrag des freien
virtuellen Speichers nach Angabe der Funktion
:hp1.DosQuerySysInfo:ehp1.. Dies ist der Betrag des freien physischen
Speichers zuzglich dem Betrag des freien Platzes in der
augenblicklichen Auslagerungsdatei und dem Betrag, um den die
Auslagerungsdatei noch wachsen k”nnte, abzglich dem Betrag des
reservierten Platzes auf dem Auslagerungslaufwerk.
:p.
:hp2.Swap-Gr”áe:ehp2. - Die augenblickliche Gr”áe der
Auslagerungsdatei des virtuellen Speichers (SWAPPER.DAT). Um diese
Datei zu finden, wird die Datei CONFIG.SYS nach
dem SWAPPATH-Eintrag durchsucht.  Dieser Eintrag liefert den vollen
Namen des Verzeichnisses der Auslagerungsdatei und zeigt den minimalen
Speicherplatz an, der auf dem Laufwerk der Auslagerungsdatei frei
bleiben muá.
:p.
:hp2.Freier Swap-Platz:ehp2. -  Der Betrag des freien
Plattenplatz auf dem logischen Laufwerk der Auslagerungsdatei
abzglich des minimalen freibleibenden Platzes. Diese Zahl
zeigt an, wie weit die Auslagerungsdatei im Bedarfsfall ausgedehnt
werden k”nnte.
:p.
:hp2.Unbenutzter Auslagerungsplatz:ehp2. - Der Betrag freien Platzes
innerhalb der augenblicklich angelegten Auslagerungsdatei.
Wenn dieser Betrag auf Null geht, muá die Auslagerungsdatei
vergr”áert werden, normalerweise um jeweils ein Megabyte.
:note.Falls Sie das IBM System Analyse 
Programm :link reftype=hd res=60003.Theseus/2:elink. auf ihrem System 
installiert haben, wird diese Angabe vollkommen korrekt sein,
allerdings erfolgt eine Autoaktualisierung nur alle zehn Sekunden.
:p.
:hp2.Spool-Gr”áe:ehp2. - Der Betrag an Plattenplatz, der von
Spool-Dateien verbraucht wird.
:p.
:hp2.CPU-Last (%):ehp2. - Die ungef„hre Prozentzahl der verfgbaren
CPU-Leistung, die in diesem Moment gebraucht wird.  Sie wird ber die
vergangene Sekunde gemittelt.
:note.Bei OS/2 Versionen frher als Release 4.0, Diese Funktion und  PULSE behindern sich gegenseitig.
:link reftype=hd res=60001.Warum?:elink.
:note.Bei OS/2 Versionen frher als Release 4.0, Wenn Sie den Treiber :link reftype=hd res=60000.HRTIMER.SYS:elink.
installiert haben, ist diese Zahl genauer.
:link reftype=hd res=60002.Warum?:elink.
:p.
:hp2.Durchschnittliche Last (%):ehp2. - Ann„hernde Prozentangabe der CPU Auslastung
in der letzten Zeit.  Die Mittelungszeit in Sekunden kann durch den Benutzer 
konfiguriert werden (siehe auch :link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Konfiguration:elink. Dialog).
:p.
:hp2.Batterie:ehp2. - Die augenblicklich freie Batteriekapazit„t bzw. deren Status
(Laden, AC) wird angezeigt.
:p.
:hp2.Anzahl aktiver Tasks:ehp2. - Die Anzahl der Eintr„ge in der
System-Fensterliste; das ist die Liste, die angezeigt wird,
wenn Sie CTRL+ESC drcken.
:note.Es werden nicht unbedingt alle Eintr„ge in der System-Taskliste
in der Fensterliste angezeigt.  Einige k”nnen als `nicht anzuzeigen&apos.
markiert sein.
:p.
:hp2.Prozeá-Anzahl:ehp2. - Die Zahl der augenblicklich im System
geladenen Prozesse.
:p.
:hp2.Thread-Anzahl:ehp2. - Die Zahl der augenblicklich im System
ablaufenden Threads.  Zu jedem Prozeá gibt es mindestens einen Thread.
:p.
:hp2.Insgesamt freier Plattenplatz:ehp2. - Die Summe des freien Platzes auf
allen Festplatten.
:p.
:hp2.Auf Laufwerk X frei:ehp2. - Der Betrag freien Speichers auf Laufwerk X.
:p.
Wie Sie schon gesehen haben, ist die Hilfeeinrichtung aktiv und die
existierenden Kommandos k”nnen ber das System-Men des Fensters
angesprochen werden.  Die folgenden Kommandos sind verfgbar&colon.
:sl compact.
:li.:link reftype=hd res=IDM_RESET_DEFAULTS.Standardwerte:elink.
:li.:link reftype=hd res=IDM_HIDE_CONTROLS.Ohne Rahmen:elink.
:li.:link reftype=hd res=IDM_CONFIGURE.Konfiguration...:elink.
:li.:link reftype=hd res=IDM_RESETLOAD.Lastmessung zurcksetzen:elink.
:li.:link reftype=hd res=IDM_RESETAVERAGE.Durchschnittliche Last zurcksetzen:elink.
:li.:link reftype=hd res=IDM_RESETDRIVES.Laufwerke zurcksetzen:elink.
:li.:link reftype=hd res=IDM_LAUNCHEMAILER.E-Mail Reader starten:elink.
:li.:link reftype=hd res=IDM_COPY.Kopieren:elink.
:li.:link reftype=hd res=IDM_ABOUT.Produktinformation:elink.
:li.:link reftype=hd res=IDM_EXIT.Schlieáen:elink.
:esl.:p.
Auáer den schon beschriebenen Vorrichtungen akzeptiert das Programm
auch Kommandos von den OS/2 2.0-Programmen zur Farb- und Schriftartpalette
und dient als DDE-Server fr alle momentan angezeigten Gr”áen.
Der DDE-Applikationsname ist MEMSIZE und die Namen der Details sind identisch
mit den Standardnamen der angezeigten Details (also z.B. Freier physischer
Speicher).

:h1 res=IDM_RESET_DEFAULTS.Standardwerte (Men-Option)
:i1.Standardwerte (Men-Option)
Diese Men-Option setzt die Schriftart- und Farbattribute des
Programms auf ihre Standardwerte zurck.

:h1 res=IDM_HIDE_CONTROLS.Ohne Rahmen (Men-Option)
:i1.Ohne Rahmen (Men-Option)
Diese Men-Option verbirgt den Rahmen des Programmfensters, d.h. also
das System-Men, den Titelbalken und den Knopf fr Symbolgr”áe.  Mit
einem Doppelklick auf irgendeinen Mausknopf kann diese Option
umgeschaltet werden.  Da ich es fr ntzlich hielt, das Fenster
bewegen zu k”nnen, auch wenn der Rahmen verborgen ist, wurde das
Fenster so eingerichtet, daá es mit jedem Mausknopf verschoben werden
kann.
:p.
:note.Dieses Kommando kann ber ALT+H direkt ausgefhrt werden.

:h1 res=IDM_CONFIGURE.Konfiguration (Men-Option)
:i1.Konfiguration (Men-Option)
Diese Men-Option fhrt zur Anzeige des Konfigurationsdialogs fr das Programm.
Fr weitere Informationen, siehe :link reftype=hd res=IDD_CONFIGURE.Konfiguration (Dialog):elink..
:note.Dieses Kommando kann ber Alt+C direkt ausgefhrt werden.

:h2 res=IDD_CONFIGURE.Konfigurieren (Dialog)
Dieser Notizbuch-Dialog erlaubt Ihnen, alle konfigurierbaren Aspekte der Operation
des Programms zu ver„ndern. Die Konfigurationsoptionen sind wie folgt in Kategorien
eingeteilt&colon.
:sl compact.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ITEMS.Details:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS1.Optionen (Seite 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_OPTIONS2.Optionen (Seite 2):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_COLORS.Farben:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_ANCHOR.Anker:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_DATEFMT.Datums Format:elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail (Seite 1):elink.
:li.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.E-Mail (Seite 2):elink.
:esl.:p.
Alle nderungen werden sofort wirksam.

:h3 res=IDD_CONFIG_PAGE_ITEMS.Details (Konfigurationsseite)
Diese Seite zeigt eine Liste anzeigbarer Details an.  
Sie k”nnen beliebige Details aus der Liste der verfgbaren Anzeigedaten
ausw„hlen.  Klicken Sie einfach auf einen Eintrag, um ihn hervorzuheben,
ein zweites Mal, um die Hervorhebung aufzuheben.  Hervorgehobene Details
werken berwacht und angezeigt.
Um die Standardbeschreibung eines Items&apos.s zu „ndern, oder um die Warn- und/oder 
Fehlerschwelle anzupassen, entweder einen doppelten Mausclick auf dem Eintrag ausfhren, 
oder einen einfachen rechten Mausclick. In beiden F„llen wird der :link reftype=hd res=IDD_LABEL.Item Attribute:elink. Dialog angezeigt.

:h3 res=IDD_CONFIG_PAGE_OPTIONS1.Optionen (Seite 1) (Konfigurationsseite)
Diese Seite zeigt eine Anzahl von Programmoptionen, die aktiviert oder deaktiviert 
werden k”nnen.
:p.
:hp2.Ohne Rahmen:ehp2.
Wenn die Uhr nicht auf Symbolgr”áe reduziert ist, kann sie mit oder ohne
Titelbalken und die Kn”pfe fr Kontextmen, Symbolgr”áe und maximale Gr”áe
angezeigt werden.  Wenn Sie diese Kn”pfe verbergen, kann das Fenster trotzdem
noch verschoben werden, indem sein mittlerer Bereich verzogen wird.
:p.
:hp2.Immer oberstes Fenster:ehp2.
Wenn dieser Knopf markiert ist, wird das Programmfenster immer auf die Spitze
des Fensterstapels aufsteigen, so daá es st„ndig sichtbar bleibt.
:p.
:hp2.Animation:ehp2.
Dieser Knopf aktiviert die Animation beim ™ffnen &amp. Schlieáen des
Fensters.  Wenn die Animation aktiv ist, ”ffnet und schlieát sich das
Programmfenster in derselben Art und Weise wie Ordner-Objekte, d.h. also mit
Ger„usch und Bewegung.  Animation erfolgt jedoch wie bei den Ordner-Objekten
nur, wenn sie auf System-Ebene eingeschaltet ist.
:p.
:hp2.Dialog Rahmen:ehp2.
Dieser Knopf bestimmt, ob das Programmhauptfenster einen einfachen Rahmen 
hat, oder einen aufwendigeren Dialograhmen.  Der einfache Rahmen 
ben”tigt weniger Platz auf dem Bildschirm, aber der Dialograhmen, 
der diesem Rahmenstil ein dreidimensionales Aussehen verleiht, kann 
besser aussehen, speziell unter OS/2 Warp 4.0 oder sp„ter.
:p.
:hp2.Tabellenformat:ehp2.
Wenn dieser Knopf markiert ist, stellt sich das Fenster im
Tabellenformat dar, wobei die angezeigten Details in Zeilen und
Spalten angeordnet sind.  Es optimiert sich selbst, so daá m”glichst
wenig vertikaler Raum ben”tigt wird.  Wenn der Knopf nicht markiert
ist, stellt sich das Fenster mit einer einzigen Spalte angezeigter
Details an.
:p.
:hp2.Stundenschlag:ehp2.
Dieser Knopf aktiviert einen Stundenschlag.  Wenn die Option aktiviert ist, piepst
das Programm zu jeder vollen Stunde zweimal.
:p.
:hp2.Laufwerke anzeigen:ehp2.
Wenn dieser Knopf markiert ist, wird der freie Platz auf Laufwerken - in 
Abh„ngigkeit von den Einschr„nkungen durch andere Optionen - angezeigt.  
Diese Option dient haupts„chlich dazu, die Laufwerksberwachung abzustellen.
:p.
:hp2.Netzwerklaufwerke anzeigen:ehp2.
Wenn dieser Knopf markiert ist, werden Netzwerklaufwerke, die momentan
mit dem System verbunden sind, in der gleichen Weise berwacht wie
Festplattenlaufwerke mit der einzigen Ausnahme, daá sie nicht in die
Berechnung des insgesamt freien Speicherplatzes eingehen.
:p.
:hp2.Dateisystemtypen anzeigen:ehp2.
Diese Option bewirkt, daá die Eintr„ge fr den freien Platz der einzelnen
Laufwerke auch den Dateisystemtyp auflisten.  Eine Nebenwirkung ist, daá
das Programmfenster breiter wird.
:p.
:hp2.Datentr„gernamen anzeigen:ehp2.
Diese Option bewirkt, daá die Eintr„ge fr den freien Plattenplatz den
jeweiligen Datentr„gernamen anzeigen. Eine Nebenwirkung ist, daá
das Programmfenster breiter wird.
:p.
:hp2.Sekunden anzeigen:ehp2.
Dieser Knopf sorgt dafr, daá die Anzeige der Uhr und der
abgelaufenen Zeit die Sekunden enth„lt.
:p.
:hp2.Theseus/2 benutzen:ehp2.
Dieser Knopf veranlaát das Programm dazu das IBM Werkzeugprogramm Theseus/2
dort zu verwenden, falls es vorhanden ist, wo es pr„zisere Informationen liefert 
als andere M”glichkeiten.  Aufgrund des groáen Overheads bei der Benutzung 
Theseus/2 findet eine Abfrage nur alle 10 Sekunden statt.  Derzeit bietet 
dieses Feature eine bessere Messung des freien Arbeitsspeichers und des unbenutzen 
Swap Platzes.
:p.
:hp2.Uhr zentrieren:ehp2.
Dieser Knopf zentriert die Zeitanzeige in seiner Zelle, anstatt sie linksbndig 
anzuzeigen.
:p.
:h3 res=IDD_CONFIG_PAGE_OPTIONS2.Optionen (Seite 2) (Konfigurationsseite)
Diese Seite zeigt weitere Programmoptionen, die komplizierter sind und daher mehr Aufmerksamkeit ben”tigen als diese nur ein- und
auszuschalten.
:p.
:hp2.&apos.K&apos.ilobytes anzeigen:ehp2.
Diese Gruppe von `Radio&apos.-Kn”pfen erlaubt Ihnen auszuw„hlen, ob groáe
Plattenkapazit„ten immer, nie oder nur oberhalb von 512K als Kilobyte
angezeigt werden.  Standard ist, Gr”áen oberhalb von 512K als Kilobyte
anzuzeigen.  
.br
Der &apos.K=1024&apos.-Knopf, sorgt bei Selektion dafr, daá die Zahlen in `echten&apos. K (1024)
angezeigt werden.  Wenn er deselektiert ist, werden die Zahlen in dezimalen K (1000)
angezeigt..
.br
Der &apos.M&apos.egabytesknopf, sorgt bei Selektion dafr, daá Zahlen oberhalb von acht Megabytes
in Megabytes angezeigt werden.
.br
The &apos.G&apos.igabytes button, if checked, will cause the numbers to be
displayed in gigabytes once their values exceed four gigabytes.
:p.
:hp2.Monitor-Priorit„t:ehp2.
Diese Steuerm”glichkeit erlaubt Ihnen, die absolute Priorit„t des
Monitor-Threads des Programms innerhalb der zeitkritischen Priorit„tsklasse
zu ver„ndern.  Sie k”nnen die Priorit„t zwischen 0 und 31 ver„ndern.
Priorit„t 31 ist die h”chstm”gliche Priorit„t im System, abgesehen von
Hardware-Unterbrechungen.  Priorit„t 0 ist einfach die niedrigste Priorit„t
innerhalb der zeitkritischen Klasse.  Normalerweise sollten Sie die maximale
Priorit„t w„hlen, um eine gr”átm”gliche Genauigkeit der Systemberwachung zu
erhalten.  Allerdings haben sich manche beschwert, daá ihre
Kommunikationsprogramme gelegentlich Daten verlieren, wenn dieses Programm
l„uft.  Aus diesem Grund ist die Priorit„t nun variabel.
:p.
:hp2.Wecker-Intervall:ehp2.
Diese Option erlaubt Ihnen, das Intervall zwischen Anpassungen des
Monitor-Fensters festzulegen.  Es kann zwischen 1.0 und 30.0 Sekunden in
Abst„nden von 1/10 Sekunde ver„ndert werden.  Die angezeigte Zahl bezieht
sich auf Zehntelsekunden.
:p.
:hp2.Mittelwert Interval:ehp2.
Diese Option erlaubt Ihnen das Interval in welchem Mittelwerte errechnet 
werden anzugeben.  Die Variationsbreite ist zwischen 10 Sekunden bis 3600 
Sekunden (eine Stunde).

:h3 res=IDD_CONFIG_PAGE_COLORS.Farben (Konfigurationsseite)
Dieser Bereich zeigt das Sortiment der drei Farben, die mit der Normal-, Warn- und
Fehlerschwelle jedes Details verbunden sind, soweit diese Vorrichtung untersttzt wird.  
Sie k”nnen die Farben ver„ndern, indem Sie von der Farbpalette des Systems andere Farben
darberziehen.

:h3 res=IDD_CONFIG_PAGE_ANCHOR.Anker (Konfigurationsseite)
Diese Steuerm”glichkeit erlaubt Ihnen, die Ecke des Monitor-Fensters zu
bestimmen, welche fixiert bleibt, wenn das Fenster seine Gr”áe „ndert.  Das
Monitor-Fenster kann seine Gr”áe „ndern, wenn Eintr„ge zur Anzeige-Liste
hinzugefgt oder davon weggenommen werden und wenn die Schriftart der Anzeige
mit Hilfe der Schriftarten-Palette ge„ndert wird.

:h3 res=IDD_CONFIG_PAGE_DATEFMT.Datums Format (Konfigurationsseite)
Dise Seite gestattet es die Anzeige des Datumsformats zu konfigurieren.  
Ein beliebiger Text und spezielle Funktionscodes k”nnen eingegeben werden.  
Die komplette Liste der gltigen Funktionscodes findet sich in einer Liste 
am Ende der Seite.  Durch einen Doppel-Click auf ein beliebiges Item aus 
der Liste wird dieses augenblicklich in den derzeitigen Text eingefgt.  
Ein Beispieltext, mit den Funktionscodes, ist unter dem Texteingabefeld 
angefhrt.

:h3 res=IDD_CONFIG_PAGE_POP31.E-Mail (Konfigurationsseite)
Diese Seite gestattet es die Parameter einzustellen mit denen die Anzeige 
anstehender Nachrichten ihre Informationen erh„lt. 
Sie mssen den POP3 Server Namen angeben, den Benutzernamen und das 
Benutzerpasswort.  Optional k”nnen sie den POP3 Port angeben falls ihr 
Server eine vom Standard abweichende Einstellung hat.  
Zus„tzlich k”nnen Sie die Verwendung einer spezifischen IP Adresse erzwingen.
:p.Die Option APOP zu verwenden gestattet es ein erweitertes Password 
šberprfungsprotokoll zu verwenden, welches Ihre ID und Ihr Passwort verschleiert.
:p.Once you have entered the server name or address, the user name and the
password, you can click on the <Validate> button to test.  You will be told
whether or not the e-mail account could be checked, and either how many
messages are waiting, or what went wrong.  To save the new parameters,
click on the <Accept> button.  To reset any changes made since the last
touch of the <Accept> button, click on <Reset>.
:p.:link reftype=hd res=IDD_CONFIG_PAGE_POP32.N„chste Seite...:elink.

:h3 res=IDD_CONFIG_PAGE_POP32.E-Mail (Konfigurationsseite)
Diese Seite gestattet es die Parameter einzustellen mit denen die Anzeige 
anstehender Nachrichten auf neue Nachrichten reagiert.
:p.Das Abfrageintervall Feld gestattet es die Anzahl der Sekunden einzustellen 
in denen neue Nachrichten abgefragt werden.
:p.Das E-Mail Reader Feld dient dazu den Kommandozeilen-Befehl einzugeben mit dem 
das gewnschte E-Mail Programm gestartet wird.
:p.Die Alarm Checkbox bestimmt, ob ein h”rbarer Alarm 
ausgel”st wird wenn neue Nachrichte eintreffen oder ob nicht.
:p.Das Mail Alarm Sound Field bestimmt die .WAV Datei die beim eintreffen neuer 
Nachrichten abgespielt wird.  Falls keine WAV Datei angegeben ist, der Alarm 
jedoch aktiviert wurde, wird eine ansteigende Tonsequenz vom PC-Lautsprecher 
abgespielt.

:h3 res=IDD_LABEL.Detail-Attribute  (Dialog)
Dieses Dialogfenster zeigt Ihnen die augenblickliche Bezeichnung fr ein Anzeigedetail 
an und deren Standardwert, auáerdem die augenblicklichen Werte fr die Warn-
und Fehlerschwelle, wenn diese Vorrichtung fr das augenblickliche Detail eingeschaltet
ist.  Um den Text zu „ndern,  schreiben Sie einfach eine neue Bezeichnung ber die
augenblickliche.  Um die Werte fr Fehler- oder Warnschwelle zu „ndern, schreiben Sie
entweder neue Werte in die Eingabefelder oder bewegen den zugeordneten Schieber.
:p.Um alle Werte auf ihre Standardwerte zurckzusetzen, klicken Sie auf den 
Standard-Knopf.

:h1 res=IDM_RESETLOAD.Lastmessung zurcksetzen (Men-Option)
:i1.Lastmessung zurcksetzen (Men-Option)
Diese Men-Option erlaubt es dem Benutzer, die Messung der CPU-Last
zu Eichzwecken rckzusetzen.  Vorherige CPU-Aktivit„t wird dann fr
die Berechnungen ignoriert.

:note.Diese Men-Option ist unter Warp 4.0 oder h”her - sowie bei h”heren 
Fixpack-Level unter Warp 3.0 - nicht verfgbar, da die neueren Techniken 
zur Messung der Last diesen Befehl berflssig machen.

:h1 res=IDM_RESETAVERAGE.Durchschnittliche Last zurcksetzen (Men Option)
:i1.Durchschnittliche Last zurcksetzen (Men Option)
Diese Men Option gestattet es dem Benutzer die Anzeige der durschnittlichen 
Last zurckzusetzen, damit werden vorhergehende CPU Activit„ten aus den 
Berechnungen ausgeschlossen.
:p.
Die Direktaufruf-Taste fr dieses Kommando ist Alt+R.

:h1 res=IDM_RESETDRIVES.Laufwerke zurcksetzen (Men-Option)
:i1.Laufwerke zurcksetzen (Men-Option)
Diese Option erlaubt es dem Benutzer, alle m”glichen Laufwerksfehler,
die sich ereignet haben k”nnen, zurckzusetzen.  Dies wird im Lauf der
Zeit sowieso automatisch erfolgen, aber mit diesem Kommando kann der
Benutzer die Fehler unmittelbar zurcksetzen.

:h1 res=IDM_LAUNCHEMAILER.E-Mail Reader starten (Men-Option)
:i1.E-Mail Reader starten (Men-Option)
Diese Option started den E-mail Reader der durch den Benutzer definiert wurde.
See :link reftype=hd res=IDD_CONFIG_PAGE_POP31.E-Mail:elink. Fr mehr
Informationen.

:h1 res=IDM_COPY.Kopieren (Men-Option)
:i1.Kopieren (Men-Option)
Diese Men-Option kopiert das laufende Anzeigefenster in die
System-Zwischenablage, und zwar im Text- und Bitmap-Format. Die
Textversion zeigt alle augenblicklich angezeigten Details in einer
Form, die geeignet ist fr das Einfgen in einen Texteditor.  Die
Bitmap-Version zeigt eine exakte Kopie des laufenden Fensters und kann
von graphischen Editoren und Bildbetrachtern verwendet werden.
:p.
Die Direktaufruf-Taste fr dieses Kommando ist Strg+Einfg (Ctrl+Ins).

:h1 res=IDM_ABOUT.Produktinformation (Men-Option)
:i1.Produktinformation (Men-Option)
Selektion dieser Option bewirkt die Anzeige der Produktinformation.
Fr weitere Informationen, siehe :link reftype=hd res=IDD_ABOUT.Produktinformation (Dialog):elink..

:h2 res=IDD_ABOUT.Produktinformation (Dialog)
Das Dialogfenster zeigt den Namen des Programms, das zugeh”rige Symbol
und die Copyright-Information an.  Das Fenster kann verlassen werden,
indem man die Eingabetaste, die Leertaste oder ESC drckt oder auf den
OK-Knopf klickt.

:h1 res=IDM_EXIT.Schlieáen (Men-Option)
:i1.Schlieáen (Men-Option)
Bei Selektion dieser Men-Option wird das Programm beendet.

:h1 res=IDM_KEYS_HELP.Hilfe fr Tasten
:i1.Hilfe fr Tasten
Die folgenden Funktionstasten wurden fr dieses Programm definiert&colon.
:sl compact.
:li.F1 - Hilfe
:li.F3 - :link reftype=hd res=IDM_EXIT.Ende:elink.
:li.Alt+C - :link reftype=hd res=IDD_CONFIGURE.Konfiguration:elink.
:li.Alt+H - :link reftype=hd res=IDM_HIDE_CONTROLS.Ohne Rahmen:elink.
:li.Alt+R - :link reftype=hd res=IDM_RESETAVERAGE.Durchschnittliche Last zurcksetzen:elink.
:li.Strg+Einfg (Ctrl+Ins) - :link reftype=hd res=IDM_COPY.Kopieren:elink.
:esl.:p.

:h1 res=IDD_PROFILE_PATH.Initialisierungspfad setzen (Dialog)
:i1.Initialisierungspfad setzen (Dialog)
Dieser Dialog wird angezeigt, wenn das Programm seine
Initialisierungsdatei (INI) nicht finden kann.  Es wird gefragt, wo
die Datei ist oder wo sie angelegt werden soll.
:p.
Nur ein gltiger
Verzeichnisname wird akzeptiert.  Wenn Sie den Namen eingegeben haben,
drcken Sie die ENTER-Tast oder klicken Sie auf den OK-Knopf, damit
das Programm fortf„hrt.
:p.
Wenn Sie die Initialisierungssequenz
abbrechen wollen, drcken Sie die ESC-Taste oder klicken auf den
`Abbruch&apos.-Knopf.

:h1 res=60000.HRTIMER - Treiber fr hochaufl”sende Zeitmessung
Der Treiber fr hochaufl”sende Zeitmessung HRTIMER.SYS erlaubt einem Programm
den Zugriff auf einen normalerweise ungenutzten Timer Chip in den IBM PCs. 
Er hat eine Aufl”sung von 840 Nanosekunden, 64K-mal besser als die normale
Aufl”sung von 55 Millisekunden.
:p.
Die einzigen mir momentan bekannten Stellen, die dieses Programm
verfgbar halten, sind 
:hp4.CompuServe:ehp4., 
wo das Programm in der 
:hp4.Device Driver Development:ehp4.
library des 
:hp4.OS/2 Developer&apos.s Forum I (OS2DF1):ehp4., 
unter dem Namen
:hp4.TIMER.ZIP:ehp4.,
liegt und die anonymous ftp sites 
:hp4.ftp.cdrom.com:ehp4. 
und
:hp4.hobbes.nmsu.edu:ehp4..
Die Datei liegt dort im Verzeichnis
:hp4./pub/os2/dev16:ehp4.,
als
:hp4.hrtimer.zip:ehp4..
:p.Der Quellcode fr diesen Einheitentreiber stammt aus der Herbstausgabe 1991
des IBM Personal Systems Developer Magazine.  Er ist Freeware, aber das
ursprngliche Urheberrecht geh”rt IBM.

:h1 res=60001.Worin besteht der Konflikt mit PULSE?
Das vorliegende Programm versucht genau wie PULSE, CPULOAD, CPUMETER und
„hnliche Programme, den Besch„ftigungsgrad des Rechners herauszufinden, indem
es prft, wieviel Arbeit es auf der niedrigsten Priorit„tsstufe leisten kann.
Die Idee ist, daá jedes dieser Programme, wenn es berhaupt irgendetwas mit
der niedrigsten Priorit„t tun kann, offensichtlich Leerlaufzeit verwendet.
Wenn es keine Leerlaufzeit gibt, ist der Rechner zu 100% besch„ftigt.  Je
mehr das Programm mit Leerlaufpriorit„t tun kann, desto weniger kann der
Rechner besch„ftigt sein.
:p.Das Problem dabei, mehrere dieser Programme laufen zu lassen, besteht
darin, daá sie um diese Leerlaufzeit konkurrieren.  Wenn Sie zwei
Kopien eines dieser Programme in einem Rechner laufen lassen, der
sonst nichts tut, zeigen sie theoretisch beide eine CPU-Last von 50%.
Wenn Sie zwei verschiedene Programme laufen lassen, also etwa das
vorliegende und PULSE, sind auch beide betroffen, aber normalerweise
zu unterschiedlichen Anteilen.

:h1 res=60002.Inwiefern hilft HRTIMER.SYS?
Wenn HRTIMER.SYS installiert ist, ist der System-Verwaltungsaufwand in der
CPU-Last enthalten.
:p.Die CPU-Last wird bestimmt, indem man vergleicht, wieviel Arbeit zur
Leerlaufpriorit„t geleistet werden konnte und wieviel das Programm h”chstens
bis zu diesem Augenblick tun konnte.  Ohne weitere Informationen wrde die
normale Hintergrund-Aktivit„t des Systems als 0% Last erscheinen. Wenn
HRTIMER.SYS installiert ist, fhrt das vorliegende Programm beim Start eine
Eich-Schleife aus, in der es bestimmt, wieviel Arbeit mit der h”chsten
verfgbaren Priorit„t geleistet werden kann.  Dieser Betrag wird dann zum
Vergleich verwendet.

:h1 res=60003.Was ist Theseus/2?
Theseus/2 ist ein von IBM entwickeltes System Memory Analyse Werkzeug als Teil des 
ehemals verkauften Produkts :hp5.System Performance Monitor/2:ehp5..
M”glicherweise wird dieses nicht mehr verkauft und jemand bei IBM bndelte
Theseus/2 als freeware distribution Anfang 1997.  Erh„ltlich als&colon.
:p.:lm margin=6.
ftp&colon.//hobbes.nmsu.edu/pub/os2/util/system/t2spm.zip
:lm margin=1.:p.
Eine neuere Version (bezeichnet als Theseus/3) findet sich unter
:p.:lm margin=6.
ftp&colon.//ftp.boulder.ibm.com/ps/products/os2/fixes/v4warp/theseus3/thes3000.exe
:lm margin=1.:p.
Theseus/2 hat zwei Vorzge die mein Programm wahrscheinlich niemals 
haben wird&colon. einen tiefen Einblick in die Struktur des OS/2 Kernel und
einen speziellen Einheitentreiber der es gestattet den Kernel direkt zu 
inspizieren. Diese Vorzge haben jedoch ihren Preis&colon. Das Programm ben”tigt 
eine relativ lange Zeit um seine Analyse durchzufhren.  Aus diesem Grund 
benutzt MemSize es nicht ”fters als alle zehn Sekunden, um das System nicht zu 
berlasten.

:euserdoc.

