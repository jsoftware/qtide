@REM Set up Windows SDK for 64bit
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsx86_amd64.bat"
set PATH=C:\Qt\Qt6.8.3\6.8.3\msvc2022_64\bin;%PATH%
set QMAKESPEC=win32-msvc
@rem set JQTSLIM=JQTSLIM
