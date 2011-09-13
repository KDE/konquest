#! /bin/sh
$EXTRACTRC *.rc *.ui >> rc.cpp
$XGETTEXT *.cc *.cpp dialogs/*.cc dialogs/*.cpp rc.cpp -o $podir/konquest.pot
