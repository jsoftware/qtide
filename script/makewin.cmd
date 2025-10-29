@REM Set up Windows SDK for 64bit
@REM argument is arch, e.g. "x64" "arm64"
@REM argument is Qt version, e.g. "5.15.2"

echo "%~1"
echo "%~2"
@rem x64 x86 arm64
IF "%~1"=="x86" GOTO L0
IF "%~1"=="x64" GOTO L0
IF "%~1" NEQ "arm64" EXIT /b 1
@rem CALL "c:\program files\microsoft visual studio\2022\community\vc\auxiliary\build\vcvarsamd64_arm64.bat"
:L0
set PATH=%GITHUB_WORKSPACE%\Qt\%1\msvc2022_64\bin;%PATH%

@rem CALL "C:\Program Files (x86)\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
echo %PATH%

IF "%~1"=="arm64" GOTO L20

@rem set NO_OPENGL=1
@rem set JQTWEBKIT=JQTWEBKIT
cd lib
IF "%~1"=="arm64" GOTO L01A
qmake
GOTO L01C
:L01A
set PATH=%GITHUB_WORKSPACE%\Qt\%1\msvc2022_arm64\bin;%PATH%
@rem qmake %GITHUB_WORKSPACE%\Qt\%1\msvc2022_arm64\bin\target_qt.conf -spec win32-arm64-msvc2022
qmake
:L01C
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
nmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
cd ..\main
IF "%~1"=="arm64" GOTO L02A
qmake
GOTO L02C
:L02A
@rem qmake -qtconf %GITHUB_WORKSPACE%\Qt\%1\msvc2022_arm64\bin\target_qt.conf -spec win32-arm64-msvc2022
qmake
:L02C
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
nmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
@rem cd ..\amalgam
@rem qmake
@rem IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
@rem nmake
@rem IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
cd ..

IF "%~1"=="arm64" GOTO L03A
move bin\win-x86_64\release jqt-win
GOTO L03C
:L03A
move bin\win-aarch64\release jqt-winarm64
:L03C
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

IF "%~1"=="arm64" GOTO L04A
cd jqt-win
powershell Compress-Archive * ..\jqt-win.zip
GOTO L04C
:L04A
cd jqt-winarm64
powershell Compress-Archive * ..\jqt-winarm64.zip
:L04C
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
cd ..

RD /S /Q release
RD /S /Q bin
RD /S /Q lib\build
RD /S /Q main\build
@rem RD /S /Q amalgam\build

:L20

set JQTSLIM=1
cd lib
IF "%~1"=="arm64" GOTO L05A
qmake
GOTO L05C
:L05A
@rem qmake -qtconf %GITHUB_WORKSPACE%\Qt\%1\msvc2022_arm64\bin\target_qt.conf -spec win32-arm64-msvc2022
qmake
:L05C
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
nmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
cd ..\main
IF "%~1"=="arm64" GOTO L06A
qmake
GOTO L06C
:L06A
@rem qmake -qtconf %GITHUB_WORKSPACE%\Qt\%1\msvc2022_arm64\bin\target_qt.conf -spec win32-arm64-msvc2022
qmake
:L06C
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
nmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
@REM cd ..\amalgam
@REM qmake
@REM IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
@REM nmake
@REM IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

cd ..
IF "%~1"=="arm64" GOTO L07A
move bin\win-x86_64\release jqt-win-slim
GOTO L07C
:L07A
move bin\win-aarch64\release jqt-winarm64-slim
:L07C
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

IF "%~1"=="arm64" GOTO L08A
cd jqt-win-slim
powershell Compress-Archive * ..\jqt-win-slim.zip
GOTO L08C
:L08A
cd jqt-winarm64-slim
powershell Compress-Archive * ..\jqt-winarm64-slim.zip
:L08C
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
cd ..

IF "%~1"=="arm64" GOTO L09A
powershell Compress-Archive Qt win-Qt.zip
GOTO L09C
:L09A
powershell Compress-Archive Qt winarm64-Qt.zip
:L09C
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

RD /S /Q release
RD /S /Q bin
RD /S /Q lib\build
RD /S /Q main\build
@REM RD /S /Q amalgam\build
