@REM Set up Windows SDK for 64bit
@REM argument is Qt version, e.g. "5.15.2"
@REM argument is arch, e.g. "x64"

@rem x64 x86 arm64
IF "%~2"=="x86" GOTO L0
IF "%~2"=="x64" GOTO L0
IF "%~2" NEQ "arm64" EXIT /b 1
set NO_WEBENGINE=NO_WEBENGINE
:L0

@rem set PATH=C:\$GITHUB_WORKSPACE\Qt\%1\win64_msvc2019_64\bin;%PATH%
@rem CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
echo %PATH%
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

@rem set NO_OPENGL=1
@rem set JQTWEBKIT=JQTWEBKIT
cd lib
qmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
nmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
cd ..\main
qmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
nmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
@rem cd ..\amalgam
@rem qmake
@rem IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
@rem nmake
@rem IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

cd ..
IF "%~2"=="arm64" GOTO L01A
move bin\win-x86_64\release jqt-win
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
GOTO L01C
:L01A
move bin\win-aarch64\release jqt-winarm64
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
:L01C

IF "%~2"=="arm64" GOTO L02A
cd jqt-win
powershell Compress-Archive * ..\jqt-win.zip
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
GOTO L02C
:L02A
cd jqt-winarm64
powershell Compress-Archive * ..\jqt-winarm64.zip
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
:L02C
cd ..

RD /S /Q release
RD /S /Q bin
RD /S /Q lib\build
RD /S /Q main\build
@rem RD /S /Q amalgam\build

set JQTSLIM=1
cd lib
qmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
nmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
cd ..\main
qmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
nmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
@REM cd ..\amalgam
@REM qmake
@REM IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
@REM nmake
@REM IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

cd ..
IF "%~2"=="arm64" GOTO L03A
move bin\win-x86_64\release jqt-win-slim
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
GOTO L03C
:L03A
move bin\win-aarch64\release jqt-winarm64-slim
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
:L03C

IF "%~2"=="arm64" GOTO L04A
cd jqt-win-slim
powershell Compress-Archive * ..\jqt-win-slim.zip
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
GOTO L04C
:L04A
cd jqt-winarm64-slim
powershell Compress-Archive * ..\jqt-winarm64-slim.zip
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
:L04C
cd ..

IF "%~2"=="arm64" GOTO L05A
powershell Compress-Archive Qt win-Qt.zip
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
GOTO L05C
:L05A
powershell Compress-Archive Qt winarm64-Qt.zip
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
:L05C

RD /S /Q release
RD /S /Q bin
RD /S /Q lib\build
RD /S /Q main\build
@REM RD /S /Q amalgam\build
