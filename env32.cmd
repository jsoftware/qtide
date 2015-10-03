@REM Set up Windows SDK for 32bit xp compatible
CALL "c:\program files (x86)\microsoft visual studio 12.0\vc\bin\vcvars32.bat"
set INCLUDE=%ProgramFiles(x86)%\Microsoft SDKs\Windows\7.1A\Include;%INCLUDE%
set PATH=c:\Qtx86\qt5.4.2\5.4\msvc2013_opengl\bin;%ProgramFiles(x86)%\Microsoft SDKs\Windows\7.1A\Bin;%PATH%
set LIB=%ProgramFiles(x86)%\Microsoft SDKs\Windows\7.1A\Lib;%LIB%
set CL=/D_USING_V110_SDK71_;%CL%
set LINK=/SUBSYSTEM:CONSOLE,5.01 %LINK%
set QMAKESPEC=win32-msvc20133
@rem set JQTSLIM=JQTSLIM
