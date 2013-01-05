rem run in Qt console
rem run qmake first and after .pro changes

cd c:\jqt\lib
call make
copy /Y bin\* ..\bin
copy /Y bin\jqt.dll "c:\program files\j64-701\bin"
dir "c:\program files\j64-701\bin\jqt*"
