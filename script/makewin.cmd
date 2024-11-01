@REM Set up Windows SDK for 64bit
@REM argument is Qt version, e.g. "5.15.2"

@rem set PATH=C:\$GITHUB_WORKSPACE\Qt\%1\win64_msvc2019_64\bin;%PATH%
@rem CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
echo %PATH%
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

cd lib
qmake
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
nmake NO_OPENGL=1
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
move bin\win-x86_64\release jqt-win
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

cd jqt-win
powershell Compress-Archive * ..\jqt-win.zip
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
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
move bin\win-x86_64\release jqt-win-slim
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

cd jqt-win-slim
powershell Compress-Archive * ..\jqt-win-slim.zip
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%
cd ..

powershell Compress-Archive Qt win-Qt.zip
IF %ERRORLEVEL% NEQ 0 EXIT /b %ERRORLEVEL%

RD /S /Q release
RD /S /Q bin
RD /S /Q lib\build
RD /S /Q main\build
@REM RD /S /Q amalgam\build
