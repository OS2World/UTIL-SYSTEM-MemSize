@Echo Off
Verify On
Md EAS
EAUtil /s BPortugu.cmd
EAUtil /s Catala.cmd
EAUtil /s Chinese.cmd
EAUtil /s Dansk.cmd
EAUtil /s Deutsch.cmd
EAUtil /s English.cmd
EAUtil /s Espanol.cmd
EAUtil /s Francais.cmd
EAUtil /s Italiano.cmd
EAUtil /s Japanese.cmd
EAUtil /s Magyar.cmd
EAUtil /s Norsk.cmd
EAUtil /s Suomi.cmd
Zap EAS

If *%TMP%==* TMP=e:
If *%1==*ALL Goto All

:UPDATE
zip -fo BPortugu
zip -fo Catala
zip -fo Chinese
zip -fo Dansk
zip -fo Deutsch
zip -fo English
zip -fo Espanol
zip -fo Francais
zip -fo Italiano
zip -fo Japanese
zip -fo Magyar
zip -fo Norsk
zip -fo Suomi
zip -fo Source * -x *.zip
zip -o Source *.zip
zip -b e: -fo a:MEMSIZ32 * -x source.zip
zip -b e: -o a:MEMSIZ32 source.zip
Goto EXIT

:ALL
Erase *.zip
Zip -9o BPortugu -@ < BPortugu.PKG
Zip -9o Catala   -@ < Catala.PKG
Zip -9o Chinese  -@ < Chinese.PKG
Zip -9o Dansk    -@ < Dansk.PKG
Zip -9o Deutsch  -@ < Deutsch.PKG
Zip -9o English  -@ < English.PKG
Zip -9o Espanol  -@ < Espanol.PKG
Zip -9o Francais -@ < Francais.PKG
Zip -9o Italiano -@ < Italiano.PKG
Zip -9o Japanese -@ < Japanese.PKG
Zip -9o Magyar   -@ < Magyar.PKG
Zip -9o Norsk    -@ < Norsk.PKG
Zip -9o Suomi    -@ < Suomi.PKG
Zip -9o Source   -@ < Source.PKG
Zip -9or MemSiz32 -@ < MemSiz32.PKG
Copy MemSiz32.zip a: /v 1>NUL
Erase MemSiz32.zip 1>NUL 2>NUL
Goto EXIT

:ERROR
Echo ERROR: Something went wrong!
Echo 

:EXIT
