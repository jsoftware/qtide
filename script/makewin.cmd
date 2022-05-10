@REM Set up Windows SDK for 64bit
@REM argument is Qt version, e.g. "5.15.2"

set PATH=C:\$GITHUB_WORKSPACE\qt\Qt\%1\win64_msvc2019_64\bin;%PATH%
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cd lib
qmake
nmake
cd ..\main
qmake
nmake
cd ..
move bin\win-x86_64\release jqt-win
powershell Compress-Archive jqt-win jqt-win.zip

RD /S /Q release
RD /S /Q bin
RD /S /Q lib\build
RD /S /Q main\build

set JQTSLIM=1
cd lib
qmake
nmake
cd ..\main
qmake
nmake
cd ..
move bin\win-x86_64\release jqt-win-slim
powershell Compress-Archive jqt-win-slim jqt-win-slim.zip
