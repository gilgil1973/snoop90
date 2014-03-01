@echo off
rem --------------------------------------------------------
rem _clear version 2014.03.01
rem Copyright (c) Gilbert Lee All rights reserved
rem --------------------------------------------------------

rem --------------------------------------------------------
rem common
rem --------------------------------------------------------
call :rm_file *.obj
call :rm_file *.user
call :rm_file *.ini
call :rm_file *.bak
call :rm_file *.log
call :rm_file *.xml
call :rm_file *.json

rem --------------------------------------------------------
rem borland
rem --------------------------------------------------------
call :rm_file *.~*
call :rm_file *.dcu
call :rm_file *.dsk
call :rm_file *.cfg
call :rm_file *.dsm
call :rm_file *.dcp
call :rm_file *.tds
call :rm_file *.#*
call :rm_file *.local
call :rm_file *.cgl
call :rm_file *.ilc
call :rm_file *.ild
call :rm_file *.ilf
call :rm_file *.ils
call :rm_file *.map
call :rm_file *.identcache

rem --------------------------------------------------------
rem microsoft
rem --------------------------------------------------------
call :rm_file *.pch
call :rm_file *.idb
call :rm_file *.pdb
call :rm_file *.exp
call :rm_file *.ilk
call :rm_file *.ncb
call :rm_file *.aps
call :rm_file *.plg
call :rm_file *.opt
call :rm_file *.bsc
call :rm_file *.sbr
call :rm_file *.dep
call :rm_file *.manifest
call :rm_file *.sdf
call :rm_file *.manifest.res
call :rm_file *.sln.cache
call :rm_file BuildLog.htm
call :rm_file /ah *.suo

rem --------------------------------------------------------
rem gcc
rem --------------------------------------------------------
call :rm_file *.o

rem --------------------------------------------------------
rem qt
rem --------------------------------------------------------
call :rm_file Makefile
call :rm_file Makefile.*
call :rm_file ui_*.h
call :rm_file qrc_*.cpp
call :rm_file moc_*.cpp
call :rm_folder GeneratedFiles

rem --------------------------------------------------------
rem other
rem --------------------------------------------------------
rem call :rm_file *.lib
rem call :rm_file *.exe

rem --------------------------------------------------------
rem folder
rem --------------------------------------------------------
call :rm_folder Debug
call :rm_folder debug
call :rm_folder Release
call :rm_folder release
rem call :rm_folder log
call :rm_folder Win32
call :rm_folder x64
call :rm_folder cache
call :rm_folder __history

rem --------------------------------------------------------
rem epilogue
rem --------------------------------------------------------
goto eof

rem --------------------------------------------------------
rem rm_file
rem --------------------------------------------------------
:rm_file
  del /s %1 %2 %3 %4 %5 %6 %7 %8 
goto eof

rem --------------------------------------------------------
rem rm_folder
rem --------------------------------------------------------
:rm_folder
  set __path__=%1
  if not defined __path__ goto eof
  for /f "tokens=*" %%I in ('dir /b /s /ad %__path__%*') do if %%~nxI==%__path__% (
    rmdir /s /q "%%I"
  )
goto eof

:eof

