@echo off
rem --------------------------------------------------------
rem _cleat.bat version 2013.02.25
rem Copyright (c) Gilbert Lee All rights reserved
rem --------------------------------------------------------

rem --------------------------------------------------------
rem common
rem --------------------------------------------------------
del /s *.obj
del /s *.user
del /s *.ini
del /s *.bak
del /s *.log
del /s *.xml
del /s *.json

rem --------------------------------------------------------
rem borland
rem --------------------------------------------------------
del /s *.~*
del /s *.dcu
del /s *.dsk
del /s *.cfg
del /s *.dsm
del /s *.dcp
del /s *.tds
del /s *.#*
del /s *.local
del /s *.cgl
del /s *.ilc
del /s *.ild
del /s *.ilf
del /s *.ils
del /s *.map
del /s *.identcache

rem --------------------------------------------------------
rem microsoft
rem --------------------------------------------------------
del /s *.pch
del /s *.idb
del /s *.pdb
del /s *.exp
del /s *.ilk
del /s *.ncb
del /s *.aps
del /s *.plg
del /s *.opt
del /s *.bsc
del /s *.sbr
del /s *.dep
del /s *.manifest
del /s *.user
del /s *.manifest.res
del /s BuildLog.htm
del /s /ah *.suo

rem --------------------------------------------------------
rem gcc
rem --------------------------------------------------------
del /s *.o

rem --------------------------------------------------------
rem qt
rem --------------------------------------------------------
del /s Makefile.*
del /s ui_*.h
del /s qrc_*.cpp
del /s moc_*.cpp
rm_folder /q GeneratedFiles

rem --------------------------------------------------------
rem other
rem --------------------------------------------------------
rem del /s *.lib
rem del /s *.exe

rem --------------------------------------------------------
rem folder
rem --------------------------------------------------------
rm_folder /q /i Debug
rm_folder /q /i Release
rm_folder /q    log
rm_folder /q    __history
