REM build both jqt.exe and jqt.dll
REM must run either env64.cmd before this
cd lib
qmake
nmake
cd ..\main
qmake
nmake
cd ..
