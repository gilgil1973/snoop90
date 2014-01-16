rem ---------------------------------------------------------------------------
rem snoop
rem ---------------------------------------------------------------------------

rem snoopd.lib
cd lib
qmake "CONFIG+=debug" "QT-=gui"
nmake
nmake clean
cd ..

rem snoopd_gui.lib
cd lib
qmake "CONFIG+=debug" "QT+=gui" "QT+=widgets"
nmake
nmake clean
cd ..

rem snoop.lib
cd lib
qmake "CONFIG+=release" "QT-=gui"
nmake
nmake clean
cd ..

rem snoop_gui.lib
cd lib
qmake "CONFIG+=release" "QT+=gui" "QT+=widgets"
nmake
rem nmake clean
cd ..

rem ---------------------------------------------------------------------------
rem app
rem ---------------------------------------------------------------------------
qmake
nmake
