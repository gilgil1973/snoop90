@echo off
rem ***************************************************************************
rem QT_PATH
rem ***************************************************************************
set QT_PATH=%1
if "%1" == "" goto usage

rem ***************************************************************************
rem header files
rem ***************************************************************************
mkdir %QT_PATH%\include\Snoop

mkdir %QT_PATH%\include\Snoop\Capture
copy .\Snoop\Capture\*.* %QT_PATH%\include\Snoop\Capture\*.*

mkdir %QT_PATH%\include\Snoop\Common
copy .\Snoop\Common\*.* %QT_PATH%\include\Snoop\Common\*.*

mkdir %QT_PATH%\include\Snoop\Graph
copy .\Snoop\Graph\*.* %QT_PATH%\include\Snoop\Graph\*.*

mkdir %QT_PATH%\include\Snoop\Libnet
copy .\Snoop\Libnet\*.* %QT_PATH%\include\Snoop\Libnet\*.*

mkdir %QT_PATH%\include\Snoop\Parse
copy .\Snoop\Parse\*.* %QT_PATH%\include\Snoop\Parse\*.*

mkdir %QT_PATH%\include\Snoop\Process
copy .\Snoop\Process\*.* %QT_PATH%\include\Snoop\Process\*.*

rem ***************************************************************************
rem lib files
rem ***************************************************************************
copy .\Bin\Snoopd.lib %QT_PATH%\lib
copy .\Bin\Snoop.lib  %QT_PATH%\lib
goto eof

rem ***************************************************************************
rem usage
rem ***************************************************************************
:usage
echo "syntax :  _install <compiler folder>"
echo "example : _install C:\QtSDK\Desktop\Qt\4.8.1\msvc2008"
goto eof

rem ***************************************************************************
rem eof
rem ***************************************************************************
:eof
