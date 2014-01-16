@echo off
rem ***************************************************************************
rem QT_PATH
rem ***************************************************************************
set QT_PATH=%1
if "%1" == "" goto usage

rem ***************************************************************************
rem header files
rem ***************************************************************************
del /q %QT_PATH%\include\Snoop\Capture\*.*
rmdir  %QT_PATH%\include\Snoop\Capture

del /q %QT_PATH%\include\Snoop\Common\*.*
rmdir  %QT_PATH%\include\Snoop\Common

del /q %QT_PATH%\include\Snoop\Graph\*.*
rmdir  %QT_PATH%\include\Snoop\Graph

del /q %QT_PATH%\include\Snoop\Libnet\*.*
rmdir  %QT_PATH%\include\Snoop\Libnet

del /q %QT_PATH%\include\Snoop\Parse\*.*
rmdir  %QT_PATH%\include\Snoop\Parse

del /q %QT_PATH%\include\Snoop\Process\*.*
rmdir  %QT_PATH%\include\Snoop\Process

rmdir  %QT_PATH%\include\Snoop

rem ***************************************************************************
rem lib files
rem ***************************************************************************
del %QT_PATH%\lib\Snoopd.lib 
del %QT_PATH%\lib\Snoop.lib
goto eof

rem ***************************************************************************
rem usage
rem ***************************************************************************
:usage
echo "syntax :  _uninstall <compiler folder>"
echo "example : _uninstall C:\QtSDK\Desktop\Qt\4.8.1\msvc2008"
goto eof

rem ***************************************************************************
rem eof
rem ***************************************************************************
:eof
