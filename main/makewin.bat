rem run in Qt console
rem run qmake first and after .pro changes

cd c:\jqt\main
call make
copy /Y bin\jqt.exe "c:\program files\j64-701\bin"
dir "c:\program files\j64-701\bin\jqt*"
