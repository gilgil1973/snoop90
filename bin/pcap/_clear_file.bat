@echo off
rem --------------------------------------------------------
rem _clear version 2014.03.01
rem Copyright (c) Gilbert Lee All rights reserved
rem --------------------------------------------------------

rem --------------------------------------------------------
rem picture
rem --------------------------------------------------------
call :rm_file *.jpg
call :rm_file *.jpeg
call :rm_file *.gif
call :rm_file *.png
call :rm_file *.ico
call :rm_file *.swf

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

